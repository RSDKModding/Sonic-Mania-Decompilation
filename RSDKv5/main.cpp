#include "RetroEngine.hpp"
#include "main.hpp"

#if RETRO_STANDALONE
int main(int argc, char *argv[]) { return RSDK_main(argc, argv, RSDK::LinkGameLogic); }
#endif

int RSDK_main(int argc, char *argv[], linkPtr linkLogicPtr)
{
    linkGameLogic = linkLogicPtr;

    parseArguments(argc, argv);
    if (initRetroEngine()) {
        runRetroEngine();
    }

	return 0;
}
