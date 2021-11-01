#include "SonicMania.h"

ObjectSilverSonic *SilverSonic;

void SilverSonic_Update(void)
{
    RSDK_THIS(SilverSonic);
    RSDK.ProcessAnimation(&entity->animator);
    entity->hitboxPtr       = RSDK.GetHitbox(&entity->animator, 0);
    entity->hitboxBadnikPtr = RSDK.GetHitbox(&entity->animator, 1);

    if (entity->position.x >= Zone->screenBoundsL2[0] + 0x180000) {
        if (entity->position.x > Zone->screenBoundsR2[0] - 0x180000) {
            entity->position.x = Zone->screenBoundsR2[0] - 0x180000;
            if (entity->state != SilverSonic_State2_Unknown6) {
                entity->direction  = FLIP_X;
                entity->velocity.x = 0;
            }
        }
    }
    else {
        entity->position.x = Zone->screenBoundsL2[0] + 0x180000;
        if (entity->state != SilverSonic_State2_Unknown6) {
            entity->direction  = FLIP_NONE;
            entity->velocity.x = 0;
        }
    }

    RSDK.ProcessTileCollisions(entity, entity->hitboxPtr, entity->hitboxBadnikPtr);
    StateMachine_Run(entity->state);
}

void SilverSonic_LateUpdate(void) {}

void SilverSonic_StaticUpdate(void) {}

void SilverSonic_Draw(void)
{
    RSDK_THIS(SilverSonic);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void SilverSonic_Create(void *data)
{
    RSDK_THIS(SilverSonic);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active          = ACTIVE_BOUNDS;
        entity->visible         = true;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x800000;
        entity->collisionLayers = Zone->fgLayers;
        entity->velocity.y      = 0x10000;
        entity->tileCollisions  = true;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &entity->animator, true, 0);
        entity->drawFX    = FX_FLIP;
        entity->inkEffect = INK_ALPHA;
        entity->state     = SilverSonic_State_Unknown1;
        entity->field_60  = RSDK.Rand(0, 3);
        if (entity->field_60 == 2)
            entity->field_60 = 1;
        entity->field_64  = 1;
        entity->drawOrder = Zone->drawOrderLow;
    }
}

void SilverSonic_StageLoad(void)
{
    SilverSonic->aniFrames  = RSDK.LoadSpriteAnimation("SSZ2/SilverSonic.bin", SCOPE_STAGE);
    SilverSonic->sfxJump    = RSDK.GetSFX("SSZ2/SSJump.wav");
    SilverSonic->sfxDash    = RSDK.GetSFX("SSZ2/SSDash.wav");
    SilverSonic->sfxBoost   = RSDK.GetSFX("SSZ2/SSBoost.wav");
    SilverSonic->sfxRebound = RSDK.GetSFX("SSZ2/SSRebound.wav");
    SilverSonic->sfxArm     = RSDK.GetSFX("SSZ2/SSArm.wav");
}

void SilverSonic_HandleNextAttack(void)
{
    RSDK_THIS(SilverSonic);

    if (entity->field_64 <= 0) {
        entity->field_64 = RSDK.Rand(0, 2);
        entity->field_60 ^= 1;
    }

    if (entity->field_60) {
        if (!--entity->field_60) {
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 1, &entity->animator, false, 0);
            entity->state = SilverSonic_State2_Unknown1;
        }
    }
    else {
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 4, &entity->animator, false, 0);
        entity->state = SilverSonic_State_Unknown2;
        RSDK.PlaySfx(SilverSonic->sfxArm, false, 255);
    }
    --entity->field_64;
}

void SilverSonic_CheckPlayerCollisions_Badnik(void)
{
    RSDK_THIS(SilverSonic);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, entity->hitboxBadnikPtr)) {
            if (Player_CheckBadnikBreak(entity, player, false)) {
                int x = entity->position.x;
                int y = entity->position.y;
                RSDK.ResetEntityPtr(entity, MSBomb->objectID, NULL);
                entity->position.x = x;
                entity->position.y = y;
                entity->velocity.y = -0x10000;
                entity->field_60   = 2;
                entity->state      = MSBomb_Unknown2;
            }
        }
    }
}

