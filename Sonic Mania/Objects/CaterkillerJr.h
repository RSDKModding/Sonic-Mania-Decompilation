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
void CaterkillerJr_Update(void);
void CaterkillerJr_LateUpdate(void);
void CaterkillerJr_StaticUpdate(void);
void CaterkillerJr_Draw(void);
void CaterkillerJr_Create(void* data);
void CaterkillerJr_StageLoad(void);
void CaterkillerJr_EditorDraw(void);
void CaterkillerJr_EditorLoad(void);
void CaterkillerJr_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CATERKILLERJR_H
