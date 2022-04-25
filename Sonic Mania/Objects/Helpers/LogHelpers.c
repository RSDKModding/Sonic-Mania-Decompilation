// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LogHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectLogHelpers *LogHelpers = NULL;

void LogHelpers_Update(void) {}

void LogHelpers_LateUpdate(void) {}

void LogHelpers_StaticUpdate(void) {}

void LogHelpers_Draw(void) {}

void LogHelpers_Create(void *data) {}

void LogHelpers_StageLoad(void) {}

void LogHelpers_Print(const char *message, ...)
{
    if (!SceneInfo->inEditor) {
        char buffer[0x100];

        // make the full string
        va_list list;
        va_start(list, message);
        vsprintf(buffer, message, list);
#if RETRO_USE_PLUS
        RSDK.PrintString(PRINT_NORMAL, buffer);
#else
        RSDK.PrintMessage((void *)buffer, 0);
#endif
        va_end(list);
    }
}

#if RETRO_INCLUDE_EDITOR
void LogHelpers_EditorDraw(void) {}

void LogHelpers_EditorLoad(void) {}
#endif

void LogHelpers_Serialize(void) {}
