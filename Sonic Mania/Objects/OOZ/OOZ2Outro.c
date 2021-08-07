#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectOOZ2Outro *OOZ2Outro;

void OOZ2Outro_Update(void)
{

}

void OOZ2Outro_LateUpdate(void)
{

}

void OOZ2Outro_StaticUpdate(void)
{

}

void OOZ2Outro_Draw(void)
{

}

void OOZ2Outro_Create(void* data)
{

}

void OOZ2Outro_StageLoad(void)
{

}

void OOZ2Outro_StageFinishCB_Act2(void)
{
    Zone->screenBoundsR1[0] = 0x4000;
    foreach_active(Player, player)
    {
        player->state      = Player_State_Air;
        player->stateInput = 0;
        player->left       = false;
        player->right      = true;
        player->up         = false;
        player->down       = false;
        player->jumpPress  = false;
        player->jumpHold   = false;
    }

    foreach_active(OOZ2Outro, outro)
    { /*outro->state = OOZ2Outro_Unknown3;*/
    }

    foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
}

void OOZ2Outro_EditorDraw(void)
{

}

void OOZ2Outro_EditorLoad(void)
{

}

void OOZ2Outro_Serialize(void)
{

}
#endif
