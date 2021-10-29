#include "SonicMania.h"

ObjectHeavyMystic *HeavyMystic;

void HeavyMystic_Update(void)
{
    RSDK_THIS(HeavyMystic);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    if (entity->timer2) {
        entity->timer2--;
        HeavyMystic_Unknown2();
    }

    StateMachine_Run(entity->state);

    entity->position.x += TornadoPath->field_8.x;
    entity->position.y += TornadoPath->field_8.y;
}

void HeavyMystic_LateUpdate(void) {}

void HeavyMystic_StaticUpdate(void)
{
    foreach_active(HeavyMystic, mystic)
    {
        if (mystic->type == 2) {
            RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(mystic));
        }
    }
}

void HeavyMystic_Draw(void)
{
    RSDK_THIS(HeavyMystic);
    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        if (entity->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 158, 0xE0E0E0);
        RSDK.DrawSprite(&entity->animator, NULL, false);
        RSDK.SetPaletteEntry(0, 158, 0x000000);
    }
}

void HeavyMystic_Create(void *data)
{
    RSDK_THIS(HeavyMystic);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            if (data)
                entity->type = voidToInt(data);
            switch (entity->type) {
                case MYSTIC_MISCHIEF:
                    entity->active    = ACTIVE_BOUNDS;
                    entity->visible   = false;
                    entity->drawOrder = Zone->drawOrderLow + 2;
                    entity->drawFX    = FX_FLIP;
                    entity->health    = 6;
                    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
                    entity->state = HeavyMystic_State0_Unknown1;
                    break;
                case MYSTIC_BOSS:
                    if (globals->gameMode >= MODE_TIMEATTACK) {
                        destroyEntity(entity);
                    }
                    else {
                        entity->active              = ACTIVE_BOUNDS;
                        entity->visible             = false;
                        entity->drawOrder           = Zone->drawOrderLow + 1;
                        entity->drawFX              = FX_FLIP;
                        entity->hitbox.left         = -22;
                        entity->hitbox.top          = -22;
                        entity->hitbox.right        = 22;
                        entity->hitbox.bottom       = 22;
                        entity->health              = 8;
                        HeavyMystic->curtainLinePos = 0xD00000;
                        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
                        entity->state = HeavyMystic_State1_Unknown1;
                    }
                    break;
                case MYSTIC_BOX:
                    if (globals->gameMode >= MODE_TIMEATTACK) {
                        destroyEntity(entity);
                    }
                    else {
                        entity->scale.x   = 0x200;
                        entity->scale.y   = 0x200;
                        entity->active    = ACTIVE_BOUNDS;
                        entity->visible   = true;
                        entity->drawOrder = Zone->drawOrderLow;
                        entity->targetPos  = entity->position;
                        entity->drawFX    = FX_SCALE | FX_ROTATE | FX_FLIP;
                        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &entity->animator, true, 0);
                        entity->state     = HeavyMystic_State2_Unknown1;
                        entity->stateDraw = HeavyMystic_StateDraw2_Unknown1;
                    }
                    break;
                case MYSTIC_CORK:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow;
                    entity->drawFX    = FX_FLIP;
                    RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 4, &entity->animator, true, 0);
                    entity->state               = HeavyMystic_State3_Unknown1;
                    entity->hitbox.left = -3;
                    entity->hitbox.top = -3;
                    entity->hitbox.right = 3;
                    entity->hitbox.bottom = 3;
                    break;
                case MYSTIC_BOMB:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow;
                    entity->drawFX    = FX_FLIP;
                    RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 11, &entity->animator, true, 0);
                    entity->state         = HeavyMystic_State4_Unknown1;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 8;
                    break;
                case MYSTIC_DEBRIS:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow;
                    entity->drawFX    = FX_FLIP;
                    RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 16, &entity->animator, true, 0);
                    entity->state         = HeavyMystic_State5_Unknown1;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 8;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void HeavyMystic_StageLoad(void)
{
    HeavyMystic->aniFrames      = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
    HeavyMystic->rouguesFrames  = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);
    HeavyMystic->active         = ACTIVE_ALWAYS;
    HeavyMystic->curtainLinePos = 0;
    HeavyMystic->sfxHit         = RSDK.GetSFX("Stage/BossHit.wav");
    HeavyMystic->sfxExplosion   = RSDK.GetSFX("Stage/Explosion2.wav");
    HeavyMystic->sfxMagicBox    = RSDK.GetSFX("MSZ/MagicBox.wav");
    HeavyMystic->sfxClack       = RSDK.GetSFX("Stage/Clack.wav");
    HeavyMystic->sfxImpact      = RSDK.GetSFX("Stage/Impact3.wav");
    HeavyMystic->sfxPowerDown   = RSDK.GetSFX("Stage/PowerDown.wav");
    HeavyMystic->sfxPon         = RSDK.GetSFX("Stage/Pon.wav");
    HeavyMystic->sfxDrop        = RSDK.GetSFX("Stage/Drop.wav");
    HeavyMystic->sfxImpact2     = RSDK.GetSFX("Stage/Impact3.wav");
    HeavyMystic->sfxAssemble    = RSDK.GetSFX("Stage/Assemble.wav");
    HeavyMystic->sfxPoof        = RSDK.GetSFX("MSZ/MysticPoof.wav");
    HeavyMystic->sfxTwinkle     = RSDK.GetSFX("MSZ/MysticTwinkle.wav");
    HeavyMystic->sfxHat         = RSDK.GetSFX("MSZ/MysticHat.wav");
    HeavyMystic->sfxHatNode     = RSDK.GetSFX("MSZ/MysticHatNode.wav");
    HeavyMystic->sfxBleeps      = RSDK.GetSFX("MSZ/MysticBleeps.wav");
    HeavyMystic->sfxAppear1     = RSDK.GetSFX("MSZ/MysticAppearAct1.wav");
    HeavyMystic->sfxTransform   = RSDK.GetSFX("MSZ/MysticTransform.wav");
}

