#ifndef OBJ_HOTARUMKII_H
#define OBJ_HOTARUMKII_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxAppear;
    uint16 sfxLaser;
    uint16 sfxFly;
    uint16 sfxCharge;
} ObjectHotaruMKII;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    int32 field_6C;
    int32 field_70;
    uint8 field_74;
    uint8 field_75;
    uint8 childCount;
    Vector2 distance;
    uint8 origin;
    Vector2 offset1;
    Vector2 offset2;
    Vector2 offset3;
    int32 offsetID;
    Entity *playerPtr;
    int32 type;
    Vector2 triggerSize;
    Hitbox hitboxTrigger;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityHotaruMKII;

// Object Struct
extern ObjectHotaruMKII *HotaruMKII;

// Standard Entity Events
void HotaruMKII_Update(void);
void HotaruMKII_LateUpdate(void);
void HotaruMKII_StaticUpdate(void);
void HotaruMKII_Draw(void);
void HotaruMKII_Create(void* data);
void HotaruMKII_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HotaruMKII_EditorDraw(void);
void HotaruMKII_EditorLoad(void);
#endif
void HotaruMKII_Serialize(void);

// Extra Entity Functions
void HotaruMKII_DebugSpawn(void);
void HotaruMKII_DebugDraw(void);

void HotaruMKII_CheckPlayerCollisions(void);
void HotaruMKII_CheckOnScreen(void);
void HotaruMKII_HandleDistances(void *p);

void HotaruMKII_State_Setup(void);
void HotaruMKII_State_Unknown1(void);
void HotaruMKII_State_Unknown6(void);
void HotaruMKII_State_Unknown2(void);
void HotaruMKII_State_Unknown3(void);
void HotaruMKII_State_Unknown4(void);
void HotaruMKII_State_Unknown5(void);

void HotaruMKII_State1_Unknown(void);

void HotaruMKII_State2_Unknown1(void);
void HotaruMKII_State2_Unknown2(void);

#endif //!OBJ_HOTARUMKII_H
