#pragma once

//******************************************************************************
//
//
//      ViewSettings
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include "view_settings.h"
#include "vector.h"
#include "game_lib.h"

//#include <Box2D/Box2D.h>

namespace GameLib
{

    //==========================================================================
    //
    //      ViewSettingsクラス
    //
    //==========================================================================
    class ViewSettings
    {
    private:
        float   scaleFactor;    // 画面に描画されるイメージのスケール
        float   box2dScale;     // Box2Dの1mがscaleFactor1.0fの状態で何ピクセルであるか
        VECTOR2 vCenter;        // 画面の中心座標
        VECTOR2 vLeftTop;       // 画面左上の座標
        VECTOR2 vLeftBottom;    // 画面左下の座標

        VECTOR2 vRightTop;       // 画面右上の座標
        VECTOR2 vRightBottom;    // 画面右下の座標

    public:
        ViewSettings();
        ~ViewSettings();

        void initialize();

        void setScaleFactor(float s);
        void setBox2dScale(float b)                 { box2dScale = b; }
        void setViewCenter(VECTOR2);
        void setViewLeftTop(VECTOR2);
        void setViewLeftBottom(VECTOR2);
        void setViewByPosition(VECTOR2, VECTOR2);

        float getScaleFactor()                      { return scaleFactor; }
        float getBox2dScale()                       { return box2dScale; }
        VECTOR2 getViewCenter()                     { return vCenter; }
        VECTOR2 getViewLeftTop()                    { return vLeftTop; }
        VECTOR2 getViewLeftBottom()                 { return vLeftBottom; }
        VECTOR2 getViewRightTop()                   { return vRightTop; }
        VECTOR2 getViewRightBottom()                { return vRightBottom; }

        VECTOR2 worldToScreen(const VECTOR2&);
        VECTOR3 worldToScreen(const VECTOR3&);
        VECTOR2 screenToWorld(VECTOR2);

        //b2Vec2  worldToBox2d(VECTOR2);
        //VECTOR2 box2dToWorld(b2Vec2);
    };
}
