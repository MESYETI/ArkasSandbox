#include <Arkas/ui.h>
#include <Arkas/map.h>
#include <Arkas/mem.h>
#include <Arkas/audio.h>
#include <Arkas/camera.h>
#include <Arkas/engine.h>
#include <Arkas/skybox.h>
#include <Arkas/backend.h>
#include <Arkas/ui/label.h>
#include <Arkas/ui/button.h>
#include <Arkas/ui/listBox.h>
#include <Arkas/ui/checkBox.h>
#include <Arkas/resources.h>
#include "titleScreen.h"

static bool          menuOpen;
static Scene*        scene;
static UI_Container* menuCon;

static Vec2 MenuResizer(UI_Container* container) {
	(void) container;

	return (Vec2) {
		video.width - (128 + 8) - 8, video.height - 16
	};
}

static void OpenMenu(void) {
	if (menuOpen) {
		UI_ClearContainer(menuCon);
		return;
	}

	menuOpen = true;
	int x    = 128 + 8;

	menuCon = UI_ManagerAddContainer(
		scene->ui, video.width - x - 8, &MenuResizer
	);
	UI_ContainerAlignLeft(menuCon, x);
	UI_ContainerAlignTop(menuCon, 8);
	menuCon->fixedHeight = video.height - 16;
	UI_ContainerSetPadding(menuCon, 5, 5, 5, 5);
}

static const char*     mapSelection;
static bool            inetServer;
static UI_ListBoxItem* mapList;
static ResourceFile*   maps;
static size_t          mapsSize;

static void PlayButton(UI_Button* this, uint8_t button) {
	(void) this;
	if (button != 0) return;

	OpenMenu();

	UI_Row* row = UI_ContainerAddRow(menuCon, 0);
	UI_RowAddElement(row, UI_NewLabel(&engine.font, "Select map", 0));
	UI_RowUpdate(row);

	row = UI_ContainerAddRow(menuCon, 0);
	UI_RowUpdate(row);

	row = UI_ContainerAddRow(menuCon, 0);
	UI_RowAddElement(row, UI_NewCheckBox(&inetServer));
	UI_RowAddElement(row, UI_NewLabel(&engine.font, "Open server to internet", 0));
	UI_RowUpdate(row);

	row = UI_ContainerAddRow(menuCon, 0);
	UI_RowAddElement(row, UI_NewButton("Start Game", false, NULL));
	UI_RowUpdate(row);

	UI_Row* selectRow = &menuCon->rows[1];
	selectRow->height = menuCon->fixedHeight - UI_ContainerTotalRowHeight(menuCon);
	selectRow->autoHeight = false;
	printf("Row height: %d\n", selectRow->height);

	maps    = Resources_List("maps:", &mapsSize);
	mapList = SafeMalloc(mapsSize * sizeof(UI_ListBoxItem));

	for (size_t i = 0; i < mapsSize; ++ i) {
		mapList[i] = (UI_ListBoxItem) {BaseName(maps[i].fullPath)};
	}

	UI_RowAddElement(selectRow, UI_NewListBox(mapList, mapsSize, &mapSelection, 0, NULL));
	UI_RowAddElement(selectRow, UI_NewScrollBar(40));
	UI_RowUpdate(selectRow);

	UI_ContainerUpdateRowY(menuCon);
}

static void Init(Scene* p_scene) {
	scene     = p_scene;
	scene->ui = UI_ManagerInit(2);

	UI_Container* container = UI_ManagerAddContainer(scene->ui, 120, NULL);
	UI_ContainerAlignLeft(container, 8);
	UI_ContainerCenterY(container);
	UI_ContainerSetPadding(container, 5, 5, 5, 5);

	UI_Row* row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewLabel(&engine.font, "Arkas Sandbox", 0));
	UI_RowUpdate(row);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Play", false, &PlayButton));
	UI_RowUpdate(row);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Join Game", false, NULL));
	UI_RowUpdate(row);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Map Editor", false, NULL));
	UI_RowUpdate(row);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Options", false, NULL));
	UI_RowUpdate(row);

	row = UI_ContainerAddRow(container, 0);
	UI_RowAddElement(row, UI_NewButton("Quit", false, NULL));
	UI_RowUpdate(row);

	// init map
	Map_Init();
	map.name      = NewString("as_titlescreen");
	map.points    = SafeMalloc(4 * sizeof(MapPoint));
	map.pointsLen = 4;

	map.points[0] = (MapPoint) {{-100.0,  100.0}};
	map.points[1] = (MapPoint) {{ 100.0,  100.0}};
	map.points[2] = (MapPoint) {{ 100.0, -100.0}};
	map.points[3] = (MapPoint) {{-100.0, -100.0}};

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
		Resources_GetRes("base:3p_textures/grass2.png", 0), NULL
	};

	// load skybox
	Skybox_Load("base:skyboxes/05");

	camera.pos    = (FVec3) {0.0, 0.0, 0.0};
	camera.pitch  = 0.0;
	camera.yaw    = -90.0;
	camera.roll   = 0.0;
	camera.sector = &map.sectors[0];

	Audio_StartAudio();
	if (Audio_PlayMusic("freight:planet-nitron.ogg", false)) {
		Log("Playing title screen music");
	}
	else {
		Log("Failed to load title screen music");
	}

	menuOpen = false;
}

static void Free(Scene* scene) {
	Audio_StopAudio();
	Map_Free();
	UI_ManagerFree(scene->ui);

	Resources_FreeFileList(maps, mapsSize);
	free(mapList);
}

static bool HandleEvent(Scene* scene, Event* e) {
	return UI_ManagerHandleEvent(scene->ui, e);
}

static void Update(Scene* scene, bool top) {
	(void) scene;
	(void) top;

	map.sectors[0].floorTexOff.x += engine.delta;

	Audio_DefaultState();
}

static void Render(Scene* scene) {
	Backend_RenderScene();
	Backend_Begin2D();
	UI_ManagerRender(scene->ui);
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
