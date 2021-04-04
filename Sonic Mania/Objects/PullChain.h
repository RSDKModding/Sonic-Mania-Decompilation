#ifndef OBJ_PULLCHAIN_H
#define OBJ_PULLCHAIN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[18]; //= { 0, 1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 3, 0, 3, 0, 3 };
    ushort value2;
    ushort value3;
} ObjectPullChain;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPullChain;

// Object Struct
extern ObjectPullChain *PullChain;

// Standard Entity Events
void PullChain_Update(void);
void PullChain_LateUpdate(void);
void PullChain_StaticUpdate(void);
void PullChain_Draw(void);
void PullChain_Create(void* data);
void PullChain_StageLoad(void);
void PullChain_EditorDraw(void);
void PullChain_EditorLoad(void);
void PullChain_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PULLCHAIN_H
