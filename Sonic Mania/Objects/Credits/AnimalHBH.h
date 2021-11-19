#ifndef OBJ_ANIMALHBH_H
#define OBJ_ANIMALHBH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 palID;
    int32 palCnt;
    uint16 aniFrames;
    uint16 silhouetteFrames;
} ObjectAnimalHBH;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 type;
    int32 timer;
    Animator animalHBHAnimator;
    Animator silhouetteAnimator;
} EntityAnimalHBH;

// Object Struct
extern ObjectAnimalHBH *AnimalHBH;

// Standard Entity Events
void AnimalHBH_Update(void);
void AnimalHBH_LateUpdate(void);
void AnimalHBH_StaticUpdate(void);
void AnimalHBH_Draw(void);
void AnimalHBH_Create(void* data);
void AnimalHBH_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void AnimalHBH_EditorDraw(void);
void AnimalHBH_EditorLoad(void);
#endif
void AnimalHBH_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ANIMALHBH_H
