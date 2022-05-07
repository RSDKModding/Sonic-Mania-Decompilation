#include "RSDK/Core/RetroEngine.hpp"
#include "main.hpp"

#ifdef __SWITCH__
#include <switch.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <unistd.h>

static int s_nxlinkSock = -1;

static void initNxLink()
{
    if (R_FAILED(socketInitializeDefault()))
        return;

    s_nxlinkSock = nxlinkStdio();
    if (s_nxlinkSock >= 0)
        printf("printf output now goes to nxlink server\n");
    else
        socketExit();
}
#endif

#if RETRO_STANDALONE

#if RETRO_RENDERDEVICE_DIRECTX9
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nShowCmd)
{
    RenderDevice::hInstance     = hInstance;
    RenderDevice::hPrevInstance = hPrevInstance;
    RenderDevice::nShowCmd      = nShowCmd;

    return RSDK_main(1, &lpCmdLine, RSDK::LinkGameLogic);
}
#else
int main(int argc, char *argv[]) { return RSDK_main(argc, argv, RSDK::LinkGameLogic); }
#endif

#endif

int RSDK_main(int argc, char **argv, void (*linkLogicPtr)(void *))
{
#ifdef __SWITCH__
    initNxLink();
#endif

    linkGameLogic = (linkPtr)linkLogicPtr;

    int exitCode = RunRetroEngine(argc, argv);

#ifdef __SWITCH__
    socketExit();
#endif

    return exitCode;
}
