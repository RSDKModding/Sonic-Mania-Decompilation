#ifndef OBJ_BARSTOOL_H
#define OBJ_BARSTOOL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 sfxSpin;
    uint16 sfxHop;
    uint16 aniFrames;
} ObjectBarStool;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 height;
    int spinSpeed;
    int spinAngle;
    int activePlayers;
    int playerVal1[PLAYER_MAX];
    int playerVal2[PLAYER_MAX];
    Animator animator1;
    Animator animator2;
} EntityBarStool;

// Object Struct
extern ObjectBarStool *BarStool;

// Standard Entity Events
void BarStool_Update(void);
void BarStool_LateUpdate(void);
void BarStool_StaticUpdate(void);
void BarStool_Draw(void);
void BarStool_Create(void* data);
void BarStool_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void BarStool_EditorDraw(void);
void BarStool_EditorLoad(void);
#endif
void BarStool_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BARSTOOL_H
