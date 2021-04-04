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
void Technosqueek_Update(void);
void Technosqueek_LateUpdate(void);
void Technosqueek_StaticUpdate(void);
void Technosqueek_Draw(void);
void Technosqueek_Create(void* data);
void Technosqueek_StageLoad(void);
void Technosqueek_EditorDraw(void);
void Technosqueek_EditorLoad(void);
void Technosqueek_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TECHNOSQUEEK_H