void SilverSonic_CheckPlayerCollisions_Ball(void)
{
    RSDK_THIS(SilverSonic);

    if (entity->invincibilityTimer > 0) {
        entity->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, entity, entity->hitboxBadnikPtr)) {
                if (Player_CheckAttacking(player, entity)) {
                    if (entity->onGround) {
                        if (abs(player->velocity.x) <= 0x30000) {
                            player->groundVel  = entity->groundVel;
                            player->velocity.x = entity->groundVel;
                            entity->groundVel  = (-0xC0 * entity->groundVel) >> 8;
                        }
                        else {
                            entity->onGround   = false;
                            entity->velocity.x = (288 * player->velocity.x) >> 8;
                            entity->velocity.y = -abs((0xC0 * player->velocity.x) >> 8);
                            player->velocity.x = (-0xC0 * player->velocity.x) >> 8;
                            player->groundVel  = player->velocity.x;
                            entity->timer      = 0;
                            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 5, &entity->animator, false, 0);
                            entity->state = SilverSonic_State2_Unknown6;
                        }
                    }
                    else {
                        if (abs(player->velocity.x) + abs(player->velocity.y) <= 0x40000) {
                            player->groundVel  = entity->velocity.x;
                            player->velocity.x = entity->velocity.x;
                            player->velocity.y = entity->velocity.y;
                            int angle          = RSDK.ATan2(entity->position.x - player->position.x, entity->position.y - player->position.y);
                            entity->velocity.x = RSDK.Cos256(angle) << 10;
                            entity->velocity.y = RSDK.Sin256(angle) << 10;
                        }
                        else {
                            entity->velocity.x = player->velocity.x;
                            entity->velocity.y = -abs(player->velocity.y);
                            player->velocity.x = (-0xC0 * player->velocity.x) >> 8;
                            player->groundVel  = player->velocity.x;
                            entity->timer      = 0;
                            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 5, &entity->animator, false, 0);
                            entity->state = SilverSonic_State2_Unknown6;
                        }
                    }

                    if (player->characterID == ID_KNUCKLES && player->playerAnimator.animationID == 48) {
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLYTIRED, &player->playerAnimator, false, 0);
                        player->state = Player_State_KnuxGlideDrop;
                    }
                    entity->invincibilityTimer = 8;
                    RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
                }
                else {
                    Player_CheckHit(player, entity);
                }
            }
        }
    }
}

void SilverSonic_CheckPlayerCollisions_Arm(void)
{
    RSDK_THIS(SilverSonic);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, entity->hitboxBadnikPtr)) {
            if (entity->direction) {
                if (player->position.x <= entity->position.x) {
                    Player_CheckHit(player, entity);
                }
                else if (Player_CheckBadnikBreak(entity, player, false)) {
                    int x = entity->position.x;
                    int y = entity->position.y;
                    RSDK.ResetEntityPtr(entity, MSBomb->objectID, NULL);
                    entity->position.x = x;
                    entity->position.y = y;
                    entity->velocity.y = -0x10000;
                    entity->field_60   = 2;
                    entity->state      = MSBomb_Unknown2;
                }
            }
            else {
                if (player->position.x >= entity->position.x) {
                    Player_CheckHit(player, entity);
                }
                else if (Player_CheckBadnikBreak(entity, player, false)) {
                    int x = entity->position.x;
                    int y = entity->position.y;
                    RSDK.ResetEntityPtr(entity, MSBomb->objectID, NULL);
                    entity->position.x = x;
                    entity->position.y = y;
                    entity->velocity.y = -0x10000;
                    entity->field_60   = 2;
                    entity->state      = MSBomb_Unknown2;
                }
            }
        }
    }
}

