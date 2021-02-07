#ifndef OBJ_AQUIS_H
#define OBJ_AQUIS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAquis : Object{

};

// Entity Class
struct EntityAquis : Entity {

};

// Object Struct
extern ObjectAquis *Aquis;

// Standard Entity Events
void Aquis_Update();
void Aquis_LateUpdate();
void Aquis_StaticUpdate();
void Aquis_Draw();
void Aquis_Create(void* data);
void Aquis_StageLoad();
void Aquis_EditorDraw();
void Aquis_EditorLoad();
void Aquis_Serialize();

// Extra Entity Functions


#endif //!OBJ_AQUIS_H