void HeavyMystic_Unknown1(int32 x, int32 y)
{
    int32 velX = RSDK.Rand(-0xC000, 0xC000);
    int32 velY = RSDK.Rand(-0xC000, 0xC000);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, x, y);
    debris->drawOrder    = Zone->drawOrderLow;
    debris->drawFX       = FX_FLIP;
    debris->direction    = RSDK.Rand(0, 4);
    debris->velocity.x   = velX;
    debris->velocity.y   = velY;
    debris->inkEffect    = INK_SUB;
    debris->alpha        = 0xE0;
    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, RSDK.Rand(0, 2) + 5, &debris->animator, true, 0);
    debris->timer = 3 * debris->animator.frameCount;

    debris             = CREATE_ENTITY(Debris, Debris_State_LightningSpark, x, y);
    debris->velocity.x = velX;
    debris->velocity.y = velY;
    debris->drawOrder  = Zone->drawOrderLow;
    debris->inkEffect  = INK_ADD;
    debris->alpha      = 0xFF;
    RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, RSDK.Rand(0, 2) + 7, &debris->animator, true, 0);
    debris->timer = 3 * debris->animator.frameCount;
}

void HeavyMystic_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);
    if (!(Zone->timer & 7)) {
        HeavyMystic_Unknown1(entity->position.x + RSDK.Rand(-0x200000, -0x100000), entity->position.y + RSDK.Rand(-0x100000, 0));
        HeavyMystic_Unknown1(entity->position.x + RSDK.Rand(0x100000, 0x200000), entity->position.y + RSDK.Rand(-0x100000, 0));
    }
}

void HeavyMystic_CheckPlayerCollisions(void)
{
    RSDK_THIS(HeavyMystic);
    if (!entity->timer2 && !entity->invincibilityTimer) {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
                HeavyMystic_Hit();
                foreach_break;
            }
        }
    }
}

void HeavyMystic_Hit(void)
{
    RSDK_THIS(HeavyMystic);
    --entity->health;
    --entity->rougeHealth;
    if (entity->health <= 0) {
        RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
        entity->timer               = 120;
        RSDK_sceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        entity->state = HeavyMystic_State_Destroyed;
    }
    else {
        entity->timer -= 30;
        entity->invincibilityTimer = 48;
        RSDK.PlaySfx(HeavyMystic->sfxHit, false, 255);
    }
}

void HeavyMystic_CheckPlayerCollisions2(void)
{
    RSDK_THIS(HeavyMystic);
    if (!entity->timer2) {
        if (entity->invincibilityTimer > 0)
            entity->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
                RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);
                if (player->position.x < entity->position.x)
                    entity->velocity.x = 0x20000;
                else
                    entity->velocity.x = -0x20000;
                entity->velocity.y = -0x40000;
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 3, &entity->animator, true, 0);
                entity->state = HeavyMystic_State_RougeHit;
            }
        }
    }
}

void HeavyMystic_CheckPlayerCollisions3(void)
{
    RSDK_THIS(HeavyMystic);
    if (!entity->timer2) {
        if (entity->invincibilityTimer > 0)
            entity->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS

                if (entity->animator.animationID == ANI_SKIDTURN
                    && ((entity->direction == FLIP_NONE && player->position.x > entity->position.x)
                        || (entity->direction == FLIP_X && player->position.x < entity->position.x))) {
                    if (!Player_CheckMightyUnspin(0x600, player, false, &player->uncurlTimer))
                        Player_CheckHit(player, entity);
                }
                else
#endif
                    if (Player_CheckBossHit(player, entity)) {
                    RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);
                    if (entity->invincibilityTimer > 0)
                        entity->invincibilityTimer--;
                    RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 15, &entity->animator, true, 0);
                    entity->state = HeavyMystic_State_RougeHit;
                }
            }
        }
    }
}

void HeavyMystic_CheckPlayerCollisions4(void)
{
    RSDK_THIS(HeavyMystic);
    if (!entity->timer2) {

        if (entity->invincibilityTimer > 0)
            entity->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
                RSDK.PlaySfx(HeavyMystic->sfxImpact, false, 255);

                if (player->position.x < entity->position.x)
                    entity->velocity.x = 0x20000;
                else
                    entity->velocity.x = -0x20000;
                entity->velocity.y = -0x40000;
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 9, &entity->animator, true, 0);
                entity->state = HeavyMystic_State_RougeHit;
            }
        }
    }
}

