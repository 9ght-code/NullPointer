#define MEMORY
#define GUI
#define FEATURES
#define ENTITY_STRUCTURE
#define CONFIG

#include "NullPointer.h"

FeaturesStates Features = { 0 };
Entity entities[64] = {0};
static Config data = { .sleepTime = 0, .sleepTriggerTime = 10, .shootInAir = 0};


DWORD WINAPI ThreadWindowFunc(LPVOID lpParam) {
	showWindow(&Features);
	return 0;
}

int main() {
	HANDLE WINThread;
	DWORD dwThreadId;

	HANDLE driver = loadDriver();
	uintptr_t client = initClient(&driver);

	if (client == 0) {
		puts("[-] INIT FAILURE [-]");
		getchar();

		return 1;
	}

	WINThread = CreateThread(NULL, 0, ThreadWindowFunc, NULL, 0, &dwThreadId);
	
	InitPointersFeatures(&Features, &entities, &data);
	InitPointersGUI(&entities, &data);

	while (TRUE) { MultiHack(&driver, client); }
	
	return 0;
}