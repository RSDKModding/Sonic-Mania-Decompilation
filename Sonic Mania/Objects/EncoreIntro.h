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
void EncoreIntro_Update();
void EncoreIntro_LateUpdate();
void EncoreIntro_StaticUpdate();
void EncoreIntro_Draw();
void EncoreIntro_Create(void* data);
void EncoreIntro_StageLoad();
void EncoreIntro_EditorDraw();
void EncoreIntro_EditorLoad();
void EncoreIntro_Serialize();

// Extra Entity Functions


#endif //!OBJ_ENCOREINTRO_H
