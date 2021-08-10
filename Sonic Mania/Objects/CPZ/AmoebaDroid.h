#ifndef OBJ_AMOEBADROID_H
#define OBJ_AMOEBADROID_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int debrisInfo1[17]; //= { 4, 0, 0, -131072, -163840, 1, 0, 131072, -163840, 2, 0, -163840, -131072, 3, 0, 163840, -131072 };
    int debrisInfo2[13]; //= { 3, 4, 0, 0, -131072, 5, 0, -163840, -131072, 5, 1, 163840, -131072 };
    int arenaLeft;
    int arenaRight;
    int startX;
    int arenaTop;
    int arenaBottom;
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
    int invincibleTimer;
    int health;
    int partAngle2;
    int partAngle;
    int partOffset;
    int partPos;
    Vector2 offsetPos;
    Entity *parts[8];
    Entity *parent;
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
void AmoebaDroid_HandleDropletMovement(void);
void AmoebaDroid_HandleDropletRelease(bool32 interact);

void AmoebaDroid_CheckHit(void);
void AmoebaDroid_CheckPlayerHit(void);

void AmoebaDroid_StateDrawMain_Unknown1(void);
void AmoebaDroid_StateDraw1_Unknown1(void);

void AmoebaDroid_StateMain_Setup(void);
void AmoebaDroid_StateMain_SetupWaterLevel(void);
void AmoebaDroid_StateMain_Unknown1(void);
void AmoebaDroid_StateMain_Unknown2(void);
void AmoebaDroid_StateMain_Unknown3(void);
void AmoebaDroid_StateMain_Unknown4(void);
void AmoebaDroid_StateMain_Unknown5(void);
void AmoebaDroid_StateMain_Unknown6(void);
void AmoebaDroid_StateMain_Unknown7(void);
void AmoebaDroid_StateMain_Unknown8(void);
void AmoebaDroid_StateMain_Unknown9(void);
void AmoebaDroid_StateMain_Unknown10(void);
void AmoebaDroid_State1_Unknown1(void);
void AmoebaDroid_State2_Unknown1(void);
void AmoebaDroid_State2_Unknown2(void);
void AmoebaDroid_State4_Unknown1(void);
void AmoebaDroid_State3_Unknown1(void);
void AmoebaDroid_StateMain_Death(void);
void AmoebaDroid_StateMain_SpawnSignpost(void);

#endif //!OBJ_AMOEBADROID_H
