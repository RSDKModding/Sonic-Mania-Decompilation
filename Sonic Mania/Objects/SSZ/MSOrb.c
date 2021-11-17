#include "SonicMania.h"

ObjectMSOrb *MSOrb;

void MSOrb_Update(void)
{
    RSDK_THIS(MSOrb);
    StateMachine_Run(entity->state);
}

void MSOrb_LateUpdate(void)
{

}

void MSOrb_StaticUpdate(void)
{

}

void MSOrb_Draw(void)
{
    RSDK_THIS(MSOrb);
    StateMachine_Run(entity->stateDraw);
}

void MSOrb_Create(void* data)
{
    RSDK_THIS(MSOrb);

    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_NORMAL;
        entity->inkEffect     = INK_ALPHA;
        entity->visible       = true;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->alpha         = 0xC0;
        entity->drawOrder     = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(MSOrb->aniFrames, 15, &entity->animator, true, 0);
        entity->state     = MSOrb_State_Orb;
        entity->stateDraw = MSOrb_StateDraw_Orb;
    }
}

void MSOrb_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ2"))
        MSOrb->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE);
    MSOrb->hitbox.left   = -6;
    MSOrb->hitbox.top    = -6;
    MSOrb->hitbox.right  = 6;
    MSOrb->hitbox.bottom = 6;
}

void MSOrb_CheckPlayerCollisions(void)
{
    RSDK_THIS(MSOrb);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MSOrb->hitbox))
            Player_CheckHit(player, entity);
    }
}

void MSOrb_State_Orb(void)
{
    RSDK_THIS(MSOrb);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    MSOrb_CheckPlayerCollisions();
}

void MSOrb_StateDraw_Orb(void)
{
    RSDK_THIS(MSOrb);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void MSOrb_EditorDraw(void)
{

}

void MSOrb_EditorLoad(void)
{

}
#endif

void MSOrb_Serialize(void)
{

}

