// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LogHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLogHelpers *LogHelpers = NULL;

void LogHelpers_Update(void) {}

void LogHelpers_LateUpdate(void) {}

void LogHelpers_StaticUpdate(void) {}

void LogHelpers_Draw(void) {}

void LogHelpers_Create(void *data) {}

void LogHelpers_StageLoad(void) {}

void LogHelpers_PrintText(const char *message)
{
#if MANIA_USE_PLUS
    if (!SceneInfo->inEditor)
        RSDK.PrintText(PRINT_NORMAL, message);
#else
    LogHelpers_Print("%s", message);
#endif
}
#if MANIA_USE_PLUS
void LogHelpers_PrintString(String *message)
{
    if (!SceneInfo->inEditor)
        RSDK.PrintString(PRINT_NORMAL, message);
}
#endif
void LogHelpers_PrintInt32(const char *message, int32 value)
{
#if MANIA_USE_PLUS
    if (!SceneInfo->inEditor)
        RSDK.PrintInt32(PRINT_NORMAL, message, value);
#else
    LogHelpers_Print("%s: %d", message, value);
#endif
}

void LogHelpers_Print(const char *message, ...)
{
    if (!SceneInfo->inEditor) {
        char messageText[0x100];

        va_list args;
        va_start(args, message);
        vsprintf(messageText, message, args);

#if MANIA_USE_PLUS
        RSDK.PrintText(PRINT_NORMAL, messageText);
#else
        int32 len            = (int32)strlen(messageText);
        messageText[len]     = '\n';
        messageText[len + 1] = 0;

        RSDK.PrintMessage((void *)messageText, MESSAGE_STRING);
#endif

        va_end(args);
    }
}

#if MANIA_USE_PLUS
void LogHelpers_PrintVector2(const char *message, Vector2 value)
{
    if (!SceneInfo->inEditor)
        RSDK.PrintVector2(PRINT_NORMAL, message, value);
}
#endif

#if GAME_INCLUDE_EDITOR
void LogHelpers_EditorDraw(void) {}

void LogHelpers_EditorLoad(void) {}
#endif

void LogHelpers_Serialize(void) {}
