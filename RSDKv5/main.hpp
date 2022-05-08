#ifndef MAIN_H
#define MAIN_H

#if !RETRO_STANDALONE
#ifdef _MSC_VER
#define DLLExport __declspec(dllexport)
#else
#define DLLExport
#endif

extern "C" {
DLLExport int RSDK_main(int argc, char **argv, void *linkLogicPtr);
}
#else
int RSDK_main(int argc, char **argv, void *linkLogicPtr);
#endif

#endif // !ifdef MAIN_H