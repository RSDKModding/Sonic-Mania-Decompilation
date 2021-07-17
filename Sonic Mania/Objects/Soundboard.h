#ifndef OBJ_SOUNDBOARD_H
#define OBJ_SOUNDBOARD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int sfxCount;
    ushort sfxList[32];
    int sfxLoopPoint[32];
    void *sfxCheckCallback[32];
    void *sfxUpdateCallback[32];
    bool32 sfxUnknown4[32];
    int sfxChannel[32];
    int sfxUnknown6[32];
    int sfxUnknown7[32];
    int sfxUnknown8[32];
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
byte Soundboard_LoadSFX(const char *sfxName, uint loopPoint, void *checkCallback, void *changeCallback);

#endif //!OBJ_SOUNDBOARD_H
