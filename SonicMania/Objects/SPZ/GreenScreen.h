#ifndef OBJ_GREENSCREEN_H
#define OBJ_GREENSCREEN_H

#include "Game.h"

// Object Class
struct ObjectGreenScreen {
    RSDK_OBJECT
    Hitbox hitbox;
    int32 parallaxFactor[5];
    int32 bgSize[5];
    uint16 aniFrames;
    bool32 greenScreenActive;
};

// Entity Class
struct EntityGreenScreen {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 paraOffset;
    int32 paraYFactor;
    bool32 paraPreview;
    bool32 showBG;
    int32 unused1;
    int32 unused2;
    Vector2 startPos;
    int32 unused3;
    Animator animator;
};

// Object Struct
extern ObjectGreenScreen *GreenScreen;

// Standard Entity Events
void GreenScreen_Update(void);
void GreenScreen_LateUpdate(void);
void GreenScreen_StaticUpdate(void);
void GreenScreen_Draw(void);
void GreenScreen_Create(void *data);
void GreenScreen_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GreenScreen_EditorDraw(void);
void GreenScreen_EditorLoad(void);
#endif
void GreenScreen_Serialize(void);

// Extra Entity Functions
void GreenScreen_DrawBG(int32 x, int32 y, uint8 bgID, int32 parallaxFactor);
void GreenScreen_DrawBackgrounds(void);
void GreenScreen_DrawSprites(void);

#endif //! OBJ_GREENSCREEN_H
