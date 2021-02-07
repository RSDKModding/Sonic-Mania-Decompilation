#ifndef OBJ_BOUNDSMARKER_H
#define OBJ_BOUNDSMARKER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBoundsMarker : Object{

};

// Entity Class
struct EntityBoundsMarker : Entity {

};

// Object Struct
extern ObjectBoundsMarker *BoundsMarker;

// Standard Entity Events
void BoundsMarker_Update();
void BoundsMarker_LateUpdate();
void BoundsMarker_StaticUpdate();
void BoundsMarker_Draw();
void BoundsMarker_Create(void* data);
void BoundsMarker_StageLoad();
void BoundsMarker_EditorDraw();
void BoundsMarker_EditorLoad();
void BoundsMarker_Serialize();

// Extra Entity Functions


#endif //!OBJ_BOUNDSMARKER_H
