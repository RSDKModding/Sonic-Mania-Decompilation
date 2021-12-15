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
        va_list args;
        va_start(args, message);
        vsprintf(buffer, message, args);
#if RETRO_USE_PLUS
        RSDK.PrintString(PRINT_NORMAL, buffer);
#else
        RSDK.PrintMessage((void *)buffer, 0);
#endif
    }
}

void LogHelpers_PrintString(TextInfo *text)
{
    char buffer[0x100];
    RSDK.GetCString(buffer, text);
    LogHelpers_Print(buffer);
}

#if RETRO_INCLUDE_EDITOR
void LogHelpers_EditorDraw(void) {}

void LogHelpers_EditorLoad(void) {}
#endif

void LogHelpers_Serialize(void) {}
