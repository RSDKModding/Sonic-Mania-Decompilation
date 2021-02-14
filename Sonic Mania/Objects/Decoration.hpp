#ifndef OBJ_DECORATION_H
#define OBJ_DECORATION_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDecoration : Object{
    ushort spriteIndex;
};

// Entity Class
struct EntityDecoration : Entity {
    EntityAnimationData data;
    byte type;
    int rotSpeed;
    Vector2 repeatTimes;
    Vector2 repeatSpacing;
    bool32 tmzFlag;
};

// Object Struct
extern ObjectDecoration *Decoration;

// Standard Entity Events
void Decoration_Update();
void Decoration_LateUpdate();
void Decoration_StaticUpdate();
void Decoration_Draw();
void Decoration_Create(void* data);
void Decoration_StageLoad();
void Decoration_EditorDraw();
void Decoration_EditorLoad();
void Decoration_Serialize();

// Extra Entity Functions
void Decoration_DrawSprite();

#endif //!OBJ_DECORATION_H
