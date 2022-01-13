#ifndef MAIN_HPP
#define MAIN_HPP

#if !RETRO_STANDALONE
#ifdef _MSC_VER
#define DLLExport __declspec(dllexport)
#else
#define DLLExport
#endif

extern "C" {
DLLExport int RSDK_main(int argc, char **argv, void (*linkLogicPtr)(RSDK::GameInfo *info));
}
#else
int RSDK_main(int argc, char **argv, void (*linkLogicPtr)(RSDK::GameInfo *info));
#endif

#endif