#ifndef OBJ_REDZ_H
#define OBJ_REDZ_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRedz;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRedz;

// Object Entity
extern ObjectRedz *Redz;

// Standard Entity Events
void Redz_Update();
void Redz_LateUpdate();
void Redz_StaticUpdate();
void Redz_Draw();
void Redz_Create(void* data);
void Redz_StageLoad();
void Redz_EditorDraw();
void Redz_EditorLoad();
void Redz_Serialize();

// Extra Entity Functions


#endif //!OBJ_REDZ_H
