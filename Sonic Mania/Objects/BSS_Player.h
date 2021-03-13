#ifndef OBJ_BSS_PLAYER_H
#define OBJ_BSS_PLAYER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBSS_Player;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBSS_Player;

// Object Struct
extern ObjectBSS_Player *BSS_Player;

// Standard Entity Events
void BSS_Player_Update();
void BSS_Player_LateUpdate();
void BSS_Player_StaticUpdate();
void BSS_Player_Draw();
void BSS_Player_Create(void* data);
void BSS_Player_StageLoad();
void BSS_Player_EditorDraw();
void BSS_Player_EditorLoad();
void BSS_Player_Serialize();

// Extra Entity Functions


#endif //!OBJ_BSS_PLAYER_H
