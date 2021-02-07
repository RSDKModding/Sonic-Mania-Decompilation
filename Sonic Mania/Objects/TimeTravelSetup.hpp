#ifndef OBJ_TIMETRAVELSETUP_H
#define OBJ_TIMETRAVELSETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTimeTravelSetup : Object{

};

// Entity Class
struct EntityTimeTravelSetup : Entity {

};

// Object Struct
extern ObjectTimeTravelSetup *TimeTravelSetup;

// Standard Entity Events
void TimeTravelSetup_Update();
void TimeTravelSetup_LateUpdate();
void TimeTravelSetup_StaticUpdate();
void TimeTravelSetup_Draw();
void TimeTravelSetup_Create(void* data);
void TimeTravelSetup_StageLoad();
void TimeTravelSetup_EditorDraw();
void TimeTravelSetup_EditorLoad();
void TimeTravelSetup_Serialize();

// Extra Entity Functions


#endif //!OBJ_TIMETRAVELSETUP_H
