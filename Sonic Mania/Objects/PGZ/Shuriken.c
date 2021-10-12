#include "SonicMania.h"

ObjectShuriken *Shuriken;

void Shuriken_Update(void)
{
    RSDK_THIS(Shuriken);
    if (entity->state) {
        StateMachine_Run(entity->state);
        if (entity->animator.animationID == 1 || entity->animator.animationID == 2 || entity->animator.animationID == 4)
            RSDK.ProcessAnimation(&entity->animator);
    }
}

void Shuriken_LateUpdate(void) {}

void Shuriken_StaticUpdate(void)
{
    EntityShuriken *groups[255];
    memset(groups, 0, sizeof(groups));

    foreach_active(Shuriken, shuriken)
    {
        if (shuriken->timerGroup) {
            if (!groups[shuriken->timerGroup - 1]) {
                groups[shuriken->timerGroup - 1] = shuriken;
                ++Shuriken->timerGroups[shuriken->timerGroup - 1];
            }
        }
    }

    for (int i = 0; i < 0xFF; ++i) {
        if (groups[i] == NULL && Shuriken->timerGroups[i])
            Shuriken->timerGroups[i] = 0;
    }
}

void Shuriken_Draw(void)
{
    RSDK_THIS(Shuriken);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Shuriken_Create(void *data)
{
    RSDK_THIS(Shuriken);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = Shuriken_State_Setup;
}

void Shuriken_StageLoad(void)
{
    Shuriken->aniFrames      = RSDK.LoadSpriteAnimation("PSZ2/Shuriken.bin", SCOPE_STAGE);
    Shuriken->hitbox1.top    = -8;
    Shuriken->hitbox1.left   = -16;
    Shuriken->hitbox1.right  = 16;
    Shuriken->hitbox1.bottom = 8;
    Shuriken->hitbox2.top    = -256;
    Shuriken->hitbox2.left   = -64;
    Shuriken->hitbox2.right  = 64;
    Shuriken->hitbox2.bottom = 256;
    Shuriken->hitbox3.top    = -3;
    Shuriken->hitbox3.left   = -16;
    Shuriken->hitbox3.right  = 16;
    Shuriken->hitbox3.bottom = 5;
    Shuriken->hitbox4.top    = 0;
    Shuriken->hitbox4.left   = -16;
    Shuriken->hitbox4.right  = 16;
    Shuriken->hitbox4.bottom = 8;
    Shuriken->sfxArrowLaunch = RSDK.GetSFX("PSZ/ArrowLaunch.wav");
    Shuriken->sfxArrowHit    = RSDK.GetSFX("PSZ/ArrowHit.wav");
}

void Shuriken_CheckPlayerCollisions(void)
{
    RSDK_THIS(Shuriken);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Shuriken->hitbox3)) {
#if RETRO_USE_PLUS
            int anim = player->playerAnimator.animationID;
            if (player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)) {
                if (!player->uncurlTimer) {
                    RSDK.PlaySfx(Player->sfx_PimPom, false, 255);
                    player->uncurlTimer = 30;
                }
                int angle           = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                entity->velocity.x  = -0x400 * RSDK.Cos256(angle);
                entity->velocity.y  = -0x600 * RSDK.Sin256(angle);
                entity->interaction = false;
                entity->state       = Shuriken_Unknown14;
            }
            else {
#endif
                Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}

void Shuriken_HandleSolidCollisions(void)
{
    RSDK_THIS(Shuriken);
    entity->activePlayers = 0;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, &Shuriken->hitbox4)) {
            if (!player->onGround && !player->sidekick && !entity->dropTimer)
                entity->dropTimer = 1;
            entity->activePlayers |= 1 << RSDK.GetEntityID(player);
            player->position.y &= 0xFFFF0000;
        }
    }
}

void Shuriken_State_Setup(void)
{
    RSDK_THIS(Shuriken);
    entity->timer  = 0;
    entity->active = ACTIVE_BOUNDS;
    RSDK.SetSpriteAnimation(Shuriken->aniFrames, 0, &entity->animator, true, 0);
    entity->drawOrder = Zone->drawOrderHigh;
    entity->state     = Shuriken_Unknown4;
    Shuriken_Unknown4();
}

void Shuriken_Unknown4(void)
{
    RSDK_THIS(Shuriken);
    if (entity->timerMode) {
        int timer = 0;
        if (entity->timerGroup)
            timer = Shuriken->timerGroups[(entity->timerGroup - 1)];
        else
            timer = Zone->timer;

        if (!((timer + entity->intervalOffset) % entity->interval)) {
            RSDK.SetSpriteAnimation(Shuriken->aniFrames, 2, &entity->animator, false, 0);
            entity->state = Shuriken_Unknown7;
            entity->timer = 0;
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (!player->sidekick && Player_CheckCollisionTouch(player, entity, &Shuriken->hitbox2)) {
                entity->active = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Shuriken->aniFrames, 1, &entity->animator, false, 0);
                entity->state = Shuriken_Unknown5;
            }
        }
    }
}

