#include "SceneGame.h"
#include "Player.h"
#include "stage.h"
#include "Enemy.h"
#include "checkpoint.h"
#include <vector>

//------< 定数 >----------------------------------------------------------------


//------< 構造体 >---------------------------------------------------------------


//------< 変数 >----------------------------------------------------------------
int game_state;
int game_timer;
VECTOR2 backPos;
VECTOR2 velocity;
float gravity;
int posNum;
int back_count;
int next_up;
int next_right;
int next_down;
int clear_timer;
bool through[6];
bool respawn;
bool goal;
extern OBJ2D player;
extern int nextScene;
GameLib::Sprite* sprite[6];
GameLib::Sprite* enemy;
std::vector<Stage*> stage;
std::vector<Jump_board*> jump_boards;
std::vector<Enemy*> enemies;
CheckPoint checkpoint;
Stage_spr stage_spr;

//--------------------------------------
//  ゲームの初期設定
//--------------------------------------
void game_init()
{
    game_state = 0;
    game_timer = 0;
    backPos = { 0,0 };
    posNum = 1;
    next_right = posNum + 3;
    next_up = posNum - 1;
    next_down = posNum + 1;
    back_count = 0;
    for (int i = 0; i < 6; i++)
    {
        through[i] = false;
    }
    stage_spr.stage_load();
    checkpoint.init();
    velocity = { -10,0 };
    gravity = 0;
    respawn = false;
    goal = false;
    clear_timer = 0;
}

//--------------------------------------
//  ゲームの終了処理
//--------------------------------------
void game_deinit()
{

    player_deinit();
    for (auto& spr : sprite)
    {
        safe_delete(spr);
    }
    safe_delete(enemy);


    stage_deinit();

    stage_spr.stage_data_delete();
}

