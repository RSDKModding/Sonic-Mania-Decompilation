#ifndef OBJ_HEAVYSHINOBI_H
#define OBJ_HEAVYSHINOBI_H

#include "Game.h"

typedef enum {
    SHINOBI_MAIN,
    SHINOBI_SLASH,
    SHINOBI_ASTERON,
    SHINOBI_ASTERONSPIKE,
    SHINOBI_BOUNDS,
} HeavyShinobiTypes;

// Object Class
struct ObjectHeavyShinobi {
    RSDK_OBJECT
    int8 health;
    uint8 invincibilityTimer;
    Animator fxTrailAnimator[4];
    uint8 storedIDs[4];
    uint8 storedAnimIDs[4];
    Vector2 storePos[16];
    Animator unusedAnimator;
    uint8 activeShurikens;
    uint16 aniFrames;
    Hitbox hitboxShinobi;
    Hitbox hitboxSlashRange;
    Hitbox hitboxUnused;
    Hitbox hitboxSlash;
    Hitbox hitboxAsteron;
    Hitbox hitboxBounds;
    Hitbox hitboxAsteronSpike;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDefeat;
    uint16 sfxDropIn;
    uint16 sfxExplode;
    uint16 sfxGlitch;
    uint16 sfxJump;
    uint16 sfxParry;
    uint16 sfxSlash;
    uint16 sfxStick;
    uint16 sfxThrow;
};

// Entity Class
struct EntityHeavyShinobi {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 glitchTimer;
    EntityPlayer *playerPtr;
    Vector2 playerDistance;
    Animator mainAnimator;
    Animator fxAnimator;
};

// Object Struct
extern ObjectHeavyShinobi *HeavyShinobi;

// Standard Entity Events
void HeavyShinobi_Update(void);
void HeavyShinobi_LateUpdate(void);
void HeavyShinobi_StaticUpdate(void);
void HeavyShinobi_Draw(void);
void HeavyShinobi_Create(void *data);
void HeavyShinobi_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HeavyShinobi_EditorDraw(void);
void HeavyShinobi_EditorLoad(void);
#endif
void HeavyShinobi_Serialize(void);

// Extra Entity Functions
void HeavyShinobi_HandleAfterFX(void);
void HeavyShinobi_HandleSlash(EntityPlayer *player);
void HeavyShinobi_StartJump(void);
void HeavyShinobi_Explode(void);

// Shinobi States
void HeavyShinobi_State_Init(void);
void HeavyShinobi_State_SetupArena(void);
void HeavyShinobi_State_StartFight(void);
void HeavyShinobi_State_Idle(void);
void HeavyShinobi_State_Slash(void);
void HeavyShinobi_State_Jump(void);
void HeavyShinobi_State_Glitched(void);
void HeavyShinobi_State_Destroyed(void);
void HeavyShinobi_State_Finished(void);
void HeavyShinobi_Draw_Shinobi(void);

// Slash States
void HeavyShinobi_StateSlash_Active(void);
void HeavyShinobi_Draw_Slash(void);

// Asteron States
void HeavyShinobi_StateAsteron_Thrown(void);
void HeavyShinobi_StateAsteron_Debris(void);
void HeavyShinobi_StateAsteron_Explode(void);
void HeavyShinobi_Draw_Asteron(void);

// Asteron Spike States
void HeavyShinobi_State_AsteronSpike(void);
void HeavyShinobi_Draw_AsteronSpike(void);

// Boundary States
void HeavyShinobi_StateBounds_WaitForPlayer(void);
void HeavyShinobi_StateBounds_Active(void);
void HeavyShinobi_Draw_Bounds(void);

#endif //! OBJ_HEAVYSHINOBI_H
