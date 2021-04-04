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
void LottoBall_Update(void);
void LottoBall_LateUpdate(void);
void LottoBall_StaticUpdate(void);
void LottoBall_Draw(void);
void LottoBall_Create(void* data);
void LottoBall_StageLoad(void);
void LottoBall_EditorDraw(void);
void LottoBall_EditorLoad(void);
void LottoBall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LOTTOBALL_H
