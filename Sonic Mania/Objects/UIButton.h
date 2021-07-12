#ifndef OBJ_UIBUTTON_H
#define OBJ_UIBUTTON_H

#include "../SonicMania.h"

#define MANIA_UI_ITEM_BASE                                                                                                                           \
    RSDK_ENTITY                                                                                                                                      \
    StateMachine(state);                                                                                                                             \
    void (*options0)(void);                                                                                                                          \
    void (*touchCB)(void);                                                                                                                           \
    void (*options2)(void);                                                                                                                          \
    void (*options3)(void);                                                                                                                          \
    void (*failCB)(void);                                                                                                                            \
    void (*options5)(void);                                                                                                                          \
    void (*options6)(void);                                                                                                                          \
    void (*options7)(void);                                                                                                                          \
    void (*options8)(void);                                                                                                                          \
    int timer;                                                                                                                                       \
    Vector2 posUnknown2;                                                                                                                             \
    Entity *parent;                                                                                                                                  \
    Vector2 touchPosStart;                                                                                                                           \
    Vector2 touchPosEnd;                                                                                                                             \
    bool32 touchPressed;                                                                                                                             \
    int field_A4;                                                                                                                                    \
    int field_A8;                                                                                                                                    \
    int field_AC;                                                                                                                                    \
    int field_B0;                                                                                                                                    \
    int field_B4;                                                                                                                                    \
    int field_B8;                                                                                                                                    \
    int field_BC;                                                                                                                                    \
    int field_C0;                                                                                                                                    \
    int field_C4;                                                                                                                                    \
    int field_C8;                                                                                                                                    \
    int field_CC;                                                                                                                                    \
    int field_D0;                                                                                                                                    \
    int field_D4;                                                                                                                                    \
    int field_D8;                                                                                                                                    \
    int field_DC;                                                                                                                                    \
    int field_E0;                                                                                                                                    \
    int field_E4;                                                                                                                                    \
    int field_E8;                                                                                                                                    \
    int field_EC;                                                                                                                                    \
    int field_F0;                                                                                                                                    \
    int field_F4;                                                                                                                                    \
    int field_F8;                                                                                                                                    \
    bool32 flag;                                                                                                                                     \
    bool32 disabled;                                                                                                                                 \
    Vector2 size;                                                                                                                                    \
    int listID;                                                                                                                                      \
    int frameID;


// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIButton;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    int align;
    int choiceCount;
    byte choiceDir;
    bool32 invisible;
    bool32 assignsP1;
    bool32 freeBindP2;
    bool32 transition;
    bool32 stopMusic;
    bool32 isDisabled;
    int dword138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int dword14C;
    int field_150;
    Vector2 posUnknown;
    int field_15C;
    int field_160;
    Animator animator;
    ushort textSpriteIndex;
    int startListID;
    int startFrameID;
} EntityUIButton;

// Object Struct
extern ObjectUIButton *UIButton;

// Standard Entity Events
void UIButton_Update(void);
void UIButton_LateUpdate(void);
void UIButton_StaticUpdate(void);
void UIButton_Draw(void);
void UIButton_Create(void* data);
void UIButton_StageLoad(void);
void UIButton_EditorDraw(void);
void UIButton_EditorLoad(void);
void UIButton_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIBUTTON_H
