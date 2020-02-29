#ifndef OBJ_SILVERSONIC_H
#define OBJ_SILVERSONIC_H

//Object Class
class SilverSonic : Object {

};

//Entity Class
class EntitySilverSonic : Entity {

};

//Entity Functions
void SilverSonic_Update();
void SilverSonic_EarlyUpdate();
void SilverSonic_LateUpdate();
void SilverSonic_Draw();
void SilverSonic_Setup(void* subtype);
void SilverSonic_StageLoad();
void SilverSonic_GetAttributes();

#endif //!OBJ_SILVERSONIC_H
