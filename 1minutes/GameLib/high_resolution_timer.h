#ifndef INCLUDED_HIGH_RESOLUTION_TIMER
#define INCLUDED_HIGH_RESOLUTION_TIMER
//******************************************************************************
//
//
//      高精度タイマー
//
//
//******************************************************************************

#include <Winbase.h>
#include "system.h"

namespace GameLib
{

    //==========================================================================
    //
    //      HighResolutionTimerクラス
    //
    //==========================================================================
    class HighResolutionTimer
    {
    public:

        HighResolutionTimer() : deltaTime(-1.0), pausedTime(0), stopped(false)
        {
            LONGLONG countsPerSec;
            QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
            secondsPerCount = 1.0 / static_cast<double>(countsPerSec);
            framesPerSec = (LONGLONG)(countsPerSec / GameLib::system::FRAME_RATE);

            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));
            baseTime = thisTime;
            lastTime = thisTime;
        }

        // Returns the total time elapsed since Reset() was called, NOT counting any
        // time when the clock is stopped.
        float timeStamp() const  // in seconds
        {
            // If we are stopped, do not count the time that has passed since we stopped.
            // Moreover, if we previously already had a pause, the distance 
            // stop_time - base_time includes paused time, which we do not want to count.
            // To correct this, we can subtract the paused time from mStopTime:  
            //
            //                     |<--paused_time-->|
            // ----*---------------*-----------------*------------*------------*------> time
            //  base_time       stop_time        start_time     stop_time    this_time

            if (stopped)
            {
                return static_cast<float>(((stopTime - pausedTime) - baseTime)*secondsPerCount);
            }

            // The distance this_time - mBaseTime includes paused time,
            // which we do not want to count.  To correct this, we can subtract 
            // the paused time from this_time:  
            //
            //  (this_time - paused_time) - base_time 
            //
            //                     |<--paused_time-->|
            // ----*---------------*-----------------*------------*------> time
            //  base_time       stop_time        start_time     this_time
            else
            {
                return static_cast<float>(((thisTime - pausedTime) - baseTime)*secondsPerCount);
            }
        }

        float timeInterval() const  // in seconds
        {
            return static_cast<float>(deltaTime);
        }

        void reset() // Call before message loop.
        {
            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));
            baseTime = thisTime;
            lastTime = thisTime;

            stopTime = 0;
            stopped = false;
        }

        void start() // Call when unpaused.
        {
            LONGLONG startTime;
            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

            // Accumulate the time elapsed between stop and start pairs.
            //
            //                     |<-------d------->|
            // ----*---------------*-----------------*------------> time
            //  base_time       stop_time        start_time     
            if (stopped)
            {
                pausedTime += (startTime - stopTime);
                lastTime = startTime;
                stopTime = 0;
                stopped = false;
            }
        }

        void stop() // Call when paused.
        {
            if (!stopped)
            {
                QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&stopTime));
                stopped = true;
            }
        }

        bool tick() // Call every frame.
        {
            if (stopped)
            {
                deltaTime = 0.0;
                return true;
            }

            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));

            //フレーム制限
            if (thisTime < lastTime + framesPerSec) return false;

            // Time difference between this frame and the previous.
            deltaTime = (thisTime - lastTime) * secondsPerCount;

            //経過時間
            LONGLONG dTime = thisTime - lastTime;

            // Prepare for next frame.
            lastTime += framesPerSec;

            if (dTime > 40000) lastTime = thisTime;

            // Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
            // processor goes into a power save mode or we get shuffled to another
            // processor, then mDeltaTime can be negative.
            if (deltaTime < 0.0)
            {
                deltaTime = 0.0;
            }

            return true;
        }

        LONGLONG getLastTime() const { return lastTime; }

        void setFrameRate(double frameRate) {
            LONGLONG countsPerSec;
            QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
            secondsPerCount = 1.0 / static_cast<double>(countsPerSec);
            framesPerSec = (LONGLONG)(countsPerSec / frameRate);
        }

    private:

        double secondsPerCount;
        double deltaTime;

        LONGLONG thisTime;
        LONGLONG baseTime;
        LONGLONG pausedTime;
        LONGLONG stopTime = 0;
        LONGLONG lastTime;
        //固定フレーム用
        LONGLONG framesPerSec;

        bool stopped;
    };

}

//******************************************************************************

#endif // !INCLUDED_HIGH_RESOLUTION_TIMER