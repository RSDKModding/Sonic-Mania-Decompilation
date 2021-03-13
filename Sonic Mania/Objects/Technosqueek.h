#ifndef OBJ_TECHNOSQUEEK_H
#define OBJ_TECHNOSQUEEK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTechnosqueek;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTechnosqueek;

// Object Struct
extern ObjectTechnosqueek *Technosqueek;

// Standard Entity Events
void Technosqueek_Update();
void Technosqueek_LateUpdate();
void Technosqueek_StaticUpdate();
void Technosqueek_Draw();
void Technosqueek_Create(void* data);
void Technosqueek_StageLoad();
void Technosqueek_EditorDraw();
void Technosqueek_EditorLoad();
void Technosqueek_Serialize();

// Extra Entity Functions


#endif //!OBJ_TECHNOSQUEEK_H
