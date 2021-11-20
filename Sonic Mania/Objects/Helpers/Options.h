#ifndef OBJ_OPTIONS_H
#define OBJ_OPTIONS_H

#include "SonicMania.h"

// Object Class
typedef struct {
#if !RETRO_USE_PLUS
    RSDK_OBJECT
#endif
    Entity *loadEntityPtr;
    void (*loadCallback)(int32);
    Entity *saveEntityPtr;
    void (*saveCallback)(int32);
    int32 state;
} ObjectOptions;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 language;
    int32 overrideLanguage;
    int32 screenShader;
    bool32 overrideShader;
    int32 volMusic;
    bool32 overrideMusicVol;
    int32 volSfx;
    bool32 overrideSfxVol;
    int32 vSync;
    uint8 windowSize;
    int32 windowBorder;
    int32 windowed;
    int32 tripleBuffering;
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
#if RETRO_INCLUDE_EDITOR
void Options_EditorDraw(void);
void Options_EditorLoad(void);
#endif
void Options_Serialize(void);

//Funcs
void Options_Reload(void);
void Options_GetWinSize(void);
void Options_LoadCallback(int32 success);
void Options_LoadOptionsBin(void);
void Options_SaveOptionsBin(void (*callback)(int32));
void Options_SetLanguage(int32 language);
void Options_LoadValuesFromSettings(EntityOptions *options);
void Options_LoadOptionsCallback(int32 statusCode);
void Options_SaveOptionsCallback(int32 statusCode);

#endif //!OBJ_OPTIONS_H
