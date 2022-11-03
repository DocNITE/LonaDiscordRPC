#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS
#include <array>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
// Windows Header Files
#include <windows.h>
#include <stdlib.h>
// Export define
#define RPC_EXPORT __declspec(dllexport)
// discord rpc
#include "discord/discord.h"

HINSTANCE   hInstance   = NULL;
HHOOK       hkb         = NULL;

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;



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

// Game version
// MAJOR = 741/1000
// MINOR = 741/100
// PATCH = (741/10)%10
// BUILD = 741%10
struct sGameVersion {
    int MAJOR   = 0;
    int MINOR   = 0;
    int PATCH   = 0;
    int BUILD   = 0;
};
sGameVersion gameVersion;

discord::Core* core{};
discord::Activity activity{};

int what = 1;

// Init discord RPC
void InitRPC()
{
        auto result = discord::Core::Create(1037330520977195018, DiscordCreateFlags_Default, &core);

        char versionInfo[256];
        sprintf(versionInfo, "%d.%d.%d.%d", 
            gameVersion.MAJOR, 
            gameVersion.MINOR, 
            gameVersion.PATCH,
            gameVersion.BUILD
        );

        activity.SetType(discord::ActivityType::Playing);
        activity.SetState("In Menu");
        activity.SetDetails("");

        activity.GetAssets().SetLargeImage("sleep");
        activity.GetAssets().SetLargeText(versionInfo);

        auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        activity.GetTimestamps().SetStart(millisec_since_epoch);
        //activity.GetTimestamps().SetEnd(6);

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
            //activity.SetState("hey fuck");
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
    default:
        return DONE;
    }
    return OK;
}

// Change version
RPC_EXPORT void changeVersion(int ver)
{
    gameVersion.MAJOR = ver/1000;
    gameVersion.MINOR = ver/100;
    gameVersion.PATCH = (ver/10)%10;
    gameVersion.BUILD = ver%10;
}

// Change rpc info
RPC_EXPORT void updateStatus(int state, int lvl, int race)
{
    // Check if discord not run
    if (dllState != RUN) return;

    if (state == MENU)
    {
        char versionInfo[256];
        sprintf(versionInfo, "%d.%d.%d.%d", 
            gameVersion.MAJOR, 
            gameVersion.MINOR, 
            gameVersion.PATCH,
            gameVersion.BUILD
        );

        activity.SetType(discord::ActivityType::Playing);
        activity.SetState("");
        activity.SetDetails("In Menu");

        //activity.GetAssets().SetSmallImage("sleep");
        //activity.GetAssets().SetSmallText("LonaRPG");
        activity.GetAssets().SetLargeImage("sleep");
        activity.GetAssets().SetLargeText(versionInfo);

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {

        });
    }
    else if (state == GAME)
    {
        activity.SetType(discord::ActivityType::Playing);
        char stateWord[256];
        switch (race)
        {
        case HUMAN:
            sprintf(stateWord, "Level: %d, Race: Human", lvl);
            activity.GetAssets().SetLargeImage("normal");
            break;
        case MIXED:
            sprintf(stateWord, "Level: %d, Race: Moot", lvl);
            activity.GetAssets().SetLargeImage("normal");
            break;
        case DEEPONE:
            sprintf(stateWord, "Level: %d, Race: Deepone", lvl);
            activity.GetAssets().SetLargeImage("happy");
            break;
        case ABOMINATION:
            sprintf(stateWord, "Level: %d, Race: Abomination", lvl);
            activity.GetAssets().SetLargeImage("overkill");
            break;
        default:
            sprintf(stateWord, "Level: %d, Race: Human", lvl);
            activity.GetAssets().SetLargeImage("normal");
            break;
        }

        char versionInfo[256];
        sprintf(versionInfo, "%d.%d.%d.%d",
            gameVersion.MAJOR,
            gameVersion.MINOR,
            gameVersion.PATCH,
            gameVersion.BUILD
        );

        activity.SetState(stateWord);
        activity.SetDetails("In Game");

        activity.GetAssets().SetLargeText(versionInfo);

        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {

        });
    }
}

// Test function
//RPC_EXPORT int test_value()
//{
//    return 65;
//}
