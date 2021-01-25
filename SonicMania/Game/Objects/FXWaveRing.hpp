#ifndef OBJ_FXWAVERING_H
#define OBJ_FXWAVERING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectFXWaveRing : Object {

};

// Entity Class
struct EntityFXWaveRing : Entity {

};

// Object Entity
extern ObjectFXWaveRing FXWaveRing;

// Standard Entity Events
void FXWaveRing_Update();
void FXWaveRing_LateUpdate();
void FXWaveRing_StaticUpdate();
void FXWaveRing_Draw();
void FXWaveRing_Create(void* data);
void FXWaveRing_StageLoad();
void FXWaveRing_EditorDraw();
void FXWaveRing_EditorLoad();
void FXWaveRing_Serialize();

// Extra Entity Functions


#endif //!OBJ_FXWAVERING_H
