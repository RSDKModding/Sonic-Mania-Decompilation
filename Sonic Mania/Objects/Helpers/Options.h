#ifndef OBJ_OPTIONS_H
#define OBJ_OPTIONS_H

#include "SonicMania.h"

// Object Class
typedef struct {
#if !RETRO_USE_PLUS
    RSDK_OBJECT
#endif
    Entity *loadEntityPtr;
    void (*loadCallback)(int);
    Entity *saveEntityPtr;
    void (*saveCallback)(int);
    int state;
} ObjectOptions;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntityOptions;

// Object Struct
extern ObjectOptions *Options;

// Standard Entity Events
void Options_Update(void);
void Options_LateUpdate(void);
void Options_StaticUpdate(void);
void Options_Draw(void);
void Options_Create(void *data);
void Options_StageLoad(void);
void Options_EditorDraw(void);
void Options_EditorLoad(void);
void Options_Serialize(void);

//Funcs
void Options_Reload(void);
void Options_GetWinSize(void);
void Options_LoadCallback(int success);
void Options_LoadOptionsBin(void);
void Options_SaveOptionsBin(void (*callback)(int));
void Options_SetLanguage(int language);
void Options_Unknown1(int *optionsRAM);
void Options_LoadOptionsCallback(int statusCode);
void Options_SaveOptionsCallback(int statusCode);

#endif //!OBJ_OPTIONS_H
