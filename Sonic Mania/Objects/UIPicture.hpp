#ifndef OBJ_UIPICTURE_H
#define OBJ_UIPICTURE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIPicture : Object{
    ushort spriteIndex;
};

// Entity Class
struct EntityUIPicture : Entity {
    int listID;
    int frameID;
    int tag;
    byte zonePalette;
    int zoneID;
    EntityAnimationData animData;
};

// Object Struct
extern ObjectUIPicture *UIPicture;

// Standard Entity Events
void UIPicture_Update();
void UIPicture_LateUpdate();
void UIPicture_StaticUpdate();
void UIPicture_Draw();
void UIPicture_Create(void* data);
void UIPicture_StageLoad();
void UIPicture_EditorDraw();
void UIPicture_EditorLoad();
void UIPicture_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIPICTURE_H
