#ifndef OBJ_LOCALIZATION_H
#define OBJ_LOCALIZATION_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLocalization : Object {
    TextInfo text;
    TextInfo strings[68];
    bool32 loaded;
    byte language;
};

// Entity Class
struct EntityLocalization : Entity {

};

// Object Struct
extern ObjectLocalization *Localization;

// Standard Entity Events
void Localization_Update();
void Localization_LateUpdate();
void Localization_StaticUpdate();
void Localization_Draw();
void Localization_Create(void* data);
void Localization_StageLoad();
void Localization_EditorDraw();
void Localization_EditorLoad();
void Localization_Serialize();

// Extra Entity Functions
void Localization_LoadStrings();
void Localization_GetString(TextInfo *textInfo, byte id);
void Localization_GetZoneName(TextInfo *info, byte zone);
void Localization_SetZoneNameShort(TextInfo *info, byte zone);

#endif //!OBJ_LOCALIZATION_H
