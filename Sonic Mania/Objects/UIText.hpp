#ifndef OBJ_UITEXT_H
#define OBJ_UITEXT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIText : Object {
    ushort spriteIndex;
};

// Entity Class
struct EntityUIText : Entity {
    void (*state)();
    void (*stateDraw)();
    TextInfo text;
    TextInfo tag;
    bool32 selectable;
    ushort listID;
    int align;
    int data0;
    int data1;
    int data2;
    int data3;
    bool32 highlighted;
    EntityAnimationData animData;
};

// Object Struct
extern ObjectUIText *UIText;

// Standard Entity Events
void UIText_Update();
void UIText_LateUpdate();
void UIText_StaticUpdate();
void UIText_Draw();
void UIText_Create(void* data);
void UIText_StageLoad();
void UIText_EditorDraw();
void UIText_EditorLoad();
void UIText_Serialize();

// Extra Entity Functions


#endif //!OBJ_UITEXT_H
