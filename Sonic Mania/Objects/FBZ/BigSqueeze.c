#include "SonicMania.h"

ObjectBigSqueeze *BigSqueeze;

void BigSqueeze_Update(void)
{
    RSDK_THIS(BigSqueeze);
    StateMachine_Run(entity->state);
}

void BigSqueeze_LateUpdate(void) {}

void BigSqueeze_StaticUpdate(void) {}

void BigSqueeze_Draw(void)
{
    RSDK_THIS(BigSqueeze);
    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void BigSqueeze_Create(void *data)
{
    RSDK_THIS(BigSqueeze);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active = ACTIVE_BOUNDS;
            switch (entity->type) {
                case 0:
                    entity->updateRange.x         = 0x800000;
                    entity->updateRange.y         = 0x800000;
                    entity->visible               = false;
                    entity->hitbox.left           = -256;
                    entity->hitbox.top            = 0;
                    entity->hitbox.right          = 256;
                    entity->hitbox.bottom         = 32;
                    entity->state                 = BigSqueeze_State1_SetupIntro;
                    break;
                case 1:
                    entity->updateRange.x         = 0x8000000;
                    entity->updateRange.y         = 0x1000000;
                    entity->visible               = true;
                    entity->drawOrder             = Zone->drawOrderHigh;
                    entity->hitbox.left           = -32;
                    entity->hitbox.top            = -16;
                    entity->hitbox.right          = 32;
                    entity->hitbox.bottom         = 16;
                    entity->timer                 = 6;
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 1, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 2, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 3, &entity->animator3, true, 0);
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 4, &entity->animator4, true, 0);
                    entity->stateDraw = BigSqueeze_StateDraw2_Unknown1;
                    break;
                case 2:
                case 3:
                    if (entity->type == 3) {
                        entity->direction = FLIP_X;
                        entity->drawFX    = FX_FLIP;
                    }
                    entity->updateRange.x = 0x8000000;
                    entity->updateRange.y = 0x1000000;
                    entity->visible       = 1;
                    entity->drawOrder     = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 0, &entity->animator1, true, 0);
                    entity->hitbox.left           = -24;
                    entity->hitbox.top            = -120;
                    entity->hitbox.right          = 24;
                    entity->hitbox.bottom         = 120;
                    entity->stateDraw             = BigSqueeze_StateDraw3_Unknown1;
                    break;
                default: return;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void BigSqueeze_StageLoad(void)
{
    BigSqueeze->active        = ACTIVE_ALWAYS;
    BigSqueeze->aniFrames     = RSDK.LoadSpriteAnimation("FBZ/BigSqueeze.bin", SCOPE_STAGE);
    BigSqueeze->sfxBossHit    = RSDK.GetSFX("Stage/BossHit.wav");
    BigSqueeze->sfxExplosion2 = RSDK.GetSFX("Stage/Explosion2.wav");
    BigSqueeze->sfxMagnet     = RSDK.GetSFX("FBZ/Magnet.wav");
    BigSqueeze->sfxOrbinaut   = RSDK.GetSFX("FBZ/Orbinaut.wav");
    BigSqueeze->value6        = 0;
    BigSqueeze->isRumbling    = false;
    Soundboard_LoadSFX("Stage/Rumble.wav", true, BigSqueeze_RumbleCheckCB, NULL);
}

bool32 BigSqueeze_RumbleCheckCB(void) { return BigSqueeze->isRumbling && SceneInfo->state == ENGINESTATE_REGULAR; }

void BigSqueeze_HandleWallCollisions(void)
{
    RSDK_THIS(BigSqueeze);
    foreach_active(Player, player)
    {
        int32 side = Player_CheckCollisionBox(player, entity, &entity->hitbox);
        if (side == 3)
            player->collisionFlagH |= 2;
        else if (side == 2)
            player->collisionFlagH |= 1;
    }

    foreach_active(SignPost, signPost)
    {
        if (signPost->state == SignPost_State_Fall) {
            if (signPost->velocity.x >= 0) {
                if (signPost->position.x > entity->position.x + ((entity->hitbox.left - 24) << 16) && signPost->position.x < entity->position.x) {
                    signPost->velocity.x = -signPost->velocity.x;
                }
            }
            else if (signPost->position.x < entity->position.x + ((entity->hitbox.right + 24) << 16) && signPost->position.x > entity->position.x) {
                signPost->velocity.x = -signPost->velocity.x;
            }
        }
    }
}

