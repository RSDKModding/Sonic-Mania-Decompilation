#ifndef OBJ_LOTTOBALL_H
#define OBJ_LOTTOBALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLottoBall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLottoBall;

// Object Struct
extern ObjectLottoBall *LottoBall;

// Standard Entity Events
void LottoBall_Update();
void LottoBall_LateUpdate();
void LottoBall_StaticUpdate();
void LottoBall_Draw();
void LottoBall_Create(void* data);
void LottoBall_StageLoad();
void LottoBall_EditorDraw();
void LottoBall_EditorLoad();
void LottoBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_LOTTOBALL_H
