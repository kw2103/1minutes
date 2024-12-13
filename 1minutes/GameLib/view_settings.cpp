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

//******************************************************************************
//
//      ViewSettings
//
//******************************************************************************

namespace GameLib
{

    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    ViewSettings::ViewSettings()
    {
        initialize();
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    ViewSettings::~ViewSettings()
    {
    }

    //--------------------------------------------------------------
    //  初期設定
    //--------------------------------------------------------------
    void ViewSettings::initialize()
    {
        scaleFactor = 1.0f;
        box2dScale = system::BOX2D_SCALE;
        setViewLeftTop(VECTOR2(0, 0));
    }

    //--------------------------------------------------------------
    //  スケールを設定
    //--------------------------------------------------------------
    void ViewSettings::setScaleFactor(float s)
    {
        scaleFactor = s;
    }

    //--------------------------------------------------------------
    //  画面中央のワールド座標を設定
    //--------------------------------------------------------------
    void ViewSettings::setViewCenter(VECTOR2 c)
    {
        vCenter = c;
#ifdef GAMELIB_PLUS_UP
        vLeftTop = vCenter + VECTOR2(-window::getWidth() / 2, window::getHeight() / 2) / scaleFactor;
        vLeftBottom = vCenter + VECTOR2(-window::getWidth() / 2, -window::getHeight() / 2) / scaleFactor;

        vRightTop = vCenter + VECTOR2(window::getWidth() / 2, window::getHeight() / 2) / scaleFactor;
        vRightBottom = vCenter + VECTOR2(window::getWidth() / 2, -window::getHeight() / 2) / scaleFactor;
#else
        vLeftTop = vCenter + VECTOR2(-window::getWidth() / 2, -window::getHeight() / 2) / scaleFactor;
        vLeftBottom = vCenter + VECTOR2(-window::getWidth() / 2, window::getHeight() / 2) / scaleFactor;

        vRightTop = vCenter + VECTOR2(window::getWidth() / 2, -window::getHeight() / 2) / scaleFactor;
        vRightBottom = vCenter + VECTOR2(window::getWidth() / 2, window::getHeight() / 2) / scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  画面左上のワールド座標を設定
    //--------------------------------------------------------------
    void ViewSettings::setViewLeftTop(VECTOR2 lt)
    {
        vLeftTop = lt;
#ifdef GAMELIB_PLUS_UP
        vCenter = vLeftTop + VECTOR2(window::getWidth() / 2, -window::getHeight() / 2) / scaleFactor;
        vLeftBottom = vLeftTop + VECTOR2(0, -window::getHeight()) / scaleFactor;

        vRightTop = vLeftTop + VECTOR2(window::getWidth(), 0) / scaleFactor;
        vRightBottom = vLeftTop + VECTOR2(window::getWidth(), -window::getHeight()) / scaleFactor;
#else
        vCenter = vLeftTop + VECTOR2(window::getWidth() / 2, window::getHeight() / 2) / scaleFactor;
        vLeftBottom = vLeftTop + VECTOR2(0, window::getHeight()) / scaleFactor;

        vRightTop = vLeftTop + VECTOR2(window::getWidth(), 0) / scaleFactor;
        vRightBottom = vLeftTop + VECTOR2(window::getWidth(), window::getHeight()) / scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  画面左下のワールド座標を設定
    //--------------------------------------------------------------
    void ViewSettings::setViewLeftBottom(VECTOR2 lb)
    {
        vLeftBottom = lb;
#ifdef GAMELIB_PLUS_UP
        vCenter = vLeftBottom + VECTOR2(window::getWidth() / 2, window::getHeight() / 2) / scaleFactor;
        vLeftTop = vLeftBottom + VECTOR2(0, window::getHeight()) / scaleFactor;

        vRightTop = vLeftBottom + VECTOR2(window::getWidth(), window::getHeight()) / scaleFactor;
        vRightBottom = vLeftBottom + VECTOR2(window::getWidth(), 0) / scaleFactor;
#else
        vCenter = vLeftBottom + VECTOR2(window::getWidth() / 2, -window::getHeight() / 2) / scaleFactor;
        vLeftTop = vLeftBottom + VECTOR2(0, -window::getHeight()) / scaleFactor;

        vRightTop = vLeftBottom + VECTOR2(window::getWidth(), -window::getHeight()) / scaleFactor;
        vRightBottom = vLeftBottom + VECTOR2(window::getWidth(), 0) / scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  画面内の任意の座標のワールド座標を設定
    //--------------------------------------------------------------
    void ViewSettings::setViewByPosition(VECTOR2 screenPos, VECTOR2 worldPos)
    {
#ifdef GAMELIB_PLUS_UP
        screenPos.y = window::getHeight() - screenPos.y;
        VECTOR2 lb = worldPos - screenPos / scaleFactor;
        setViewLeftBottom(lb);
#else
        VECTOR2 lt = worldPos - screenPos / scaleFactor;
        setViewLeftTop(lt);
#endif
    }

    //--------------------------------------------------------------
    //  ワールド座標をスクリーン座標に変換
    //--------------------------------------------------------------
    VECTOR2 ViewSettings::worldToScreen(const VECTOR2& wPos)
    {
#ifdef GAMELIB_PLUS_UP
        return (wPos - vLeftBottom) * scaleFactor;
#else
        return (wPos - vLeftTop) * scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  ワールド座標をスクリーン座標に変換（VECTOR3）
    //--------------------------------------------------------------
    VECTOR3 ViewSettings::worldToScreen(const VECTOR3& wPos)
    {
#ifdef GAMELIB_PLUS_UP
        return (wPos - vLeftBottom) * scaleFactor;
#else
        return (wPos - vLeftTop) * scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  スクリーン座標をワールド座標に変換
    //--------------------------------------------------------------
    VECTOR2 ViewSettings::screenToWorld(VECTOR2 sPos)
    {
#ifdef GAMELIB_PLUS_UP
        sPos.y = window::getHeight() - sPos.y;
        return vLeftBottom + sPos / scaleFactor;
#else
        return vLeftTop + sPos / scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  ワールド座標をBox2D座標に変換
    //--------------------------------------------------------------
    //b2Vec2  ViewSettings::worldToBox2d(VECTOR2 wPos)
    //{
    //    return b2Vec2(wPos.x / box2dScale, wPos.y / box2dScale);
    //}

    //--------------------------------------------------------------
    //  Box2D座標をワールド座標に変換
    //--------------------------------------------------------------
    //VECTOR2 ViewSettings::box2dToWorld(b2Vec2 b2Pos)
    //{
    //    return VECTOR2(b2Pos.x * box2dScale, b2Pos.y * box2dScale);
    //}

}
