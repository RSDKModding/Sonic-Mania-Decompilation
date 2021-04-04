#ifndef OBJ_ANIMALHBH_H
#define OBJ_ANIMALHBH_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int palID;
    int palCnt;
    ushort animalHBHSprite;
    ushort silhouetteSprite;
} ObjectAnimalHBH;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte type;
    int timer;
    AnimationData animalHBHData;
    AnimationData silhouetteData;
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
void AnimalHBH_EditorDraw(void);
void AnimalHBH_EditorLoad(void);
void AnimalHBH_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ANIMALHBH_H
