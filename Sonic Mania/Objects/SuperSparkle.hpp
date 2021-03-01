#ifndef OBJ_SUPERSPARKLE_H
#define OBJ_SUPERSPARKLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSuperSparkle : Object {
    ushort spriteIndex;
};

// Entity Class
struct EntitySuperSparkle : Entity {
    EntityPlayer *player;
    int timer;
    bool32 activeFlag;
};

// Object Struct
extern ObjectSuperSparkle *SuperSparkle;

// Standard Entity Events
void SuperSparkle_Update();
void SuperSparkle_LateUpdate();
void SuperSparkle_StaticUpdate();
void SuperSparkle_Draw();
void SuperSparkle_Create(void* data);
void SuperSparkle_StageLoad();
void SuperSparkle_EditorDraw();
void SuperSparkle_EditorLoad();
void SuperSparkle_Serialize();

// Extra Entity Functions


#endif //!OBJ_SUPERSPARKLE_H