void HeavyMystic_Explode(void)
{
    RSDK_THIS(HeavyMystic);
    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(UberCaterkiller->sfxExplosion2, false, 255);

        if (!(Zone->timer & 7)) {
            int x = RSDK.Rand(-19, 20) << 16;
            int y = RSDK.Rand(-24, 25) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x + entity->position.x, y + entity->position.y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void HeavyMystic_ScanlineCB(ScanlineInfo *scanlines)
{
    int curtainPos = HeavyMystic->curtainLinePos;
    int move       = (0x1100000 - curtainPos) >> 6;
    int max        = maxVal(curtainPos - 0x400000, 0x100000);

    int posY = (RSDK_screens->position.y + 207) << 16;
    if (curtainPos > max) {
        int id = HeavyMystic->curtainLinePos >> 16;

        for (int i = 0; i < ((curtainPos - max - 1) >> 16) + 1; ++i) {
            scanlines[id--].position.y = posY;
            posY -= move;
        }
    }

    for (int i = HeavyMystic->curtainLinePos >> 16; i < 208; ++i) {
        scanlines[i].position.y = 0x6200000;
    }
}

void HeavyMystic_State0_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&entity->animator);
    if (++entity->timer >= 8) {
        entity->velocity.x = 0x20000;
        if (StarPost->postIDs[0])
            HeavyMystic->curtainLinePos = 1;
        entity->targetPos.x = entity->position.x;
        entity->targetPos.y = entity->position.y;
        entity->position.y += 0xC00000;
        entity->visible    = true;
        entity->timer      = 120;
        entity->state      = HeavyMystic_State0_Unknown2;
    }
}

void HeavyMystic_State1_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    if (++entity->timer >= 8) {
        entity->timer        = 0;
        HeavyMystic->boundsL = (Zone->screenBoundsL1[0] + 64) << 16;
        HeavyMystic->boundsR = (Zone->screenBoundsR1[0] - 64) << 16;
        HeavyMystic->boundsM = entity->position.x;
        HeavyMystic->boundsT = (Zone->screenBoundsT1[0] + 48) << 16;
        HeavyMystic->boundsB = (Zone->screenBoundsB1[0] - 8) << 16;
        entity->state        = HeavyMystic_State1_Unknown2;
    }
}

void HeavyMystic_State1_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);

    if (entity->timer) {
        entity->timer++;
        if (entity->timer == 120) {
            Zone->screenBoundsT1[0] = Zone->screenBoundsB1[0] - RSDK_screens->height;
            MSZSetup_Unknown1(entity->position.x + 0x4000000, 0);
            entity->position.y += 0x500000;
            entity->targetPos.x                                 = entity->position.x;
            entity->targetPos.y                                 = entity->position.y;
            entity->timer                                      = 0;
            entity->inkEffect                                  = INK_ALPHA;
            entity->visible                                    = true;
            entity->state                                      = HeavyMystic_State1_Unknown3;
            RSDK.GetSceneLayer(Zone->fgHigh)->scanlineCallback = HeavyMystic_ScanlineCB;

            foreach_active(MSZSpotlight, spotlight) { spotlight->state = MSZSpotlight_Unknown1; }
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.y > entity->position.y) {
            Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);
            Zone->playerBoundActiveL[0] = true;
            Zone->playerBoundActiveR[0] = true;
            Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
            Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
            Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 256;
            ++entity->timer;
            entity->active = ACTIVE_NORMAL;
        }
    }
}

void HeavyMystic_State0_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();

    if (entity->timer > 0) {
        entity->timer--;
    }
    else {
        entity->velocity.y -= 0x1800;
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
    }
    if (entity->timer == 50)
        RSDK.PlaySfx(HeavyMystic->sfxAppear1, false, 255);

    if (entity->position.y <= entity->targetPos.y)
        entity->state = HeavyMystic_State0_Unknown3;
}

void HeavyMystic_State0_Unknown3(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();
    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->position.y >= entity->targetPos.y) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->position.y = entity->targetPos.y;
        RSDK.PlaySfx(HeavyMystic->sfxBleeps, false, 255);
        entity->state = HeavyMystic_State0_Unknown4;
    }
}

void HeavyMystic_State1_Unknown3(void)
{
    RSDK_THIS(HeavyMystic);

    if (RSDK_sceneInfo->entity->alpha > 0xC0)
        HeavyMystic_Unknown2();

#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
        if (entity->timer < 256) {
            entity->timer += 3;
            RSDK.SetLimitedFade(0, 1, 5, entity->timer, 128, 255);
        }
    }
#endif

    if (HeavyMystic->curtainLinePos <= 0x100000) {
        entity->timer     = 0;
        entity->inkEffect = INK_NONE;
        entity->state     = HeavyMystic_State1_Unknown4;
    }
    else {
        HeavyMystic->curtainLinePos -= 0x20000;
    }
}

void HeavyMystic_State0_Unknown7(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();
}

void HeavyMystic_State0_Unknown4(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += 0x8000;

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();

    if (++entity->timer == 90) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 1, &entity->animator, true, 0);
        RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);
        entity->state = HeavyMystic_State0_Unknown5;
    }
}

void HeavyMystic_State0_Unknown9(void)
{
    RSDK_THIS(HeavyMystic);

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();

    if (++entity->timer == 60) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 3, &entity->animator, true, 0);
        RSDK.PlaySfx(HeavyMystic->sfxTransform, false, 255);
        entity->state = HeavyMystic_State0_Unknown10;
    }
}

void HeavyMystic_State0_Unknown6(void)
{
    RSDK_THIS(HeavyMystic);

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();

    if (++entity->timer == 60) {
        if (RSDK.CheckStageFolder("MSZCutscene")) {
            entity->state = HeavyMystic_State0_Unknown7;
        }
        else {
            entity->timer = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 2, &entity->animator, true, 0);
            RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);
            entity->state = HeavyMystic_State0_Unknown8;
        }
    }
}

