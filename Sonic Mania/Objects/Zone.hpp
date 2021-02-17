#ifndef OBJ_ZONE_H
#define OBJ_ZONE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectZone : Object{
    int actID;
    void (*stageFinishCallback)();
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54[4];
    byte field_64;
    char field_65;
    char field_66;
    char field_67;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int ringFrame;
    int timer;
    int timer2;
    int field_84[4];
    int field_94[4];
    int field_A4[4];
    int field_B4[4];
    int field_C4[4];
    int field_D4[4];
    int field_E4[4];
    int field_F4[4];
    int field_104[4];
    int field_114[4];
    int field_124[4];
    int field_134[4];
    int field_144[4];
    int field_154;
    int field_158;
    int field_15C;
    void (*timeOverState)(void);
    ushort fgLayers;
    ushort fgLow;
    ushort fgHigh;
    ushort moveLayer;
    ushort scratchLayer;
    ushort fgLowID;
    ushort fgHighID;
    ushort moveID;
    byte fgLayerLow;
    byte playerDrawHigh;
    byte drawOrderLow;
    byte drawOrderHigh;
    byte playerDrawLow;
    byte uiDrawLow;
    byte uiDrawHigh;
    byte field_17B;
    ushort sfx_fail;
    byte data[17826];
    int swapGameMode;
    int field_4724;
    int timeStart;
};

// Entity Class
struct EntityZone : Entity {
    void (*state)(void);
    void (*stateDraw)(void);
    int screenID;
    int timer;
    int fade1;
    int fade2;
};

// Object Struct
extern ObjectZone *Zone;

// Standard Entity Events
void Zone_Update();
void Zone_LateUpdate();
void Zone_StaticUpdate();
void Zone_Draw();
void Zone_Create(void* data);
void Zone_StageLoad();
void Zone_EditorDraw();
void Zone_EditorLoad();
void Zone_Serialize();

// Extra Entity Functions
int Zone_GetZoneID();
void Zone_StoreEntities(int xOffset, int yOffset);
void Zone_Unknown1(int fade1, int fade2);
void Zone_Unknown2();
void Zone_Unknown3(Entity *entity, Vector2 *pos, int angle);
void Zone_Unknown4(int screen);
void Zone_Unknown5();
void Zone_Unknown6();
void Zone_Unknown7();
bool32 Zone_Unknown8();
int Zone_Unknown9();
void Zone_Unknown10();
void Zone_Unknown11();
void Zone_Unknown12();
void Zone_Unknown13();
void Zone_Unknown14();
void Zone_Unknown15();
void Zone_Unknown16();
void Zone_Unknown17();
void Zone_Unknown18();
void Zone_Unknown19();
void Zone_Unknown20();
void Zone_Unknown21();

#endif //!OBJ_ZONE_H
