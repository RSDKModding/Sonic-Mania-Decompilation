#ifndef OBJ_MSHOLOGRAM_H
#define OBJ_MSHOLOGRAM_H

//Object Class
class MSHologram : Object {

};

//Entity Class
class EntityMSHologram : Entity {

};

//Entity Functions
void MSHologram_Update();
void MSHologram_EarlyUpdate();
void MSHologram_LateUpdate();
void MSHologram_Draw();
void MSHologram_Setup(void* subtype);
void MSHologram_StageLoad();
void MSHologram_GetAttributes();

#endif //!OBJ_MSHOLOGRAM_H
