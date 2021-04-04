#ifndef OBJ_BUNGEE_H
#define OBJ_BUNGEE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBungee;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBungee;

// Object Struct
extern ObjectBungee *Bungee;

// Standard Entity Events
void Bungee_Update(void);
void Bungee_LateUpdate(void);
void Bungee_StaticUpdate(void);
void Bungee_Draw(void);
void Bungee_Create(void* data);
void Bungee_StageLoad(void);
void Bungee_EditorDraw(void);
void Bungee_EditorLoad(void);
void Bungee_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUNGEE_H
