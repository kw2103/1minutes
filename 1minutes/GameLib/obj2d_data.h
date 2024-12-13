#ifndef INCLUDED_OBJ2D_DATA
#define INCLUDED_OBJ2D_DATA

//******************************************************************************
//
//
//      OBJ2D_Data
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include "sprite.h"

namespace GameLib
{

    //==========================================================================
    //
    //      SpriteData
    //
    //==========================================================================
    struct SpriteData
    {
        int     texNo;              // テクスチャのインデックス
        float   left, top;          // 画像切り出し位置（左、上）
        float   width, height;      // 画像サイズ
        float   centerX, centerY;   // 画像の中心

        void draw(
            const VECTOR2&,
            const VECTOR2& scale = (VECTOR2(1, 1)),
            float angle = (0.0f),
            const VECTOR4& color = (VECTOR4(1, 1, 1, 1)), 
            float z = (0.5f), 
            bool world = (false));

        void draw(
            float x, float y,
            float scaleX = (1.0f), float scaleY = (1.0f),
            float angle = (0.0f),
            float r = (1.0f), float g = (1.0f), float b = (1.0f), float a = (1.0f), 
            float z = (0.5f), 
            bool world = (false)
            );
    };

    //==========================================================================
    //
    //      AnimeData
    //
    //==========================================================================
    struct AnimeData
    {
        SpriteData* data;   // スプライトデータ
        int         frame;  // 表示時間（フレーム数）

        AnimeData(SpriteData* d, int f)
            :data(d)
            , frame(f) {}
    };

    // アニメーション用ワーク
    struct Anime
    {
        int        frame;    // 表示時間（フレーム数）
        int        patNum;   // パターン番号
        AnimeData* pPrev;    // 前回のアニメーションデータ

        Anime()
            :frame(0)
            , patNum(0)
            , pPrev(nullptr) {}
    };

    //==========================================================================
    //
    //      fRECT
    //
    //==========================================================================
    struct fRECT
    {
        float left;
        float top;
        float right;
        float bottom;

        fRECT() : left(0), top(0), right(0), bottom(0) {}
        fRECT(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b) {}
    };

}

#endif// ! INCLUDED_OBJ2D_DATA
