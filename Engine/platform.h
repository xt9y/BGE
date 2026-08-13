#ifndef BGE_APP_INTERNAL_H
#define BGE_APP_INTERNAL_H

#include <stdbool.h>
#include "state.h"

#define TITLE "BGE"
#define WIDTH 1270
#define HEIGHT 800
#define RENDER_BASE_W 200
#define PALETTE_LEVELS 32.0f

bool app_platform_init(void);
bool app_frame_begin(void);
void app_frame_end(void);
void app_platform_shutdown(void);
double app_get_fps(void);
bool fbo_resize(i32 w, i32 h);
void post_blit(i32 src_w, i32 src_h, i32 dst_w, i32 dst_h);
u32 app_framebuffer(void);

#endif
