#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int inputLocked;
    int field_C;
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
    int field_C8;
    int timer;
    int field_D0;
    int field_D4;
} ObjectUIControl;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    int activeEntityID;
    TextInfo tag;
    TextInfo parentTag;
    bool32 activeOnLoad;
    bool32 noWidgets;
    bool32 resetSelection;
    byte buttonCount;
#if RETRO_USE_PLUS
    byte promptCount;
#endif
    byte rowCount;
    byte columnCount;
    byte startingID;
    Vector2 size;
    Vector2 cameraOffset;
    Vector2 scrollSpeed;
#if RETRO_USE_PLUS
    bool32 noClamp;
    bool32 noWrap;
#endif
    Vector2 startPos;
    Vector2 posUnknown;
    int childHasFocus;
    int dialogHasFocus;
    int popoverHasFocus;
    int dwordC4;
    bool32 selectionDisabled;
    int dwordCC;
    int backoutTimer;
    int storedEntityID;
    int field_D8;
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
    int field_210;
    int field_214;
    int field_218;
    int field_21C;
    int field_220;
    int field_224;
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
void UIControl_EditorDraw(void);
void UIControl_EditorLoad(void);
void UIControl_Serialize(void);

// Extra Entity Functions
int UIControl_Unknown1(EntityUIControl *control, EntityUIButton *entity);
void UIControl_Unknown2(EntityUIControl *control);
void UIControl_Unknown3(EntityUIControl *entity);
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
void UIControl_Unknown15(EntityUIControl *entity, int x, int y);
void UIControl_Unknown16(void);
void UIControl_ProcessInputs(void);
void UIControl_ProcessButtonInput(void);

#endif //!OBJ_UICONTROL_H
