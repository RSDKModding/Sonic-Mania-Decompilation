#ifndef OBJ_UIBACKGROUND_H
#define OBJ_UIBACKGROUND_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
#if RETRO_USE_PLUS
    colour bgColours[21]; //= { 15779840, 15764504, 8429744, 5298336, 15765528, 3715272, 3715272, 15757352, 5298336, 13121640, 5298336, 15765528,
                          //12632216, 3715272, 8954040, 11030768, 8964097, 110832, 5288088, 561352, 15249424 };
#else
    colour bgColours[15]; //= { 15779840, 15764504, 8429744, 5298336, 15765528, 3715272, 3715272, 15757352, 5298336, 13121640, 5298336, 15765528,
                          //12632216, 3715272, 8954040  };
#endif
    colour *activeColours;
    ushort aniFrames;
} ObjectUIBackground;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(stateDraw);
    int type;
    int timer;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
} EntityUIBackground;

// Object Struct
extern ObjectUIBackground *UIBackground;

// Standard Entity Events
void UIBackground_Update(void);
void UIBackground_LateUpdate(void);
void UIBackground_StaticUpdate(void);
void UIBackground_Draw(void);
void UIBackground_Create(void* data);
void UIBackground_StageLoad(void);
void UIBackground_EditorDraw(void);
void UIBackground_EditorLoad(void);
void UIBackground_Serialize(void);

// Extra Entity Functions
void UIBackground_DrawNormal(void);

#endif //!OBJ_UIBACKGROUND_H
