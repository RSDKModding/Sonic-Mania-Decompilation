#ifndef OBJ_FXFADE_H
#define OBJ_FXFADE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFXFade : Object {
	//nothin 
};

// Entity Class
struct EntityFXFade : Entity {
    void (*state)();
    int timer;
    int speedIn;
    int wait;
    int speedOut;
    uint color;
    bool32 oneWay;
    bool32 eventOnly;
    bool32 overHUD;
    bool32 fadeOutBlack;
    int dword80;
};

// Object Struct
extern ObjectFXFade *FXFade;

// Standard Entity Events
void FXFade_Update();
void FXFade_LateUpdate();
void FXFade_StaticUpdate();
void FXFade_Draw();
void FXFade_Create(void* data);
void FXFade_StageLoad();
void FXFade_EditorDraw();
void FXFade_EditorLoad();
void FXFade_Serialize();

// Extra Entity Functions
void FXFade_StopAll();
void FXFade_State_FadeIn();
void FXFade_State_Wait();
void FXFade_State_FadeOut();
void FXFade_State_FadeOutBlack();

#endif //!OBJ_FXFADE_H
