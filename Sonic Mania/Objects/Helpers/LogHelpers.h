#ifndef OBJ_LOGHELPERS_H
#define OBJ_LOGHELPERS_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
} ObjectLogHelpers;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityLogHelpers;

// Object Entity
extern ObjectLogHelpers *LogHelpers;

// Standard Entity Events
void LogHelpers_Update(void);
void LogHelpers_LateUpdate(void);
void LogHelpers_StaticUpdate(void);
void LogHelpers_Draw(void);
void LogHelpers_Create(void *data);
void LogHelpers_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LogHelpers_EditorDraw(void);
void LogHelpers_EditorLoad(void);
#endif
void LogHelpers_Serialize(void);

// Extra Entity Functions
void LogHelpers_Print(const char *message, ...);
void LogHelpers_PrintString(TextInfo *text);

#endif //! OBJ_LOGHELPERS_H
