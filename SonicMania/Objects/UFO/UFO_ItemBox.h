#ifndef OBJ_UFO_ITEMBOX_H
#define OBJ_UFO_ITEMBOX_H

#include "Game.h"

typedef enum {
    UFO_ITEMBOX_RING,
    UFO_ITEMBOX_SPHERE,
    UFO_ITEMBOX_LVLUP,
#if MANIA_USE_PLUS
    UFO_ITEMBOX_BUMPER,
#endif
} UFO_ItemBoxTypes;

// Object Class
struct ObjectUFO_ItemBox {
    RSDK_OBJECT
    int32 breakCount;
    int32 unused1;
    int32 unused2;
    uint16 aniFrames;
    uint16 meshFrames;
    uint16 sceneIndex;
    uint16 sfxDestroy;
    uint16 sfxRockemSockem;
    uint16 sfxBumper;
};

// Entity Class
struct EntityUFO_ItemBox {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    int32 height;
    int32 contentsTimer;
    int32 worldX;
    int32 worldY;
    Vector2 drawPos;
    Matrix unusedMatrix1;
    Matrix matTransform;
    Matrix matWorld;
    Matrix matNormal;
    int32 sfxTimer;
    Animator contentsAnimator;
    Animator unusedAnimator1;
};

// Object Struct
extern ObjectUFO_ItemBox *UFO_ItemBox;

// Standard Entity Events
void UFO_ItemBox_Update(void);
void UFO_ItemBox_LateUpdate(void);
void UFO_ItemBox_StaticUpdate(void);
void UFO_ItemBox_Draw(void);
void UFO_ItemBox_Create(void *data);
void UFO_ItemBox_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_ItemBox_EditorDraw(void);
void UFO_ItemBox_EditorLoad(void);
#endif
void UFO_ItemBox_Serialize(void);

// Extra Entity Functions
void UFO_ItemBox_State_HasContents(void);
void UFO_ItemBox_State_ShowContents(void);

#endif //! OBJ_UFO_ITEMBOX_H
