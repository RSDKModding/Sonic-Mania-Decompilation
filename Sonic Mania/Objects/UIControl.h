#ifndef OBJ_UICONTROL_H
#define OBJ_UICONTROL_H

#include "../SonicMania.h"

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
    bool32 startPress[4];
    bool32 keyUp;
    bool32 keyDown;
    bool32 keyLeft;
    bool32 keyRight;
    int keyBack;
    int keyConfirm;
    int keyY;
    int keyX;
    int keyStart;
    int flagA;
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
    byte unknownCount1;
    byte unknownCount2;
    byte rowCount;
    byte columnCount;
    byte startingID;
    Vector2 size;
    Vector2 cameraOffset;
    Vector2 scrollSpeed;
    int noClamp;
    int noWrap;
    Vector2 startPos;
    Vector2 posUnknown;
    int childHasFocus;
    int dialogHasFocus;
    int popoverHasFocus;
    int dwordC4;
    int userdataInitialized;
    int dwordCC;
    int backoutTimer;
    int dwordD4;
    int field_D8;
    Entity *heading;
    Entity *shifter;
    EntityUICarousel *carousel;
    EntityUIButtonPrompt *unknown2[4];
    EntityUIButton *entities[7];
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    Animator animator;
    int field_168;
    int field_16C;
    int field_170;
    int field_174;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    int field_18C;
    int field_190;
    int field_194;
    int field_198;
    int field_19C;
    int field_1A0;
    int field_1A4;
    int field_1A8;
    int field_1AC;
    int field_1B0;
    int field_1B4;
    int field_1B8;
    int field_1BC;
    int field_1C0;
    int field_1C4;
    int field_1C8;
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
    int field_1E8;
    int field_1EC;
    int field_1F0;
    int field_1F4;
    bool32 (*backPressCB)(void);
    void (*unknownCallback2)(void);
    void (*unknownCallback3)(int, int *);
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

#endif //!OBJ_UICONTROL_H