void HeavyMystic_State0_Unknown5(void)
{
    RSDK_THIS(HeavyMystic);

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
        entity->state = HeavyMystic_State0_Unknown6;
    }
}

void HeavyMystic_State0_Unknown8(void)
{
    RSDK_THIS(HeavyMystic);

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();
    if (entity->animator.frameID == 8 && !entity->timer) {
        RSDK.PlaySfx(HeavyMystic->sfxHatNode, false, 255);
        CREATE_ENTITY(Hatterkiller, intToVoid(RSDK.Rand(-0x20000, 0x20000)), entity->position.x + 0x320000, entity->position.y);
        entity->timer = 1;
    }

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
        entity->state = HeavyMystic_State0_Unknown9;
    }
}

void HeavyMystic_State0_Unknown10(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        if (!HeavyMystic->curtainLinePos) {
            HeavyMystic->curtainLinePos = 1;
            CREATE_ENTITY(Vultron, intToVoid(1), entity->position.x, entity->position.y);
            CREATE_ENTITY(Vultron, intToVoid(1), entity->position.x, entity->position.y);
        }
        RSDK.PlaySfx(HeavyMystic->sfxPoof, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
        destroyEntity(entity);
    }
}

void HeavyMystic_State_Destroyed(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Explode();
    if (--entity->timer <= 0) {
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
        entity->state = HeavyMystic_State_Finish;
    }
}

void HeavyMystic_State_Finish(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();

    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x3800;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        entity->state = HeavyMystic_State_CloseCurtains;
    }
}

void HeavyMystic_State_CloseCurtains(void)
{
    RSDK_THIS(HeavyMystic);

#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
        if (entity->timer < 256) {
            entity->timer += 3;
            RSDK.SetLimitedFade(0, 5, 1, entity->timer, 128, 255);
        }
    }
#endif

    if (HeavyMystic->curtainLinePos >= 0xD00000) {
        HeavyMystic->curtainLinePos                        = 0xD00000;
        RSDK.GetSceneLayer(Zone->fgHigh)->scanlineCallback = 0;
        Zone->screenBoundsR1[0] += 0x350;
        destroyEntity(entity);
    }
    else {
        HeavyMystic->curtainLinePos += 0x20000;
    }
}

void HeavyMystic_State1_Unknown4(void) {}

void HeavyMystic_State1_Unknown5(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    HeavyMystic_Unknown2();
    HeavyMystic_Unknown2();

    if (++entity->timer == 45) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 1, &entity->animator, true, 0);
        entity->state = HeavyMystic_State1_Unknown6;
    }
}

void HeavyMystic_State1_Unknown6(void)
{
    RSDK_THIS(HeavyMystic);

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();
    if (entity->animator.frameID == entity->animator.frameCount - 8)
        RSDK.PlaySfx(HeavyMystic->sfxHat, false, 255);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
        entity->state = HeavyMystic_State1_Unknown7;
    }
}

void HeavyMystic_State1_Unknown7(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    if (entity->angle == 192) {
        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX)
                boss->state = HeavyMystic_State2_Unknown4;
        }
        RSDK.PlaySfx(HeavyMystic->sfxClack, false, 255);
        entity->state = HeavyMystic_State1_Unknown8;
    }
}

void HeavyMystic_State1_Unknown8(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    if (entity->angle == 252)
        entity->state = HeavyMystic_State1_Unknown9;
}

void HeavyMystic_State1_Unknown9(void)
{
    RSDK_THIS(HeavyMystic);

    if (++entity->timer == 60) {
        entity->timer   = 0;
        entity->visible = false;

        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX)
                boss->state = HeavyMystic_State2_Unknown5;
        }
        entity->state = HeavyMystic_State1_Unknown10;
    }
}

void HeavyMystic_State1_Unknown10(void)
{
    RSDK_THIS(HeavyMystic);

    foreach_active(HeavyMystic, boss)
    {
        if (boss->type == MYSTIC_BOX && boss->state == HeavyMystic_State2_Unknown8) {
            entity->position.x = boss->position.x;
            entity->position.y = boss->position.y;
            boss->position     = boss->targetPos;
            entity->visible    = 1;
            entity->state      = HeavyMystic_State1_ShowRouge;
        }
    }
}

void HeavyMystic_State1_ShowRouge(void)
{
    RSDK_THIS(HeavyMystic);
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    entity->rougeHealth   = 2;
    RSDK.PlaySfx(HeavyMystic->sfxTwinkle, false, 255);

    entity->velocity.y = -0x40000;
    entity->timer2     = 75;
    switch (entity->rougeID) {
        case 0:
        case 3:
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 2, &entity->animator, true, 4);
            entity->state    = HeavyMystic_State1_Unknown12;
            entity->field_7C = 8;
            break;
        case 1:
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 6, &entity->animator, true, 3);
            entity->state = HeavyMystic_State1_Unknown15;
            break;
        case 2:
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 13, &entity->animator, true, 3);
            entity->state = HeavyMystic_State1_Unknown19;
            break;
        default: break;
    }
}

void HeavyMystic_State1_MysticReveal(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    HeavyMystic_CheckPlayerCollisions();
    if (--entity->timer <= 0) {
        foreach_active(HeavyMystic, boss)
        {
            if (boss->type == MYSTIC_BOX) {
                entity->targetPos.x = boss->position.x;
                entity->targetPos.y = boss->position.y;
            }
        }

        if (entity->rougeHealth > 0) {
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 3, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown24;
        }
        else {
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown22;
        }
    }
}

