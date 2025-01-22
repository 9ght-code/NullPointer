#define MEMORY
#define GUI
#define FEATURES
#define ENTITY_STRUCTURE
#define CONFIG
#define UTILS
#include "NullPointer.h"

static FeaturesStates Features = { 0 };
static Entity entities[64] = {0};
static Config dataNullPointer = { .sleepTime = 10, .sleepTriggerTime = 10, .shootInAir = 0,
	.keyEntry.keys[0].keyID = NULL, .keyEntry.keys[0].name = "None",
	.keyEntry.keys[1].keyID = VK_RBUTTON, .keyEntry.keys[1].name = "Right Mouse Button",
	.keyEntry.keys[2].keyID = VK_LBUTTON, .keyEntry.keys[2].name = "Left Mouse Button",
	.keyEntry.keys[3].keyID = 0x56, .keyEntry.keys[3].name = "V",
	.keyEntry.keys[4].keyID = 0x58, .keyEntry.keys[4].name = "X",
	.keyEntry.keys[5].keyID = VK_LMENU, .keyEntry.keys[5].name = "Left Alt",
	.keyEntry.keys[6].keyID = VK_LSHIFT, .keyEntry.keys[6].name = "Shift",
	.keyEntry.selectedKeyAim = 5

};


DWORD WINAPI ThreadWindowFunc(LPVOID lpParam) {
	showWindow(&Features);
	return 0;
}

int main() {
	HANDLE driver = loadDriver();
	uintptr_t client = initClient(&driver);

	InitializeMemoryReader();

	if (client == 0) {
		puts("[-] INIT FAILURE [-]");
		getchar();

		return 2;
	}

	CreateThread(NULL, 0, ThreadWindowFunc, NULL, 0, NULL);
	
	InitPointersFeatures(&Features, &entities, &dataNullPointer);
	InitPointersGUI(&entities, &dataNullPointer);

	while (TRUE) { MultiHack(&driver, client); }
	
	return 0;
}