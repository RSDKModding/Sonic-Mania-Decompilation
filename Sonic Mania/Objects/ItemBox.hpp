#ifndef OBJ_ITEMBOX_H
#define OBJ_ITEMBOX_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectItemBox : Object {
    Hitbox hitbox;
    Hitbox hiddenHitbox;
    int brokenFrame;
    ushort spriteIndex;
    ushort sfx_Destroy;
    ushort sfx_Teleport;
    ushort sfx_HyperRing;
    ushort sfx_PowerDown;
    ushort sfx_Revovery;
};

// Entity Class
struct EntityItemBox : Entity {
    void (*state)();
    int type;
    Vector2 contentsPos;
    int field_68;
    int field_6C;
    int contentsSpeed;
    Entity *storedEntity;
    int randomValue;
    bool32 isFalling;
    bool32 isContents;
    bool32 hidden;
    int planeFilter;
    bool32 lrzConvPhys;
    EntityAnimationData brokenData;
    EntityAnimationData contentsData;
    EntityAnimationData overlayData;
    EntityAnimationData debrisData;
    Entity *parent;
};

// Object Struct
extern ObjectItemBox *ItemBox;

// Standard Entity Events
void ItemBox_Update();
void ItemBox_LateUpdate();
void ItemBox_StaticUpdate();
void ItemBox_Draw();
void ItemBox_Create(void* data);
void ItemBox_StageLoad();
void ItemBox_EditorDraw();
void ItemBox_EditorLoad();
void ItemBox_Serialize();

// Extra Entity Functions
void ItemBox_DebugDraw();
void ItemBox_DebugSpawn();

//Entity States
void ItemBox_State_Broken();
void ItemBox_State_Contents();
void ItemBox_Unknown5();
void Itembox_State_Normal();

#endif //!OBJ_ITEMBOX_H
