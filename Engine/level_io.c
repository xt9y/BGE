#include "level.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    level_quad_t* items;
    i32 count;
    i32 capacity;
} level_quad_block_t;

static char* level_io_dup_range(const char* begin, size_t len)
{
    char* out = malloc(len + 1);
    if (!out) return NULL;
    memcpy(out, begin, len);
    out[len] = '\0';
    return out;
}

static const char* level_io_basename(const char* path)
{
    const char* base = path;
    if (!path) return "scene.h";
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\') base = p + 1;
    return base;
}

static char* level_io_quoted_value(const char* line, const char* field)
{
    const char* p = strstr(line, field);
    if (!p) return NULL;
    p = strchr(p, '"');
    if (!p) return NULL;
    const char* end = strchr(p + 1, '"');
    if (!end) return NULL;
    return level_io_dup_range(p + 1, (size_t)(end - (p + 1)));
}

static bool level_io_block_index(const char* line, i32* out)
{
    const char* p = strstr(line, "_sector");
    if (!p) return false;
    p += 7;
    char* end = NULL;
    long value = strtol(p, &end, 10);
    if (end == p || value < 0 || value >= MAX_SECTORS_PER_LEVEL || strncmp(end, "_quads", 6) != 0)
        return false;
    *out = (i32)value;
    return true;
}

static bool level_io_push_quad(level_quad_block_t* block, const level_quad_t* quad)
{
    if (block->count == block->capacity) {
        i32 next = block->capacity ? block->capacity * 2 : 16;
        level_quad_t* items = realloc(block->items, sizeof(*items) * (size_t)next);
        if (!items) return false;
        block->items = items;
        block->capacity = next;
    }
    block->items[block->count++] = *quad;
    return true;
}

static bool level_io_bool(const char* token)
{
    while (*token == ' ' || *token == '\t') ++token;
    return strncmp(token, "true", 4) == 0;
}

static bool level_io_parse_quad(const char* line, level_quad_t* quad)
{
    char solid[16] = {0};
    char invisible[16] = {0};
    char billboard[16] = {0};
    char flip[16] = {0};
    memset(quad, 0, sizeof(*quad));

    const int matched = sscanf(
        line,
        " { .pos = {%f, %f, %f}, .rot = {%f, %f, %f}, .size = {%f, %f}, .tex_id = %d, .is_solid = %15[^,], .is_invisible = %15[^,], .is_billboard = %15[^,], .portal_side_flip = %15[^,], .color = {%ff, %ff, %ff}, .portal_id = %d, .sector_id = %d }",
        &quad->pos.x, &quad->pos.y, &quad->pos.z,
        &quad->rot.x, &quad->rot.y, &quad->rot.z,
        &quad->size.x, &quad->size.y,
        &quad->tex_id,
        solid, invisible, billboard, flip,
        &quad->color.x, &quad->color.y, &quad->color.z,
        &quad->portal_id, &quad->sector_id);

    if (matched != 18) return false;
    quad->is_solid = level_io_bool(solid);
    quad->is_invisible = level_io_bool(invisible);
    quad->is_billboard = level_io_bool(billboard);
    quad->portal_side_flip = level_io_bool(flip);
    return true;
}

static bool level_io_parse_sector(
    const char* line,
    level_quad_block_t blocks[MAX_SECTORS_PER_LEVEL],
    level_sector_data_t* sector)
{
    memset(sector, 0, sizeof(*sector));
    if (sscanf(
            line,
            " { .id = %d, .light = {%ff, %ff, %ff}",
            &sector->id,
            &sector->light.x,
            &sector->light.y,
            &sector->light.z) != 4)
        return false;

    if (strstr(line, ".quads = NULL")) return true;

    i32 block_index = -1;
    const char* quads = strstr(line, ".quads = ");
    if (!quads || !level_io_block_index(quads, &block_index)) return false;

    sector->quads = blocks[block_index].items;
    sector->quad_count = blocks[block_index].count;
    sector->quad_capacity = blocks[block_index].count;
    blocks[block_index].items = NULL;
    blocks[block_index].count = 0;
    blocks[block_index].capacity = 0;
    return true;
}

static void level_io_free_blocks(level_quad_block_t blocks[MAX_SECTORS_PER_LEVEL])
{
    for (i32 i = 0; i < MAX_SECTORS_PER_LEVEL; ++i) free(blocks[i].items);
}