void HeavyMystic_State1_Unknown22(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();

    if (entity->position.x != entity->targetPos.x) {
        if (entity->position.x <= entity->targetPos.x) {
            entity->position.x += entity->velocity.x;
            if (entity->position.x > entity->targetPos.x)
                entity->position.x = entity->targetPos.x;
            else if (entity->velocity.x < 0x30000)
                entity->velocity.x += 0x3800;
        }
        else {
            entity->position.x += entity->velocity.x;
            if (entity->position.x < entity->targetPos.x)
                entity->position.x = entity->targetPos.x;
            else if (entity->velocity.x > -0x30000)
                entity->velocity.x -= 0x3800;
        }
    }

    if (entity->position.y <= entity->targetPos.y) {
        entity->position.y = entity->targetPos.y;
        entity->angle      = 128;
        if (entity->position.x == entity->targetPos.x) {
            ++entity->rougeID;
            entity->state = HeavyMystic_State1_Unknown7;
        }
        else {
            entity->state = HeavyMystic_State1_Unknown23;
        }
    }
    else {
        entity->position.y += entity->velocity.y;
        if (entity->velocity.y > -0x20000)
            entity->velocity.y -= 0x2000;
    }
}

void HeavyMystic_State1_Unknown23(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();

    entity->angle      = (entity->angle + 4) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 11) + entity->targetPos.y) & 0xFFFF0000;

    if (entity->position.x == entity->targetPos.x) {
        ++entity->rougeID;
        entity->position.y = entity->targetPos.y;
        entity->state      = HeavyMystic_State1_Unknown7;
    }
    else if (entity->position.x > entity->targetPos.x) {
        entity->position.x += entity->velocity.x;
        if (entity->position.x >= entity->targetPos.x) {
            if (entity->velocity.x > -0x30000)
                entity->velocity.x -= 0x3800;
        }
        else
            entity->position.x = entity->targetPos.x;
    }
    else {
        entity->position.x += entity->velocity.x;
        if (entity->position.x > entity->targetPos.x)
            entity->position.x = entity->targetPos.x;
        else if (entity->velocity.x < 0x30000)
            entity->velocity.x += 0x3800;
    }
}

void HeavyMystic_State1_Unknown24(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&entity->animator);
    HeavyMystic_Unknown2();
    if (entity->animator.frameID == entity->animator.frameCount - 5)
        RSDK.PlaySfx(HeavyMystic->sfxTransform, false, 255);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
        entity->velocity.x                                                                                     = 0;
        entity->velocity.y                                                                                     = -0x20000;
        RSDK.PlaySfx(HeavyMystic->sfxPoof, false, 255);

        entity->velocity.y = -0x40000;
        entity->timer2     = 75;
        switch (entity->rougeID) {
            case 0:
            case 3:
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 2, &entity->animator, true, 4);
                entity->state    = HeavyMystic_State1_Unknown12;
                entity->field_7C = 8;
                break;
            case 1:
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 6, &entity->animator, true, 3);
                entity->state = HeavyMystic_State1_Unknown15;
                break;
            case 2:
                RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 13, &entity->animator, true, 3);
                entity->state = HeavyMystic_State1_Unknown19;
                break;
            default: break;
        }
    }
}

void HeavyMystic_State1_Unknown13(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK.ProcessAnimation(&entity->animator);
    entity->direction = player1->position.x < entity->position.x;
    if (!--entity->timer) {
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 1, &entity->animator, true, 0);
        entity->state = HeavyMystic_State1_Unknown14;
    }
    HeavyMystic_CheckPlayerCollisions2();
}

void HeavyMystic_State1_Unknown14(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        entity->velocity.y = -0x50000;
        entity->field_7C   = 8;
        entity->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 2, &entity->animator, true, 0);
        entity->state = HeavyMystic_State1_Unknown12;
    }
    HeavyMystic_CheckPlayerCollisions2();
}

void HeavyMystic_State1_Unknown12(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->direction = player1->position.x < entity->position.x;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.x >= 0) {
        if (entity->position.x > (RSDK_screens->position.x + RSDK_screens->width - 16) << 16)
            entity->velocity.x = -entity->velocity.x;
    }
    else if (entity->position.x < (RSDK_screens->position.x + 16) << 16)
        entity->velocity.x = -entity->velocity.x;

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->velocity.y > 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, true)) {
        if (--entity->field_7C > 0) {
            entity->velocity.y = -0x50000;
            entity->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 2, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown12;
        }
        else {
            entity->timer      = 15;
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 0, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown13;
        }
    }

    if (entity->field_7C && !(Zone->timer & 0x3F)) {
        RSDK.PlaySfx(HeavyMystic->sfxPon, false, 255);
        EntityHeavyMystic *boss = CREATE_ENTITY(HeavyMystic, intToVoid(MYSTIC_CORK), entity->position.x, entity->position.y - 0x40000);
        if (entity->direction == FLIP_NONE) {
            boss->position.x += 0x180000;
            boss->velocity.x = 0x20000;
            boss->direction  = entity->direction;
        }
        else {
            boss->position.x -= 0x180000;
            boss->velocity.x = -0x20000;
            boss->direction  = entity->direction;
        }
    }
    HeavyMystic_CheckPlayerCollisions2();
}

