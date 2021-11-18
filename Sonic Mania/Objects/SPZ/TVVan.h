#ifndef OBJ_TVVAN_H
#define OBJ_TVVAN_H

#include "SonicMania.h"

typedef enum {
    TVVAN_VAN_LEFT,
    TVVAN_VAN_RIGHT,
    TVVAN_SATELLITE_NE_SW,
    TVVAN_SATELLITE_SE_NW,
    TVVAN_SATELLITE_NW_SW,
    TVVAN_SATELLITE_NE_SE,
    TVVAN_SATELLITE_NE_NW,
    TVVAN_SATELLITE_SE_SW,
    TVVAN_SATELLITE_SW_N,
    TVVAN_SATELLITE_NW_S,
    TVVAN_SATELLITE_SE_N,
    TVVAN_SATELLITE_NE_S,
    TVVAN_UNUSED1,
    TVVAN_UNUSED2,
    TVVAN_EXITTV,
    TVVAN_TRANSPORT,
}TVVanTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    Hitbox hitbox5;
    uint16 sfxDoor;
    uint16 sfxSatellite1;
    uint16 sfxSatellite2;
    uint16 sfxExplosion;
} ObjectTVVan;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw[DRAWLAYER_COUNT]);
    uint8 type;
    Vector2 drawPos;
    Vector2 drawPos2;
    int32 field_B0;
    int32 field_B4;
    int32 field_B8;
    int32 field_BC;
    int32 timer;
    int32 field_C4[2];
    int32 field_CC[2];
    Entity *nextNode;
    EntityPlayer *player;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Animator animator8;
    Animator animator9;
} EntityTVVan;

// Object Struct
extern ObjectTVVan *TVVan;

// Standard Entity Events
void TVVan_Update(void);
void TVVan_LateUpdate(void);
void TVVan_StaticUpdate(void);
void TVVan_Draw(void);
void TVVan_Create(void* data);
void TVVan_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TVVan_EditorDraw(void);
void TVVan_EditorLoad(void);
#endif
void TVVan_Serialize(void);

// Extra Entity Functions
void TVVan_Unknown1(void);

void TVVan_StateDraw_Low(void);
void TVVan_StateDraw_High(void);
void TVVan_StateDraw2_High(void);
void TVVan_StateDraw3_High(void);
void TVVan_StateDraw4_High(void);
void TVVan_StateDraw5_High(void);
void TVVan_StateDraw6_High(void);
void TVVan_StateDraw7_High(void);
void TVVan_StateDraw8_High(void);
void TVVan_StateDraw9_High(void);
void TVVan_StateDraw10_High(void);
void TVVan_StateDraw11_High(void);
void TVVan_StateDraw15_High(void);
void TVVan_StateDraw14_High(void);
void TVVan_StateDraw14_Low(void);

void TVVan_State_Unknown1(void);
void TVVan_CheckPlayerCollisions(void);
void TVVan_State_Unknown2(void);
void TVVan_State_Unknown3(void);
void TVVan_State15_Unknown1(void);
void TVVan_State15_Unknown2(void);
void TVVan_State15_Unknown3(void);
void TVVan_State15_Unknown4(void);
void TVVan_State14_Unknown1(void);
void TVVan_State14_Unknown2(void);
void TVVan_State14_Unknown3(void);
void TVVan_State15_Unknown5(void);

#endif //!OBJ_TVVAN_H
