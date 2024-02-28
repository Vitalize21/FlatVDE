#pragma once

#include "GlobalInclude.h"

namespace GlobalField
{
	void Initialize()
	{
		SCREEN_MANAGER.SystemInit();
		TOUCH_CONTROLER.Begining();
	}

	void Release()
	{
		SCREEN_MANAGER.SystemRelease();
	}

}
