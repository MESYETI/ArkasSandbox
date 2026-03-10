#include <Arkas/engine.h>
#include "titleScreen.h"

int main(int argc, const char** argv) {
	Engine_Init("Arkas Sandbox", argc, argv);

	SceneManager_AddScene(TitleScreenScene());

	while (engine.running) {
		Engine_Update();
	}
	Engine_Free();
}
