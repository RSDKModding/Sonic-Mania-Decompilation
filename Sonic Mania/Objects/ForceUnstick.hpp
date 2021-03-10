#ifndef OBJ_FORCEUNSTICK_H
#define OBJ_FORCEUNSTICK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectForceUnstick : Object {
    ushort spriteIndex;
    EntityAnimationData animData;
};

// Entity Class
struct EntityForceUnstick : Entity {
    byte width;
    byte height;
    bool32 breakClimb;
    Hitbox hitbox;
};

// Object Struct
extern ObjectForceUnstick *ForceUnstick;

// Standard Entity Events
void ForceUnstick_Update();
void ForceUnstick_LateUpdate();
void ForceUnstick_StaticUpdate();
void ForceUnstick_Draw();
void ForceUnstick_Create(void* data);
void ForceUnstick_StageLoad();
void ForceUnstick_EditorDraw();
void ForceUnstick_EditorLoad();
void ForceUnstick_Serialize();

// Extra Entity Functions
void ForceUnstick_DrawSprites();


#endif //!OBJ_FORCEUNSTICK_H
