#ifndef OBJ_PAINTINGEYES_H
#define OBJ_PAINTINGEYES_H

//Object Class
class PaintingEyes : Object {

};

//Entity Class
class EntityPaintingEyes : Entity {

};

//Entity Functions
void PaintingEyes_Update();
void PaintingEyes_EarlyUpdate();
void PaintingEyes_LateUpdate();
void PaintingEyes_Draw();
void PaintingEyes_Setup(void* subtype);
void PaintingEyes_StageLoad();
void PaintingEyes_GetAttributes();

#endif //!OBJ_PAINTINGEYES_H
