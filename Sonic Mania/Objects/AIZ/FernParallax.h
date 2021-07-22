#ifndef OBJ_FERNPARALLAX_H
#define OBJ_FERNPARALLAX_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    byte entityBuffer[2][ENTITY_SIZE];
} ObjectFernParallax;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    Animator animator;
    byte aniID;
    Vector2 parallaxFactor;
} EntityFernParallax;

// Object Struct
extern ObjectFernParallax *FernParallax;

// Standard Entity Events
void FernParallax_Update(void);
void FernParallax_LateUpdate(void);
void FernParallax_StaticUpdate(void);
void FernParallax_Draw(void);
void FernParallax_Create(void* data);
void FernParallax_StageLoad(void);
void FernParallax_EditorDraw(void);
void FernParallax_EditorLoad(void);
void FernParallax_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_FERNPARALLAX_H
