#ifndef OBJ_LAVAGEYSER_H
#define OBJ_LAVAGEYSER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLavaGeyser;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLavaGeyser;

// Object Struct
extern ObjectLavaGeyser *LavaGeyser;

// Standard Entity Events
void LavaGeyser_Update(void);
void LavaGeyser_LateUpdate(void);
void LavaGeyser_StaticUpdate(void);
void LavaGeyser_Draw(void);
void LavaGeyser_Create(void* data);
void LavaGeyser_StageLoad(void);
void LavaGeyser_EditorDraw(void);
void LavaGeyser_EditorLoad(void);
void LavaGeyser_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LAVAGEYSER_H
