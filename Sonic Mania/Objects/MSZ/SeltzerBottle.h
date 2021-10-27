#ifndef OBJ_SELTZERBOTTLE_H
#define OBJ_SELTZERBOTTLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 layerID;
    uint16 layerBitFlags;
    uint16 aniFrames;
    uint16 sfxSpray;
} ObjectSeltzerBottle;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 sprayTime;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntitySeltzerBottle;

// Object Struct
extern ObjectSeltzerBottle *SeltzerBottle;

// Standard Entity Events
void SeltzerBottle_Update(void);
void SeltzerBottle_LateUpdate(void);
void SeltzerBottle_StaticUpdate(void);
void SeltzerBottle_Draw(void);
void SeltzerBottle_Create(void* data);
void SeltzerBottle_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SeltzerBottle_EditorDraw(void);
void SeltzerBottle_EditorLoad(void);
#endif
void SeltzerBottle_Serialize(void);

// Extra Entity Functions
void SeltzerBottle_Unknown1(void);
void SeltzerBottle_Unknown2(void);

#endif //!OBJ_SELTZERBOTTLE_H
