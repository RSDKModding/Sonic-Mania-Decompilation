#ifndef OBJ_ICESPRING_H
#define OBJ_ICESPRING_H

#include "Game.h"

typedef enum {
    ICESPRING_VERTICAL,
    ICESPRING_HORIZONTAL,
    ICESPRING_DIAGONAL,
} IceSpringTypes;

// Object Class
struct ObjectIceSpring {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxBounce;
    uint16 sfxShatter;
    Animator animators[PLAYER_COUNT];
};

// Entity Class
struct EntityIceSpring {
    // Spring "Inherit"
    RSDK_ENTITY
    StateMachine(state);
    IceSpringTypes type;
    FlipFlags flipFlag;
    uint8 planeFilter;
    int32 sfxTimer;
    Animator animator;
    Hitbox hitbox;
    // END SPRING INHERITANCE

    int32 shatterTimer;
    uint8 activePlayers;
};

// Object Struct
extern ObjectIceSpring *IceSpring;

// Standard Entity Events
void IceSpring_Update(void);
void IceSpring_LateUpdate(void);
void IceSpring_StaticUpdate(void);
void IceSpring_Draw(void);
void IceSpring_Create(void *data);
void IceSpring_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void IceSpring_EditorDraw(void);
void IceSpring_EditorLoad(void);
#endif
void IceSpring_Serialize(void);

// Extra Entity Functions
void IceSpring_Shatter(int32 velX, int32 velY);

#endif //! OBJ_ICESPRING_H
