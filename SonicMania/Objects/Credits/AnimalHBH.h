#ifndef OBJ_ANIMALHBH_H
#define OBJ_ANIMALHBH_H

#include "Game.h"

// Object Class
struct ObjectAnimalHBH {
    RSDK_OBJECT
    int32 currentColor;
    int32 colorCount;
    uint16 aniFrames;
    uint16 silhouetteFrames;
};

// Entity Class
struct EntityAnimalHBH {
    RSDK_ENTITY
    uint8 type;
    int32 timer;
    Animator animalHBHAnimator;
    Animator silhouetteAnimator;
};

// Object Struct
extern ObjectAnimalHBH *AnimalHBH;

// Standard Entity Events
void AnimalHBH_Update(void);
void AnimalHBH_LateUpdate(void);
void AnimalHBH_StaticUpdate(void);
void AnimalHBH_Draw(void);
void AnimalHBH_Create(void *data);
void AnimalHBH_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void AnimalHBH_EditorDraw(void);
void AnimalHBH_EditorLoad(void);
#endif
void AnimalHBH_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_ANIMALHBH_H