//--------------------------------------
//  ゲームの更新処理
//--------------------------------------
void game_update()
{
    std::vector<Stage*>::iterator init_stageItr;
    std::vector<Enemy*>::iterator init_enemyItr;
    switch (game_state)
    {
    case 0:
        //////// 初期設定 ////////

        sprite[0] = GameLib::sprite_load(L"./Data/Images/1.png");
        sprite[1] = GameLib::sprite_load(L"./Data/Images/2.png");
        sprite[2] = GameLib::sprite_load(L"./Data/Images/3.png");
        sprite[3] = GameLib::sprite_load(L"./Data/Images/4.png");
        sprite[4] = GameLib::sprite_load(L"./Data/Images/5.png");
        sprite[5] = GameLib::sprite_load(L"./Data/Images/6.png");
        enemy = GameLib::sprite_load(L"./Data/Images/zombie.png");


        player_init();

        game_state++;
        /*fallthrough*/

    case 1:
        //////// パラメータの設定 ////////

        GameLib::setBlendMode(GameLib::Blender::BS_ALPHA);
        for (int i = 0; i < QUANTITY; i++)
        {
            stage.push_back(new Stage(posNum, 0, back_count, i, backPos, stage_spr.spr[rand() % 8]));
            init_stageItr = stage.end() - 1;
            if ((*init_stageItr)->stage_exsit() == false)
            {
                delete* init_stageItr;
                init_stageItr = stage.erase(init_stageItr);
            }
            else
            {
                for (int j = 0; j < ENEMY_MAX; j++)
                {
                    enemies.push_back(new Enemy(posNum, 0, back_count, i, j, backPos, (*init_stageItr)->stage_position(), enemy));
                    init_enemyItr = enemies.end() - 1;
                    if ((*init_enemyItr)->get_enemyExsit() == false)
                    {
                        delete* init_enemyItr;
                        init_enemyItr = enemies.erase(init_enemyItr);
                    }
                }
            }


        }
        for (int i = 0; i < QUANTITY; i++)
        {
            stage.push_back(new Stage(posNum, 1, back_count, i, backPos, stage_spr.spr[rand() % 8]));
            init_stageItr = stage.end() - 1;
            if ((*init_stageItr)->stage_exsit() == false)
            {
                delete* init_stageItr;
                init_stageItr = stage.erase(init_stageItr);
            }
            else
            {
                for (int j = 0; j < ENEMY_MAX; j++)
                {
                    enemies.push_back(new Enemy(posNum, 1, back_count, i, j, backPos, (*init_stageItr)->stage_position(), enemy));
                    init_enemyItr = enemies.end() - 1;
                    if ((*init_enemyItr)->get_enemyExsit() == false)
                    {
                        delete* init_enemyItr;
                        init_enemyItr = enemies.erase(init_enemyItr);
                    }
                }
            }
        }
        for (int i = 0; i < QUANTITY; i++)
        {
            stage.push_back(new Stage(posNum, 2, back_count, i, backPos, stage_spr.spr[rand() % 8]));
            init_stageItr = stage.end() - 1;
            if ((*init_stageItr)->stage_exsit() == false)
            {
                delete* init_stageItr;
                init_stageItr = stage.erase(init_stageItr);
            }
            else
            {
                for (int j = 0; j < ENEMY_MAX; j++)
                {
                    enemies.push_back(new Enemy(posNum, 2, back_count, i, j, backPos, (*init_stageItr)->stage_position(), enemy));
                    init_enemyItr = enemies.end() - 1;
                    if ((*init_enemyItr)->get_enemyExsit() == false)
                    {
                        delete* init_enemyItr;
                        init_enemyItr = enemies.erase(init_enemyItr);
                    }
                }
            }
        }
        through[0] = true;
        through[1] = true;
        through[2] = true;




        game_state++;
        /*fallthrough*/

    case 2:
        //////// 通常時 ////////

        GameLib::debug::setString("");
        GameLib::debug::setString("A D   : LEFT, RIGHT");
        GameLib::debug::setString("SPACE : JUMP");
        GameLib::debug::setString("posNum%d", posNum);
        GameLib::debug::setString("back_count%d", back_count);
        GameLib::debug::setString("next_up%d", next_up);
        GameLib::debug::setString("next_down%d", next_down);
        GameLib::debug::setString("through[0]%d", checkpoint.through[0]);
        GameLib::debug::setString("through[1]%d", checkpoint.through[1]);
        GameLib::debug::setString("through[2]%d", checkpoint.through[2]);
        GameLib::debug::setString("through[3]%d", checkpoint.through[3]);
        GameLib::debug::setString("");
        if (GameLib::input::TRG(0) & GameLib::input::PAD_SELECT)
        {
            nextScene = SCENE_TITLE;
            break;
        }


        player_update();

        back_move();


        if (backPos.x <= -SCREEN_W)
        {
            //backPos背景が映らなくなったらそこにあったステージとエネミーを消す
            for (auto stage_itr = stage.begin(); stage_itr != stage.end();)
            {
                switch (back_count)
                {
                case 0:
                    if ((*stage_itr)->stage_erase(posNum, back_count))
                    {
                        delete* stage_itr;
                        stage_itr = stage.erase(stage_itr);
                    }
                    else
                    {
                        stage_itr++;
                    }
                    break;
                default:
                    if ((*stage_itr)->stage_erase(posNum, back_count - 1))
                    {
                        delete* stage_itr;
                        stage_itr = stage.erase(stage_itr);
                    }
                    else
                    {
                        stage_itr++;
                    }
                }

                for (auto enemies_itr = enemies.begin(); enemies_itr != enemies.end();)
                {
                    switch (back_count)
                    {
                    case 0:
                        if ((*enemies_itr)->enemy_erase(posNum, back_count))
                        {
                            delete* enemies_itr;
                            enemies_itr = enemies.erase(enemies_itr);
                        }
                        else
                        {
                            enemies_itr++;
                        }
                        break;
                    default:
                        if ((*enemies_itr)->enemy_erase(posNum, back_count - 1))
                        {
                            delete* enemies_itr;
                            enemies_itr = enemies.erase(enemies_itr);
                        }
                        else
                        {
                            enemies_itr++;
                        }
                    }
                }

            }
            //次の背景を出すための変数をリセットや更新
            if (posNum / 3 == 1)
            {
                through[3] = false;
                through[4] = false;
                through[5] = false;
            }
            else if (posNum / 3 == 0)
            {
                through[0] = false;
                through[1] = false;
                through[2] = false;
            }
            posNum = next_right;
            next_right = (posNum + 3) % 6;
            next_up = posNum - 1;
            next_down = posNum + 1;
            if (posNum >= 3 && posNum < 6) back_count++;
            backPos.x += SCREEN_W;
        }

        if (backPos.y <= -SCREEN_H)
        {
            if (posNum == next_down && (posNum == 2 || posNum == 5))   respawn = true;
            posNum = next_down;
            next_right = (posNum + 3) % 6;
            next_down++;
            next_up++;
            backPos.y += SCREEN_H;
        }
        if ((posNum == 2 || posNum == 5))
        {
            next_down = posNum;
        }

        if (backPos.y >= SCREEN_H)
        {
            posNum = next_up;
            next_right = (posNum + 3) % 6;
            next_up--;
            next_down--;
            backPos.y -= SCREEN_H;
        }
        if ((posNum == 0 || posNum == 3))
        {
            next_up = posNum;
        }

        for (auto stage_itr = stage.begin(); stage_itr != stage.end(); stage_itr++)
        {
            for (auto enemies_itr = enemies.begin(); enemies_itr != enemies.end(); enemies_itr++)
            {
                 (*stage_itr)->enemy_onStage(*enemies_itr);
            }
            (*stage_itr)->stage_move(velocity, gravity);
            (*stage_itr)->checkPoint_through(&checkpoint, backPos, player.pos.x);
            if ((*stage_itr)->goal(player.pos))  goal = true;
        }
        for (auto enemies_itr = enemies.begin(); enemies_itr != enemies.end(); enemies_itr++)
        {
            (*enemies_itr)->enemy_move(velocity, gravity);
            (*enemies_itr)->enemy_through_reset();
        }
        for (auto jump_itr = jump_boards.begin(); jump_itr != jump_boards.end(); jump_itr++)
        {
            (*jump_itr)->jumpBoard_move(velocity, gravity);
        }

        if (goal)
        {
            if (velocity.x < 0)
                velocity.x += 0.2f;
            else
            {
                velocity.x = 0;
                clear_timer++;
            }
            if (clear_timer < 180)
            {
                clear_timer++;
            }
            else
            {
                nextScene = SCENE_TITLE;
            }
            break;
        }

        if (respawn) checkPoint_respawn();

        stage_make(false);
    }

    game_timer++;
}

