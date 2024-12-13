#ifndef INCLUDED_FRAME_RATE
#define INCLUDED_FRAME_RATE
//******************************************************************************
//
//
//      FrameRate
//
//
//******************************************************************************

//------< インクルード >----------------------------------------------------------
#include <Windows.h>
#include "high_resolution_timer.h"

namespace GameLib
{
    void calculateFrameStats(HWND, HighResolutionTimer*);
}

#endif // !INCLUDED_FRAME_RATE