void BigSqueeze_CheckPlayerCollisions(void)
{
    RSDK_THIS(BigSqueeze);
    if (!entity->field_74) {
        if (entity->invincibilityTimer > 0)
            entity->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
                BigSqueeze_Hit();
            }
        }
    }
}

void BigSqueeze_CheckPlayerCollisions2(void)
{
    RSDK_THIS(BigSqueeze);
    if (!entity->field_74) {
        if (entity->invincibilityTimer > 0)
            entity->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox)) {
                if (player->invincibleTimer || player->blinkTimer > 0 || player->shield == SHIELD_LIGHTNING) {
                    if (Player_CheckBossHit(player, entity))
                        BigSqueeze_Hit();
                }
                else {
                    Player_CheckHit(player, entity);
                }
            }
        }
    }
}

void BigSqueeze_Hit(void)
{
    RSDK_THIS(BigSqueeze);
    if (--entity->timer <= 0) {
        foreach_active(BigSqueeze, boss)
        {
            if (boss->type == 2 || boss->type == 3)
                boss->state = BigSqueeze_HandleWallCollisions;
        }

        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator5, true, 0);

        entity->state               = BigSqueeze_State2_Die;
        entity->timer2              = 0;
        SceneInfo->timeEnabled = false;
        EntityPlayer *player        = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        Player_GiveScore(player, 1000);
    }
    else {
        entity->invincibilityTimer = 48;
        RSDK.PlaySfx(BigSqueeze->sfxBossHit, false, 255);
    }
}

void BigSqueeze_Unknown6(void)
{
    RSDK_THIS(BigSqueeze);
    if (BigSqueeze->value6 < 10)
        entity->position.x += entity->velocity.x;

    foreach_active(BigSqueeze, boss)
    {
        if (boss->type == 2) {
            if (entity->position.x < boss->position.x + 0x340000 && entity->velocity.x < 0)
                entity->velocity.x = -entity->velocity.x;
        }
        else if (boss->type == 3) {
            if (entity->position.x > boss->position.x - 0x340000 && entity->velocity.x > 0)
                entity->velocity.x = -entity->velocity.x;
        }
    }

    RSDK.ProcessAnimation(&entity->animator4);
}

// debrisData info:
// - count
// <for count entries>
// - frameID
// - velocityX
// - velocityY

void BigSqueeze_Unknown7(int32 *debrisData)
{
    RSDK_THIS(BigSqueeze);
    if (debrisData) {
        int32 count = debrisData[0];
        debrisData++;

        for (int32 i = 0; i < count; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 6, &debris->animator, true, debrisData[0]);
            debris->velocity.x    = debrisData[1];
            debris->velocity.y    = debrisData[2];
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x800000;
            debris->updateRange.y = 0x800000;

            debrisData += 3;
        }
    }
}

void BigSqueeze_StateDraw2_Unknown1(void)
{
    RSDK_THIS(BigSqueeze);
    Vector2 drawPos;

    if (entity->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 156, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 33, 0xE0E0E0);
    }
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator5, NULL, false);
    RSDK.SetPaletteEntry(0, 156, 0x202020);
    RSDK.SetPaletteEntry(0, 33, 0x303840);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    drawPos.x -= 0x140000;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x += 0x280000;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);
}

