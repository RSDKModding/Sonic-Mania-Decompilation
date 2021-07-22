#ifndef OBJ_TRANSPORTTUBE_H
#define OBJ_TRANSPORTTUBE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4[4];
    ushort field_14;
    ushort sfxTravel;
} ObjectTransportTube;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    byte type;
    byte dirMask;
    int directionCount;
    Vector2 dirVelocity[8];
    int directionIDs[8];
    int playerTimers[4];
    EntityPlayer *players[4];
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
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
void TransportTube_Unknown2(int velX, int velY);

void TransportTube_State_Type0(void);
void TransportTube_State_Type1(void);
void TransportTube_State_Type23(void);
void TransportTube_State_Type4(void);
void TransportTube_State_Type5(void);
void TransportTube_State_Type6(void);

#endif //!OBJ_TRANSPORTTUBE_H
