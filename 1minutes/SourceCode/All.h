#pragma once
#include <iostream>
#include "Main.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "Obstacles.h"
#include "Player.h"
#include "Enemy.h"
#include "stage.h"
#include "checkpoint.h"

#include "../GameLib/game_lib.h"

#define SCENE_NONE      (-1)
#define SCENE_TITLE     (0)
#define SCENE_GAME      (1)

//  íËêî
const FLOAT PI{ DirectX::XM_PI };
const LONG SCREEN_W{ 1920 };
const LONG SCREEN_H{ 1080 };
const BOOL FULLSCREEN{ FALSE };
const LPCWSTR APPLICATION_NAME{ L"C++ project template" };