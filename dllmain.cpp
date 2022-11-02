#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <stdlib.h>
// Export define
#define RPC_EXPORT __declspec(dllexport)
// discord rpc
#include "discord/discord.h"

HINSTANCE   hInstance   = NULL;
HHOOK       hkb         = NULL;


// DISCORD EX
#define _CRT_SECURE_NO_WARNINGS

#include <array>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>



//Main DLL entrance
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hInstance = hModule;
		break;
    case DLL_PROCESS_DETACH:
        FreeLibrary(hModule);
        break;
    }
    return TRUE;
}

// Run procces. Need for check how works dll update method
enum eRunResult {
    DONE,
    OK
};
// Process state. Needed for initialize
enum eDllState {
    START,
    RUN,
    EXIT
};
// Game state
enum eGameState {
    MENU,
    GAME
};
// Race 
enum eGameRace {
    HUMAN,
    MIXED,
    DEEPONE,
    ABOMINATION
};

// Dll State
int dllState = START;

discord::Core* core{};
discord::Activity activity{};

int what = 1;

// Init discord RPC
void InitRPC()
{
        auto result = discord::Core::Create(1037330520977195018, DiscordCreateFlags_Default, &core);

        //discord::Activity activity{};

        activity.SetType(discord::ActivityType::Playing);
        activity.SetState("");
        activity.SetDetails("In Game");

        activity.GetAssets().SetLargeImage("overkill");
        activity.GetAssets().SetLargeText("LonaRPG");

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
            activity.SetState("hey fuck");
        });

        dllState = RUN;
}

// Test function
RPC_EXPORT int update()
{
    switch(dllState)
    {
    case START:
        InitRPC();
    case RUN:
        core->RunCallbacks();
        //activity.SetState("hey fuck");
    default:
        return DONE;
    }
    return OK;
}

// Change rpc info
/*
RPC_EXPORT void updateStatus(int state, int lvl, int race)
{
    if (state == MENU)
    {
        activity.SetType(discord::ActivityType::Playing);
        activity.SetState("");
        activity.SetDetails("In Menu");

        activity.GetAssets().SetSmallImage("sleep");
        activity.GetAssets().SetSmallText("LonaRPG");
        activity.GetAssets().SetLargeImage("sleep");
        activity.GetAssets().SetLargeText("LonaRPG");

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {

        });
    }
    else if (state == GAME)
    {
        activity.SetType(discord::ActivityType::Playing);
        const char* stateWord = "Level: " + char(lvl) + char(", Race: ");
        switch (race)
        {
        case HUMAN:
            stateWord = stateWord + char("Human");
            break;
        case MIXED:
            stateWord = stateWord + char("Moot");
            break;
        case DEEPONE:
            stateWord = stateWord + char("Deepone");
            break;
        case ABOMINATION:
            stateWord = stateWord + char("Abomination");
            break;
        default:
            stateWord = stateWord + char("Human");
            break;
        }

        activity.SetState(stateWord);
        activity.SetDetails("In Game");

        activity.GetAssets().SetSmallImage("overkill");
        activity.GetAssets().SetSmallText("LonaRPG");
        activity.GetAssets().SetLargeImage("overkill");
        activity.GetAssets().SetLargeText("LonaRPG");

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {

        });
    }
}
*/
// Test function
//RPC_EXPORT int test_value()
//{
//    return 65;
//}
