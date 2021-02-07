#ifndef OBJ_MAGSPIKEBALL_H
#define OBJ_MAGSPIKEBALL_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMagSpikeBall : Object{

};

// Entity Class
struct EntityMagSpikeBall : Entity {

};

// Object Struct
extern ObjectMagSpikeBall *MagSpikeBall;

// Standard Entity Events
void MagSpikeBall_Update();
void MagSpikeBall_LateUpdate();
void MagSpikeBall_StaticUpdate();
void MagSpikeBall_Draw();
void MagSpikeBall_Create(void* data);
void MagSpikeBall_StageLoad();
void MagSpikeBall_EditorDraw();
void MagSpikeBall_EditorLoad();
void MagSpikeBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_MAGSPIKEBALL_H
