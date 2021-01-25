#ifndef OBJ_PIMPOM_H
#define OBJ_PIMPOM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPimPom : Object {

};

// Entity Class
struct EntityPimPom : Entity {

};

// Object Entity
extern ObjectPimPom PimPom;

// Standard Entity Events
void PimPom_Update();
void PimPom_LateUpdate();
void PimPom_StaticUpdate();
void PimPom_Draw();
void PimPom_Create(void* data);
void PimPom_StageLoad();
void PimPom_EditorDraw();
void PimPom_EditorLoad();
void PimPom_Serialize();

// Extra Entity Functions


#endif //!OBJ_PIMPOM_H
