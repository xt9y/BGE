#include "level.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define BGE_MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define BGE_MKDIR(path) mkdir((path), 0755)
#endif

static char* bge_strdup(const char* s)
{
    if (!s) s = "";
    const size_t n = strlen(s) + 1;
    char* out = (char*)malloc(n);
    if (out) memcpy(out, s, n);
    return out;
}

void level_free_owned(level_data_t* level)
{
    if (!level) return;
    if (level->sectors) {
        for (i32 i = 0; i < level->sector_count; ++i)
            free(level->sectors[i].quads);
        free(level->sectors);
    }
    free((void*)level->name);
    free((void*)level->path);
    memset(level, 0, sizeof(*level));
}

bool level_clone_owned(const level_data_t* src, level_data_t* out)
{
    if (!src || !out || src == out) return false;
    memset(out, 0, sizeof(*out));

    out->name = bge_strdup(src->name && src->name[0] ? src->name : "Level");
    out->path = bge_strdup(src->path ? src->path : "");
    out->cam = src->cam;
    if (!out->name || !out->path) {
        level_free_owned(out);
        return false;
    }

    if (src->sector_count < 0 || src->sector_count > MAX_SECTORS_PER_LEVEL) {
        level_free_owned(out);
        return false;
    }
    if (src->sector_count > 0 && !src->sectors) {
        level_free_owned(out);
        return false;
    }
    const i32 count = src->sector_count > 0 ? src->sector_count : 1;
    out->sectors = (level_sector_data_t*)calloc((size_t)count, sizeof(*out->sectors));
    if (!out->sectors) {
        level_free_owned(out);
        return false;
    }
    out->sector_count = count;
    out->sector_capacity = count;

    if (src->sector_count <= 0 || !src->sectors) {
        out->sectors[0].id = 0;
        out->sectors[0].light = (vec3s){1.0f, 1.0f, 1.0f};
        return true;
    }

    for (i32 s = 0; s < count; ++s) {
        out->sectors[s] = src->sectors[s];
        out->sectors[s].quads = NULL;
        out->sectors[s].quad_capacity = 0;

        const i32 qcount = src->sectors[s].quad_count;
        if (qcount < 0 || qcount > MAX_QUADS_PER_SECTOR) {
            level_free_owned(out);
            return false;
        }
        if (qcount > 0 && !src->sectors[s].quads) {
            level_free_owned(out);
            return false;
        }

        out->sectors[s].quad_count = qcount;
        out->sectors[s].quad_capacity = qcount;
        if (!qcount) continue;

        out->sectors[s].quads = (level_quad_t*)malloc(sizeof(level_quad_t) * (size_t)qcount);
        if (!out->sectors[s].quads) {
            level_free_owned(out);
            return false;
        }
        memcpy(out->sectors[s].quads, src->sectors[s].quads,
            sizeof(level_quad_t) * (size_t)qcount);
    }
    return true;
}

bool level_set_owned_path(level_data_t* level, const char* path)
{
    if (!level || !path || !path[0]) return false;
    char* copy = bge_strdup(path);
    if (!copy) return false;
    free((void*)level->path);
    level->path = copy;
    return true;
}

static bool ensure_parent_dirs(const char* path)
{
    char buffer[1024];
    const size_t n = strlen(path);
    if (n >= sizeof(buffer)) return false;
    memcpy(buffer, path, n + 1);

    for (size_t i = 1; i < n; ++i) {
        if (buffer[i] != '/' && buffer[i] != '\\') continue;
        const char saved = buffer[i];
        buffer[i] = '\0';
        if (buffer[0] && BGE_MKDIR(buffer) != 0 && errno != EEXIST) return false;
        buffer[i] = saved;
    }
    return true;
}

static void write_c_string(FILE* file, const char* string)
{
    fputc('"', file);
    if (string) {
        for (; *string; ++string) {
            const unsigned char c = (unsigned char)*string;
            if (c == '\\' || c == '"') { fputc('\\', file); fputc(c, file); }
            else if (c == '\n') fputs("\\n", file);
            else if (c == '\r') fputs("\\r", file);
            else if (c == '\t') fputs("\\t", file);
            else if (c >= 32 && c < 127) fputc(c, file);
            else fprintf(file, "\\%03o", c);
        }
    }
    fputc('"', file);
}

static bool valid_identifier(const char* string)
{
    if (!string || !(isalpha((unsigned char)string[0]) || string[0] == '_')) return false;
    for (const char* p = string + 1; *p; ++p)
        if (!(isalnum((unsigned char)*p) || *p == '_')) return false;
    return true;
}

static bool finite_vec2(const vec2s v)
{
    return isfinite(v.x) && isfinite(v.y);
}

static bool finite_vec3(const vec3s v)
{
    return isfinite(v.x) && isfinite(v.y) && isfinite(v.z);
}

