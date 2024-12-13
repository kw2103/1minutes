//******************************************************************************
//
//
//      OBJ2D Data
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include "obj2d_data.h"
#include "game_lib.h"

namespace GameLib
{

    //--------------------------------------------------------------
    //  スプライトデータの描画
    //--------------------------------------------------------------
    void SpriteData::draw(float x, float y, float scaleX, float scaleY, float angle,
        float r, float g, float b, float a, float z, bool world)
    {
        texture::begin(texNo);
        texture::draw(texNo,
            x, y, scaleX, scaleY,
            left, top, width, height,
            centerX, centerY, angle,
            r, g, b, a, 
            z, 
            world);
        texture::end(texNo);
    }

    //--------------------------------------------------------------
    //  スプライトデータの描画
    //--------------------------------------------------------------
    void SpriteData::draw(const VECTOR2& position, const VECTOR2& scale, float angle,
        const VECTOR4& color, float z, bool world)
    {
        texture::begin(texNo);
        texture::draw(texNo,
            position.x, position.y, scale.x, scale.y,
            left, top, width, height,
            centerX, centerY, angle,
            color.x, color.y, color.z, color.w, 
            z, 
            world);
        texture::end(texNo);
    }

}
