#ifndef OBJ_LAVAGEYSER_H
#define OBJ_LAVAGEYSER_H

//Object Class
class LavaGeyser : Object {

};

//Entity Class
class EntityLavaGeyser : Entity {

};

//Entity Functions
void LavaGeyser_Update();
void LavaGeyser_EarlyUpdate();
void LavaGeyser_LateUpdate();
void LavaGeyser_Draw();
void LavaGeyser_Setup(void* subtype);
void LavaGeyser_StageLoad();
void LavaGeyser_GetAttributes();

#endif //!OBJ_LAVAGEYSER_H
