#ifndef OBJ_TWISTEDTUBES_H
#define OBJ_TWISTEDTUBES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTwistedTubes;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTwistedTubes;

// Object Struct
extern ObjectTwistedTubes *TwistedTubes;

// Standard Entity Events
void TwistedTubes_Update(void);
void TwistedTubes_LateUpdate(void);
void TwistedTubes_StaticUpdate(void);
void TwistedTubes_Draw(void);
void TwistedTubes_Create(void* data);
void TwistedTubes_StageLoad(void);
void TwistedTubes_EditorDraw(void);
void TwistedTubes_EditorLoad(void);
void TwistedTubes_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TWISTEDTUBES_H
