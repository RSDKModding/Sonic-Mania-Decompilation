#ifndef OBJ_REXON_H
#define OBJ_REXON_H

//Object Class
class Rexon : Object {

};

//Entity Class
class EntityRexon : Entity {

};

//Entity Functions
void Rexon_Update();
void Rexon_EarlyUpdate();
void Rexon_LateUpdate();
void Rexon_Draw();
void Rexon_Setup(void* subtype);
void Rexon_StageLoad();
void Rexon_GetAttributes();

#endif //!OBJ_REXON_H
