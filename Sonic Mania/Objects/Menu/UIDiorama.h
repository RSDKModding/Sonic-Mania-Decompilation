#ifndef OBJ_UIDIORAMA_H
#define OBJ_UIDIORAMA_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort capsuleFrames;
    ushort sonicFrames;
    ushort tailsFrames;
    ushort knuxFramesAIZ;
    ushort knuxFramesHCZ;
    ushort mightyFrames;
    ushort rayFrames;
    ushort ringFrames;
    ushort speedGateFrames;
    ushort bssSonicFrames;
    ushort bssFrames;
    byte dioramaAlt;
} ObjectUIDiorama;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int dioramaID;
    int dioramaSubID;
    int timer;
    byte field_64;
    bool32 flag;
    StateMachine(state);
    StateMachine(stateDraw);
    Entity *parent;
    int maskColour;
    Vector2 dioramaPos;
    Vector2 dioramaSize;
    Animator animator1;
    Animator animator2;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    Vector2 field_FC;
    Vector2 field_104;
    Vector2 field_10C;
    Vector2 field_114;
    Vector2 field_11C;
    Vector2 field_124;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    Animator animators[8];
    int field_23C;
    int field_240;
    int field_244;
    int field_248;
    int field_24C;
    int field_250;
    int field_254;
    int field_258;
    int field_25C;
    int field_260;
    int field_264;
    int field_268;
    int field_26C;
    int field_270;
    int field_274;
    int field_278;
    int field_27C;
    int field_280;
    int field_284;
    int field_288;
    int field_28C;
    int field_290;
    int field_294;
    int field_298;
    int field_29C;
    int field_2A0;
    int field_2A4;
    int field_2A8;
    int field_2AC;
    int field_2B0;
    int field_2B4;
    int field_2B8;
    int field_2BC;
    int field_2C0;
    int field_2C4;
    int field_2C8;
    int field_2CC;
    int field_2D0;
    int field_2D4;
    int field_2D8;
    int field_2DC;
    int field_2E0;
    int field_2E4;
    int field_2E8;
    int field_2EC;
    int field_2F0;
    int field_2F4;
    int field_2F8;
    TextInfo textInfo;
    int field_304;
    int field_308;
    int field_30C;
    int field_310;
    int field_314;
    int field_318;
    int field_31C;
    int field_320;
    int field_324;
    int field_328;
    int field_32C;
    int field_330;
    int field_334;
    int field_338;
    int field_33C;
    int field_340;
    int field_344;
    int field_348;
    int field_34C;
    int field_350;
    int field_354;
    int field_358;
    int field_35C;
    int field_360;
    int field_364;
    int field_368;
    int field_36C;
    int field_370;
    int field_374;
    int field_378;
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
void UIDiorama_EditorDraw(void);
void UIDiorama_EditorLoad(void);
void UIDiorama_Serialize(void);

// Extra Entity Functions
void UIDiorama_ManageStates(byte dioramaID);
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
