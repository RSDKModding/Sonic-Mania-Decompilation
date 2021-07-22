#ifndef OBJ_BLADEPOLE_H
#define OBJ_BLADEPOLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBladePole;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBladePole;

// Object Struct
extern ObjectBladePole *BladePole;

// Standard Entity Events
void BladePole_Update(void);
void BladePole_LateUpdate(void);
void BladePole_StaticUpdate(void);
void BladePole_Draw(void);
void BladePole_Create(void* data);
void BladePole_StageLoad(void);
void BladePole_EditorDraw(void);
void BladePole_EditorLoad(void);
void BladePole_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BLADEPOLE_H
