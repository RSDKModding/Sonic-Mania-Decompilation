#ifndef OBJ_SOUNDBOARD_H
#define OBJ_SOUNDBOARD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSoundboard : Object {
    int sfxCount;
    ushort sfxList[32];
    int sfxLoopPoint[32];
    void *sfxCheckCallback[32];
    void *sfxChangeCallback[32];
    bool32 sfxUnknown4[32];
    int sfxUnknownVal[32];
    int sfxUnknown6[32];
    int sfxUnknown7[32];
    int sfxUnknown8[32];
};

// Entity Class
struct EntitySoundboard : Entity {
    //nothin extra
};

// Object Struct
extern ObjectSoundboard *Soundboard;

// Standard Entity Events
void Soundboard_Update();
void Soundboard_LateUpdate();
void Soundboard_StaticUpdate();
void Soundboard_Draw();
void Soundboard_Create(void* data);
void Soundboard_StageLoad();
void Soundboard_EditorDraw();
void Soundboard_EditorLoad();
void Soundboard_Serialize();

// Extra Entity Functions
byte Soundboard_LoadSFX(const char *sfxName, uint loopPoint, void *checkCallback, void *changeCallback);

#endif //!OBJ_SOUNDBOARD_H
