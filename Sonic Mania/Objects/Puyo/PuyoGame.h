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
void PuyoGame_EditorDraw(void);
void PuyoGame_EditorLoad(void);
void PuyoGame_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PUYOGAME_H