static bool valid_level_for_save(const level_data_t* level)
{
    if (!level || !level->sectors || level->sector_count <= 0 ||
        level->sector_count > MAX_SECTORS_PER_LEVEL ||
        !finite_vec3(level->cam.pos) || !isfinite(level->cam.yaw) || !isfinite(level->cam.pitch))
        return false;

    for (i32 s = 0; s < level->sector_count; ++s) {
        const level_sector_data_t* sector = &level->sectors[s];
        if (sector->quad_count < 0 || sector->quad_count > MAX_QUADS_PER_SECTOR ||
            !finite_vec3(sector->light))
            return false;
        if (sector->quad_count > 0 && !sector->quads) return false;

        for (i32 q = 0; q < sector->quad_count; ++q) {
            const level_quad_t* quad = &sector->quads[q];
            if (!finite_vec3(quad->pos) || !finite_vec3(quad->rot) ||
                !finite_vec3(quad->color) || !finite_vec2(quad->size) ||
                quad->size.x < 0.0f || quad->size.y < 0.0f)
                return false;
        }
    }
    return true;
}

bool level_save_header(const level_data_t* level, const char* path, const char* loader_name)
{
    if (!valid_level_for_save(level) || !path || !path[0] || !valid_identifier(loader_name))
        return false;
    if (!ensure_parent_dirs(path)) return false;

    char tmp_path[1200];
    if (snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", path) >= (int)sizeof(tmp_path)) return false;
    FILE* file = fopen(tmp_path, "w");
    if (!file) return false;

    char guard[256];
    size_t gi = 0;
    const char* prefix = "BGE_GENERATED_";
    for (const char* p = prefix; *p && gi + 1 < sizeof(guard); ++p) guard[gi++] = *p;
    for (const char* p = loader_name; *p && gi + 3 < sizeof(guard); ++p)
        guard[gi++] = (char)toupper((unsigned char)*p);
    guard[gi++] = '_';
    guard[gi++] = 'H';
    guard[gi] = '\0';

    fprintf(file, "#ifndef %s\n#define %s\n\n#include \"level.h\"\n\n", guard, guard);

    for (i32 s = 0; s < level->sector_count; ++s) {
        const level_sector_data_t* sector = &level->sectors[s];
        if (sector->quad_count <= 0) continue;
        fprintf(file, "static level_quad_t bge_%s_sector%d_quads[] = {\n", loader_name, s);
        for (i32 q = 0; q < sector->quad_count; ++q) {
            const level_quad_t* quad = &sector->quads[q];
            fprintf(file,
                "    { .pos = {%.6ff, %.6ff, %.6ff}, .rot = {%.6ff, %.6ff, %.6ff}, .size = {%.6ff, %.6ff}, .tex_id = %d, .sector_id = %d, .portal_id = %d, .is_solid = %s, .is_invisible = %s, .is_billboard = %s, .portal_side_flip = %s, .color = {%.6ff, %.6ff, %.6ff} },\n",
                quad->pos.x, quad->pos.y, quad->pos.z,
                quad->rot.x, quad->rot.y, quad->rot.z,
                quad->size.x, quad->size.y,
                quad->tex_id, quad->sector_id, quad->portal_id,
                quad->is_solid ? "true" : "false",
                quad->is_invisible ? "true" : "false",
                quad->is_billboard ? "true" : "false",
                quad->portal_side_flip ? "true" : "false",
                quad->color.x, quad->color.y, quad->color.z);
        }
        fputs("};\n\n", file);
    }

    fprintf(file, "static level_sector_data_t bge_%s_sectors[] = {\n", loader_name);
    for (i32 s = 0; s < level->sector_count; ++s) {
        const level_sector_data_t* sector = &level->sectors[s];
        if (sector->quad_count > 0) {
            fprintf(file,
                "    { .id = %d, .quad_count = %d, .quad_capacity = 0, .light = {%.6ff, %.6ff, %.6ff}, .quads = bge_%s_sector%d_quads },\n",
                sector->id, sector->quad_count,
                sector->light.x, sector->light.y, sector->light.z,
                loader_name, s);
        } else {
            fprintf(file,
                "    { .id = %d, .quad_count = 0, .quad_capacity = 0, .light = {%.6ff, %.6ff, %.6ff}, .quads = NULL },\n",
                sector->id, sector->light.x, sector->light.y, sector->light.z);
        }
    }
    fputs("};\n\n", file);

    fprintf(file, "static inline level_data_t %s(void)\n{\n    return (level_data_t){\n        .name = ", loader_name);
    write_c_string(file, level->name && level->name[0] ? level->name : "Level");
    fputs(",\n        .path = ", file);
    write_c_string(file, path);
    fprintf(file,
        ",\n        .sectors = bge_%s_sectors,\n        .sector_count = %d,\n        .sector_capacity = 0,\n        .cam = { .pos = {%.6ff, %.6ff, %.6ff}, .yaw = %.6ff, .pitch = %.6ff }\n    };\n}\n\n#endif\n",
        loader_name, level->sector_count,
        level->cam.pos.x, level->cam.pos.y, level->cam.pos.z,
        level->cam.yaw, level->cam.pitch);

    const bool write_ok = ferror(file) == 0;
    const bool close_ok = fclose(file) == 0;
    if (!write_ok || !close_ok) {
        remove(tmp_path);
        return false;
    }
#ifdef _WIN32
    if (!MoveFileExA(tmp_path, path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        remove(tmp_path);
        return false;
    }
#else
    if (rename(tmp_path, path) != 0) {
        remove(tmp_path);
        return false;
    }
#endif
    return true;
}
