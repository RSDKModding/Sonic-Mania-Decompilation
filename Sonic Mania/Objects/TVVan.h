#ifndef OBJ_TVVAN_H
#define OBJ_TVVAN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTVVan;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTVVan;

// Object Struct
extern ObjectTVVan *TVVan;

// Standard Entity Events
void TVVan_Update(void);
void TVVan_LateUpdate(void);
void TVVan_StaticUpdate(void);
void TVVan_Draw(void);
void TVVan_Create(void* data);
void TVVan_StageLoad(void);
void TVVan_EditorDraw(void);
void TVVan_EditorLoad(void);
void TVVan_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TVVAN_H