void BigSqueeze_StateDraw3_Unknown1(void)
{
    RSDK_THIS(BigSqueeze);
    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 1;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void BigSqueeze_State1_SetupIntro(void)
{
    RSDK_THIS(BigSqueeze);
    if (++entity->timer2 >= 8) {
        entity->timer2 = 0;
        foreach_all(CollapsingPlatform, platform)
        {
            platform->collapseDelay = 1;
            platform->active        = ACTIVE_NEVER;
        }
        foreach_active(Eggman, eggman) { eggman->direction = FLIP_X; }

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - 212;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 308;
        entity->state               = BigSqueeze_State1_SetupEggman;
    }
}

void BigSqueeze_State1_SetupEggman(void)
{
    RSDK_THIS(BigSqueeze);
    if (entity->timer2) {
        entity->timer2++;
        EntityEggman *eggmanPtr = (EntityEggman *)entity->eggman;
        if (entity->timer2 == 104)
            RSDK.SetSpriteAnimation(Eggman->aniFrames, 5, &eggmanPtr->animator, true, 0);
        if (entity->timer2 == 120) {
            entity->timer2        = 0;
            entity->state         = BigSqueeze_State1_SetupBossArena;
            eggmanPtr->onGround   = false;
            eggmanPtr->velocity.x = -0x10000;
            eggmanPtr->velocity.y = -0x40000;
            eggmanPtr->state      = Eggman_Unknown4;
            eggmanPtr->animID     = 2;
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++entity->timer2;
            foreach_active(LightBarrier, barrier) { barrier->enabled = true; }

            foreach_active(Eggman, eggman)
            {
                entity->eggman = (Entity *)eggman;
                RSDK.SetSpriteAnimation(Eggman->aniFrames, 2, &eggman->animator, true, 0);
                eggman->state  = Eggman_Unknown2;
                eggman->animID = 0;
            }
        }
    }
}

void BigSqueeze_State1_SetupBossArena(void)
{
    RSDK_THIS(BigSqueeze);
    EntityEggman *eggman = (EntityEggman *)entity->eggman;
    if (eggman->state == Eggman_Unknown1) {
        foreach_all(CollapsingPlatform, platform) { platform->active = ACTIVE_BOUNDS; }

        Zone->screenBoundsR1[0] = (entity->position.x >> 16) + 212;
        Zone->screenBoundsB1[0] = 1792;
        Zone->screenBoundsT1[0] = Zone->screenBoundsB1[0] - ScreenInfo->height;

        BigSqueeze->value7 = (Zone->screenBoundsB1[0] - 16) << 16;
        eggman->state      = Eggman_Unknown2;
        eggman->animID     = 0;
        entity->state      = BigSqueeze_State1_SetupBoss;
    }
}

void BigSqueeze_State1_SetupBoss(void)
{
    RSDK_THIS(BigSqueeze);
    if (++entity->timer2 == 48) {
        foreach_active(BigSqueeze, boss)
        {
            switch (boss->type) {
                case 1:
                    boss->field_74   = 1;
                    boss->state      = BigSqueeze_State2_Unknown1;
                    boss->velocity.x = -0x10000;
                    break;
                case 2:
                    boss->state      = BigSqueeze_State3_Unknown1;
                    boss->velocity.x = 0x8000;
                    boss->hitbox.top = -0x7FFF;
                    break;
                case 3:
                    boss->state      = BigSqueeze_State3_Unknown1;
                    boss->velocity.x = -0x8000;
                    boss->hitbox.top = -0x7FFF;
                    break;
            }
        }
        entity->state = BigSqueeze_State1_Unknown5;
    }
}

void BigSqueeze_State2_Unknown1(void)
{
    RSDK_THIS(BigSqueeze);

    if (entity->timer2 > 30)
        BigSqueeze_Unknown6();
    if (++entity->timer2 == 180) {
        entity->field_74 = 0;
        entity->timer2   = 0;
        RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 5, &entity->animator5, true, 0);
        entity->state = BigSqueeze_State2_Unknown2;
    }
    BigSqueeze_CheckPlayerCollisions();
}

void BigSqueeze_State2_Unknown2(void)
{
    RSDK_THIS(BigSqueeze);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator5);
    if (!entity->timer2)
        RSDK.PlaySfx(BigSqueeze->sfxOrbinaut, false, 255);
    if (++entity->timer2 == 16)
        FBZTrash_Unknown2(entity->position.x, entity->position.y + 0x300000);
    if (entity->timer2 == 120) {
        entity->timer2 = 0;
        RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 2, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator5, true, 0);
        entity->state = BigSqueeze_State2_Unknown1;
    }
    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(BigSqueeze->sfxMagnet, false, 255);
    BigSqueeze_CheckPlayerCollisions2();
}

