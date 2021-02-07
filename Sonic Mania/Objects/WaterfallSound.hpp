#ifndef OBJ_WATERFALLSOUND_H
#define OBJ_WATERFALLSOUND_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWaterfallSound : Object{

};

// Entity Class
struct EntityWaterfallSound : Entity {

};

// Object Struct
extern ObjectWaterfallSound *WaterfallSound;

// Standard Entity Events
void WaterfallSound_Update();
void WaterfallSound_LateUpdate();
void WaterfallSound_StaticUpdate();
void WaterfallSound_Draw();
void WaterfallSound_Create(void* data);
void WaterfallSound_StageLoad();
void WaterfallSound_EditorDraw();
void WaterfallSound_EditorLoad();
void WaterfallSound_Serialize();

// Extra Entity Functions


#endif //!OBJ_WATERFALLSOUND_H
