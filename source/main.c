#include <Arkas/engine.h>
#include <Arkas/splashScreen.h>
#include "commands.h"
#include "titleScreen.h"

static void SplashCallback(void) {
	SceneManager_PopScene();
	SceneManager_AddScene(TitleScreenScene());
}

int main(int argc, const char** argv) {
	Engine_Init("Arkas Sandbox", argc, argv);
	InitASCommands();

	// SceneManager_AddScene(
	// 	NewSplashScreen("sandbox:splash.png", &SplashCallback, 2.0)
	// );
	if (!engine.server) {
		SceneManager_AddScene(TitleScreenScene());
	}

	while (engine.running) {
		Engine_Update();
	}
	Engine_Free();
}
