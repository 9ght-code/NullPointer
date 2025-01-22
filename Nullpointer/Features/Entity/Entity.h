#pragma once
#include "../../dependencies.h"

Entity InitializeEnemy(PHANDLE driver, uintptr_t listEntry2, int pawnHandle, PViewMatrix4x4 matrix);
Entity InitializePlayer(PHANDLE driver, uintptr_t client);