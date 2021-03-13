#ifndef OBJ_LAVAGEYSER_H
#define OBJ_LAVAGEYSER_H

#include "../SonicMania.h"

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
void LavaGeyser_Update();
void LavaGeyser_LateUpdate();
void LavaGeyser_StaticUpdate();
void LavaGeyser_Draw();
void LavaGeyser_Create(void* data);
void LavaGeyser_StageLoad();
void LavaGeyser_EditorDraw();
void LavaGeyser_EditorLoad();
void LavaGeyser_Serialize();

// Extra Entity Functions


#endif //!OBJ_LAVAGEYSER_H
