#ifndef OBJ_GHZSETUP_H
#define OBJ_GHZSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectGHZSetup : Object {
    int waterFadeAmount;
    int delayA;
    int frameA;
    int delayListA[8];
    int delayB;
    int frameB;
    int delayListB[16];
    ushort aniTiles;
};

// Entity Class
struct EntityGHZSetup : Entity {
    //nothin 
};

// Object Struct
extern ObjectGHZSetup *GHZSetup;

// Standard Entity Events
void GHZSetup_Update();
void GHZSetup_LateUpdate();
void GHZSetup_StaticUpdate();
void GHZSetup_Draw();
void GHZSetup_Create(void* data);
void GHZSetup_StageLoad();
void GHZSetup_EditorDraw();
void GHZSetup_EditorLoad();
void GHZSetup_Serialize();

// Extra Entity Functions
void GHZSetup_SetupStartingBG();
void GHZSetup_SetupActTransition();
void GHZSetup_HandleActTransition();
void GHZSetup_SetupBGSwitchA1();
void GHZSetup_SetupBGSwitchA2();
void GHZSetup_SetupBGSwitchB1();
void GHZSetup_SetupBGSwitchB2();
void GHZSetup_SpawnGHZ2Outro();

#endif //!OBJ_GHZSETUP_H
