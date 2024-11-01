#define MEMORY
#define GUI
#define FEATURES
#include "NullPointer.h"


FeaturesStates Features = { .TriggerBot = 0, .Wallhack = 0, .Bhop = 0, .AntiFlash = 0, .RadarHack = 0, .UNLOAD = FALSE};

DWORD WINAPI ThreadWindowFunc(LPVOID lpParam) {
	showWindow(&Features);
	return 0;
}

int main() {
	HANDLE WINThread;
	DWORD dwThreadId;

	HANDLE driver = loadDriver();
	uintptr_t client = initClient(driver);

	if (client == 0) {
		puts("[-] INIT FAILURE [-]");
		getchar();

		return 1;
	}

	WINThread = CreateThread(NULL, 0, ThreadWindowFunc, NULL, 0, &dwThreadId);
	InitFeatures(&Features);

	while (TRUE) {

		MultiHack(&driver, client);
	}
	
	return 0;
}