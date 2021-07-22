#ifndef OBJ_UIPICTURE_H
#define OBJ_UIPICTURE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectUIPicture;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int listID;
    int frameID;
    int tag;
    byte zonePalette;
    int zoneID;
    Animator animator;
} EntityUIPicture;

// Object Struct
extern ObjectUIPicture *UIPicture;

// Standard Entity Events
void UIPicture_Update(void);
void UIPicture_LateUpdate(void);
void UIPicture_StaticUpdate(void);
void UIPicture_Draw(void);
void UIPicture_Create(void* data);
void UIPicture_StageLoad(void);
void UIPicture_EditorDraw(void);
void UIPicture_EditorLoad(void);
void UIPicture_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIPICTURE_H
