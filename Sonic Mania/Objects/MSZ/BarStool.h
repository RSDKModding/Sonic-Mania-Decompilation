#ifndef OBJ_BARSTOOL_H
#define OBJ_BARSTOOL_H

#include "SonicMania.h"

typedef enum { BARSTOOL_HEIGHT_SHORT, BARSTOOL_HEIGHT_TALL } BarStoolHeights;

// Object Class
struct ObjectBarStool {
    RSDK_OBJECT
    Hitbox hitboxStool;
    uint16 sfxSpin;
    uint16 sfxHop;
    uint16 aniFrames;
};

// Entity Class
struct EntityBarStool {
    RSDK_ENTITY
    StateMachine(state);
    uint8 height;
    int32 spinSpeed;
    int32 spinAngle;
    int32 activePlayers;
    int32 playerAngle[PLAYER_MAX];
    int32 playerPos[PLAYER_MAX];
    Animator stoolAnimator;
    Animator poleAnimator;
};

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
