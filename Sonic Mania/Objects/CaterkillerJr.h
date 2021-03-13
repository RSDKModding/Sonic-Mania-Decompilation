#ifndef OBJ_CATERKILLERJR_H
#define OBJ_CATERKILLERJR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCaterkillerJr;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCaterkillerJr;

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
