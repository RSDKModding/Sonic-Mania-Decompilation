#ifndef OBJ_UIPICTURE_H
#define OBJ_UIPICTURE_H

#include "../SonicMania.h"

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
    AnimationData data;
} EntityUIPicture;

// Object Struct
extern ObjectUIPicture *UIPicture;

// Standard Entity Events
void UIPicture_Update();
void UIPicture_LateUpdate();
void UIPicture_StaticUpdate();
void UIPicture_Draw();
void UIPicture_Create(void* data);
void UIPicture_StageLoad();
void UIPicture_EditorDraw();
void UIPicture_EditorLoad();
void UIPicture_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIPICTURE_H
