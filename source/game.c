#include <Arkas/map.h>
#include <Arkas/client.h>
#include <Arkas/server.h>
#include <Arkas/engine.h>
#include "game.h"

static void Render(Scene* scene) {
	(void) scene;

	if (!map.active) {
		const char* text = "Loading map...";

		int w = strlen(text) * engine.font.charWidth;
		Text_Render(
			&engine.font, text, (video.width / 2) - (w / 2),
			(video.height / 2) - (engine.font.charHeight / 2)
		);
	}
}

Scene GameScene(void) {
	return (Scene) {
		.type        = SCENE_TYPE_GAME,
		.init        = NULL,
		.free        = NULL,
		.handleEvent = NULL,
		.update      = NULL,
		.render      = &Render
	};
}

void StartLocalGame(bool inet) {
	SceneManager_ScheduleAdd(GameScene());

	// setup server
	serverConf.inet     = inet;
	serverConf.inetPort = 16709; // 0x4145, or 0x41 0x45, or "AE"
	serverConf.local    = true;
	Server_Start();

	// setup client
	Client_StartLocal();
}