//--------------------------------------
//  ゲームの描画処理
//--------------------------------------
void game_render()
{
    GameLib::clear(0.2f, 0.2f, 0.4f);


    sprite_render(sprite[posNum], backPos.x, backPos.y, 1, 1, 0, 0, SCREEN_W, SCREEN_H, 0, 0, 0, 1, 1, 1, 1);
    if (backPos.y < 0)     sprite_render(sprite[next_down], backPos.x, backPos.y + SCREEN_H, 1, 1, 0, 0, SCREEN_W, SCREEN_H, 0, 0, 0, 1, 1, 1, 1);
    if (backPos.y > 0)     sprite_render(sprite[next_up], backPos.x, backPos.y - SCREEN_H, 1, 1, 0, 0, SCREEN_W, SCREEN_H, 0, 0, 0, 1, 1, 1, 1);

    if (backPos.x < 0)
    {
        sprite_render(sprite[next_right], backPos.x + SCREEN_W, backPos.y, 1, 1, 0, 0, SCREEN_W, SCREEN_H, 0, 0, 0, 1, 1, 1, 1);
        if (backPos.y < 0)         sprite_render(sprite[(next_down + 3) % 6], backPos.x + SCREEN_W, backPos.y + SCREEN_H);
        if (backPos.y > 0)         sprite_render(sprite[(next_up + 3) % 6], backPos.x + SCREEN_W, backPos.y - SCREEN_H);
    }



    for (auto stage_itr = stage.begin(); stage_itr != stage.end(); stage_itr++)
    {
        (*stage_itr)->stage_render();
    }

    for (auto enemies_itr = enemies.begin(); enemies_itr != enemies.end(); enemies_itr++)
    {
        (*enemies_itr)->enemy_render();
    }

    for (auto jump_itr = jump_boards.begin(); jump_itr != jump_boards.end(); jump_itr++)
    {
        (*jump_itr)->jumpBoard_render();
    }

    player_render();
}

