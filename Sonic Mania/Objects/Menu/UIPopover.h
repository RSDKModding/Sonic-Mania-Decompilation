#ifndef OBJ_UIPOPOVER_H
#define OBJ_UIPOPOVER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Entity *activeEntity;
    EntityUIControl *storedControl;
    StateMachine(storedControlState);
} ObjectUIPopover;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 timer2;
    int32 field_64;
    int32 field_68;
    int32 buttonCount;
    Vector2 size;
    int32 field_78;
    int32 field_7C;
    EntityUIControl *parent;
    Entity* storedEntity;
    uint8 frameIDs[4];
    void (*callbacks[4])(void);
    bool32 flags[4];
    EntityUIButton *buttons[4];
    StateMachine(unknownCallback);
    int32 field_C0;
    int32 field_C4;
    uint8 field_C8;
} EntityUIPopover;

// Object Struct
extern ObjectUIPopover *UIPopover;

// Standard Entity Events
void UIPopover_Update(void);
void UIPopover_LateUpdate(void);
void UIPopover_StaticUpdate(void);
void UIPopover_Draw(void);
void UIPopover_Create(void* data);
void UIPopover_StageLoad(void);
void UIPopover_EditorDraw(void);
void UIPopover_EditorLoad(void);
void UIPopover_Serialize(void);

// Extra Entity Functions
EntityUIPopover *UIPopover_CreatePopover(void);
void UIPopover_AddButton(EntityUIPopover *popover, uint8 frameID, void (*callback)(void), bool32 flag);
void UIPopover_Setup(EntityUIPopover *popover, int32 posX, int32 posY);
void UIPopover_DrawSprites(void);
void UIPopover_SetupButtonPositions(void);
void UIPopover_Close(void);
bool32 UIPopover_BackPressCB(void);
void UIPopover_Unknown8(void);
void UIPopover_Unknown9(void);
void UIPopover_Unknown10(void);
void UIPopover_Unknown11(void);

#endif

#endif //!OBJ_UIPOPOVER_H
