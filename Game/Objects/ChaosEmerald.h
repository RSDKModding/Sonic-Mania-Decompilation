#ifndef OBJ_CHAOSEMERALD_H
#define OBJ_CHAOSEMERALD_H

//Object Class
class ChaosEmerald : Object {

};

//Entity Class
class EntityChaosEmerald : Entity {

};

//Entity Functions
void ChaosEmerald_Update();
void ChaosEmerald_EarlyUpdate();
void ChaosEmerald_LateUpdate();
void ChaosEmerald_Draw();
void ChaosEmerald_Setup(void* subtype);
void ChaosEmerald_StageLoad();
void ChaosEmerald_GetAttributes();

#endif //!OBJ_CHAOSEMERALD_H
