#ifndef OBJ_INVINCIBLESTARS_H
#define OBJ_INVINCIBLESTARS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectInvincibleStars : Object {
    ushort spriteIndex;
};

// Entity Class
struct EntityInvincibleStars : Entity {
    Entity *parent;
    Vector2 starPos[8];
    int starAngle[4];
    int starOffset;
    EntityAnimationData starsData[4];
};

// Object Struct
extern ObjectInvincibleStars *InvincibleStars;

// Standard Entity Events
void InvincibleStars_Update();
void InvincibleStars_LateUpdate();
void InvincibleStars_StaticUpdate();
void InvincibleStars_Draw();
void InvincibleStars_Create(void* data);
void InvincibleStars_StageLoad();
void InvincibleStars_EditorDraw();
void InvincibleStars_EditorLoad();
void InvincibleStars_Serialize();

// Extra Entity Functions


#endif //!OBJ_INVINCIBLESTARS_H
