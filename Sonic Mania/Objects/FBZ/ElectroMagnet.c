#include "SonicMania.h"

ObjectElectroMagnet *ElectroMagnet;

void ElectroMagnet_Update(void)
{
    RSDK_THIS(ElectroMagnet);
    StateMachine_Run(entity->state);
}

void ElectroMagnet_LateUpdate(void) {}

void ElectroMagnet_StaticUpdate(void)
{
    if (ElectroMagnet->field_18) {
        if (!ElectroMagnet->playingMagnetSFX) {
            RSDK.PlaySfx(ElectroMagnet->sfxMagnet, true, 255);
            ElectroMagnet->playingMagnetSFX = true;
        }
        ElectroMagnet->field_18 = 0;
    }
    else if (ElectroMagnet->playingMagnetSFX) {
        RSDK.StopSFX(ElectroMagnet->sfxMagnet);
        ElectroMagnet->playingMagnetSFX = false;
    }
}

void ElectroMagnet_Draw(void)
{
    RSDK_THIS(ElectroMagnet);
    if (entity->state == ElectroMagnet_Unknown2) {
        entity->animator.frameID = Zone->timer & 3;
        RSDK.DrawSprite(&entity->animator, NULL, false);
        entity->animator.frameID = (Zone->timer & 1) + 4;
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
}

void ElectroMagnet_Create(void *data)
{
    RSDK_THIS(ElectroMagnet);
    if (!entity->interval)
        entity->interval = 600;

    entity->active              = ACTIVE_BOUNDS;
    entity->updateRange.x       = 0x800000;
    entity->visible             = !entity->invisible;
    entity->updateRange.y       = (entity->height + 64) << 16;
    entity->drawOrder           = Zone->drawOrderHigh;
    entity->hitbox.left         = -64;
    entity->hitbox.top          = 0;
    entity->hitbox.right        = 64;
    entity->hitbox.bottom       = entity->height;
    entity->playerHitbox.left   = -64;
    entity->playerHitbox.top    = 0;
    entity->playerHitbox.right  = 64;
    entity->playerHitbox.bottom = entity->shieldRange;
    RSDK.SetSpriteAnimation(ElectroMagnet->aniFrames, 0, &entity->animator, true, 0);
    entity->state = ElectroMagnet_Unknown1;
}

void ElectroMagnet_StageLoad(void)
{
    ElectroMagnet->active = ACTIVE_ALWAYS;
    if (RSDK.CheckStageFolder("FBZ"))
        ElectroMagnet->aniFrames = RSDK.LoadSpriteAnimation("FBZ/ElectroMagnet.bin", SCOPE_STAGE);
    ElectroMagnet->hitbox.left     = -1;
    ElectroMagnet->hitbox.top      = -1;
    ElectroMagnet->hitbox.right    = 1;
    ElectroMagnet->hitbox.bottom   = 1;
    ElectroMagnet->onScreenRange.x = 0x800000;
    ElectroMagnet->onScreenRange.y = 0x400000;
    ElectroMagnet->sfxMagnet       = RSDK.GetSFX("Stage/Magnet.wav");
}

void ElectroMagnet_Unknown1(void)
{
    RSDK_THIS(ElectroMagnet);
    int32 time = (Zone->timer + entity->intervalOffset) % entity->interval;

    if (time <= entity->duration) {
        entity->active = ACTIVE_NORMAL;
        entity->timer = entity->duration - time;
        entity->state = ElectroMagnet_Unknown2;
    }
}

void ElectroMagnet_Unknown2(void)
{
    RSDK_THIS(ElectroMagnet);

    if (entity->timer <= 0) {
        if (entity->playerHitbox.top != entity->playerHitbox.bottom) {
            foreach_active(Player, player)
            {
                if (RSDK.CheckObjectCollisionTouchBox(player, &ElectroMagnet->hitbox, entity, &entity->playerHitbox) && player->invertGravity) {
                    player->invertGravity = false;
                    player->onGround      = false;
                }
            }
        }
        entity->active = ACTIVE_BOUNDS;
        entity->state  = ElectroMagnet_Unknown1;
    }
    else {
        entity->timer--;

        foreach_all(Blaster, blaster)
        {
            if (RSDK.CheckObjectCollisionTouchBox(blaster, &ElectroMagnet->hitbox, entity, &entity->hitbox) && blaster->animator.animationID < 2
                && blaster->state != Blaster_State_Setup) {
                blaster->state  = Blaster_State_MagnetAttract;
                blaster->active = ACTIVE_NORMAL;
            }
        }

        foreach_all(MagSpikeBall, spikeBall)
        {
            if (RSDK.CheckObjectCollisionTouchBox(spikeBall, &ElectroMagnet->hitbox, entity, &entity->hitbox)) {
                spikeBall->direction = FLIP_X;
                spikeBall->active    = ACTIVE_NORMAL;
            }
        }

        foreach_all(MagPlatform, platform)
        {
            if (RSDK.CheckObjectCollisionTouchBox(platform, &ElectroMagnet->hitbox, entity, &entity->hitbox)) {
                platform->state  = MagPlatform_Unknown3;
                platform->active = ACTIVE_NORMAL;
            }
        }

        if (RSDK.CheckOnScreen(entity, &ElectroMagnet->onScreenRange))
            ++ElectroMagnet->field_18;

        if (entity->playerHitbox.top != entity->playerHitbox.bottom) {
            foreach_active(Player, player)
            {
                if (RSDK.CheckObjectCollisionTouchBox(player, &ElectroMagnet->hitbox, entity, &entity->playerHitbox)) {
                    if (player->characterID == ID_KNUCKLES
                        && (player->state == Player_State_KnuxWallClimb || player->state == Player_State_KnuxLedgePullUp)) {
                        player->invertGravity = false;
                    }
                    else if (player->state == Player_State_FlyCarried) {
                        player->invertGravity = false;
                    }
                    else if (player->shield == SHIELD_LIGHTNING) {
                        if (player->collisionMode != CMODE_ROOF)
                            player->onGround = false;
                        player->invertGravity = true;
                        if (player->state != Player_State_Transform) {
                            if (player->velocity.y > -0x68000)
                                player->velocity.y -= 2 * player->gravityStrength;
                            if (RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_ROOF, player->collisionPlane, 0, -0x160000, false)) {
                                player->collisionMode = CMODE_ROOF;
                                player->angle         = 128;
                                player->onGround      = true;
                                player->groundVel     = -player->velocity.x;
                            }
                            else {
                                player->state = Player_State_Air;
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                            }
                        }
                    }
                }
            }
        }
    }
}

void ElectroMagnet_EditorDraw(void) {}

void ElectroMagnet_EditorLoad(void) {}

void ElectroMagnet_Serialize(void)
{
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_UINT16, duration);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_ENUM, shieldRange);
    RSDK_EDITABLE_VAR(ElectroMagnet, VAR_BOOL, invisible);
}
