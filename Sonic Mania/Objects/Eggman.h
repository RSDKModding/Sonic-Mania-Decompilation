#ifndef OBJ_EGGMAN_H
#define OBJ_EGGMAN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectEggman;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    void(*stateStore)();
    int timer;
    ushort animID;
    Entity* parent;
    Vector2 offset;
    Hitbox hitbox;
    AnimationData data;
} EntityEggman;

// Object Struct
extern ObjectEggman *Eggman;

// Standard Entity Events
void Eggman_Update();
void Eggman_LateUpdate();
void Eggman_StaticUpdate();
void Eggman_Draw();
void Eggman_Create(void* data);
void Eggman_StageLoad();
void Eggman_EditorDraw();
void Eggman_EditorLoad();
void Eggman_Serialize();

// Extra Entity Functions
void Eggman_Unknown1();
void Eggman_Unknown2();
void Eggman_Unknown3();
void Eggman_Unknown4();
void Eggman_Unknown5();
void Eggman_Unknown6();
void Eggman_Unknown7();

#endif //!OBJ_EGGMAN_H