void HeavyMystic_State_RougeHit(void)
{
    RSDK_THIS(HeavyMystic);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.x >= 0) {
        if (entity->position.x > (RSDK_screens->position.x + RSDK_screens->width - 16) << 16)
            entity->velocity.x = 0;
    }
    else if (entity->position.x < (RSDK_screens->position.x + 16) << 16)
        entity->velocity.x = 0;

    entity->visible ^= 1;
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->velocity.y > 0) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x240000, true)) {
            RSDK.PlaySfx(HeavyMystic->sfxPowerDown, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
            entity->direction ^= FLIP_X;
            entity->timer      = 90;
            entity->field_7C   = 0;
            entity->visible    = true;
            entity->targetPos.y = entity->position.y;
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 4, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_MysticReveal;
        }
    }
}

void HeavyMystic_State1_Unknown20(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK.ProcessAnimation(&entity->animator);
    entity->direction = player1->position.x < entity->position.x;
    if (!--entity->timer) {
        entity->velocity.y = -0x40000;
        entity->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
        if (entity->field_7C == 1) {
            entity->timer = 240;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 14, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown21;
        }
        else {
            entity->field_7C = 1;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 13, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown19;
        }
    }
    HeavyMystic_CheckPlayerCollisions3();
}

void HeavyMystic_State1_Unknown21(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);

    if (entity->animator.frameID == 3 || entity->animator.frameID == 10) {
        if (entity->field_7C != entity->animator.frameID)
            RSDK.PlaySfx(HeavyMystic->sfxImpact2, false, 255);
        entity->field_7C = entity->animator.frameID;
    }
    if (--entity->timer <= 0) {
        entity->field_7C   = 0;
        entity->timer      = 15;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 12, &entity->animator, true, 0);
        entity->state = HeavyMystic_State1_Unknown20;
    }
    if (entity->field_7C != 1) {
        if (!(Zone->timer & 3))
            Camera_ShakeScreen(0, 0, 2);

        if (!(Zone->timer & 0xF)) {
            int x = RSDK.Rand(0, RSDK_screens->width >> 5);
            EntityHeavyMystic *boss = CREATE_ENTITY(HeavyMystic, intToVoid(MYSTIC_DEBRIS), (RSDK_screens->position.x + 16 + 32 * x) << 16,
                                                    (RSDK_screens->position.y - 16) << 16);
            boss->animator.frameID = RSDK.Rand(0, 6);
        }
    }
    HeavyMystic_CheckPlayerCollisions3();
}

void HeavyMystic_State1_Unknown19(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->direction = player1->position.x < entity->position.x;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.x >= 0) {
        if (entity->position.x > (RSDK_screens->position.x + RSDK_screens->width - 16) << 16)
            entity->velocity.x = -entity->velocity.x;
    }
    else if (entity->position.x < (RSDK_screens->position.x + 16) << 16)
        entity->velocity.x = -entity->velocity.x;

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->velocity.y > 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        entity->timer = 15;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 12, &entity->animator, true, 0);
        entity->state = HeavyMystic_State1_Unknown20;
    }
    HeavyMystic_CheckPlayerCollisions3();
}

void HeavyMystic_State1_Unknown16(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->direction = player1->position.x < entity->position.x;
    RSDK.ProcessAnimation(&entity->animator);

    if (!--entity->timer) {
        entity->velocity.y = -0x68000;
        entity->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x20000 : 0x20000;
        if (entity->field_7C == 1) {
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 8, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown17;
        }
        else {
            entity->field_7C = 1;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 6, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown15;
        }
    }
    HeavyMystic_CheckPlayerCollisions4();
}

void HeavyMystic_State1_Unknown17(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->field_7C == 1 && entity->animator.frameID == 3) {
        entity->field_7C = 2;
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);
        EntityHeavyMystic *boss = CREATE_ENTITY(HeavyMystic, intToVoid(MYSTIC_BOMB), entity->position.x, entity->position.y + 0x130000);
        if (entity->direction) {
            boss->position.x += 0xB0000;
            boss->velocity.x = -0x40000;
        }
        else {
            boss->position.x -= 0xB0000;
            boss->velocity.x = 0x40000;
        }
        boss->velocity.y = -0x40000;
        boss->direction  = entity->direction;
    }
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        entity->field_7C = 1;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 7, &entity->animator, true, 0);
        entity->state = HeavyMystic_State1_Unknown18;
    }
    HeavyMystic_CheckPlayerCollisions4();
}

void HeavyMystic_State1_Unknown18(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->field_7C == 1 && entity->animator.frameID == 2) {
        entity->field_7C = 2;
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);
        EntityHeavyMystic *boss = CREATE_ENTITY(HeavyMystic, intToVoid(MYSTIC_BOMB), entity->position.x, entity->position.y - 0x130000);
        if (entity->direction) {
            boss->position.x += 0xB0000;
            boss->velocity.x = -0x40000;
        }
        else {
            boss->position.x -= 0xB0000;
            boss->velocity.x = 0x40000;
        }
        boss->velocity.y = -0x40000;
        boss->direction  = entity->direction;
    }
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        entity->field_7C   = 0;
        entity->timer      = 5;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 5, &entity->animator, true, 0);
        entity->state = HeavyMystic_State1_Unknown16;
    }
    HeavyMystic_CheckPlayerCollisions4();
}

