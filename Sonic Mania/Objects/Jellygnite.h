#ifndef OBJ_JELLYGNITE_H
#define OBJ_JELLYGNITE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectJellygnite;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityJellygnite;

// Object Struct
extern ObjectJellygnite *Jellygnite;

// Standard Entity Events
void Jellygnite_Update();
void Jellygnite_LateUpdate();
void Jellygnite_StaticUpdate();
void Jellygnite_Draw();
void Jellygnite_Create(void* data);
void Jellygnite_StageLoad();
void Jellygnite_EditorDraw();
void Jellygnite_EditorLoad();
void Jellygnite_Serialize();

// Extra Entity Functions


#endif //!OBJ_JELLYGNITE_H
