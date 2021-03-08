#ifndef OBJ_DIALOGRUNNER_H
#define OBJ_DIALOGRUNNER_H

#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
// Object Class
struct ObjectDialogRunner : Object {
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    Entity *entityPtr;
    int field_1C;
    int field_20;
};

// Entity Class
struct EntityDialogRunner : Entity {
    void (*state)();
    int timer;
    void (*callback)();
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    Entity *entityPtr;
    int field_88;
};

// Object Struct
extern ObjectDialogRunner *DialogRunner;

// Standard Entity Events
void DialogRunner_Update();
void DialogRunner_LateUpdate();
void DialogRunner_StaticUpdate();
void DialogRunner_Draw();
void DialogRunner_Create(void* data);
void DialogRunner_StageLoad();
void DialogRunner_EditorDraw();
void DialogRunner_EditorLoad();
void DialogRunner_Serialize();

// Extra Entity Functions
void DialogRunner_HandleCallback();
#endif

#endif //!OBJ_DIALOGRUNNER_H
