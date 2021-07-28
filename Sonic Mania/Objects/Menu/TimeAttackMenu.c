#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectTimeAttackMenu *TimeAttackMenu;

void TimeAttackMenu_Update(void)
{
    RSDK_THIS(TimeAttackMenu);
    StateMachine_Run(entity->state);
}

void TimeAttackMenu_LateUpdate(void)
{

}

void TimeAttackMenu_StaticUpdate(void)
{

}

void TimeAttackMenu_Draw(void)
{

}

void TimeAttackMenu_Create(void* data)
{

}

void TimeAttackMenu_StageLoad(void)
{

}

void TimeAttackMenu_EditorDraw(void)
{

}

void TimeAttackMenu_EditorLoad(void)
{

}

void TimeAttackMenu_Serialize(void)
{

}
#endif

