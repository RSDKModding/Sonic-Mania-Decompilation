#include "RetroEngine.hpp"

int main(int argc, char *argv[])
{
    parseArguments(argc, argv);
    initRetroEngine();
    runRetroEngine();

	return 0;
}
