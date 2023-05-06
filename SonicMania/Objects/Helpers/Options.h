#ifndef OBJ_OPTIONS_H
#define OBJ_OPTIONS_H

#include "Game.h"

// Using a seperate OptionsRAM struct
// Normally (and officially) the EntityOptions struct was used here
// but due to v5U updating the entity (and thus the OptionsRAM "spec")
// EntityOptions is no longer easily compatible across versions
// so I gave it dummy data and will be using this struct to interact with optionsRAM
typedef struct {
    uint8 padding[0x58];

    uint8 language;
    int32 overrideLanguage;
    int32 screenShader;
    bool32 overrideShader;
    int32 volMusic;
    bool32 overrideMusicVol;
    int32 volSfx;
    bool32 overrideSfxVol;
    bool32 vSync;
    uint8 windowSize;
    bool32 windowBorder;
    bool32 windowed;
    bool32 tripleBuffering;
} OptionsRAM;

// Object Class
struct ObjectOptions {
#if !MANIA_USE_PLUS
    RSDK_OBJECT
    Entity *loadEntityPtr;
    void (*loadCallback)(bool32 success);
    Entity *saveEntityPtr;
    void (*saveCallback)(void);
#else
    Entity *loadEntityPtr;
    void (*loadCallback)(bool32 success);
    Entity *saveEntityPtr;
    void (*saveCallback)(bool32 success);
#endif
    bool32 changed;
};

// Entity Class
struct EntityOptions {
    RSDK_ENTITY
    // padding to match whatever it would be normally
    // not required, but its for safety :)
    uint8 padding[sizeof(OptionsRAM) - sizeof(Entity)];
};

// Object Struct
extern ObjectOptions *Options;

// Standard Entity Events
void Options_Update(void);
void Options_LateUpdate(void);
void Options_StaticUpdate(void);
void Options_Draw(void);
void Options_Create(void *data);
void Options_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Options_EditorDraw(void);
void Options_EditorLoad(void);
#endif
void Options_Serialize(void);

// Extra Entity Functions
OptionsRAM *Options_GetOptionsRAM(void);
void Options_Reload(void);
void Options_GetWinSize(void);
void Options_LoadCallback(bool32 success);
void Options_LoadFile(void (*callback)(bool32 success));
#if MANIA_USE_PLUS
void Options_SaveFile(void (*callback)(bool32 success));
#else
void Options_SaveFile(void (*callback)(void));
#endif
void Options_SetLanguage(int32 language);
void Options_LoadValuesFromSettings(OptionsRAM *options);
void Options_LoadOptionsCallback(int32 status);
void Options_SaveOptionsCallback(int32 status);

#endif //! OBJ_OPTIONS_H
