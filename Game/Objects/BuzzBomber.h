#ifndef OBJ_BUZZBOMBER_H
#define OBJ_BUZZBOMBER_H

//Object Class
class BuzzBomber : Object {

};

//Entity Class
class EntityBuzzBomber : Entity {

};

//Entity Functions
void BuzzBomber_Update();
void BuzzBomber_EarlyUpdate();
void BuzzBomber_LateUpdate();
void BuzzBomber_Draw();
void BuzzBomber_Setup(void* subtype);
void BuzzBomber_StageLoad();
void BuzzBomber_GetAttributes();

#endif //!OBJ_BUZZBOMBER_H
