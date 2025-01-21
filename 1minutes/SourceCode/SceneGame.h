#ifndef SCENE_GAME_H
#define SCENE_GAME_H

//******************************************************************************
//
//
//      scene_game.h
//
//
//******************************************************************************

// 関数のプロトタイプ宣言
void game_init();
void game_deinit();
void game_update();
void game_render();
void stage_make(bool init);
void back_move();
void stage_deinit();
void checkPoint_respawn();

#endif//SCENE_GAME_H
