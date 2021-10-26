#ifndef OBJ_LRZ3OUTRO_H
#define OBJ_LRZ3OUTRO_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sfxBlastoff;
    uint16 sfxLittlePlanet;
} ObjectLRZ3Outro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    TileLayer *bg1;
    TileLayer *bg2;
    Entity *littlePlanet;
    int32 blastoffChannel;
    float blastoffVolume;
} EntityLRZ3Outro;

// Object Struct
extern ObjectLRZ3Outro *LRZ3Outro;

// Standard Entity Events
void LRZ3Outro_Update(void);
void LRZ3Outro_LateUpdate(void);
void LRZ3Outro_StaticUpdate(void);
void LRZ3Outro_Draw(void);
void LRZ3Outro_Create(void* data);
void LRZ3Outro_StageLoad(void);
void LRZ3Outro_EditorDraw(void);
void LRZ3Outro_EditorLoad(void);
void LRZ3Outro_Serialize(void);

// Extra Entity Functions
void LRZ3Outro_HandleExplosions(void);

void LRZ3Outro_State_Unknown1(void);
void LRZ3Outro_State_Unknown2(void);
void LRZ3Outro_State_Unknown3(void);

void LRZ3Outro_StageFinishCB(void);

bool32 LRZ3Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host);
bool32 LRZ3Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host);

#endif

#endif //!OBJ_LRZ3OUTRO_H