void BigSqueeze_State2_Die(void)
{
    RSDK_THIS(BigSqueeze);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(BigSqueeze->sfxExplosion2, false, 255);
        if (Zone->timer & 4) {
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2),
                          (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16) + entity->position.x,
                          (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16) + entity->position.y)
                ->drawOrder = Zone->drawOrderHigh;
        }
    }

    ++entity->timer2;
    if (entity->type == 1) {
        switch (entity->timer2) {
            case 20:
                BigSqueeze_Unknown7(BigSqueeze->value1);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
                break;
            case 40:
                BigSqueeze_Unknown7(BigSqueeze->value2);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
                break;
            case 60:
                BigSqueeze_Unknown7(BigSqueeze->value3);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
                break;
            case 80:
                Music_TransitionTrack(TRACK_STAGE, 0.0125);
                entity->timer2  = 0;
                entity->visible = false;
                entity->state   = BigSqueeze_State2_SpawnSignPost;
                foreach_active(FBZTrash, trash)
                {
                    if (trash->state != FBZTrash_Unknown3) {
                        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), trash->position.x, trash->position.y)->drawOrder = Zone->drawOrderHigh;
                        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
                        destroyEntity(trash);
                    }
                }
                break;
        }
    }
    else if (entity->timer == 80) {
        destroyEntity(entity);
    }
}

void BigSqueeze_State2_SpawnSignPost(void)
{
    RSDK_THIS(BigSqueeze);
    BigSqueeze->isRumbling = false;
    if (++entity->timer2 == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = entity->position.x;
            signPost->active     = ACTIVE_NORMAL;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }

        entity->state = StateMachine_None;
    }
}

void BigSqueeze_Unknown18(void)
{
    RSDK_THIS(BigSqueeze);
    entity->position.y = BigSqueeze->value7;
}

void BigSqueeze_State1_Unknown5(void)
{
    RSDK_THIS(BigSqueeze);
    entity->position.y = BigSqueeze->value7;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox)) {
            if (abs(player->groundVel) > 0x20000 && !(Zone->timer & 7)) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, player->position.x + RSDK.Rand(-0x40000, 0x40000),
                                                     player->position.y + 0x40000 + RSDK.Rand(-0x40000, 0x40000));

                RSDK.SetSpriteAnimation(FBZTrash->aniFrames, RSDK.Rand(0, 2) + 9, &debris->animator, false, 0);
                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y    = -0x20000;
                debris->gravity       = 0x3800;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x200000;
                debris->updateRange.y = 0x200000;
            }
        }
    }

    foreach_active(FBZSinkTrash, sinkTrash)
    {
        sinkTrash->position.y = BigSqueeze->value7 + (sinkTrash->size.y >> 1);
        sinkTrash->size.x     = BigSqueeze->value4[3] - BigSqueeze->value4[2];
    }
}

void BigSqueeze_State3_Unknown1(void)
{
    RSDK_THIS(BigSqueeze);
    BigSqueeze_HandleWallCollisions();
    BigSqueeze->value4[entity->type] = entity->position.x;
    if (++entity->timer2 == 8) {
        BigSqueeze->isRumbling = false;
    }

    if (entity->timer2 == 300) {
        entity->timer2         = 0;
        entity->state          = BigSqueeze_State3_Unknown2;
        BigSqueeze->isRumbling = true;
        if (entity->velocity.x > 0) {
            ++BigSqueeze->value6;
        }
    }
}

void BigSqueeze_State3_Unknown2(void)
{
    RSDK_THIS(BigSqueeze);
    BigSqueeze_HandleWallCollisions();
    entity->position.x += entity->velocity.x;
    BigSqueeze->value4[entity->type] = entity->position.x;

    if (!(Zone->timer & 3))
        Camera_ShakeScreen(0, 0, 4);
    if (entity->type == 2) {
        BigSqueeze->value7 -= 0x4000;
    }

    if (++entity->timer2 == 32) {
        if (BigSqueeze->value6 < 10) {
            entity->timer2 = 0;
            entity->state  = BigSqueeze_State3_Unknown1;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void BigSqueeze_EditorDraw(void) {}

void BigSqueeze_EditorLoad(void) {}
#endif

void BigSqueeze_Serialize(void) { RSDK_EDITABLE_VAR(BigSqueeze, VAR_ENUM, type); }
