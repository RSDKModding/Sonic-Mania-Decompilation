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
        char messageText[0x100];

        va_list list;
        va_start(list, message);
        vsprintf(messageText, message, list);
#if RETRO_USE_PLUS
        RSDK.PrintText(PRINT_NORMAL, messageText);
#else
        RSDK.PrintMessage((void *)messageText, 0);
#endif
        va_end(list);
    }
}

#if RETRO_INCLUDE_EDITOR
void LogHelpers_EditorDraw(void) {}

void LogHelpers_EditorLoad(void) {}
#endif

void LogHelpers_Serialize(void) {}
