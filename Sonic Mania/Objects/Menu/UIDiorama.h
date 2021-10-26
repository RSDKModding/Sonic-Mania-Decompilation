#ifndef OBJ_UIDIORAMA_H
#define OBJ_UIDIORAMA_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 capsuleFrames;
    uint16 sonicFrames;
    uint16 tailsFrames;
    uint16 knuxFramesAIZ;
    uint16 knuxFramesHCZ;
    uint16 mightyFrames;
    uint16 rayFrames;
    uint16 ringFrames;
    uint16 speedGateFrames;
    uint16 bssSonicFrames;
    uint16 bssFrames;
    uint8 dioramaAlt;
} ObjectUIDiorama;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 dioramaID;
    int32 dioramaSubID;
    int32 timer;
    uint8 field_64;
    bool32 flag;
    StateMachine(state);
    StateMachine(stateDraw);
    Entity *parent;
    int32 maskColour;
    Vector2 dioramaPos;
    Vector2 dioramaSize;
    Animator animator1;
    Animator animator2;
    int32 field_BC;
    int32 field_C0;
    int32 field_C4;
    int32 field_C8;
    int32 field_CC;
    int32 field_D0;
    int32 field_D4;
    int32 field_D8;
    int32 field_DC;
    int32 field_E0;
    int32 field_E4;
    int32 field_E8;
    int32 field_EC;
    int32 field_F0;
    int32 field_F4;
    int32 field_F8;
    Vector2 field_FC;
    Vector2 field_104;
    Vector2 field_10C;
    Vector2 field_114;
    Vector2 field_11C;
    Vector2 field_124;
    int32 field_12C;
    int32 field_130;
    int32 field_134;
    int32 field_138;
    int32 field_13C;
    int32 field_140;
    int32 field_144;
    int32 field_148;
    int32 field_14C;
    int32 field_150;
    int32 field_154;
    int32 field_158;
    int32 field_15C;
    int32 field_160;
    int32 field_164;
    int32 field_168;
    int32 field_16C;
    int32 field_170;
    int32 field_174;
    int32 field_178;
    Animator animators[8];
    int32 field_23C;
    int32 field_240;
    int32 field_244;
    int32 field_248;
    int32 field_24C;
    int32 field_250;
    int32 field_254;
    int32 field_258;
    int32 field_25C;
    int32 field_260;
    int32 field_264;
    int32 field_268;
    int32 field_26C;
    int32 field_270;
    int32 field_274;
    int32 field_278;
    int32 field_27C;
    int32 field_280;
    int32 field_284;
    int32 field_288;
    int32 field_28C;
    int32 field_290;
    int32 field_294;
    int32 field_298;
    int32 field_29C;
    int32 field_2A0;
    int32 field_2A4;
    int32 field_2A8;
    int32 field_2AC;
    int32 field_2B0;
    int32 field_2B4;
    int32 field_2B8;
    int32 field_2BC;
    int32 field_2C0;
    int32 field_2C4;
    int32 field_2C8;
    int32 field_2CC;
    int32 field_2D0;
    int32 field_2D4;
    int32 field_2D8;
    int32 field_2DC;
    int32 field_2E0;
    int32 field_2E4;
    int32 field_2E8;
    int32 field_2EC;
    int32 field_2F0;
    int32 field_2F4;
    int32 field_2F8;
    TextInfo textInfo;
    int32 field_304;
    int32 field_308;
    int32 field_30C;
    int32 field_310;
    int32 field_314;
    int32 field_318;
    int32 field_31C;
    int32 field_320;
    int32 field_324;
    int32 field_328;
    int32 field_32C;
    int32 field_330;
    int32 field_334;
    int32 field_338;
    int32 field_33C;
    int32 field_340;
    int32 field_344;
    int32 field_348;
    int32 field_34C;
    int32 field_350;
    int32 field_354;
    int32 field_358;
    int32 field_35C;
    int32 field_360;
    int32 field_364;
    int32 field_368;
    int32 field_36C;
    int32 field_370;
    int32 field_374;
    int32 field_378;
} EntityUIDiorama;

// Object Struct
extern ObjectUIDiorama *UIDiorama;

// Standard Entity Events
void UIDiorama_Update(void);
void UIDiorama_LateUpdate(void);
void UIDiorama_StaticUpdate(void);
void UIDiorama_Draw(void);
void UIDiorama_Create(void* data);
void UIDiorama_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIDiorama_EditorDraw(void);
void UIDiorama_EditorLoad(void);
#endif
void UIDiorama_Serialize(void);

// Extra Entity Functions
void UIDiorama_ManageStates(uint8 dioramaID);
void UIDiorama_SetText(TextInfo *info);

void UIDiorama_State_ManiaMode_Alt0(void);
void UIDiorama_State_ManiaMode_Alt1(void);
void UIDiorama_State_ManiaMode_Alt2(void);
void UIDiorama_State_PlusUpsell(void);
void UIDiorama_State_EncoreMode(void);
void UIDiorama_State_TimeAttack(void);
void UIDiorama_State_Competition(void);
void UIDiorama_State_Options(void);
void UIDiorama_State_Extras(void);
void UIDiorama_State_Exit(void);

void UIDiorama_StateDraw_ManiaMode(void);
void UIDiorama_StateDraw_PlusUpsell(void);
void UIDiorama_StateDraw_EncoreMode(void);
void UIDiorama_StateDraw_TimeAttack(void);
void UIDiorama_StateDraw_Competition(void);
void UIDiorama_StateDraw_Options(void);
void UIDiorama_StateDraw_Extras(void);
void UIDiorama_StateDraw_Exit(void);

#endif

#endif //!OBJ_UIDIORAMA_H
