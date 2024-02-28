#include	"AppSceneMediator.h"
#include	"VitalizeFramework.h"

#include	"sceneMain.h"

void AppSceneMediator::changeScene(int id)
{
	switch (id)
	{
	default: case 0: VMF_SCENE_CHANGE(new sceneMain()); break;
	}
	
}

