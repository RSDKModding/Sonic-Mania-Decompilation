#ifndef OBJ_LETTERBOARD_H
#define OBJ_LETTERBOARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLetterboard;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLetterboard;

// Object Struct
extern ObjectLetterboard *Letterboard;

// Standard Entity Events
void Letterboard_Update(void);
void Letterboard_LateUpdate(void);
void Letterboard_StaticUpdate(void);
void Letterboard_Draw(void);
void Letterboard_Create(void* data);
void Letterboard_StageLoad(void);
void Letterboard_EditorDraw(void);
void Letterboard_EditorLoad(void);
void Letterboard_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LETTERBOARD_H