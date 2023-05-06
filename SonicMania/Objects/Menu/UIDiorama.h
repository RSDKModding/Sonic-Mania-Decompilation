#ifndef OBJ_UIDIORAMA_H
#define OBJ_UIDIORAMA_H

#include "Game.h"

#if MANIA_USE_PLUS

typedef enum {
    UIDIORAMA_MANIAMODE,
    UIDIORAMA_PLUSUPSELL,
    UIDIORAMA_ENCOREMODE,
    UIDIORAMA_TIMEATTACK,
    UIDIORAMA_COMPETITION,
    UIDIORAMA_OPTIONS,
    UIDIORAMA_EXTRAS,
    UIDIORAMA_EXIT,
} UIDioramaIDs;

typedef enum {
    UIDIORAMA_ALT_RUN,
    UIDIORAMA_ALT_JOG,
    UIDIORAMA_ALT_LOOP,
} UIDioramaAlts;

//=======================
// STATE-STRUCTS BEGIN
//=======================

typedef struct {
    int32 scrollPos;
    int32 clouds1ScrollPos;
    int32 clouds2ScrollPos;
    int32 clouds3ScrollPos;
    int32 valuesPadding[12];

    Vector2 playerPos;
    Vector2 vectorsPadding[15];

    Animator terrainAnimator;
    Animator clouds1Animator;
    Animator clouds2Animator;
    Animator clouds3Animator;
    Animator mountainsAnimator;
    Animator backgroundAnimator;
    Animator sonicAnimator;
    Animator tailsAnimator;
    Animator animatorsPadding[8];
} UIDiorama_StateInfo_ManiaMode;

typedef struct {
    bool32 showFlash;
    int32 flashAlpha;
    int32 unused;
    int32 delay;
    int32 lineCount;
    int32 linePos[4];
    int32 valuesPadding[7];

    Vector2 plusPos;
    Vector2 plusVelocity;
    Vector2 vectorsPadding[14];

    Animator dioramaAnimator;
    Animator flashAnimator;
    Animator logoAnimator;
    Animator plusAnimator;
    Animator arrowAnimator;
    Animator lightningAnimator;
    Animator textAnimator;
    Animator animatorsPadding[9];
} UIDiorama_StateInfo_PlusUpsell;

typedef struct {
    int32 valuesPadding[16];

    Vector2 vectorsPadding[16];

    Animator dioramaAnimator;
    Animator capsuleAnimator;
    Animator buttonAnimator;
    Animator glassAnimator;
    Animator mightyAnimator;
    Animator rayAnimator;
    Animator animatorsPadding[10];
} UIDiorama_StateInfo_EncoreMode;

typedef struct {
    int32 valuesPadding[16];

    Vector2 vectorsPadding[16];

    Animator dioramaAnimator;
    Animator sonicAnimator;
    Animator ringAnimator;
    Animator gateBaseAnimator;
    Animator gateTopAnimator;
    Animator gateFinsAnimator;
    Animator animatorsPadding[10];
} UIDiorama_StateInfo_TimeAttack;

typedef struct {
    int32 tailsAngle;
    int32 rayAngle;
    int32 scrollPos[11];
    int32 valuesPadding[3];

    Vector2 terrainPos;
    Vector2 platformPos;
    Vector2 tailsPos;
    Vector2 knuxPos;
    Vector2 rayPos;
    Vector2 mightyPos;
    Vector2 vectorsPadding[10];

    Animator dioramaAnimator;
    Animator platformAnimator;
    Animator ringAnimator;
    Animator tailsAnimator;
    Animator knuxAnimator;
    Animator rayAnimator;
    Animator mightyAnimator;
    Animator animatorsPadding[9];
} UIDiorama_StateInfo_Competition;

typedef struct {
    int32 itemAlpha;
    int32 contAlpha;
    int32 audioAlpha;
    int32 itemAngle;
    int32 contAngle;
    int32 audioAngle;
    int32 valuesPadding[10];

    Vector2 itemPos;
    Vector2 contPos;
    Vector2 audioPos;
    Vector2 itemOffset;
    Vector2 contOffset;
    Vector2 audioOffset;
    Vector2 vectorsPadding[10];

    Animator dioramaAnimator;
    Animator sonicAnimator;
    Animator tailsAnimator;
    Animator knuxAnimator;
    Animator itemConstellationAnimator;
    Animator contConstellationAnimator;
    Animator audioConstellationAnimator;
    Animator animatorsPadding[9];
} UIDiorama_StateInfo_Options;

typedef struct {
    int32 valuesPadding[16];

    Vector2 vectorsPadding[16];

    Animator dioramaAnimator;
    Animator medalAnimator;
    Animator sonicAnimator;
    Animator animatorsPadding[13];
} UIDiorama_StateInfo_Extras;

typedef struct {
    int32 animDelay;
    bool32 processVelocity;
    bool32 isOffScreen;
    int32 valuesPadding[13];

    Vector2 sonicPos;
    Vector2 sonicVelocity;
    Vector2 vectorsPadding[14];

    Animator dioramaAnimator;
    Animator sonicAnimator;
    Animator animatorsPadding[14];
} UIDiorama_StateInfo_Exit;

//=======================
// STATE-STRUCTS END
//=======================

// Object Class
struct ObjectUIDiorama {
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
};

// Entity Class
struct EntityUIDiorama {
    RSDK_ENTITY
    int32 dioramaID;
    int32 lastDioramaID;
    int32 timer;
    uint8 parentActivity;
    bool32 needsSetup;
    StateMachine(state);
    StateMachine(stateDraw);
    EntityUIControl *parent;
    int32 maskColor;
    Vector2 dioramaPos;
    Vector2 dioramaSize;
    Animator maskAnimator;
    Animator staticAnimator;
    int32 values[16];
    Vector2 vectors[16];
    Animator animators[16];
    String texts[16];
};

// Object Struct
extern ObjectUIDiorama *UIDiorama;

// Standard Entity Events
void UIDiorama_Update(void);
void UIDiorama_LateUpdate(void);
void UIDiorama_StaticUpdate(void);
void UIDiorama_Draw(void);
void UIDiorama_Create(void *data);
void UIDiorama_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIDiorama_EditorDraw(void);
void UIDiorama_EditorLoad(void);
#endif
void UIDiorama_Serialize(void);

// Extra Entity Functions
void UIDiorama_ChangeDiorama(uint8 dioramaID);
void UIDiorama_SetText(String *string);

void UIDiorama_State_ManiaMode_Alt_Run(void);
void UIDiorama_State_ManiaMode_Alt_Jog(void);
void UIDiorama_State_ManiaMode_Alt_Loop(void);
void UIDiorama_State_PlusUpsell(void);
void UIDiorama_State_EncoreMode(void);
void UIDiorama_State_TimeAttack(void);
void UIDiorama_State_Competition(void);
void UIDiorama_State_Options(void);
void UIDiorama_State_Extras(void);
void UIDiorama_State_Exit(void);

void UIDiorama_Draw_ManiaMode(void);
void UIDiorama_Draw_PlusUpsell(void);
void UIDiorama_Draw_EncoreMode(void);
void UIDiorama_Draw_TimeAttack(void);
void UIDiorama_Draw_Competition(void);
void UIDiorama_Draw_Options(void);
void UIDiorama_Draw_Extras(void);
void UIDiorama_Draw_Exit(void);

#endif

#endif //! OBJ_UIDIORAMA_H
