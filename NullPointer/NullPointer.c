#include <stdio.h>
#include <Windows.h>
#include "../Driver/IO_Operations.h"
#include "memory.h"
#include "NullPointer.h"
#include "Offsets.h"
#include "Features.h"


int main() {
	boolean triggerBotEnabled = FALSE;
	boolean AntiFlashEnabled = FALSE;
	boolean WallHackEnabled = FALSE;
	boolean BhopEnabled = FALSE;

	HANDLE driver = loadDriver();

	uintptr_t client = initClient(driver);

	if (client == 0) {
		puts("[-] INIT FAILURE [-]");
		getchar();

		return 1;
	}

	puts("Press F1 to activate rge TriggerBot feature\nPress F2 to activate the Anti-Flash feature");
	puts("Press F3 to activate the WallHack Feature\nPress F4 to activate the Bhop feature\nPress F5 to close the cheat\nPress F6 to unload the driver");


	while (TRUE) {

		if (GetAsyncKeyState(VK_F6)) {
			UnloadDriver(driver);
			system("cls");

			puts("[+] Driver Unloaded [+]");
			getchar();

			exit(0);
		}

		if (GetAsyncKeyState(VK_F5))
			exit(0);

		if (GetAsyncKeyState(VK_F1)) {
			triggerBotEnabled = !triggerBotEnabled;
			Beep(500, 800);
		}

		if (GetAsyncKeyState(VK_F2)) {
			AntiFlashEnabled = !AntiFlashEnabled;
			Beep(250, 800);
		}

		if (GetAsyncKeyState(VK_F3)) {
			WallHackEnabled = !WallHackEnabled;
			Beep(300, 800);
		}


		if (GetAsyncKeyState(VK_F4)) {
			BhopEnabled = !BhopEnabled;
			Beep(700, 800);
		}

		if (triggerBotEnabled)
			TriggerBot(driver, client);

		if (AntiFlashEnabled)
			AntiFlashBang(driver, client);

		if (WallHackEnabled)
			GlowWallHack(driver, client);

		if (BhopEnabled)
			Bhop(driver, client);
		
	}
	
	return 0;
}