void SilverSonic_State_Unknown1(void)
{
    RSDK_THIS(SilverSonic);

    if (!entity->alpha)
        entity->direction = Player_GetNearestPlayerX()->position.x < entity->position.x;

    if (entity->alpha >= 256) {
        entity->inkEffect = INK_NONE;
        SilverSonic_HandleNextAttack();
    }
    else {
        entity->alpha += 8;
    }
}

void SilverSonic_State2_Unknown5(void)
{
    RSDK_THIS(SilverSonic);

    if (++entity->timer == 60) {
        entity->timer = 0;
        SilverSonic_HandleNextAttack();
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Unknown2(void)
{
    RSDK_THIS(SilverSonic);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 2, &entity->animator, false, 0);
        entity->state = SilverSonic_State_Unknown3;
    }
    SilverSonic_CheckPlayerCollisions_Arm();
}

void SilverSonic_State2_Unknown1(void)
{
    RSDK_THIS(SilverSonic);

    if (++entity->timer == 30) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 6, &entity->animator, false, 0);
        entity->state = SilverSonic_State2_Unknown2;
        RSDK.PlaySfx(SilverSonic->sfxDash, false, 255);
    }
}

void SilverSonic_State2_Unknown2(void)
{
    RSDK_THIS(SilverSonic);

    if (++entity->timer == 60) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 5, &entity->animator, false, 0);
        if (entity->direction)
            entity->velocity.x = -0x80000;
        else
            entity->velocity.x = 0x80000;
        if (Zone->timer & 2) {
            entity->groundVel = entity->velocity.x;
            entity->state     = SilverSonic_State2_Unknown3;
            RSDK.PlaySfx(SilverSonic->sfxBoost, false, 255);
        }
        else {
            entity->velocity.y = -0x68000;
            entity->onGround   = false;
            entity->state      = SilverSonic_State2_Unknown4;
            RSDK.PlaySfx(SilverSonic->sfxJump, false, 255);
        }
    }
    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State2_Unknown3(void)
{
    RSDK_THIS(SilverSonic);

    if (entity->position.x < Zone->screenBoundsL2[0] + 0x180000) {
        entity->velocity.x = 0x80000;
        entity->direction  = FLIP_NONE;
        entity->state      = SilverSonic_State2_Unknown4;
        entity->onGround   = false;
        entity->velocity.y = -0x68000;
        entity->position.x = Zone->screenBoundsL2[0] + 0x180000;
    }
    else if (entity->position.x > Zone->screenBoundsR2[0] - 0x180000) {
        entity->velocity.x = -0x80000;
        entity->direction  = FLIP_X;
        entity->state      = SilverSonic_State2_Unknown4;
        entity->onGround   = false;
        entity->velocity.y = -0x68000;
        entity->position.x = Zone->screenBoundsR2[0] - 0x180000;
    }
    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State2_Unknown4(void)
{
    RSDK_THIS(SilverSonic);

    entity->velocity.y += 0x3800;
    if (entity->position.x < Zone->screenBoundsL2[0] + 0x180000) {
        entity->velocity.x = 0;
        entity->position.x = Zone->screenBoundsL2[0] + 0x180000;
    }
    else if (entity->position.x > Zone->screenBoundsR2[0] - 0x180000) {
        entity->velocity.x = 0;
        entity->position.x = Zone->screenBoundsR2[0] - 0x180000;
    }

    if (entity->onGround) {
        entity->direction ^= FLIP_X;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &entity->animator, false, 0);
        entity->state = SilverSonic_State2_Unknown5;
    }
    else {
        SilverSonic_CheckPlayerCollisions_Ball();
    }
}

