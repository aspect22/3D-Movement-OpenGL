#include "debug.h"

// debug functions

DWORD WINAPI debug(void* data) {
    while (true) {
        if (GetAsyncKeyState(VK_F1) & 1) {
            debugtoggle = !debugtoggle;
            if (debugtoggle)
                printf("Debug mode enabled\n");
            else
                printf("Debug mode disabled\n");
        }

        switch (debugtoggle) {
        case true:
            // Debug functions in here
            break;
        case false:
            break;
        }
    }
    return 0;
}