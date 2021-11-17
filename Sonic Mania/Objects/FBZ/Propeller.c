#include "SonicMania.h"

ObjectPropeller *Propeller;

void Propeller_Update(void)
{
    RSDK_THIS(Propeller);
    RSDK.ProcessAnimation(&entity->animator);
    Hitbox *hitbox                 = RSDK.GetHitbox(&entity->animator, 0);
    entity->hitbox2.bottom = -8;
    entity->hitbox2.top            = (RSDK.Sin256(2 * Zone->timer) >> 4) - 64;
    entity->hitbox.top     = -96;
    entity->hitbox.bottom  = -8;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        bool32 flag  = 0;
        int32 anim     = player->playerAnimator.animationID;
        if (anim != ANI_SHAFTSWING && anim != ANI_HURT && entity->fanEnabled
            && RSDK.CheckObjectCollisionTouchBox(entity, &entity->hitbox2, player, &entity->playerHitbox)) {
            flag = true;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->playerAnimator, false, 0);
            player->state      = Player_State_Air;
            player->onGround   = false;
            player->velocity.y = 0;
            player->position.y += (entity->position.y + (entity->hitbox2.top << 16) - player->position.y) >> 4;
        }

        if (RSDK.CheckObjectCollisionTouchBox(entity, &entity->hitbox, player, &entity->playerHitbox)) {
            if (!((1 << playerID) & entity->activePlayers) && flag) {
                RSDK.PlaySfx(Propeller->sfxFan, 0, 255);
                entity->activePlayers |= (1 << playerID);
            }
        }
        else {
            entity->activePlayers &= ~(1 << playerID);
        }

        if (Player_CheckCollisionTouch(player, entity, hitbox)) {
            Player_CheckHit(player, entity);
        }
    }
}

void Propeller_LateUpdate(void)
{

}

void Propeller_StaticUpdate(void)
{

}

void Propeller_Draw(void)
{
    RSDK_THIS(Propeller);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Propeller_Create(void* data)
{
    RSDK_THIS(Propeller);
    if (!SceneInfo->inEditor) {
        entity->active              = ACTIVE_BOUNDS;
        entity->visible             = true;
        entity->drawOrder           = Zone->drawOrderHigh;
        entity->updateRange.x       = 0x800000;
        entity->updateRange.y       = 0x800000;
        entity->playerHitbox.left   = -1;
        entity->playerHitbox.top    = -1;
        entity->playerHitbox.right  = 1;
        entity->playerHitbox.bottom = 1;
        entity->hitbox.left         = -104;
        entity->hitbox.right        = 104;
        entity->hitbox2.left        = -104;
        entity->hitbox2.right       = 104;
        RSDK.SetSpriteAnimation(Propeller->aniFrames, 0, &entity->animator, true, 0);
    }
}

void Propeller_StageLoad(void)
{
    Propeller->aniFrames  = RSDK.LoadSpriteAnimation("FBZ/Propeller.bin", SCOPE_STAGE);
    Propeller->sfxFan = RSDK.GetSFX("FBZ/FBZFan.wav");
}

#if RETRO_INCLUDE_EDITOR
void Propeller_EditorDraw(void)
{

}

void Propeller_EditorLoad(void)
{

}
#endif

void Propeller_Serialize(void) { RSDK_EDITABLE_VAR(Propeller, VAR_BOOL, fanEnabled); }