void SilverSonic_State2_Unknown6(void)
{
    RSDK_THIS(SilverSonic);

    entity->velocity.y += 0x1800;
    if (entity->position.x < Zone->screenBoundsL2[0] + 0x180000) {
        if (entity->velocity.x < 0) {
            entity->direction  = FLIP_NONE;
            entity->velocity.x = -entity->velocity.x;
            Camera_ShakeScreen(2, 0, 0);
            RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
        }
    }
    else if (entity->position.x > Zone->screenBoundsR2[0] - 0x180000) {
        if (entity->velocity.x > 0) {
            entity->direction  = FLIP_X;
            entity->velocity.x = -entity->velocity.x;
            Camera_ShakeScreen(2, 0, 0);
            RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
        }
    }

    if (entity->onGround) {
        if (abs(entity->groundVel) <= 0x40000) {
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &entity->animator, false, 0);
            entity->groundVel = 0;
            entity->state     = SilverSonic_State2_Unknown5;
        }
        else {
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 3, &entity->animator, false, 0);
            entity->state = SilverSonic_State_Unknown5;
        }
    }

    foreach_active(MetalSonic, metal)
    {
        if (RSDK.CheckObjectCollisionTouchBox(metal, metal->hitboxPtr, entity, entity->hitboxPtr)) {
            entity->velocity.x  = 0;
            entity->velocity.y  = 0;
            entity->state       = SilverSonic_State2_Explode;
            MetalSonic->field_8 = 32;
            Camera_ShakeScreen(-4, 0, 1);
            metal->field_70 -= 2;
            if (metal->field_70 <= 0) {
                metal->timer = 0;
                // metal->state = MetalSonic_Unknown29;
            }
        }
    }
    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State_Unknown3(void)
{
    RSDK_THIS(SilverSonic);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 3, &entity->animator, false, 0);
        entity->velocity.y = -0x30000;
        entity->onGround   = false;
        entity->state      = SilverSonic_State_Unknown4;
        RSDK.PlaySfx(SilverSonic->sfxJump, false, 255);
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Unknown4(void)
{
    RSDK_THIS(SilverSonic);

    if (entity->onGround) {
        entity->state = SilverSonic_State_Unknown5;
        if (entity->direction == FLIP_NONE)
            entity->groundVel = 0x60000;
        else
            entity->groundVel = -0x60000;
        RSDK.PlaySfx(SilverSonic->sfxBoost, false, 255);
    }
    else {
        entity->velocity.y += 0x2800;
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Unknown5(void)
{
    RSDK_THIS(SilverSonic);

    if (entity->position.x < Zone->screenBoundsL2[0] + 0x180000 || entity->position.x > Zone->screenBoundsR2[0] - 0x180000) {
        if (entity->position.x < Zone->screenBoundsL2[0] + 0x180000)
            entity->position.x = Zone->screenBoundsL2[0] + 0x180000;
        else
            entity->position.x = Zone->screenBoundsR2[0] - 0x180000;

        entity->velocity.x = 0;
        entity->velocity.y = -0x30000;
        entity->onGround   = false;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &entity->animator, false, 0);
        entity->state = SilverSonic_State_Unknown6;
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Unknown6(void)
{
    RSDK_THIS(SilverSonic);

    if (entity->onGround) {
        entity->state = SilverSonic_State2_Unknown5;
    }
    else {
        int vel = entity->velocity.y;
        entity->velocity.y += 0x2800;
        if (entity->velocity.y > 0 && vel <= 0)
            entity->direction = entity->position.x > Zone->screenBoundsL2[0] + 0x800000;
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State2_Explode(void)
{
    RSDK_THIS(SilverSonic);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);
        if (Zone->timer & 4) {
            int x = entity->position.x + RSDK.Rand(-0x100000, 0x100000);
            int y = entity->position.y + RSDK.Rand(-0x100000, 0x100000);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
    entity->visible = Zone->timer & 1;
    if (++entity->timer == 16)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void SilverSonic_EditorDraw(void)
{
    RSDK_THIS(SilverSonic);
    RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &entity->animator, false, 0);
    SilverSonic_Draw();
}

void SilverSonic_EditorLoad(void) { SilverSonic->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SilverSonic.bin", SCOPE_STAGE); }
#endif

void SilverSonic_Serialize(void) {}
