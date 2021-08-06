#ifndef OBJ_SOUNDBOARD_H
#define OBJ_SOUNDBOARD_H

#include "SonicMania.h"

#define Soundboard_SFXLimit 32

// Object Class
typedef struct {
    RSDK_OBJECT
    int sfxCount;
    ushort sfxList[Soundboard_SFXLimit];
    int sfxLoopPoint[Soundboard_SFXLimit];
    bool32 (*sfxCheckCallback[Soundboard_SFXLimit])(void);
    void (*sfxUpdateCallback[Soundboard_SFXLimit])(int);
    bool32 sfxUnknown4[Soundboard_SFXLimit];
    int sfxChannel[Soundboard_SFXLimit];
    int sfxUnknown6[Soundboard_SFXLimit];
    int sfxUnknown7[Soundboard_SFXLimit];
    int sfxUnknown8[Soundboard_SFXLimit];
} ObjectSoundboard;

// Entity Class
typedef struct {
    RSDK_ENTITY
} EntitySoundboard;

// Object Struct
extern ObjectSoundboard *Soundboard;

// Standard Entity Events
void Soundboard_Update(void);
void Soundboard_LateUpdate(void);
void Soundboard_StaticUpdate(void);
void Soundboard_Draw(void);
void Soundboard_Create(void* data);
void Soundboard_StageLoad(void);
void Soundboard_EditorDraw(void);
void Soundboard_EditorLoad(void);
void Soundboard_Serialize(void);

// Extra Entity Functions
byte Soundboard_LoadSFX(const char *sfxName, uint loopPoint, bool32 (*checkCallback)(void), void (*updateCallback)(int));

#endif //!OBJ_SOUNDBOARD_H
