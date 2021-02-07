#ifndef OBJ_CATERKILLERJR_H
#define OBJ_CATERKILLERJR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCaterkillerJr : Object{

};

// Entity Class
struct EntityCaterkillerJr : Entity {

};

// Object Struct
extern ObjectCaterkillerJr *CaterkillerJr;

// Standard Entity Events
void CaterkillerJr_Update();
void CaterkillerJr_LateUpdate();
void CaterkillerJr_StaticUpdate();
void CaterkillerJr_Draw();
void CaterkillerJr_Create(void* data);
void CaterkillerJr_StageLoad();
void CaterkillerJr_EditorDraw();
void CaterkillerJr_EditorLoad();
void CaterkillerJr_Serialize();

// Extra Entity Functions


#endif //!OBJ_CATERKILLERJR_H
