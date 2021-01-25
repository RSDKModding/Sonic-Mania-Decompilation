#ifndef OBJ_SWINGROPE_H
#define OBJ_SWINGROPE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSwingRope : Object {

};

// Entity Class
struct EntitySwingRope : Entity {

};

// Object Entity
extern ObjectSwingRope SwingRope;

// Standard Entity Events
void SwingRope_Update();
void SwingRope_LateUpdate();
void SwingRope_StaticUpdate();
void SwingRope_Draw();
void SwingRope_Create(void* data);
void SwingRope_StageLoad();
void SwingRope_EditorDraw();
void SwingRope_EditorLoad();
void SwingRope_Serialize();

// Extra Entity Functions


#endif //!OBJ_SWINGROPE_H
