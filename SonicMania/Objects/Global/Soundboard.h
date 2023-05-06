#ifndef OBJ_SOUNDBOARD_H
#define OBJ_SOUNDBOARD_H

#include "Game.h"

#define SOUNDBOARD_SFX_COUNT (32)

// Object Class
struct ObjectSoundboard {
    RSDK_OBJECT
    int32 sfxCount;
    uint16 sfxList[SOUNDBOARD_SFX_COUNT];
    int32 sfxLoopPoint[SOUNDBOARD_SFX_COUNT];
    bool32 (*sfxCheckCallback[SOUNDBOARD_SFX_COUNT])(void);
    void (*sfxUpdateCallback[SOUNDBOARD_SFX_COUNT])(int32 sfxID);
    bool32 sfxIsPlaying[SOUNDBOARD_SFX_COUNT];
    int32 sfxChannel[SOUNDBOARD_SFX_COUNT];
    int32 sfxPlayingTimer[SOUNDBOARD_SFX_COUNT];
    int32 sfxFadeOutTimer[SOUNDBOARD_SFX_COUNT];
    int32 sfxFadeOutDuration[SOUNDBOARD_SFX_COUNT];
};

// Entity Class
struct EntitySoundboard {
    RSDK_ENTITY
};

// Object Struct
extern ObjectSoundboard *Soundboard;

// Standard Entity Events
void Soundboard_Update(void);
void Soundboard_LateUpdate(void);
void Soundboard_StaticUpdate(void);
void Soundboard_Draw(void);
void Soundboard_Create(void *data);
void Soundboard_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Soundboard_EditorDraw(void);
void Soundboard_EditorLoad(void);
#endif
void Soundboard_Serialize(void);

// Extra Entity Functions
uint8 Soundboard_LoadSfx(const char *sfxName, uint32 loopPoint, bool32 (*checkCallback)(void), void (*updateCallback)(int32 sfxID));

#endif //! OBJ_SOUNDBOARD_H
