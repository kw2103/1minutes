#ifndef INCLUDED_UTIL
#define INCLUDED_UTIL

//******************************************************************************
//
//
//      ユーティリティー
//
//
//******************************************************************************

//------< inline function >-----------------------------------------------------

template <typename T>
inline void safe_delete(T*& p)
{
    if (p != nullptr)
    {
        delete (p);
        (p) = nullptr;
    }
}

template <typename T>
inline void safe_delete_array(T*& p)
{
    if (p != nullptr)
    {
        delete[](p);
        (p) = nullptr;
    }
}

template <typename T>
inline void safe_release(T*& p)
{
    if (p != nullptr)
    {
        (p)->Release();
        (p) = nullptr;
    }
}

//******************************************************************************

#endif// !INCLUDED_UTIL
