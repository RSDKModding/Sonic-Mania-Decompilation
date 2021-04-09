#ifndef OBJ_ITEMBOX_H
#define OBJ_ITEMBOX_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox hiddenHitbox;
    int brokenFrame;
    ushort spriteIndex;
    ushort sfx_Destroy;
    ushort sfx_Teleport;
    ushort sfx_HyperRing;
    ushort sfx_PowerDown;
    ushort sfx_Revovery;
} ObjectItemBox;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    Vector2 contentsPos;
    Vector2 unknownPos;
    int contentsSpeed;
    Entity *storedEntity;
    int timer;
    bool32 isFalling;
    bool32 isContents;
    bool32 hidden;
    int planeFilter;
    bool32 lrzConvPhys;
    Animator brokenData;
    Animator contentsData;
    Animator overlayData;
    Animator debrisData;
    Entity *parent;
} EntityItemBox;

// Object Struct
extern ObjectItemBox *ItemBox;

// Standard Entity Events
void ItemBox_Update(void);
void ItemBox_LateUpdate(void);
void ItemBox_StaticUpdate(void);
void ItemBox_Draw(void);
void ItemBox_Create(void* data);
void ItemBox_StageLoad(void);
void ItemBox_EditorDraw(void);
void ItemBox_EditorLoad(void);
void ItemBox_Serialize(void);

// Extra Entity Functions
void ItemBox_DebugDraw(void);
void ItemBox_DebugSpawn(void);
void ItemBox_CheckHit(void);
void ItemBox_GivePowerup(void);
void ItemBox_Break(EntityItemBox *itemBox, void *p);
bool32 ItemBox_HandleFallingCollision(void);
bool32 ItemBox_HandlePlatformCollision(void *p);
void ItemBox_HandleObjectCollisions(void);

//Entity States
void ItemBox_State_Broken(void);
void ItemBox_State_Contents(void);
void ItemBox_State_Unknown(void);
void ItemBox_State_Normal(void);
void ItemBox_State_Falling(void);
void ItemBox_State_Conveyor(void);

#endif //!OBJ_ITEMBOX_H
