#ifndef OBJ_TWISTINGSLIDE_H
#define OBJ_TWISTINGSLIDE_H

#include "Game.h"

typedef enum {
    TWISTINGSLIDE_START,
    TWISTINGSLIDE_STRIP,
    TWISTINGSLIDE_TWIST,
    TWISTINGSLIDE_END,
} TwistingSlideTypes;

// Object Class
struct ObjectTwistingSlide {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityTwistingSlide {
    RSDK_ENTITY
    uint8 type;
    int32 endLen;
    int32 activePlayers;
    int32 playerAngles[PLAYER_COUNT];
    int32 minY;
    int32 maxY;
    Hitbox hitboxSlideStart;
    Hitbox hitboxSlideEnd;
    Hitbox hitboxSlide;
    Animator animator;
};

// Object Struct
extern ObjectTwistingSlide *TwistingSlide;

// Standard Entity Events
void TwistingSlide_Update(void);
void TwistingSlide_LateUpdate(void);
void TwistingSlide_StaticUpdate(void);
void TwistingSlide_Draw(void);
void TwistingSlide_Create(void *data);
void TwistingSlide_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TwistingSlide_EditorDraw(void);
void TwistingSlide_EditorLoad(void);
#endif
void TwistingSlide_Serialize(void);

// Extra Entity Functions
void TwistingSlide_SetupHitboxes(void);

#endif //! OBJ_TWISTINGSLIDE_H
