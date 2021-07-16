#ifndef OBJ_SELTZERBOTTLE_H
#define OBJ_SELTZERBOTTLE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    ushort layerID;
    ushort layerBitFlags;
    ushort aniFrames;
    ushort sfxSpray;
} ObjectSeltzerBottle;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int sprayTime;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
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
void SeltzerBottle_EditorDraw(void);
void SeltzerBottle_EditorLoad(void);
void SeltzerBottle_Serialize(void);

// Extra Entity Functions
void SeltzerBottle_Unknown1(void);
void SeltzerBottle_Unknown2(void);

#endif //!OBJ_SELTZERBOTTLE_H