void stage_make(bool init)
{
    std::vector<Stage*>::iterator stage_itr;
    std::vector<Enemy*>::iterator enemy_itr;
    std::vector<Jump_board*>::iterator jump_itr;
    if (init)
    {
        if (!through[0])
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 0, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 0, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 0, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }

            through[0] = true;
        }

        if (!through[1])
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 1, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 1, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 1, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }        through[1] = true;
        }

        if (!through[2])
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 2, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 2, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 2, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }
            through[2] = true;
        }

        if (!through[3])
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 3, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 3, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 3, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }
            through[3] = true;
        }

        if (!through[4])
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 4, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 4, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 4, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }
            through[4] = true;
        }

        if (!through[5])
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 5, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 5, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 5, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }
            through[5] = true;
        }
    }
    else
    {
        if (!through[0] && backPos.x < 0)
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 0, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 0, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 0, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }

            through[0] = true;
        }

        if (!through[1] && backPos.x < 0)
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 1, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 1, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 1, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }        through[1] = true;
        }

        if (!through[2] && backPos.x < 0)
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 2, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 2, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 2, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }
            through[2] = true;
        }

        if (!through[3] && backPos.x < 0)
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 3, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 3, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 3, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }
            through[3] = true;
        }

        if (!through[4] && backPos.x < 0)
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 4, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 4, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 4, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }
            through[4] = true;
        }

        if (!through[5] && backPos.x < 0)
        {
            for (int j = 0; j < QUANTITY; j++)
            {
                stage.push_back(new Stage(posNum, 5, back_count, j, backPos, stage_spr.spr[rand() % 8]));
                stage_itr = stage.end() - 1;
                if ((*stage_itr)->stage_exsit() == false)
                {
                    delete* stage_itr;
                    stage_itr = stage.erase(stage_itr);
                }
                else
                {
                    for (int k = 0; k < ENEMY_MAX; k++)
                    {
                        enemies.push_back(new Enemy(posNum, 5, back_count, j, k, backPos, (*stage_itr)->stage_position(), enemy));
                        enemy_itr = enemies.end() - 1;
                        if ((*enemy_itr)->get_enemyExsit() == false)
                        {
                            delete* enemy_itr;
                            enemy_itr = enemies.erase(enemy_itr);
                        }
                    }
                }
            }
            jump_boards.push_back(new Jump_board(posNum, 5, back_count, backPos, enemy));
            jump_itr = jump_boards.end() - 1;
            if (!(*jump_itr)->exsit)
            {
                delete* jump_itr;
                jump_itr = jump_boards.erase(jump_itr);
            }
            through[5] = true;
        }
    }
}

void back_move()
{
    bool stage_hit = false;
    VECTOR2 stagePos;
    for (auto stage_itr = stage.begin(); stage_itr != stage.end(); stage_itr++)
    {
        if ((*stage_itr)->stage_judge(&player))
        {
            gravity = 0.0f;
            velocity.y = 0.0f;
            stagePos = (*stage_itr)->stage_position();
            if (player.pos.y > stagePos.y)
            {
                velocity.y += player.pos.y - stagePos.y;
            }
            stage_hit = true;
            break;
        }
        else
        {
            gravity -= 0.03f;
        }

    }

    for (auto jump_itr = jump_boards.begin(); jump_itr != jump_boards.end(); jump_itr++)
    {
        if ((*jump_itr)->player_onJumpBoard(&player))
        {
            velocity.y = 20.0f;
        }
    }

    velocity.x -= 0.01f;

    //if (STATE(0) & PAD_RIGHT)
    //    velocity.x = -20.0f;
    //if (STATE(0) & PAD_LEFT)
    //    velocity.x = 20.0f;
    //if (STATE(0) & PAD_UP)
    //    velocity.y = 20.0f;
    //if (STATE(0) & PAD_DOWN)
    //    velocity.y = -20.0f;

    //if (TRG_RELEASE(0) & PAD_RIGHT)
    //    velocity.x = 0.0f;
    //if (TRG_RELEASE(0) & PAD_LEFT)
    //    velocity.x = 0.0f;
    //if (TRG_RELEASE(0) & PAD_UP)
    //    velocity.y = 0.0f;
    //if (TRG_RELEASE(0) & PAD_DOWN)
    //    velocity.y = 0.0f;

    backPos.x += velocity.x;
    backPos.y += velocity.y + gravity;
}

void stage_deinit()
{
    if (stage.empty() == false)
    {
        for (auto stage_itr = stage.begin(); stage_itr != stage.end(); stage_itr++)
        {
            delete* stage_itr;
        }
        stage.clear();
    }

    if (enemies.empty() == false)
    {
        for (auto enemies_itr = enemies.begin(); enemies_itr != enemies.end(); enemies_itr++)
        {
            delete* enemies_itr;
        }
        enemies.clear();
    }

    if (jump_boards.empty() == false)
    {
        for (auto jump_itr = jump_boards.begin(); jump_itr != jump_boards.end(); jump_itr++)
        {
            delete* jump_itr;
        }
        jump_boards.clear();
    }
}

void checkPoint_respawn()
{
    stage_deinit();

    velocity = { 0,0 };
    gravity = 0;
    respawn = false;
    posNum = checkpoint.backNum;
    backPos = { 0,0 };
    velocity.x = -10.0f;
    back_count = checkpoint.backCount;
    next_right = posNum + 3;
    next_up = posNum - 1;
    next_down = posNum + 1;
    for (auto& thr : through)
        thr = 0;
    stage_make(true);

}