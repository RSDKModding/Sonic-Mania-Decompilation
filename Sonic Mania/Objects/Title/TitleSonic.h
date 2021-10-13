#ifndef OBJ_TITLESONIC_H
#define OBJ_TITLESONIC_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectTitleSonic;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator data1;
    Animator animator2;
} EntityTitleSonic;

// Object Struct
extern ObjectTitleSonic *TitleSonic;

// Standard Entity Events
void TitleSonic_Update(void);
void TitleSonic_LateUpdate(void);
void TitleSonic_StaticUpdate(void);
void TitleSonic_Draw(void);
void TitleSonic_Create(void* data);
void TitleSonic_StageLoad(void);
void TitleSonic_EditorDraw(void);
void TitleSonic_EditorLoad(void);
void TitleSonic_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TITLESONIC_H
