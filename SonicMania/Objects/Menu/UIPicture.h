#ifndef OBJ_UIPICTURE_H
#define OBJ_UIPICTURE_H

#include "Game.h"

// Object Class
struct ObjectUIPicture {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIPicture {
    RSDK_ENTITY
    int32 listID;
    int32 frameID;
    int32 tag;
    uint8 zonePalette;
    int32 zoneID;
    Animator animator;
};

// Object Struct
extern ObjectUIPicture *UIPicture;

// Standard Entity Events
void UIPicture_Update(void);
void UIPicture_LateUpdate(void);
void UIPicture_StaticUpdate(void);
void UIPicture_Draw(void);
void UIPicture_Create(void *data);
void UIPicture_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIPicture_EditorDraw(void);
void UIPicture_EditorLoad(void);
#endif
void UIPicture_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UIPICTURE_H
