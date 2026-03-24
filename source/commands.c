#include <Arkas/client.h>
#include <Arkas/console.h>
#include "game.h"
#include "commands.h"

static void JoinLocal(size_t argc, char** argv) {
	Client_StartLocal();
	SceneManager_SchedulePop();
	SceneManager_ScheduleAdd(GameScene());
}

void InitASCommands(void) {
	Console_AddCommand(false, "join-local", &JoinLocal);
}
