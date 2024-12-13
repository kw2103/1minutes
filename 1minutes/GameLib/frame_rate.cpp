//******************************************************************************
//
//
//		フレームレート用
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include "frame_rate.h"
#include <sstream>

namespace GameLib
{

    //--------------------------------------------------------------
    //  FPS計算
    //--------------------------------------------------------------
    void calculateFrameStats(HWND hWnd, HighResolutionTimer *timer)
    {
        // Code computes the average frames per second, and also the 
        // average time it takes to render one frame.  These stats 
        // are appended to the window caption bar.
        static int frames           = 0;
        static float timeElapsed    = 0.0f;

        frames++;

        // Compute averages over one second period.
        if ((timer->timeStamp() - timeElapsed) >= 1.0f)
        {
            float fps = static_cast<float>(frames); // fps = frameCnt / 1
            float mspf = 1000.0f / fps;
            std::ostringstream outs;
            outs.precision(6);
            outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
            SetWindowTextA(hWnd, outs.str().c_str());

            // Reset for next average.
            frames = 0;
            timeElapsed += 1.0f;
        }
    }

}

//******************************************************************************