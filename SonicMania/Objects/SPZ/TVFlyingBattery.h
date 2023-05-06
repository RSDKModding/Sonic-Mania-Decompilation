#ifndef OBJ_TVFLYINGBATTERY_H
#define OBJ_TVFLYINGBATTERY_H

#include "Game.h"

// Object Class
struct ObjectTVFlyingBattery {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFlyover;
    bool32 isPlayingFlyover;
    int32 flyoverChannel;
    EntityWeatherTV *weatherTV;
};

// Entity Class
struct EntityTVFlyingBattery {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    int32 unused1;
    int32 oscillation;
    int32 unused2;
    int32 unused3;
    Animator shipAnimator;
    Animator bladeLAnimator;
    Animator bladeRAnimator;
};

// Object Struct
extern ObjectTVFlyingBattery *TVFlyingBattery;

// Standard Entity Events
void TVFlyingBattery_Update(void);
void TVFlyingBattery_LateUpdate(void);
void TVFlyingBattery_StaticUpdate(void);
void TVFlyingBattery_Draw(void);
void TVFlyingBattery_Create(void *data);
void TVFlyingBattery_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TVFlyingBattery_EditorDraw(void);
void TVFlyingBattery_EditorLoad(void);
#endif
void TVFlyingBattery_Serialize(void);

// Extra Entity Functions
void TVFlyingBattery_DrawSection(Vector2 drawPos, bool32 flipBlades);
void TVFlyingBattery_DrawSprites(void);

#endif //! OBJ_TVFLYINGBATTERY_H
