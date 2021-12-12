#ifndef OBJ_TRANSPORTTUBE_H
#define OBJ_TRANSPORTTUBE_H

#include "SonicMania.h"

typedef enum {
    TRANSPORTTUBE_CHANGEDIR,
    TRANSPORTTUBE_ENTRY,
    TRANSPORTTUBE_TOTARGET_NEXT,
    TRANSPORTTUBE_TOTARGET_PREV,
    TRANSPORTTUBE_TOTARGET_NODE,
    TRANSPORTTUBE_CHOOSEDIR,
    TRANSPORTTUBE_EXIT,
}TransportTubeTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 nextSlot[4];
    uint16 aniFrames;
    uint16 sfxTravel;
} ObjectTransportTube;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 dirMask;
    int32 directionCount;
    Vector2 dirVelocity[8];
    int32 directionIDs[8];
    int32 playerTimers[4];
    EntityPlayer *players[4];
    Animator animator;
} EntityTransportTube;

// Object Struct
extern ObjectTransportTube *TransportTube;

// Standard Entity Events
void TransportTube_Update(void);
void TransportTube_LateUpdate(void);
void TransportTube_StaticUpdate(void);
void TransportTube_Draw(void);
void TransportTube_Create(void* data);
void TransportTube_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TransportTube_EditorDraw(void);
void TransportTube_EditorLoad(void);
#endif
void TransportTube_Serialize(void);

// Extra Entity Functions
void TransportTube_SetupDirections(EntityTransportTube *entity);
void TransportTube_HandleVelocityChange(int32 velX, int32 velY);

void TransportTube_State_ChangeDir(void);
void TransportTube_State_Entry(void);
void TransportTube_State_ToTargetEntity(void);
void TransportTube_State_TargetSeqNode(void);
void TransportTube_State_ChooseDir(void);
void TransportTube_State_Exit(void);

#endif //!OBJ_TRANSPORTTUBE_H