void level_free_loaded_header(level_data_t* level)
{
    if (!level) return;
    if (level->sectors) {
        for (i32 i = 0; i < level->sector_count; ++i) free(level->sectors[i].quads);
        free(level->sectors);
    }
    free((void*)level->name);
    free((void*)level->path);
    memset(level, 0, sizeof(*level));
}

bool level_load_header(const char* path, level_data_t* out)
{
    if (!path || !*path || !out) return false;
    memset(out, 0, sizeof(*out));

    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "BGE: cannot open startup scene '%s'\n", path);
        return false;
    }

    level_quad_block_t blocks[MAX_SECTORS_PER_LEVEL] = {0};
    level_sector_data_t sectors[MAX_SECTORS_PER_LEVEL] = {0};
    i32 sector_count = 0;
    i32 current_block = -1;
    bool in_sector_table = false;
    camera_t camera = {0};
    char* name = NULL;
    char* stored_path = NULL;
    char line[4096];
    i32 line_number = 0;
    bool ok = true;

    while (fgets(line, sizeof(line), f)) {
        ++line_number;

        if (strstr(line, "static level_quad_t ") && strstr(line, "_quads[] = {")) {
            if (!level_io_block_index(line, &current_block)) { ok = false; break; }
            continue;
        }

        if (current_block >= 0) {
            if (strstr(line, "};")) {
                current_block = -1;
                continue;
            }
            if (strstr(line, ".pos =")) {
                level_quad_t quad;
                if (!level_io_parse_quad(line, &quad) || !level_io_push_quad(&blocks[current_block], &quad)) {
                    ok = false;
                    break;
                }
            }
            continue;
        }

        if (strstr(line, "static level_sector_data_t ") && strstr(line, "_sectors[] = {")) {
            in_sector_table = true;
            continue;
        }

        if (in_sector_table) {
            if (strstr(line, "};")) {
                in_sector_table = false;
                continue;
            }
            if (strstr(line, ".id =")) {
                if (sector_count >= MAX_SECTORS_PER_LEVEL ||
                    !level_io_parse_sector(line, blocks, &sectors[sector_count])) {
                    ok = false;
                    break;
                }
                ++sector_count;
            }
            continue;
        }

        if (strstr(line, ".name =")) {
            char* value = level_io_quoted_value(line, ".name =");
            if (value) { free(name); name = value; }
            continue;
        }

        if (strstr(line, ".path =")) {
            char* value = level_io_quoted_value(line, ".path =");
            if (value) { free(stored_path); stored_path = value; }
            continue;
        }

        if (strstr(line, ".cam =")) {
            if (sscanf(
                    line,
                    " .cam = { .pos = {%ff, %ff, %ff}, .yaw = %ff, .pitch = %ff }",
                    &camera.pos.x, &camera.pos.y, &camera.pos.z,
                    &camera.yaw, &camera.pitch) != 5) {
                ok = false;
                break;
            }
        }
    }

    fclose(f);

    if (!ok || sector_count <= 0) {
        fprintf(stderr, "BGE: malformed startup scene '%s' near line %d\n", path, line_number);
        for (i32 i = 0; i < sector_count; ++i) free(sectors[i].quads);
        level_io_free_blocks(blocks);
        free(name);
        free(stored_path);
        return false;
    }

    level_sector_data_t* owned_sectors = malloc(sizeof(*owned_sectors) * (size_t)sector_count);
    if (!owned_sectors) {
        for (i32 i = 0; i < sector_count; ++i) free(sectors[i].quads);
        level_io_free_blocks(blocks);
        free(name);
        free(stored_path);
        return false;
    }
    memcpy(owned_sectors, sectors, sizeof(*owned_sectors) * (size_t)sector_count);
    level_io_free_blocks(blocks);

    if (!name) {
        const char* base = level_io_basename(path);
        name = level_io_dup_range(base, strlen(base));
    }
    if (!stored_path) {
        const char* base = level_io_basename(path);
        stored_path = level_io_dup_range(base, strlen(base));
    }
    if (!name || !stored_path) {
        free(name);
        free(stored_path);
        for (i32 i = 0; i < sector_count; ++i) free(owned_sectors[i].quads);
        free(owned_sectors);
        return false;
    }

    out->name = name;
    out->path = stored_path;
    out->sectors = owned_sectors;
    out->sector_count = sector_count;
    out->sector_capacity = sector_count;
    out->cam = camera;
    return true;
}
