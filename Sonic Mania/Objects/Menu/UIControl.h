#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 inputLocked;
    int32 field_C;
    bool32 upPress[4];
    bool32 downPress[4];
    bool32 leftPress[4];
    bool32 rightPress[4];
    bool32 backPress[4];
    bool32 confirmPress[4];
    bool32 yPress[4];
    bool32 xPress[4];
#if RETRO_USE_PLUS
    bool32 startPress[4];
#endif
    bool32 keyUp;
    bool32 keyDown;
    bool32 keyLeft;
    bool32 keyRight;
    bool32 keyConfirm;
    bool32 keyBack;
    bool32 keyY;
    bool32 keyX;
#if RETRO_USE_PLUS
    bool32 keyStart;
#endif
    bool32 flagA;
    int32 field_C8;
    int32 timer;
    int32 field_D0;
    int32 field_D4;
} ObjectUIControl;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    int32 activeEntityID;
    TextInfo tag;
    TextInfo parentTag;
    bool32 activeOnLoad;
    bool32 noWidgets;
    bool32 resetSelection;
    uint8 buttonCount;
#if RETRO_USE_PLUS
    uint8 promptCount;
#endif
    uint8 rowCount;
    uint8 columnCount;
    uint8 startingID;
    Vector2 size;
    Vector2 cameraOffset;
    Vector2 scrollSpeed;
#if RETRO_USE_PLUS
    bool32 noClamp;
    bool32 noWrap;
#endif
    Vector2 startPos;
    Vector2 posUnknown;
    bool32 childHasFocus;
    bool32 dialogHasFocus;
#if RETRO_USE_PLUS
    bool32 popoverHasFocus;
#endif
    int32 dwordC4;
    bool32 selectionDisabled;
    int32 dwordCC;
    int32 backoutTimer;
    int32 storedEntityID;
    int32 field_D8;
    Entity *heading;
    Entity *shifter;
#if RETRO_USE_PLUS
    EntityUICarousel *carousel;
#endif
    EntityUIButtonPrompt *prompts[4];
    EntityUIButton *buttons[64];
    bool32 (*backPressCB)(void);
    void (*processButtonInputCB)(void);
    void (*unknownCallback3)(void);
    void (*unknownCallback4)(void);
    void (*yPressCB)(void);
    void (*xPressCB)(void);
    int32 field_210;
    int32 field_214;
    int32 field_218;
    int32 field_21C;
    int32 field_220;
    int32 field_224;
} EntityUIControl;

// Object Struct
extern ObjectUIControl *UIControl;

// Standard Entity Events
void UIControl_Update(void);
void UIControl_LateUpdate(void);
void UIControl_StaticUpdate(void);
void UIControl_Draw(void);
void UIControl_Create(void* data);
void UIControl_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIControl_EditorDraw(void);
void UIControl_EditorLoad(void);
#endif
void UIControl_Serialize(void);

// Extra Entity Functions
int32 UIControl_Unknown1(EntityUIControl *control, EntityUIButton *entity);
void UIControl_Unknown2(EntityUIControl *control);
#if RETRO_USE_PLUS
void UIControl_Unknown3(EntityUIControl *entity);
#endif
void UIControl_Unknown4(EntityUIControl *entity);
void UIControl_Unknown5(EntityUIControl *entity);
void UIControl_Unknown6(EntityUIControl *entity);
void UIControl_Unknown7(void);
EntityUIControl *UIControl_GetUIControl(void);
bool32 UIControl_Unknown9(EntityUIControl *entity);
void UIControl_MatchMenuTag(const char *text);
void UIControl_Unknown11(TextInfo *info);
void UIControl_Unknown12(Entity *control);
void UIControl_Unknown13(void);
void UIControl_ClearInputs(char id);
void UIControl_Unknown15(EntityUIControl *entity, int32 x, int32 y);
void UIControl_Unknown16(void);
void UIControl_ProcessInputs(void);
void UIControl_ProcessButtonInput(void);

#endif //!OBJ_UICONTROL_H
