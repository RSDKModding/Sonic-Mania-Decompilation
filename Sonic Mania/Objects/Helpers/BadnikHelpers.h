#ifndef OBJ_BADNIKHELPERS_H
#define OBJ_BADNIKHELPERS_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBadnikHelpers;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBadnikHelpers;

// Object Entity
extern ObjectBadnikHelpers *BadnikHelpers;

// Standard Entity Events
void BadnikHelpers_Update(void);
void BadnikHelpers_LateUpdate(void);
void BadnikHelpers_StaticUpdate(void);
void BadnikHelpers_Draw(void);
void BadnikHelpers_Create(void* data);
void BadnikHelpers_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void BadnikHelpers_EditorDraw(void);
void BadnikHelpers_EditorLoad(void);
#endif
void BadnikHelpers_Serialize(void);

// Extra Entity Functions
//Handles oscillations
int BadnikHelpers_Oscillate(int origin, int speed, int power);

//Handles Boss Explosions
void BadnikHelpers_HandleExplode(int xMin, int xMax, int yMin, int yMax, uint8 drawOrder);
// Handles Boss Explosions (but assumes minMax is 16bit-shifted)
void BadnikHelpers_HandleExplode16(int xMin, int xMax, int yMin, int yMax, uint8 drawOrder);

#endif //!OBJ_BADNIKHELPERS_H
