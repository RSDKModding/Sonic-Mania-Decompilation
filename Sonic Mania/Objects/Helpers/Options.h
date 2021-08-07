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
    byte language;
    int overrideLanguage;
    int screenShader;
    int field_60;
    int volMusic;
    int field_68;
    int volSfx;
    int field_70;
    int vSync;
    byte windowSize;
    int windowBorder;
    int windowed;
    int tripleBuffering;
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
void Options_Unknown1(EntityOptions *options);
void Options_LoadOptionsCallback(int statusCode);
void Options_SaveOptionsCallback(int statusCode);

#endif //!OBJ_OPTIONS_H
