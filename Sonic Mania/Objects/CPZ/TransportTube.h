#ifndef OBJ_TRANSPORTTUBE_H
#define OBJ_TRANSPORTTUBE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4[4];
    uint16 field_14;
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
    int32 field_E4;
    int32 field_E8;
    int32 field_EC;
    int32 field_F0;
    int32 field_F4;
    int32 field_F8;
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
void TransportTube_EditorDraw(void);
void TransportTube_EditorLoad(void);
void TransportTube_Serialize(void);

// Extra Entity Functions
void TransportTube_SetupDirections(EntityTransportTube *entity);
void TransportTube_Unknown2(int32 velX, int32 velY);

void TransportTube_State_Type0(void);
void TransportTube_State_Type1(void);
void TransportTube_State_Type23(void);
void TransportTube_State_Type4(void);
void TransportTube_State_Type5(void);
void TransportTube_State_Type6(void);

#endif //!OBJ_TRANSPORTTUBE_H