void HeavyMystic_State1_Unknown15(void)
{
    RSDK_THIS(HeavyMystic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->direction = player1->position.x < entity->position.x;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.x >= 0) {
        if (entity->position.x > (RSDK_screens->position.x + RSDK_screens->width - 16) << 16)
            entity->velocity.x = -entity->velocity.x;
    }
    else if (entity->position.x < (RSDK_screens->position.x + 16) << 16)
        entity->velocity.x = -entity->velocity.x;

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->velocity.y > 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, true) == true) {
        if (--entity->field_7C <= 0) {
            entity->field_7C   = 1;
            entity->timer      = 15;
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 5, &entity->animator, true, 0);
            entity->state = HeavyMystic_State1_Unknown16;
        }
        else {
            entity->velocity.y = -0x50000;
            entity->velocity.x = RSDK.Rand(0, 2) != 0 ? -0x10000 : 0x10000;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 6, &entity->animator, true, 0);
        }
    }
    HeavyMystic_CheckPlayerCollisions4();
}

void HeavyMystic_State3_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            int anim = player->playerAnimator.animationID;
            if (player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)) {
                if (!player->uncurlTimer) {
                    RSDK.PlaySfx(Player->sfx_PimPom, false, 255);
                    player->uncurlTimer = 30;
                }
                int angle          = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                entity->velocity.x = -0x300 * RSDK.Cos256(angle);
                entity->velocity.y = -0x400 * RSDK.Sin256(angle);
                entity->state      = HeavyMystic_State3_Unknown2;
            }
            else {
#endif
                Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
            }
#endif
            RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
            destroyEntity(entity);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void HeavyMystic_State3_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);

    entity->visible ^= 1;
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void HeavyMystic_State4_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x300, player, false, &player->uncurlTimer))
#endif
                Player_CheckHit(player, entity);
            RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
            destroyEntity(entity);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void HeavyMystic_State5_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x300, player, false, &player->uncurlTimer))
#endif
                Player_CheckHit(player, entity);
        }
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void HeavyMystic_State2_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    entity->timer = 2;
    if (HeavyMystic->curtainLinePos <= 0x100000) {
        entity->state     = HeavyMystic_State2_Unknown2;
        entity->stateDraw = HeavyMystic_StateDraw2_Unknown2;
    }
}

void HeavyMystic_State2_Unknown2(void) {}

void HeavyMystic_State2_Unknown4(void)
{
    RSDK_THIS(HeavyMystic);
    entity->scale.x -= 32;
    if (entity->scale.x <= 0) {
        if (entity->timer == 2) {
            foreach_active(HeavyMystic, boss)
            {
                if (boss->state == HeavyMystic_State1_Unknown4)
                    boss->state = HeavyMystic_State1_Unknown5;
            }

            entity->timer     = 1;
            entity->stateDraw = HeavyMystic_StateDraw2_Unknown1;
            entity->state     = HeavyMystic_State2_Unknown3;
        }
        else {
            entity->timer     = 2;
            entity->stateDraw = HeavyMystic_StateDraw2_Unknown2;
            entity->state     = HeavyMystic_State2_Unknown3;
        }
    }
}

void HeavyMystic_State2_Unknown3(void)
{
    RSDK_THIS(HeavyMystic);

    entity->scale.x += 32;
    if (entity->scale.x >= 512)
        entity->state = HeavyMystic_State2_Unknown2;
}

void HeavyMystic_State2_Unknown5(void)
{
    RSDK_THIS(HeavyMystic);
    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyMystic->sfxMagicBox, false, 255);
        int x = RSDK.Rand(-24, 25) << 16;
        int y = RSDK.Rand(-24, 25) << 16;
        RSDK.CreateEntity(Explosion->objectID, intToVoid(EXPLOSION_ENEMY), x + entity->position.x, y + entity->position.y)->drawOrder =
            Zone->drawOrderHigh + 2;
    }

    entity->scale.x = RSDK.Cos256(entity->angle + 64) + 0x200;
    entity->scale.y = RSDK.Sin256(entity->angle) + 0x200;
    if (entity->angle == 768) {
        RSDK.StopSFX(HeavyMystic->sfxMagicBox);
        RSDK.PlaySfx(HeavyMystic->sfxPowerDown, false, 255);
        entity->angle     = 0;
        entity->timer     = 0;
        entity->state     = HeavyMystic_State2_Unknown6;
        entity->stateDraw = HeavyMystic_StateDraw2_Unknown3;
    }
    else {
        entity->angle += 8;
    }
}

void HeavyMystic_State2_Unknown6(void)
{
    RSDK_THIS(HeavyMystic);
    if (++entity->timer > 30) {
        RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);
        entity->state = HeavyMystic_State2_Unknown7;
    }
}

void HeavyMystic_State2_Unknown7(void)
{
    RSDK_THIS(HeavyMystic);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
        Camera_ShakeScreen(0, 0, 6);
        RSDK.PlaySfx(HeavyMystic->sfxImpact2, false, 255);
        RSDK.PlaySfx(HeavyMystic->sfxExplosion, false, 255);

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        debris->drawOrder    = Zone->drawOrderLow;
        debris->drawFX       = FX_FLIP;
        debris->direction    = FLIP_NONE;
        debris->velocity.y   = -0x40000;
        debris->gravity      = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 0;

        debris             = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x280000, entity->position.y);
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->drawFX     = FX_FLIP;
        debris->direction  = FLIP_NONE;
        debris->velocity.x = -0x20000;
        debris->velocity.y = -0x40000;
        debris->gravity    = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 2;

        debris             = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x280000, entity->position.y);
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->drawFX     = FX_FLIP;
        debris->direction  = FLIP_X;
        debris->velocity.x = 0x20000;
        debris->velocity.y = -0x40000;
        debris->gravity    = 0x2800;
        RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &debris->animator, true, 0);
        debris->animator.frameID = 2;

        entity->timer      = 0;
        entity->visible    = 0;
        entity->velocity.y = 0;
        entity->state      = HeavyMystic_State2_Unknown8;
    }
}

