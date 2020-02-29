#ifndef OBJ_SWINGROPE_H
#define OBJ_SWINGROPE_H

//Object Class
class SwingRope : Object {

};

//Entity Class
class EntitySwingRope : Entity {

};

//Entity Functions
void SwingRope_Update();
void SwingRope_EarlyUpdate();
void SwingRope_LateUpdate();
void SwingRope_Draw();
void SwingRope_Setup(void* subtype);
void SwingRope_StageLoad();
void SwingRope_GetAttributes();

#endif //!OBJ_SWINGROPE_H
