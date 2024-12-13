#ifndef SYSTEM_H_
#define SYSTEM_H_

namespace GameLib
{
    namespace system
    {
        static const float BOX2D_SCALE                  = 100.0f;   // Box2D用
        static const float FRAME_RATE                   = 60.0f;    // フレームレート

        static const int TEXTURE_NUM                    = 256U;     // テクスチャの最大枚数
        static const int DEBUG_INSTANCE_NUM             = 8192U;    // デバッグ文字の最大表示数
        static const int PRIMITIVE_BATCH_INSTANCE_NUM   = 4096U;    // プリミティブバッチの最大インスタンス数
    }
}

#endif//SYSTEM_H_
