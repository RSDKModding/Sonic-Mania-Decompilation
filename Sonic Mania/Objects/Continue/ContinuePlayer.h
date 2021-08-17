#ifndef OBJ_CONTINUEPLAYER_H
#define OBJ_CONTINUEPLAYER_H

#include "SonicMania.h"

typedef enum {
    CONTPLR_ANI_IDLE_SONIC,
    CONTPLR_ANI_REACT_SONIC,
    CONTPLR_ANI_IDLE_TAILS,
    CONTPLR_ANI_REACT_TAILS,
    CONTPLR_ANI_IDLE_KNUX,
    CONTPLR_ANI_REACT_KNUX,
#if RETRO_USE_PLUS
    CONTPLR_ANI_IDLE_MIGHTY,
    CONTPLR_ANI_REACT_MIGHTY,
    CONTPLR_ANI_IDLE_RAY,
    CONTPLR_ANI_REACT_RAY,
#endif
    CONTPLR_ANI_IDLE_AI,
    CONTPLR_ANI_ICON,
}ContinuePlayerAniIDs;

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort playerAniFrames;
    ushort tailAniFrames;
    ushort sfxRoll;
    ushort sfxCharge;
    ushort sfxRelease;
} ObjectContinuePlayer;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    bool32 isPlayer2;
    int timer;
    int field_64;
    int field_68;
    Animator animator;
    ushort aniFrames;
} EntityContinuePlayer;

// Object Struct
extern ObjectContinuePlayer *ContinuePlayer;

// Standard Entity Events
void ContinuePlayer_Update(void);
void ContinuePlayer_LateUpdate(void);
void ContinuePlayer_StaticUpdate(void);
void ContinuePlayer_Draw(void);
void ContinuePlayer_Create(void* data);
void ContinuePlayer_StageLoad(void);
void ContinuePlayer_EditorDraw(void);
void ContinuePlayer_EditorLoad(void);
void ContinuePlayer_Serialize(void);

// Extra Entity Functions
void ContinuePlayer_Unknown1(void);
void ContinuePlayer_Unknown2(void);
void ContinuePlayer_Unknown3(void);
void ContinuePlayer_Unknown4(void);

#endif //!OBJ_CONTINUEPLAYER_H
