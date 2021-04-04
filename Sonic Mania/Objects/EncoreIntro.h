#ifndef OBJ_ENCOREINTRO_H
#define OBJ_ENCOREINTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEncoreIntro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEncoreIntro;

// Object Struct
extern ObjectEncoreIntro *EncoreIntro;

// Standard Entity Events
void EncoreIntro_Update(void);
void EncoreIntro_LateUpdate(void);
void EncoreIntro_StaticUpdate(void);
void EncoreIntro_Draw(void);
void EncoreIntro_Create(void* data);
void EncoreIntro_StageLoad(void);
void EncoreIntro_EditorDraw(void);
void EncoreIntro_EditorLoad(void);
void EncoreIntro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ENCOREINTRO_H
