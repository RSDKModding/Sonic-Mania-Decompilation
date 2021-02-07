#ifndef OBJ_AIZTORNADO_H
#define OBJ_AIZTORNADO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectAIZTornado : Object{

};

// Entity Class
struct EntityAIZTornado : Entity {

};

// Object Struct
extern ObjectAIZTornado *AIZTornado;

// Standard Entity Events
void AIZTornado_Update();
void AIZTornado_LateUpdate();
void AIZTornado_StaticUpdate();
void AIZTornado_Draw();
void AIZTornado_Create(void* data);
void AIZTornado_StageLoad();
void AIZTornado_EditorDraw();
void AIZTornado_EditorLoad();
void AIZTornado_Serialize();

// Extra Entity Functions


#endif //!OBJ_AIZTORNADO_H
