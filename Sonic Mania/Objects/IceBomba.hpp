#ifndef OBJ_ICEBOMBA_H
#define OBJ_ICEBOMBA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectIceBomba : Object{

};

// Entity Class
struct EntityIceBomba : Entity {

};

// Object Struct
extern ObjectIceBomba *IceBomba;

// Standard Entity Events
void IceBomba_Update();
void IceBomba_LateUpdate();
void IceBomba_StaticUpdate();
void IceBomba_Draw();
void IceBomba_Create(void* data);
void IceBomba_StageLoad();
void IceBomba_EditorDraw();
void IceBomba_EditorLoad();
void IceBomba_Serialize();

// Extra Entity Functions


#endif //!OBJ_ICEBOMBA_H
