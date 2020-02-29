#ifndef OBJ_RISINGLAVA_H
#define OBJ_RISINGLAVA_H

//Object Class
class RisingLava : Object {

};

//Entity Class
class EntityRisingLava : Entity {

};

//Entity Functions
void RisingLava_Update();
void RisingLava_EarlyUpdate();
void RisingLava_LateUpdate();
void RisingLava_Draw();
void RisingLava_Setup(void* subtype);
void RisingLava_StageLoad();
void RisingLava_GetAttributes();

#endif //!OBJ_RISINGLAVA_H
