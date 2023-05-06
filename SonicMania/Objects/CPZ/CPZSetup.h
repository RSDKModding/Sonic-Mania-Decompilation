#ifndef OBJ_CPZSETUP_H
#define OBJ_CPZSETUP_H

#include "Game.h"

typedef enum {
    CPZ_BG_CPZ2,
    CPZ_BG_CPZ1,
} BGSwitchIDsCPZ;

typedef enum { CPZ_DECORATION_WARNSIGN } DecorationTypesCPZ;

typedef enum {
    CPZ_PARALLAXSPRITE_GIRDER,
    CPZ_PARALLAXSPRITE_CHEMICAL1,
    CPZ_PARALLAXSPRITE_CHEMICAL2,
} ParallaxSpriteAniIDsCPZ;

// Object Class
struct ObjectCPZSetup {
    RSDK_OBJECT
    uint16 aniTiles;
    TileLayer *background;
    TABLE(int32 deformation[64], { 1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3,
                                   1, 2, 1, 3, 1, 2, 2, 1, 2, 3, 1, 2, 1, 2, 0, 0, 2, 0, 3, 2, 2, 3, 2, 2, 1, 3, 0, 0, 1, 0, 1, 3 });
    int32 bgTowerLightPalIndex;
    int32 rainbowLightPalIndex;
    int32 chemLiquidPalIndex1;
    int32 chemLiquidPalIndex2;
    int32 chemLiquidPalIndex3;
    uint16 aniTileFrame;
    EntityCPZ2Outro *outro;
};

// Entity Class
struct EntityCPZSetup {
    RSDK_ENTITY
};

// Object Struct
extern ObjectCPZSetup *CPZSetup;

// Standard Entity Events
void CPZSetup_Update(void);
void CPZSetup_LateUpdate(void);
void CPZSetup_StaticUpdate(void);
void CPZSetup_Draw(void);
void CPZSetup_Create(void *data);
void CPZSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CPZSetup_EditorDraw(void);
void CPZSetup_EditorLoad(void);
#endif
void CPZSetup_Serialize(void);

// Extra Entity Functions
void CPZSetup_BGSwitch_Act2BG(void);
void CPZSetup_BGSwitch_Act1BG(void);
void CPZSetup_StageFinish_EndAct1(void);
void CPZSetup_StageFinish_EndAct2(void);

#endif //! OBJ_CPZSETUP_H
