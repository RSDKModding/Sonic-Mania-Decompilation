#ifndef OBJ_PUYOGAME_H
#define OBJ_PUYOGAME_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Entity *managers[2];
    Entity *indicators[2];
    EntityCollapsingPlatform *platforms[2];
    Entity *score1[2];
    Entity *score2[2];
    Entity *levelSel[2];
    EntityFXFade *fxFade;
} ObjectPuyoGame;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    Vector2 startPos;
    int32 timer;
    int32 selectedLevels[2];
    int32 field_74;
    int32 field_78;
    Animator animator;
    bool32 started;
    int32 field_98;
    int32 field_9C;
    uint8 field_A0;
    uint8 field_A1;
    uint8 field_A2;
    uint8 field_A3;
} EntityPuyoGame;

// Object Struct
extern ObjectPuyoGame *PuyoGame;

// Standard Entity Events
void PuyoGame_Update(void);
void PuyoGame_LateUpdate(void);
void PuyoGame_StaticUpdate(void);
void PuyoGame_Draw(void);
void PuyoGame_Create(void* data);
void PuyoGame_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PuyoGame_EditorDraw(void);
void PuyoGame_EditorLoad(void);
#endif
void PuyoGame_Serialize(void);

// Extra Entity Functions
void PuyoGame_SetupStartingEntities(void);
void PuyoGame_SetupPlayer(uint8 player);
void PuyoGame_DestroyPuyoBeans(void);
void PuyoGame_Unknown4(void);
void PuyoGame_Unknown5(void);
void PuyoGame_Unknown6(void);
void PuyoGame_Unknown7(void);
void PuyoGame_Unknown8(void);
void PuyoGame_Unknown9(void);
void PuyoGame_SetupEntities(void);
void PuyoGame_Unknown11(void);
void PuyoGame_Unknown12(void);
void PuyoGame_Unknown13(void);
void PuyoGame_Unknown14(void);

#endif //!OBJ_PUYOGAME_H
