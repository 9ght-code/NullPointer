#define MEMORY
#define GUI
#define FEATURES
#define ENTITY_STRUCTURE
#define CONFIG

#include "NullPointer.h"

static FeaturesStates Features = { 0 };
static Entity entities[64] = {0};
static Config dataNullPointer = { .sleepTime = 10, .sleepTriggerTime = 10, .shootInAir = 0};

static aimKey = VK_LMENU;
char aimKeyName[32] = "LEFT ALT";


DWORD WINAPI ThreadWindowFunc(LPVOID lpParam) {
	showWindow(&Features);
	return 0;
}

int main() {
	HANDLE driver = loadDriver();
	InitializeMemoryReader();

	uintptr_t client = initClient(&driver);

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