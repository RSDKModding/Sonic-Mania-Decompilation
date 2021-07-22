#ifndef OBJ_AMOEBADROID_H
#define OBJ_AMOEBADROID_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4[17]; //= { 4, 0, 0, -131072, -163840, 1, 0, 131072, -163840, 2, 0, -163840, -131072, 3, 0, 163840, -131072 };
    int field_48[13]; //= { 3, 4, 0, 0, -131072, 5, 0, -163840, -131072, 5, 1, 163840, -131072 };
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    ushort sfxHit;
    ushort sfxExplosion;
    ushort sfxGather;
    ushort sfxBounce;
    ushort sfxRelease;
    ushort aniFrames;
    ushort waterFrames;
} ObjectAmoebaDroid;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int type;
    int timer;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    Entity *field_88[8];
    int field_A8;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Hitbox hitbox;
} EntityAmoebaDroid;

// Object Struct
extern ObjectAmoebaDroid *AmoebaDroid;

// Standard Entity Events
void AmoebaDroid_Update(void);
void AmoebaDroid_LateUpdate(void);
void AmoebaDroid_StaticUpdate(void);
void AmoebaDroid_Draw(void);
void AmoebaDroid_Create(void* data);
void AmoebaDroid_StageLoad(void);
void AmoebaDroid_EditorDraw(void);
void AmoebaDroid_EditorLoad(void);
void AmoebaDroid_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_AMOEBADROID_H
