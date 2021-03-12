#ifndef OBJ_BSS_COLLECTABLE_H
#define OBJ_BSS_COLLECTABLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBSS_Collectable : Object {
    EntityAnimationData value1;
    byte value2;
    int value3[32]; //= { 2, 4, 4, 4, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30, 32, 32, 32 };
    int value4[32]; //= { 2, 4, 4, 4, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 13, 14, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 22, 23, 24 };
    int value5[32]; //= { 4, 4, 5, 5, 6, 6, 7, 7, 8, 10, 12, 14, 16, 18, 20, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 32, 32, 32, 32, 32, 32, 32 };
    int value6;
    int value7;
    ushort value8;
    ushort value9;
};

// Entity Class
struct EntityBSS_Collectable : Entity {

};

// Object Struct
extern ObjectBSS_Collectable BSS_Collectable;

// Standard Entity Events
void BSS_Collectable_Update();
void BSS_Collectable_LateUpdate();
void BSS_Collectable_StaticUpdate();
void BSS_Collectable_Draw();
void BSS_Collectable_Create(void* data);
void BSS_Collectable_StageLoad();
void BSS_Collectable_EditorDraw();
void BSS_Collectable_EditorLoad();
void BSS_Collectable_Serialize();

// Extra Entity Functions


#endif //!OBJ_BSS_COLLECTABLE_H
