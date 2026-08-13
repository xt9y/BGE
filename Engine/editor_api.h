#ifndef BGE_EDITOR_API_H
#define BGE_EDITOR_API_H
#include "cam.h"
#include "level.h"
typedef enum{EDITOR_IDLE=0,EDITOR_DRAG,EDITOR_RESIZE_TOP,EDITOR_RESIZE_RIGHT,EDITOR_PAINT}editor_e;
typedef enum{EDITOR_MOD_NONE=0,EDITOR_MOD_SECTOR=1<<0,EDITOR_MOD_TEXTURE=1<<1,EDITOR_MOD_COLOR=1<<2,EDITOR_MOD_ROTATION=1<<3,EDITOR_MOD_ALL=0xFF}editor_mod_e;
#define MAX_WALL_WINDOWS 64
typedef struct{u32 id;i32 sector_id;i32 quad_idx;}editor_wall_window_t;
typedef struct{level_data_t*level;i32 selected_sector_id,selected_wall_idx;editor_e id,hover_id;vec3s drag_start_hit,drag_quad_start_pos,drag_quad_start_rot,drag_plane_normal,drag_cam_start_pos;vec2s drag_quad_start_size;level_quad_t template_quad;u32 template_mods;bool portal_link_mode;i32 portal_link_sector_id,portal_link_wall_idx;bool ui_sector_open;i32 ui_sector_idx;editor_wall_window_t ui_wall_windows[MAX_WALL_WINDOWS];i32 ui_wall_window_count;u32 next_wall_window_id;bool mouse_was_pressed;}editor_t;
typedef struct{bool hit;i32 sector_id,wall_id;vec3s hit_position,local_hit;f32 distance;}editor_look_at_info_t;
void editor_init(void);void editor_shutdown(void);void editor_update(void);void editor_ui(void);void editor_render_borders(void);level_sector_data_t*editor_sector_by_id(i32);level_sector_data_t*editor_selected_sector(void);level_quad_t*editor_selected_quad(void);void editor_clear_selection(void);bool editor_add_sector(level_data_t*);bool editor_add_quad(level_sector_data_t*,const level_quad_t*);bool editor_add_template_quad(void);bool editor_delete_selected_quad(void);bool editor_move_quad_to_sector(level_sector_data_t*,level_sector_data_t*,i32);void editor_reset_selected_quad(void);void editor_toggle_paint_mode(void);void editor_rotate_selected_or_template(i32,f32);void editor_open_wall_window(i32,i32);i32 count_portal_quads(const level_data_t*,i32);void editor_clear_portal_selected(void);
static inline level_quad_t get_default_quad(const camera_t*c){vec3s p=c?(vec3s){roundf(c->pos.x+c->front.x*3),roundf(c->pos.y+c->front.y*3),roundf(c->pos.z+c->front.z*3)}:(vec3s){0,0,0};return(level_quad_t){.pos=p,.rot={0,0,0},.color={1,1,1},.size={2,2},.tex_id=0,.sector_id=0,.portal_id=0,.is_solid=true};}
#endif
