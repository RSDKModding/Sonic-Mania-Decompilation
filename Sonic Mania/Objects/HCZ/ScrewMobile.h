#ifndef OBJ_SCREWMOBILE_H
#define OBJ_SCREWMOBILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    bool32 playingFanSfx;
    uint8 shouldPlayFanSfx;
    uint16 aniFrames;
    uint16 sfxButton;
    uint16 sfxEggMobile;
    uint16 sfxBigFan;
    uint16 sfxImpact;
} ObjectScrewMobile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 startPos;
    bool32 bombPress;
    uint8 timer;
    int32 whirlPoolTimer;
    int32 field_74;
    int32 invincibilityTimer;
    uint8 field_7C;
    Entity *whirlpool;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
} EntityScrewMobile;

// Object Struct
extern ObjectScrewMobile *ScrewMobile;

// Standard Entity Events
void ScrewMobile_Update(void);
void ScrewMobile_LateUpdate(void);
void ScrewMobile_StaticUpdate(void);
void ScrewMobile_Draw(void);
void ScrewMobile_Create(void* data);
void ScrewMobile_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ScrewMobile_EditorDraw(void);
void ScrewMobile_EditorLoad(void);
#endif
void ScrewMobile_Serialize(void);

// Extra Entity Functions
void ScrewMobile_State_CheckPlayerEnter(void);
void ScrewMobile_State_PlayerRiding(void);
void ScrewMobile_State_BossFinished(void);
void ScrewMobile_State_Idle(void);
void ScrewMobile_StateDraw1_Unknown(void);

void ScrewMobile_State2_Unknown1(void);
void ScrewMobile_State2_Unknown2(void);
void ScrewMobile_StateDraw2_Unknown(void);

#endif //!OBJ_SCREWMOBILE_H
