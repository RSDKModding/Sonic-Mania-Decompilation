#ifndef OBJ_COLLAPSINGPLATFORM_H
#define OBJ_COLLAPSINGPLATFORM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCollapsingPlatform : Object {
    byte shift;
    EntityAnimationData animData;
    ushort spriteIndex;
    ushort sfx_Crumble;
};

// Entity Class
struct EntityCollapsingPlatform : Entity {
    void (*state)(void);
    Vector2 size;
    bool32 respawn;
    ushort targetLayer;
    byte type;
    int delay;
    bool32 eventOnly;
    bool32 mightyOnly;
    int field_78;
    int collapseDelay;
    ushort storedTiles[256];
    Hitbox hitbox;
    Vector2 playerPos;
};

// Object Struct
extern ObjectCollapsingPlatform *CollapsingPlatform;

// Standard Entity Events
void CollapsingPlatform_Update();
void CollapsingPlatform_LateUpdate();
void CollapsingPlatform_StaticUpdate();
void CollapsingPlatform_Draw();
void CollapsingPlatform_Create(void* data);
void CollapsingPlatform_StageLoad();
void CollapsingPlatform_EditorDraw();
void CollapsingPlatform_EditorLoad();
void CollapsingPlatform_Serialize();

// Extra Entity Functions
void CollapsingPlatform_State_Left();
void CollapsingPlatform_State_Right();
void CollapsingPlatform_State_Center();
void CollapsingPlatform_State_LeftOrRight();
void CollapsingPlatform_State_PlayerPos();

#endif //!OBJ_COLLAPSINGPLATFORM_H
