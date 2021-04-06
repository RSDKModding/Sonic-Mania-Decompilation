#include "RetroEngine.hpp"

int main(int argc, char *argv[])
{
    parseArguments(argc, argv);
    if (initRetroEngine()) {
        runRetroEngine();
    }

	return 0;
}
