// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSOrb Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMSOrb *MSOrb;

void MSOrb_Update(void)
{
    RSDK_THIS(MSOrb);

    StateMachine_Run(self->state);
}

void MSOrb_LateUpdate(void) {}

void MSOrb_StaticUpdate(void) {}

void MSOrb_Draw(void)
{
    RSDK_THIS(MSOrb);

    StateMachine_Run(self->stateDraw);
}

void MSOrb_Create(void *data)
{
    RSDK_THIS(MSOrb);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->inkEffect     = INK_ALPHA;
        self->visible       = true;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->alpha         = 0xC0;
        self->drawGroup     = Zone->objectDrawGroup[0];

        RSDK.SetSpriteAnimation(MSOrb->aniFrames, 15, &self->animator, true, 0);
        self->state     = MSOrb_State_Orb;
        self->stateDraw = MSOrb_Draw_Orb;
    }
}

void MSOrb_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ2"))
        MSOrb->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE);

    MSOrb->hitboxOrb.left   = -6;
    MSOrb->hitboxOrb.top    = -6;
    MSOrb->hitboxOrb.right  = 6;
    MSOrb->hitboxOrb.bottom = 6;
}

void MSOrb_CheckPlayerCollisions(void)
{
    RSDK_THIS(MSOrb);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MSOrb->hitboxOrb))
            Player_Hurt(player, self);
    }
}

void MSOrb_State_Orb(void)
{
    RSDK_THIS(MSOrb);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    MSOrb_CheckPlayerCollisions();
}

void MSOrb_Draw_Orb(void)
{
    RSDK_THIS(MSOrb);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void MSOrb_EditorDraw(void)
{
    RSDK_THIS(MSOrb);

    self->inkEffect     = INK_ALPHA;
    self->visible       = true;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->alpha         = 0xC0;
    self->drawGroup     = Zone->objectDrawGroup[0];
    RSDK.SetSpriteAnimation(MSOrb->aniFrames, 15, &self->animator, true, 0);

    MSOrb_Draw_Orb();
}

void MSOrb_EditorLoad(void) { MSOrb->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE); }
#endif

void MSOrb_Serialize(void) {}
