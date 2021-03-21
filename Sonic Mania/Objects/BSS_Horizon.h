#ifndef OBJ_BSS_HORIZON_H
#define OBJ_BSS_HORIZON_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectBSS_Horizon;

// Entity Class
typedef struct {
    RSDK_ENTITY
    AnimationData horizonData;
} EntityBSS_Horizon;

// Object Struct
extern ObjectBSS_Horizon *BSS_Horizon;

// Standard Entity Events
void BSS_Horizon_Update();
void BSS_Horizon_LateUpdate();
void BSS_Horizon_StaticUpdate();
void BSS_Horizon_Draw();
void BSS_Horizon_Create(void* data);
void BSS_Horizon_StageLoad();
void BSS_Horizon_EditorDraw();
void BSS_Horizon_EditorLoad();
void BSS_Horizon_Serialize();

// Extra Entity Functions


#endif //!OBJ_BSS_HORIZON_H
