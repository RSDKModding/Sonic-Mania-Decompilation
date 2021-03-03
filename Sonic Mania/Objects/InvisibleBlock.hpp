#ifndef OBJ_INVISIBLEBLOCK_H
#define OBJ_INVISIBLEBLOCK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectInvisibleBlock : Object {
    ushort spriteIndex;
    EntityAnimationData data;
};

// Entity Class
struct EntityInvisibleBlock : Entity {
    byte width;
    byte height;
    int planeFilter;
    int noCrush;
    bool32 activeNormal;
    bool32 timeAttackOnly;
    bool32 noChibi;
    Hitbox hitbox;
};

// Object Struct
extern ObjectInvisibleBlock *InvisibleBlock;

// Standard Entity Events
void InvisibleBlock_Update();
void InvisibleBlock_LateUpdate();
void InvisibleBlock_StaticUpdate();
void InvisibleBlock_Draw();
void InvisibleBlock_Create(void* data);
void InvisibleBlock_StageLoad();
void InvisibleBlock_EditorDraw();
void InvisibleBlock_EditorLoad();
void InvisibleBlock_Serialize();

// Extra Entity Functions
void InvisibleBlock_DrawSprites();

#endif //!OBJ_INVISIBLEBLOCK_H
