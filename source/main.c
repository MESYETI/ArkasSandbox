#include <Arkas/engine.h>
#include "titleScreen.h"

int main(void) {
	Engine_Init("Arkas Sandbox");

	SceneManager_AddScene(TitleScreenScene());

	while (engine.running) {
		Engine_Update();
	}
	Engine_Free();
}
