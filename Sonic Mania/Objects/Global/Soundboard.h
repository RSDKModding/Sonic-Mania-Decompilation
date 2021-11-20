#ifndef OBJ_SOUNDBOARD_H
#define OBJ_SOUNDBOARD_H

#include "SonicMania.h"

#define Soundboard_SFXLimit 32

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 sfxCount;
    uint16 sfxList[Soundboard_SFXLimit];
    int32 sfxLoopPoint[Soundboard_SFXLimit];
    bool32 (*sfxCheckCallback[Soundboard_SFXLimit])(void);
    void (*sfxUpdateCallback[Soundboard_SFXLimit])(int32);
    bool32 sfxIsPlaying[Soundboard_SFXLimit];
    int32 sfxChannel[Soundboard_SFXLimit];
    int32 sfxPlayingTimer[Soundboard_SFXLimit];
    int32 sfxFadeOutTimer[Soundboard_SFXLimit];
    int32 sfxFadeOutDuration[Soundboard_SFXLimit];
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
#if RETRO_INCLUDE_EDITOR
void Soundboard_EditorDraw(void);
void Soundboard_EditorLoad(void);
#endif
void Soundboard_Serialize(void);

// Extra Entity Functions
uint8 Soundboard_LoadSFX(const char *sfxName, uint32 loopPoint, bool32 (*checkCallback)(void), void (*updateCallback)(int32));

#endif //!OBJ_SOUNDBOARD_H
