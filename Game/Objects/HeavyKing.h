#ifndef OBJ_HEAVYKING_H
#define OBJ_HEAVYKING_H

//Object Class
class HeavyKing : Object {

};

//Entity Class
class EntityHeavyKing : Entity {

};

//Entity Functions
void HeavyKing_Update();
void HeavyKing_EarlyUpdate();
void HeavyKing_LateUpdate();
void HeavyKing_Draw();
void HeavyKing_Setup(void* subtype);
void HeavyKing_StageLoad();
void HeavyKing_GetAttributes();

#endif //!OBJ_HEAVYKING_H
