#include "RetroEngine.hpp"
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
int main(int argc, char *argv[]) { return RSDK_main(argc, argv, RSDK::LinkGameLogic); }
#endif

int RSDK_main(int argc, char *argv[], linkPtr linkLogicPtr)
{
#ifdef __SWITCH__
    initNxLink();
#endif

    linkGameLogic = linkLogicPtr;

    parseArguments(argc, argv);
    if (initRetroEngine()) {
        runRetroEngine();
    }

#ifdef __SWITCH__
    socketExit();
#endif

    return 0;
}

int RSDK_main(int argc, char **argv, void (*ptr)(void*)) {
    return RSDK_main(argc, argv, (linkPtr)ptr); //switch won't link w/o this
}
