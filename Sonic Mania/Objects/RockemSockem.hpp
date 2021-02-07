#ifndef OBJ_ROCKEMSOCKEM_H
#define OBJ_ROCKEMSOCKEM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectRockemSockem : Object{

};

// Entity Class
struct EntityRockemSockem : Entity {

};

// Object Struct
extern ObjectRockemSockem *RockemSockem;

// Standard Entity Events
void RockemSockem_Update();
void RockemSockem_LateUpdate();
void RockemSockem_StaticUpdate();
void RockemSockem_Draw();
void RockemSockem_Create(void* data);
void RockemSockem_StageLoad();
void RockemSockem_EditorDraw();
void RockemSockem_EditorLoad();
void RockemSockem_Serialize();

// Extra Entity Functions


#endif //!OBJ_ROCKEMSOCKEM_H
