#include <Arkas/ui.h>
#include <Arkas/map.h>
#include <Arkas/mem.h>
#include <Arkas/camera.h>
#include <Arkas/engine.h>
#include <Arkas/skybox.h>
#include <Arkas/backend.h>
#include <Arkas/ui/label.h>
#include <Arkas/ui/button.h>
#include <Arkas/resources.h>
#include "titleScreen.h"

static void Init(Scene* scene) {
	UI_ManagerInit(&scene->ui, 1);

	UI_Container* container = UI_ManagerAddContainer(&scene->ui, 120);
	UI_ContainerAlignLeft(container, 8);
	UI_ContainerCenterY(container);
	UI_ContainerSetPadding(container, 5, 5, 5, 5);

	UI_Row* row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewLabel(&engine.font, "Arkas Sandbox", 0));
	UI_RowFinish(row, true);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Singleplayer", false, NULL));
	UI_RowFinish(row, true);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Multiplayer", false, NULL));
	UI_RowFinish(row, true);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Map Editor", false, NULL));
	UI_RowFinish(row, true);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Options", false, NULL));
	UI_RowFinish(row, true);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Quit", false, NULL));
	UI_RowFinish(row, true);

	// init map
	Map_Init();
	map.name      = NewString("as_titlescreen");
	map.points    = SafeMalloc(4 * sizeof(MapPoint));
	map.pointsLen = 4;

	map.points[0] = (MapPoint) {{-100.0, -100.0}};
	map.points[1] = (MapPoint) {{ 100.0, -100.0}};
	map.points[2] = (MapPoint) {{ 100.0,  100.0}};
	map.points[3] = (MapPoint) {{-100.0,  100.0}};

	map.walls    = SafeMalloc(4 * sizeof(Wall));
	map.wallsLen = 4;
	map.walls[0] = (Wall) {true, false, 0, NULL};
	map.walls[1] = (Wall) {true, false, 0, NULL};
	map.walls[2] = (Wall) {true, false, 0, NULL};
	map.walls[3] = (Wall) {true, false, 0, NULL};

	map.sectors    = SafeMalloc(1 * sizeof(Sector));
	map.sectorsLen = 1;

	map.sectors[0] = (Sector) {
		0, 4, 0.5, -0.5, (FVec2) {0.0, 0.0}, (FVec2) {0.0, 0.0}, false, true,
		Resources_GetRes(":base/3p_textures/grass2.png", 0), NULL
	};

	// load skybox
	Skybox_Load(":base/skyboxes/05");

	camera.pos    = (FVec3) {0.0, 0.0, 0.0};
	camera.pitch  = 0.0;
	camera.yaw    = -90.0;
	camera.roll   = 0.0;
	camera.sector = &map.sectors[0];
}

static void Free(Scene* scene) {
	Map_Free();
	UI_ManagerFree(&scene->ui);
}

static bool HandleEvent(Scene* scene, Event* e) {
	return UI_ManagerHandleEvent(&scene->ui, e);
}

static void Update(Scene* scene, bool top) {
	(void) scene;
	(void) top;
}

static void Render(Scene* scene) {
	Backend_RenderScene();
	Backend_Begin2D();
	UI_ManagerRender(&scene->ui);
}

Scene TitleScreenScene(void) {
	return (Scene) {
		.type        = SCENE_TYPE_OTHER,
		.name        = "Title Screen",
		.init        = &Init,
		.free        = &Free,
		.handleEvent = &HandleEvent,
		.update      = &Update,
		.render      = &Render
	};
}
