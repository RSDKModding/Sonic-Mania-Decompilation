#ifndef OBJ_LOGOSETUP_H
#define OBJ_LOGOSETUP_H

#include "Game.h"

// Object Class
struct ObjectLogoSetup {
    RSDK_OBJECT
    int32 timer;
    uint16 sfxSega;
};

// Entity Class
struct EntityLogoSetup {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
};

// Object Struct
extern ObjectLogoSetup *LogoSetup;

// Standard Entity Events
void LogoSetup_Update(void);
void LogoSetup_LateUpdate(void);
void LogoSetup_StaticUpdate(void);
void LogoSetup_Draw(void);
void LogoSetup_Create(void *data);
void LogoSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LogoSetup_EditorDraw(void);
void LogoSetup_EditorLoad(void);
#endif
void LogoSetup_Serialize(void);

// Extra Entity Functions
bool32 LogoSetup_ImageCallback(void);
void LogoSetup_State_CESAScreen(void);
void LogoSetup_State_ShowLogos(void);
void LogoSetup_State_FadeToNextLogos(void);
void LogoSetup_State_NextLogos(void);
void LogoSetup_Draw_Fade(void);

#endif //! OBJ_LOGOSETUP_H