void HeavyMystic_State2_Unknown8(void)
{
    RSDK_THIS(HeavyMystic);
    if (++entity->timer > 96) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
        entity->visible                                                                                         = true;
        entity->scale.x                                                                                         = 0;
        entity->scale.y                                                                                         = 0;
        RSDK.PlaySfx(HeavyMystic->sfxAssemble, false, 255);
        entity->stateDraw = HeavyMystic_StateDraw2_Unknown3;
        entity->state     = HeavyMystic_State2_Unknown9;
    }
}

void HeavyMystic_State2_Unknown9(void)
{
    RSDK_THIS(HeavyMystic);
    if (entity->scale.x < 512) {
        entity->scale.y += 8;
        entity->scale.x = entity->scale.y;
        entity->rotation -= 8;
    }
    else {
        if (entity->rotation & 0x1FF) {
            entity->rotation -= 8;
        }
        else {
            RSDK.PlaySfx(HeavyMystic->sfxClack, false, 255);
            entity->timer     = 2;
            entity->stateDraw = HeavyMystic_StateDraw2_Unknown2;
            entity->state     = HeavyMystic_State2_Unknown4;
        }
        entity->rotation &= 0x1FF;
    }
}

void HeavyMystic_StateDraw2_Unknown1(void)
{
    RSDK_THIS(HeavyMystic);
    Vector2 drawPos;

    if (RSDK_sceneInfo->currentDrawGroup == RSDK_sceneInfo->entity->drawOrder) {
        entity->drawFX           = FX_NONE;
        entity->animator.frameID = 0;
        RSDK.DrawSprite(&entity->animator, NULL, false);

        drawPos.x                = entity->position.x - 0x280000;
        drawPos.y                = entity->position.y;
        entity->animator.frameID = entity->timer;
        entity->drawFX           = FX_SCALE | FX_ROTATE | FX_FLIP;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        drawPos.x += 0x500000;
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->direction = FLIP_NONE;
    }
}

void HeavyMystic_StateDraw2_Unknown2(void)
{
    RSDK_THIS(HeavyMystic);
    Vector2 drawPos;

    if (RSDK_sceneInfo->currentDrawGroup == RSDK_sceneInfo->entity->drawOrder) {
        entity->drawFX = FX_NONE;
        if (entity->state != HeavyMystic_State2_Unknown5) {
            entity->animator.frameID = 0;
            RSDK.DrawSprite(&entity->animator, NULL, false);
        }
    }
    else {
        drawPos.y                = entity->position.y;
        entity->animator.frameID = entity->timer;
        entity->drawFX           = FX_SCALE | FX_ROTATE | FX_FLIP;
        drawPos.x                = entity->position.x - 0x280000;
        if (entity->state == HeavyMystic_State2_Unknown5)
            drawPos.x += 0x1400 * (0x200 - entity->scale.x);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        entity->direction = FLIP_X;
        drawPos.x         = entity->position.x + 0x280000;
        if (entity->state == HeavyMystic_State2_Unknown5)
            drawPos.x += 0x1400 * (entity->scale.x - 0x200);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        entity->direction = FLIP_NONE;
    }
}

void HeavyMystic_StateDraw2_Unknown3(void)
{
    RSDK_THIS(HeavyMystic);
    Vector2 drawPos;

    if (RSDK_sceneInfo->currentDrawGroup == RSDK_sceneInfo->entity->drawOrder) {
        entity->drawFX           = FX_SCALE | FX_ROTATE;
        entity->animator.frameID = 3;
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void HeavyMystic_EditorDraw(void)
{
    RSDK_THIS(HeavyMystic);

    switch (entity->type) {
        case MYSTIC_MISCHIEF:
            entity->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
            break;
        case MYSTIC_BOSS:
            entity->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 0, &entity->animator, true, 0);
            break;
        case MYSTIC_BOX:
            entity->drawFX = FX_SCALE | FX_ROTATE | FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->aniFrames, 10, &entity->animator, true, 0);
            break;
        case MYSTIC_CORK:
            entity->drawFX    = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 4, &entity->animator, true, 0);
            break;
        case MYSTIC_BOMB:
            entity->drawFX    = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 11, &entity->animator, true, 0);
            break;
        case MYSTIC_DEBRIS:
            entity->drawFX    = FX_FLIP;
            RSDK.SetSpriteAnimation(HeavyMystic->rouguesFrames, 16, &entity->animator, true, 0);
            break;
        default: break;
    }

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void HeavyMystic_EditorLoad(void)
{
    HeavyMystic->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);
    HeavyMystic->rouguesFrames = RSDK.LoadSpriteAnimation("MSZ/Rogues.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(HeavyMystic, type);
    RSDK_ENUM_VAR(MYSTIC_MISCHIEF);
    RSDK_ENUM_VAR(MYSTIC_BOSS);
    RSDK_ENUM_VAR(MYSTIC_BOX);
    RSDK_ENUM_VAR(MYSTIC_CORK);
    RSDK_ENUM_VAR(MYSTIC_BOMB);
    RSDK_ENUM_VAR(MYSTIC_DEBRIS);
}
#endif

void HeavyMystic_Serialize(void) { RSDK_EDITABLE_VAR(HeavyMystic, VAR_ENUM, type); }