void Shuriken_Unknown5(void)
{
    RSDK_THIS(Shuriken);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (!player->sidekick && Player_CheckCollisionTouch(player, entity, &Shuriken->hitbox2)) {
            flag = true;
            foreach_break;
        }
    }

    if (!flag) {
        entity->timer = entity->delay;
        entity->state = Shuriken_Unknown6;
    }
}

void Shuriken_Unknown6(void)
{
    RSDK_THIS(Shuriken);
    if (entity->timer) {
        entity->timer--;
    }
    else {
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 2, &entity->animator, false, 0);
        entity->state = Shuriken_Unknown7;
        Shuriken_Unknown7();
    }
}

void Shuriken_Unknown7(void)
{
    RSDK_THIS(Shuriken);
    if (entity->timer >= 15) {
        RSDK.PlaySfx(Shuriken->sfxArrowLaunch, false, 255);
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 3, &entity->animator, false, 0);
        entity->state            = Shuriken_Unknown8;
        EntityShuriken *shuriken = CREATE_ENTITY(Shuriken, entity, entity->position.x, entity->position.y);
        shuriken->state          = Shuriken_Unknown10;
        shuriken->direction      = entity->direction;
        shuriken->active         = ACTIVE_NORMAL;
        if (entity->direction == FLIP_NONE)
            shuriken->velocity.x = 0x40000;
        else
            shuriken->velocity.x = -0x40000;
    }
    else {
        entity->timer++;
    }
}

void Shuriken_Unknown8(void)
{
    RSDK_THIS(Shuriken);

    bool32 flag = false;
    foreach_active(Shuriken, shuriken)
    {
        if (shuriken->state == Shuriken_Unknown11) {
            if (RSDK.CheckObjectCollisionTouchBox(entity, &Shuriken->hitbox1, shuriken, &Shuriken->hitbox3)) {
                flag = true;
                foreach_break;
            }
        }
    }

    if (!flag) {
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 4, &entity->animator, false, 0);
        entity->state = Shuriken_Unknown9;
        entity->timer = 0;
    }
}

void Shuriken_Unknown9(void)
{
    RSDK_THIS(Shuriken);
    if (entity->timer >= 10)
        entity->state = Shuriken_State_Setup;
    else
        entity->timer++;
}

void Shuriken_Unknown10(void)
{
    RSDK_THIS(Shuriken);
    entity->timer     = 0;
    entity->dropTimer = 0;
    entity->active    = ACTIVE_NORMAL;
    entity->drawOrder = Zone->drawOrderHigh - 1;
    RSDK.SetSpriteAnimation(Shuriken->aniFrames, 5, &entity->animator, true, 0);
    entity->state = Shuriken_Unknown11;
    Shuriken_Unknown11();
}

void Shuriken_Unknown11(void)
{
    RSDK_THIS(Shuriken);
    RSDK.ProcessAnimation(&entity->animator);
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
    Shuriken_CheckPlayerCollisions();
    entity->position.x += entity->velocity.x;

    bool32 flag = false;
    if (entity->direction)
        flag = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_RWALL, 0, -0xE0000, 0, 2);
    else
        flag = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_LWALL, 0, 0xE0000, 0, 2);
    if (flag) {
        foreach_active(Shuriken, shuriken)
        {
            if (shuriken != entity) {
                if (RSDK.CheckObjectCollisionTouchBox(entity, &Shuriken->hitbox3, shuriken, &Shuriken->hitbox3)) {
                    shuriken->velocity.y = 0;
                    shuriken->state      = Shuriken_Unknown13;
                }
            }
        }
        entity->velocity.x = 0;
        RSDK.PlaySfx(Shuriken->sfxArrowHit, false, 255);
        entity->state = Shuriken_Unknown12;
    }
}

void Shuriken_Unknown12(void)
{
    RSDK_THIS(Shuriken);
    if (entity->timer < 300)
        entity->timer++;

    if (entity->dropTimer > 0 && entity->dropTimer < 60)
        entity->dropTimer++;
    Shuriken_HandleSolidCollisions();

    bool32 flag = false;
    if (entity->direction)
        flag = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_RWALL, 0, -0xE0000, 0, 2);
    else
        flag = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_LWALL, 0, 0xE0000, 0, 2);
    if (!flag || entity->timer >= 300 || entity->dropTimer >= 60) {
        entity->updateRange.x = 0x4000000;
        entity->updateRange.y = 0x4000000;
        entity->velocity.y    = 0;
        entity->state         = Shuriken_Unknown13;
    }
}

void Shuriken_Unknown13(void)
{
    RSDK_THIS(Shuriken);
    if (entity->velocity.y >= 0x40000)
        entity->velocity.y = 0x40000;
    else
        entity->velocity.y += 0x4000;
    entity->position.y += entity->velocity.y;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void Shuriken_Unknown14(void)
{
    RSDK_THIS(Shuriken);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->visible ^= 1;
    entity->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void Shuriken_EditorDraw(void) {}

void Shuriken_EditorLoad(void) {}

void Shuriken_Serialize(void)
{
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, delay);
    RSDK_EDITABLE_VAR(Shuriken, VAR_BOOL, timerMode);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT8, timerGroup);
}
