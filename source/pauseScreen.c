#include <Arkas/engine.h>
#include <Arkas/backend.h>
#include <Arkas/ui/label.h>
#include <Arkas/ui/button.h>
#include "titleScreen.h"
#include "pauseScreen.h"

static void ResumeButton(UI_Button* this, uint8_t button) {
	(void) this;
	if (button != 0) return;

	SceneManager_SchedulePop();
}

static void QuitButton(UI_Button* this, uint8_t button) {
	(void) this;
	if (button != 0) return;

	engine.running = false;
}

static void Init(Scene* scene) {
	scene->ui = UI_ManagerInit(1);

	UI_Container* container = UI_ManagerAddContainer(scene->ui, 240, NULL);
	UI_ContainerCenterX(container);
	UI_ContainerCenterY(container);
	UI_ContainerSetPadding(container, 5, 5, 5, 5);

	UI_RowUpdate(UI_ContainerAddSingleElemRow(container, 0,
		UI_NewLabel(&engine.font, "Game paused", UI_LABEL_CENTERED)
	));
	UI_RowUpdate(UI_ContainerAddSingleElemRow(container, 0,
		UI_NewButton("Resume", false, &ResumeButton)
	));
	UI_RowUpdate(UI_ContainerAddSingleElemRow(container, 0,
		UI_NewButton("Return to titlescreen", false, NULL)
	));
	UI_RowUpdate(UI_ContainerAddSingleElemRow(container, 0,
		UI_NewButton("Quit", false, &QuitButton)
	));
}

static void Free(Scene* scene) {
	UI_ManagerFree(scene->ui);
}

static bool HandleEvent(Scene* scene, Event* e) {
	UI_ManagerHandleEvent(scene->ui, e);
	return true;
}

static void Render(Scene* scene) {
	(void) scene;

	Backend_EnableAlpha(true);
	Backend_RenderRect(
		(Rect) {0, 0, video.width, video.height}, (Colour) {0, 0, 0, 128}
	);
	UI_ManagerRender(scene->ui);
}

Scene PauseScreenScene(void) {
	return (Scene) {
		.type        = SCENE_TYPE_OTHER,
		.init        = &Init,
		.free        = &Free,
		.handleEvent = &HandleEvent,
		.render      = &Render
	};
}
