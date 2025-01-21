#ifndef CHECKPOINT_H
#define CHECKPOINT_H


#include "All.h"


class CheckPoint
{
public:
	VECTOR2 backPos;
	int backCount;
	int backNum;
	bool through[4];
	void init()
	{
		backPos = { 0,0 };
		for (auto& thr : through)
		{
			thr = false;
		}
	}

};

#endif // !CHECKPOINT_H