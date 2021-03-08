#ifndef OBJ_UIBACKGROUND_H
#define OBJ_UIBACKGROUND_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIBackground : Object {
    colour bgColours[21]; //= { 15779840, 15764504, 8429744, 5298336, 15765528, 3715272, 3715272, 15757352, 5298336, 13121640, 5298336, 15765528, 12632216, 3715272, 8954040, 11030768, 8964097, 110832, 5288088, 561352, 15249424 };
    colour *activeColours;
    ushort value3;
};

// Entity Class
struct EntityUIBackground : Entity {
    void (*state)();
    int type;
    int timer;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
};

// Object Struct
extern ObjectUIBackground *UIBackground;

// Standard Entity Events
void UIBackground_Update();
void UIBackground_LateUpdate();
void UIBackground_StaticUpdate();
void UIBackground_Draw();
void UIBackground_Create(void* data);
void UIBackground_StageLoad();
void UIBackground_EditorDraw();
void UIBackground_EditorLoad();
void UIBackground_Serialize();

// Extra Entity Functions
void UIBackground_DrawNormal();

#endif //!OBJ_UIBACKGROUND_H