#include "memory.h"
#include "Features.h"
#include "GUI.h"

FeaturesStates Features = { .TriggerBot = 0, .Wallhack = 0, .Bhop = 0, .AntiFlash = 0, .UNLOAD = FALSE};

DWORD WINAPI ThreadWindowFunc(LPVOID lpParam) {
	showWindow(&Features);
	return 0;
}

int main() {
	HANDLE WINThread;
	DWORD dwThreadId;

	WINThread = CreateThread(NULL, 0, ThreadWindowFunc, NULL, 0, &dwThreadId);

	HANDLE driver = loadDriver();

	uintptr_t client = initClient(driver);

	if (client == 0) {
		puts("[-] INIT FAILURE [-]");
		getchar();

		return 1;
	}

	while (TRUE) {

		if (Features.TriggerBot == 1)
			TriggerBot(driver, client);

		if (Features.AntiFlash == 1)
			AntiFlashBang(driver, client);

		if (Features.Wallhack == 1)
			GlowWallHack(driver, client);

		if (Features.Bhop == 1)
			Bhop(driver, client);

		if (Features.UNLOAD)
			UnloadDriver(driver);
		
	}
	
	return 0;
}