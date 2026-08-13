#include "gun.h"
#include "Engine/state.h"
#include <glad/glad.h>
#include <string.h>
void game_gun_reset(game_gun_t*g){if(g)memset(g,0,sizeof(*g));}
bool game_gun_init(game_gun_t*g){if(!g)return false;static const vertex_t v[6]={{{0,0,0},{0,0},{1,1,1,1}},{{1,1,0},{1,1},{1,1,1,1}},{{1,0,0},{1,0},{1,1,1,1}},{{0,0,0},{0,0},{1,1,1,1}},{{0,1,0},{0,1},{1,1,1,1}},{{1,1,0},{1,1},{1,1,1,1}}};glGenVertexArrays(1,&g->vao);glGenBuffers(1,&g->vbo);if(!g->vao||!g->vbo){game_gun_shutdown(g);return false;}glBindVertexArray(g->vao);glBindBuffer(GL_ARRAY_BUFFER,g->vbo);glBufferData(GL_ARRAY_BUFFER,sizeof(v),v,GL_DYNAMIC_DRAW);glEnableVertexAttribArray(0);glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_t),(void*)0);glEnableVertexAttribArray(1);glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vertex_t),(void*)(sizeof(f32)*3));glEnableVertexAttribArray(2);glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE,sizeof(vertex_t),(void*)(sizeof(f32)*5));glBindVertexArray(0);return true;}
void game_gun_shutdown(game_gun_t*g){if(!g)return;if(g->vbo)glDeleteBuffers(1,&g->vbo);if(g->vao)glDeleteVertexArrays(1,&g->vao);game_gun_reset(g);}
void game_gun_shot(game_gun_t*g){if(g&&state.id==STATE_PLAYING){g->flash_timer=.1f;g->animation_time=.001f;}}
void game_gun_next(game_gun_t*g){if(g&&g->count>0)g->current=(g->current+1)%g->count;}
