#ifndef OBJ_RING_H
#define OBJ_RING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRing : Object {
    Hitbox hitbox;
    int pan;
    short spriteIndex;
    short sfx_Ring;
};

// Entity Class
struct EntityRing : Entity {
    void *state;
    void *stateDraw;
    int type;
    int planeFilter;
    int ringAmount;
    int timer;
    int maxFrameCount;
    int sparkleType;
    Entity *storedPlayer;
    int moveType;
    Vector2 amplitude;
    int moveSpeed;
    Vector2 offset;
    EntityAnimationData animData;
};

// Object Entity
extern ObjectRing Ring;

// Standard Entity Events
void Ring_Update();
void Ring_LateUpdate();
void Ring_StaticUpdate();
void Ring_Draw();
void Ring_Create(void* data);
void Ring_StageLoad();
void Ring_EditorDraw();
void Ring_EditorLoad();
void Ring_Serialize();

// Extra Entity Functions


#endif //!OBJ_RING_H
