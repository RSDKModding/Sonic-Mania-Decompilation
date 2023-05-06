#ifndef OBJ_UITABANNER_H
#define OBJ_UITABANNER_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectUITABanner {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUITABanner {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    EntityUIControl *parent;
    String zoneName;
    int32 timer;
    uint8 characterID;
    uint8 zoneID;
    uint8 actID;
    int32 isEncore;
    int32 startDir;
    Animator unusedAnimator1;
    Animator zoneIconAnimator;
    Animator fuzzAnimator;
    Animator textAnimator;
    Animator unusedAnimator2;
    Animator zoneNameAnimator;
    Animator labelAnimator;
};

// Object Struct
extern ObjectUITABanner *UITABanner;

// Standard Entity Events
void UITABanner_Update(void);
void UITABanner_LateUpdate(void);
void UITABanner_StaticUpdate(void);
void UITABanner_Draw(void);
void UITABanner_Create(void *data);
void UITABanner_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UITABanner_EditorDraw(void);
void UITABanner_EditorLoad(void);
#endif
void UITABanner_Serialize(void);

// Extra Entity Functions
void UITABanner_SetupDetails(EntityUITABanner *banner, uint8 zoneID, uint8 actID, uint8 characterID, bool32 isEncore);
void UITABanner_DrawBGShapes(int32 drawX, int32 drawY, bool32 isEncore);
void UITABanner_DrawStageInfo(int32 drawX, int32 drawY, uint8 actID, uint8 characterID, bool32 isEncore);
void UITABanner_DrawZoneIcon(int32 drawX, int32 drawY, int32 zoneID);
void UITABanner_DrawBannerInfo(int32 drawX, int32 drawY, uint8 zoneID, uint8 actID, uint8 characterID, bool32 isEncore);

#endif

#endif //! OBJ_UITABANNER_H
