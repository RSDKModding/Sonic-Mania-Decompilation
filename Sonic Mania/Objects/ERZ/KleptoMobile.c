#include "SonicMania.h"

ObjectKleptoMobile *KleptoMobile;

void KleptoMobile_Update(void)
{
    RSDK_THIS(KleptoMobile);
    KleptoMobile_HandleAnimations();
    StateMachine_Run(entity->state);
}

void KleptoMobile_LateUpdate(void) {}

void KleptoMobile_StaticUpdate(void) {}

void KleptoMobile_Draw(void)
{
    RSDK_THIS(KleptoMobile);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void KleptoMobile_Create(void *data)
{
    RSDK_THIS(KleptoMobile);

    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->updateRange.x   = 0x800000;
            entity->updateRange.y   = 0x800000;
            entity->visible         = true;
            entity->drawOrder       = Zone->drawOrderLow;
            entity->drawFX          = FX_FLIP;
            entity->explosionVolume = 0x200;
            entity->type            = voidToInt(data);
            switch (entity->type) {
                case 0:
                    entity->hitbox.left   = -22;
                    entity->hitbox.top    = -22;
                    entity->hitbox.right  = 0x16;
                    entity->hitbox.bottom = 0x16;
                    entity->active        = ACTIVE_NORMAL;
                    entity->health        = 8;
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &entity->animator2, true, 1);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &entity->animator3, true, 0);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &entity->animator4, true, 0);
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &entity->animator11, true, 0);
                    KleptoMobile->defeated = false;
                    entity->stateDraw      = KleptoMobile_StateDraw_Unknown1;
                    entity->state          = KleptoMobile_State_SetupArena;
                    break;
                case 1:
                case 2:
                    entity->hitbox.left   = -32;
                    entity->hitbox.top    = -8;
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->hitbox.right  = 32;
                    entity->hitbox.bottom = 8;
                    if (entity->type == 2) {
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 7, &entity->animator7, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 8, &entity->animator8, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 9, &entity->animator9, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 10, &entity->animator10, true, 0);
                    }
                    else {
                        entity->drawOrder = Zone->playerDrawLow;
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 3, &entity->animator7, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 4, &entity->animator8, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 5, &entity->animator9, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 6, &entity->animator10, true, 0);
                    }
                    entity->stateDraw = KleptoMobile_StateDraw1_Unknown1;
                    entity->state     = KleptoMobile_State1_Unknown1;
                    break;
                case 3:
                    entity->active  = ACTIVE_NORMAL;
                    entity->visible = true;
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 12, &entity->animator8, true, 5);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 17, &entity->animator7, true, 0);
                    entity->stateDraw = KleptoMobile_StateDraw3_Unknown1;
                    entity->state     = KleptoMobile_State3_Unknown1;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void KleptoMobile_StageLoad(void)
{
    KleptoMobile->aniFrames    = RSDK.LoadSpriteAnimation("Phantom/KleptoMobile.bin", SCOPE_STAGE);
    KleptoMobile->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    KleptoMobile->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    KleptoMobile->sfxFlail     = RSDK.GetSFX("SSZ1/Flail.wav");
    KleptoMobile->sfxWhack     = RSDK.GetSFX("Stage/Whack.wav");
    KleptoMobile->sfxPowerUp   = RSDK.GetSFX("Stage/PowerUp.wav");
    KleptoMobile->sfxRocketJet = RSDK.GetSFX("Stage/RocketJet.wav");
}

void KleptoMobile_HandleAnimations(void)
{
    RSDK_THIS(KleptoMobile);

    bool32 flag = false;
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    if (entity->animator3.animationID == 13) {
        foreach_active(Player, player)
        {
            if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == ERZStart_State_PlayerRebound)
                flag = true;
        }
        if (flag == true)
            RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 14, &entity->animator3, true, 0);
    }
    else if (entity->animator3.animationID == 14) {
        foreach_active(Player, player)
        {
            if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == ERZStart_State_PlayerRebound)
                flag = true;
        }

        if (entity->animator3.frameID >= 14) {
            if (flag)
                RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 14, &entity->animator3, true, 7);
            else
                RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &entity->animator3, true, 0);
        }
    }
    else {
        if (entity->animator3.animationID == 15 && !entity->invincibilityTimer)
            RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &entity->animator3, true, 0);
    }
}

void KleptoMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(KleptoMobile);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
            KleptoMobile_Hit();
        }
    }
}

void KleptoMobile_Hit(void)
{
    RSDK_THIS(KleptoMobile);

    if (--entity->health <= 0) {
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 16, &entity->animator3, true, 0);
        entity->state = KleptoMobile_State_Destroyed;
        entity->timer = 0;
        if (PhantomKing->defeated)
            RSDK_sceneInfo->timeEnabled = false;

        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        entity->invincibilityTimer = 48;
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 15, &entity->animator3, true, 0);
        RSDK.PlaySfx(KleptoMobile->sfxHit, false, 255);

        foreach_all(PhantomRuby, ruby)
        {
            ruby->position.x = entity->circlePos.x;
            ruby->position.y = entity->circlePos.y;
            ruby->velocity.x = -0x20000;
            ruby->velocity.y = -0x40000;
            ruby->state      = PhantomRuby_Unknown6;

            EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, KleptoMobile);
            arm1->timer              = 0;
            arm1->state              = KleptoMobile_State1_Unknown1;

            EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, KleptoMobile);
            arm2->timer              = 0;
            arm2->state              = KleptoMobile_State1_Unknown1;
        }

        entity->field_120 = false;
        entity->state     = KleptoMobile_State_HitFall;
    }
}

void KleptoMobile_HandleFrames(void)
{
    RSDK_THIS(KleptoMobile);

    entity->rotation = RSDK.Sin512(2 * Zone->timer) >> 6;
    entity->angle2   = (entity->angle2 + 12) & 0x3FF;

    int moveX = 0x1C00 * RSDK.Sin512(-entity->rotation) + entity->position.x;
    int moveY = 0x1C00 * RSDK.Cos512(-entity->rotation) + entity->position.y;

    int angle = entity->angle2;

    for (int i = 0; i < 10; i += 2) {
        entity->framePositions[i].x = moveX + 2 * RSDK.Cos1024(angle) * RSDK.Cos512(entity->rotation);
        entity->framePositions[i].y = moveY + 2 * RSDK.Cos1024(angle) * RSDK.Sin512(entity->rotation);
        entity->frameIDs[i]         = angle & 0x3FF;
        angle += 0x200;

        entity->framePositions[i + 1].x = moveX + 2 * RSDK.Cos1024(angle) * RSDK.Cos512(entity->rotation);
        entity->framePositions[i + 1].y = moveY + 2 * RSDK.Cos1024(angle) * RSDK.Sin512(entity->rotation);
        entity->frameIDs[i + 1]         = angle & 0x3FF;

        moveX += RSDK.Sin512(-entity->rotation) << 10;
        moveY += RSDK.Cos512(-entity->rotation) << 10;
        angle += 0x240;
    }
}

void KleptoMobile_SwitchToKing(void)
{
    EntityPhantomKing *kingPtr = NULL;

    foreach_active(PhantomKing, king)
    {
        if (!king->type) {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            king->direction       = FLIP_X;
            if (RSDK.Rand(0, 2))
                king->position.x = player1->position.x + 0x1000000;
            else
                king->position.x = player1->position.x - 0x1000000;
            king->position.y   = player1->position.y + 0x800000;
            king->posUnknown.y = player1->position.y + 0x800000;
            king->rubyPos      = king->position;
            king->rubyPos.x -= 0x1400 * RSDK.Sin512(-king->rotation);
            king->rubyPos.y -= 0x1400 * RSDK.Cos512(-king->rotation);
            king->velocity.x = 0;
            king->velocity.y = 0;
            king->drawRuby  = true;

            for (int i = 0; i < 0x3FC; i += 0xAA) {
                EntityPKingAttack *attack = CREATE_ENTITY(PKingAttack, intToVoid(1), king->rubyPos.x, king->rubyPos.y);
                attack->angle             = i;
                attack->target            = (Entity *)king;
            }

            king->state = PhantomKing_State_Unknown8;
            kingPtr     = king;
            foreach_break;
        }
    }

    foreach_active(PhantomKing, kingArm)
    {
        if (kingArm->type) {
            kingArm->position.x = kingPtr->position.x;
            kingArm->position.y = kingPtr->position.y;
            for (int i = 0; i < 10; ++i) kingArm->framePositions[i] = kingPtr->position;
            kingArm->velocity.x = 0;
            kingArm->velocity.y = 0;
            kingArm->state      = PhantomKing_StateArm1_Unknown2;
        }
    }
}

void KleptoMobile_StateDraw_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);

    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 112, 0, 136, 8);
        RSDK.CopyPalette(2, 120, 0, 11, 1);
        RSDK.CopyPalette(2, 121, 0, 25, 1);
        RSDK.CopyPalette(2, 122, 0, 197, 6);

        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        entity->animator4.frameID = 2;
        RSDK.DrawSprite(&entity->animator4, NULL, false);

        entity->animator4.frameID = 0;
        RSDK.DrawSprite(&entity->animator4, NULL, false);

        RSDK.CopyPalette(1, 136, 0, 136, 8);
        RSDK.CopyPalette(1, 11, 0, 11, 1);
        RSDK.CopyPalette(1, 25, 0, 25, 1);
        RSDK.CopyPalette(1, 197, 0, 197, 6);
    }
    else {
        entity->animator1.frameID = 1;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        entity->animator4.frameID = 2;
        RSDK.DrawSprite(&entity->animator4, NULL, false);

        entity->animator4.frameID = 0;
        RSDK.DrawSprite(&entity->animator4, NULL, false);
    }

    RSDK.DrawCircle(entity->circlePos.x, entity->circlePos.y, entity->circleRadius, 0, 255, INK_LOOKUP, false);
}

void KleptoMobile_State_SetupArena(void)
{
    RSDK_THIS(KleptoMobile);

    entity->active = ACTIVE_NORMAL;

    EntityKleptoMobile *hand = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 2, KleptoMobile);
    RSDK.ResetEntityPtr(hand, KleptoMobile->objectID, intToVoid(3));
    hand->position.x = entity->position.x;
    hand->position.y = entity->position.y;
    hand->parent     = (Entity *)entity;

    EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, KleptoMobile);
    RSDK.ResetEntityPtr(arm2, KleptoMobile->objectID, intToVoid(2));
    arm2->position.x = entity->position.x;
    arm2->position.y = entity->position.y;
    arm2->parent     = (Entity *)entity;

    EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, KleptoMobile);
    RSDK.ResetEntityPtr(arm1, KleptoMobile->objectID, intToVoid(1));
    arm1->position.x = entity->position.x;
    arm1->position.y = entity->position.y;
    arm1->parent     = (Entity *)entity;

    entity->field_70.x    = entity->position.x;
    entity->field_70.y    = entity->position.y;
    KleptoMobile->boundsM = entity->position.x;
    KleptoMobile->boundsL = KleptoMobile->boundsM - 0x800000;
    KleptoMobile->boundsR = KleptoMobile->boundsM + 0x800000;
    KleptoMobile->boundsT = (Zone->screenBoundsT1[0] + 48) << 16;
    KleptoMobile->boundsB = (Zone->screenBoundsB1[0] - 96) << 16;
    entity->state         = StateMachine_None;
}

void KleptoMobile_State_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->field_70.y) & 0xFFFF0000;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->field_70.x += entity->velocity.x;
    entity->field_70.y += entity->velocity.y;
    KleptoMobile_HandleFrames();
}

void KleptoMobile_State_Unknown2(void)
{
    RSDK_THIS(KleptoMobile);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->field_70.y) & 0xFFFF0000;
    KleptoMobile_CheckPlayerCollisions();

    int angle = RSDK.ATan2(entity->position.x - player1->position.x, entity->field_70.y - player1->position.y);
    int x     = (RSDK.Cos256(angle) << 15) + player1->position.x;
    int y     = (RSDK.Sin256(angle) << 15) + player1->position.y;

    if (entity->circleRadius > 0)
        entity->circleRadius -= 8;

    if (x <= entity->position.x) {
        if (entity->velocity.x > -0x20000) {
            entity->velocity.x -= 0x800;
        }
    }
    else if (entity->velocity.x < 0x20000) {
        entity->velocity.x += 0x800;
    }

    int bottom   = Zone->screenBoundsB1[0] << 16;
    int boundary = bottom - 0x800000;
    if (y <= bottom - 0x800000) {
        boundary = bottom + 0x800000;
        if (y >= (Zone->screenBoundsT1[0] + 128) << 16)
            boundary = y;
    }

    if (boundary <= entity->field_70.y) {
        if (entity->velocity.y > -0x20000)
            entity->velocity.y -= 0x800;
    }
    else {
        if (entity->velocity.y < 0x20000)
            entity->velocity.y += 0x800;
    }

    entity->position.x += entity->velocity.x;
    entity->field_70.y += entity->velocity.y;
    entity->direction = player1->position.x > entity->position.x;
    if (--entity->field_10C == 1) {
        entity->field_10C = 0;
        entity->timer     = 0;
        RSDK.PlaySfx(KleptoMobile->sfxPowerUp, false, 255);
        EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, KleptoMobile);
        arm2->timer              = 0;
        arm2->state              = KleptoMobile_State1_Unknown4;

        EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, KleptoMobile);
        arm1->timer              = 0;
        arm1->state              = KleptoMobile_State1_Unknown4;
        entity->state            = KleptoMobile_State_Unknown3;
    }
    else {
        if (entity->field_114) {
            if (entity->field_10C <= 0 && ++entity->timer == 15) {
                entity->timer = 0;
                RSDK.PlaySfx(KleptoMobile->sfxFlail, false, 0xFF);
                int armSlot = entity->field_110 ? RSDK_sceneInfo->entitySlot + 1 : RSDK_sceneInfo->entitySlot - 1;

                EntityKleptoMobile *arm = RSDK_GET_ENTITY(armSlot, KleptoMobile);

                x = entity->direction == FLIP_X ? entity->position.x - 0x30000 : entity->position.x + 0x30000;
                if (arm->type == 2) {
                    x += 0x180000;
                }
                y = entity->position.y + 0xD0000;

                angle           = RSDK.ATan2(player1->position.x - x, player1->position.y - y);
                arm->field_F8.x = x + (RSDK.Cos256(angle) << 15);
                arm->field_F8.y = y + (RSDK.Sin256(angle) << 15);
                arm->timer      = 0;
                arm->state      = KleptoMobile_State1_Unknown3;
                entity->field_110 ^= 1;
                if (++entity->field_108 >= 5) {
                    entity->field_108 = 0;
                    entity->field_10C = 90;
                    entity->field_114 = false;
                }
            }
        }
        else {
            if (abs(player1->position.x - entity->position.x) < 0x1800000 && abs(player1->position.y - entity->position.y) < 0x1800000)
                entity->field_114 = true;
        }
        KleptoMobile_HandleFrames();
    }
}

void KleptoMobile_State_Unknown3(void)
{
    RSDK_THIS(KleptoMobile);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->field_70.y) & 0xFFFF0000;

    KleptoMobile_CheckPlayerCollisions();
    if (++entity->timer < 180) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (abs(player1->position.x - entity->position.x) < 0xC00000 && abs(player1->position.y - entity->position.y) < 0xC00000) {
            RSDK.StopSFX(KleptoMobile->sfxPowerUp);
            entity->timer     = 14;
            entity->field_10C = 0;
            entity->field_114 = true;
            entity->state     = KleptoMobile_State_Unknown2;
        }
    }
    else {
        entity->timer = 0;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0xA0000;
        else
            entity->velocity.x = 0xA0000;
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
        RSDK.PlaySfx(KleptoMobile->sfxRocketJet, false, 255);
        entity->state = KleptoMobile_State_Unknown5;
    }
    KleptoMobile_HandleFrames();
}

void KleptoMobile_HandleArmPositions(void)
{
    foreach_active(KleptoMobile, eggman)
    {
        if (eggman->type) {
            EntityKleptoMobile *parent = (EntityKleptoMobile *)eggman->parent;
            eggman->position.x         = parent->position.x;
            eggman->position.y         = parent->position.y;
            for (int i = 0; i < 10; ++i) eggman->framePositions[i] = parent->position;
            eggman->velocity.x = 0;
            eggman->velocity.y = 0;
        }
    }
}

void KleptoMobile_State_Unknown4(void)
{
    RSDK_THIS(KleptoMobile);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->timer = 0;

    PhantomRuby_PlaySFX(RSDK.Rand(RUBYSFX_ATTACK1, RUBYSFX_REDCUBE));
    if (!RSDK.Rand(0, 2))
        entity->position.x = player1->position.x - 0x1800000;
    else
        entity->position.x = player1->position.x + 0x1800000;

      do {
        do {
            entity->field_70.y = (RSDK.Rand(-2, 3) << 21) + player1->position.y;
        } while (entity->field_70.y > (Zone->screenBoundsB1[0] - 64) << 16);
      } while (entity->field_70.y < (Zone->screenBoundsT1[0] + 64) << 16);

    entity->circleRadius = 128;
    if (++entity->field_108 == 4) {
        entity->field_108  = 0;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->state      = KleptoMobile_State_Unknown2;
    }
    else if (player1->position.x >= entity->position.x) {
        entity->velocity.x = 0xA0000;
        entity->direction  = FLIP_X;
    }
    else {
        entity->velocity.x = -0xA0000;
        entity->direction  = FLIP_NONE;
    }
    KleptoMobile_HandleArmPositions();
}

void KleptoMobile_State_Unknown5(void)
{
    RSDK_THIS(KleptoMobile);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->field_70.y) & 0xFFFF0000;

    if (entity->circleRadius < 128)
        entity->circleRadius += 8;

    if (++entity->timer >= 180) {
        KleptoMobile_State_Unknown4();
        entity->state = KleptoMobile_State_Unknown6;
    }
    entity->position.x += entity->velocity.x;
    entity->field_70.y += entity->velocity.y;
    KleptoMobile_HandleFrames();
}

void KleptoMobile_State_Unknown6(void)
{
    RSDK_THIS(KleptoMobile);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 9) + entity->field_70.y) & 0xFFFF0000;

    KleptoMobile_CheckPlayerCollisions();
    if (entity->activeScreens) {
        if (entity->circleRadius > 0)
            entity->circleRadius -= 8;
    }
    if (++entity->timer >= 180)
        KleptoMobile_State_Unknown4();
    entity->position.x += entity->velocity.x;
    entity->field_70.y += entity->velocity.y;
    KleptoMobile_HandleFrames();
}

void KleptoMobile_State_Switch(void)
{
    RSDK_THIS(KleptoMobile);

    if (++entity->timer >= 120) {
        entity->timer = 0;
        KleptoMobile_SwitchToKing();
        entity->state = StateMachine_None;
    }
}

void KleptoMobile_State_HitFall(void)
{
    RSDK_THIS(KleptoMobile);

    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x3800;
    if (entity->position.y < -0x1000000) {
        entity->field_114  = false;
        entity->field_10C  = 0;
        entity->velocity.y = 0;
        entity->timer      = 0;
        entity->state      = KleptoMobile_State_Switch;
    }
    KleptoMobile_HandleFrames();
}

void KleptoMobile_State3_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)entity->parent;

    int parentX = parent->position.x;
    int parentY = parent->position.y;

    int moveX = 0;
    int moveY = -0x200000 - (RSDK.Sin256((((entity->type << 7) - 0x80) ^ 0x80) + 2 * Zone->timer) << 12);

    entity->direction = parent->direction;
    int x             = parent->position.x;
    int y             = parent->position.y - 0x180000;

    int x2 = 0;
    int y2 = 0;
    if (entity->direction == FLIP_X) {
        moveX = parentX + 0x200000;
        x2    = ((entity->position.x + parent->position.x) >> 1) + 0x80000;
    }
    else {
        moveX = parentX - 0x200000;
        x2    = ((entity->position.x + parent->position.x) >> 1) - 0x80000;
    }

    y2 = ((parent->position.y - 0x180000 + entity->position.y) >> 1) - 0x300000;

    entity->velocity.x += ((moveX - entity->position.x) >> 5) - (entity->velocity.x >> 3);
    entity->velocity.y += (((moveY + parentY) - entity->position.y) >> 5) - (entity->velocity.y >> 3);
    entity->position.y += entity->velocity.y;
    entity->position.x += entity->velocity.x;

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        entity->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, entity->position.x, entity->position.y);
        percent += 0x2000;
    }

    parent->circlePos.x = entity->framePositions[6].x;
    parent->circlePos.y = entity->framePositions[6].y;

    if (entity->direction)
        parent->circlePos.x += 0x80000;
    else
        parent->circlePos.x -= 0x80000;
    parent->circlePos.y += 0x80000;
}

void KleptoMobile_State3_Unknown2(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)entity->parent;

    entity->direction = parent->direction;
    int parentX       = parent->position.x;
    int parentY       = parent->position.y - 0x180000;

    int x = 0;
    if (entity->direction == FLIP_X)
        x = ((parent->position.x + entity->position.x) >> 1) + 0x80000;
    else
        x = ((parent->position.x + entity->position.x) >> 1) - 0x80000;
    int y = ((parentY + entity->position.y) >> 1) - 0x300000;

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        entity->framePositions[i] = MathHelpers_Unknown5(percent, parentX, parentY, x, y, x, y, entity->position.x, entity->position.y);
        percent += 0x2000;
    }

    parent->circlePos.x = entity->framePositions[6].x;
    parent->circlePos.y = entity->framePositions[6].y;

    if (entity->direction)
        parent->circlePos.x += 0x80000;
    else
        parent->circlePos.x -= 0x80000;
    parent->circlePos.y += 0x80000;
}

void KleptoMobile_StateDraw3_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)entity->parent;

    if (parent->field_120)
        RSDK.DrawSprite(&parent->animator11, &parent->circlePos, false);

    for (int i = 0; i < 6; ++i) RSDK.DrawSprite(&entity->animator7, &entity->framePositions[i], false);

    RSDK.DrawSprite(&entity->animator8, &entity->framePositions[6], false);
}

void KleptoMobile_CheckPlayerCollisions_Arm(void)
{
    RSDK_THIS(KleptoMobile);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            if (player->superState == SUPERSTATE_SUPER) {
                if (!player->blinkTimer) {
                    RSDK.PlaySfx(KleptoMobile->sfxWhack, false, 255);
                    player->blinkTimer = 120;
                    if (entity->state == KleptoMobile_State1_Unknown4) {
                        player->velocity.x = entity->parent->velocity.x >> 2;
                        player->groundVel  = player->velocity.x;
                        if (player->position.y <= entity->position.y || player->onGround == true) {
                            player->onGround   = false;
                            player->velocity.y = -0xA0000;
                        }
                        else {
                            player->velocity.y = 0xA0000;
                        }
                    }
                    else {
                        player->velocity.x = entity->velocity.x >> 2;
                        player->velocity.y = entity->velocity.y >> 2;
                        player->groundVel  = player->velocity.x;
                    }
                    player->rotation = 0;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_RUN, &player->playerAnimator, false, 0);
                    player->state = ERZStart_State_PlayerRebound;
                }
            }
            else {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void KleptoMobile_State1_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)entity->parent;

    int parentX = parent->position.x;
    int parentY = parent->position.y;

    int moveX         = 0;
    int moveY         = ((RSDK.Sin256((((entity->type << 7) - 128) ^ 0x80) + 2 * Zone->timer) + 512) << 12) + parentY;
    entity->direction = parent->direction;

    int x = 0, y = 0;
    int x2 = 0, y2 = 0;

    if (entity->direction == FLIP_X) {
        moveX = parentX + 0x300000;
        x     = parentX - 0x30000;
        y     = parentY + 0xD0000;
        if (entity->type == 2) {
            x += 0x180000;
            moveX = parentX + 0x600000;
        }
        x2 = ((entity->position.x + x) >> 1) - 0x100000;
        y2 = ((entity->position.y + y) >> 1) + 0x100000;
    }
    else {
        moveX = parentX - 0x300000;
        x     = parentX + 0x30000;
        y     = parentY + 0xD0000;
        if (entity->type == 2) {
            x -= 0x180000;
            moveX = parentX - 0x600000;
        }
        x2 = ((entity->position.x + x) >> 1) + 0x200000;
        y2 = ((entity->position.y + y) >> 1) + 0x200000;
    }

    entity->velocity.x += ((moveX - entity->position.x) >> 5) - (entity->velocity.x >> 3);
    entity->velocity.y += ((moveY - entity->position.y) >> 5) - (entity->velocity.y >> 3);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        entity->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, entity->position.x, entity->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&entity->animator9);
    RSDK.ProcessAnimation(&entity->animator10);
}

void KleptoMobile_State1_Unknown2(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)entity->parent;

    entity->direction = parent->direction;

    int x = 0;
    int y = parent->position.y + 0x60000;
    if (entity->direction == FLIP_X) {
        x = parent->position.x - 0x90000;
        if (entity->type == 2)
            x += 0x180000;
    }
    else {
        x = parent->position.x + 0x90000;
        if (entity->type == 2)
            x -= 0x180000;
    }

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        entity->framePositions[i] = MathHelpers_Unknown5(percent, x, y, entity->field_F0.x, entity->field_F0.y, entity->field_F0.x,
                                                         entity->field_F0.y, entity->position.x, entity->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&entity->animator9);
    RSDK.ProcessAnimation(&entity->animator10);
}

void KleptoMobile_State1_Unknown3(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)entity->parent;

    if (!entity->timer) {
        entity->field_100.x = entity->position.x;
        entity->field_100.y = entity->position.y;
    }

    entity->direction = parent->direction;

    int x = 0;
    int y = parent->position.y + 0xD0000;

    int x2 = 0, y2 = 0;
    int x3 = 0, y3 = 0;

    if (entity->direction == FLIP_X) {
        x = parent->position.x - 0x30000;
        if (entity->type == 2)
            x += 0x180000;
    }
    else {
        x = parent->position.x + 0x30000;
        if (entity->type == 2)
            x -= 0x180000;
    }

    if (entity->timer >= 4) {
        x2                 = x;
        y2                 = y;
        x3                 = entity->position.x;
        y3                 = entity->position.y;
        entity->position.x = entity->field_F8.x;
        entity->position.y = entity->field_F8.y;
    }
    else {
        entity->velocity.x = (entity->field_F8.x - entity->field_100.x) / 4;
        entity->velocity.y = (entity->field_F8.y - entity->field_100.y) / 4;

        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;

        int distX = entity->position.x - x;
        int distY = entity->position.y - y;

        int distValX = -distX;
        int distValY = distY;
        if (distX > 0) {
            distValX = distX;
            distValY = -distY;
        }

        int timerVal = 4 - entity->timer;
        x2           = x + ((x + (distX >> 1) + distValY) - x) * timerVal / 4;
        y2           = y + (((distY >> 1) + distValX) * timerVal) / 4;
        x3           = entity->position.x + ((x + (distX >> 1) + distValY) - entity->position.x) * timerVal / 4;
        y3           = entity->position.y + (((distY >> 1) + distValX - entity->position.y) * timerVal / 4);
    }

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        entity->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x3, y3, entity->position.x, entity->position.y);
        percent += 0x2000;
    }

    if (++entity->timer > 15) {
        entity->timer = 0;
        entity->state = KleptoMobile_State1_Unknown1;
    }
    RSDK.ProcessAnimation(&entity->animator9);
    RSDK.ProcessAnimation(&entity->animator10);

    if (parent->health > 0)
        KleptoMobile_CheckPlayerCollisions_Arm();
}

void KleptoMobile_State1_Unknown4(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)entity->parent;

    int parentX = parent->position.x;
    int parentY = parent->position.y;

    int moveX = 0;
    int moveY = parentY + 0x180000;
    if (entity->type == 2)
        moveY = parentY - 0x180000;

    moveX = parentX - 0x400000;
    if (entity->direction == FLIP_X)
        moveX = parentX + 0x400000;

    int x = 0;
    int y = parent->position.y + 0xD0000;
    if (entity->direction == FLIP_X) {
        moveX += 0x300000;
        x = parent->position.x - 0x30000;
        if (entity->type == 2) {
            x += 0x180000;
            moveX += 0x300000;
        }
    }
    else {
        moveX -= 0x300000;
        x = parent->position.x + 0x30000;
        if (entity->type == 2) {
            x -= 0x180000;
            moveX -= 0x300000;
        }
    }

    int x2 = (x + entity->position.x) >> 1;
    int y2 = (y + entity->position.y) >> 1;

    entity->velocity.x += ((moveX - entity->position.x) >> 5) - (entity->velocity.x >> 3);
    entity->velocity.y += ((moveY - entity->position.y) >> 5) - (entity->velocity.y >> 3);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        entity->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, entity->position.x, entity->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&entity->animator9);
    RSDK.ProcessAnimation(&entity->animator10);

    if (parent->health > 0)
        KleptoMobile_CheckPlayerCollisions_Arm();
}

void KleptoMobile_StateDraw1_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);

    for (int i = 0; i < 6; ++i) RSDK.DrawSprite(&entity->animator7, &entity->framePositions[i], false);

    RSDK.DrawSprite(&entity->animator8, &entity->framePositions[6], false);

    if (entity->direction) {
        if (entity->type == 2) {
            entity->framePositions[6].x += 0x280000;
            entity->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);

            entity->framePositions[6].y += 0x100000;
            RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);

            entity->framePositions[6].x -= 0x40000;
            entity->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&entity->animator10, &entity->framePositions[6], false);
        }
        else {
            entity->framePositions[6].x += 0x280000;
            RSDK.DrawSprite(&entity->animator10, &entity->framePositions[6], false);

            entity->framePositions[6].x -= 0x40000;
            entity->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);

            entity->framePositions[6].y += 0x100000;
            RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);

            entity->framePositions[6].y -= 0x80000;
        }
    }
    else {
        if (entity->type == 2) {
            entity->framePositions[6].x -= 0x280000;
            entity->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);
            entity->framePositions[6].y += 0x100000;
            RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);
            entity->framePositions[6].x += 0x40000;
            entity->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&entity->animator10, &entity->framePositions[6], false);
        }
        else {
            entity->framePositions[6].x -= 0x280000;
            RSDK.DrawSprite(&entity->animator10, &entity->framePositions[6], false);

            entity->framePositions[6].x += 0x40000;
            entity->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);

            entity->framePositions[6].y += 0x100000;
            RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);

            entity->framePositions[6].y -= 0x80000;
        }
    }
}

void KleptoMobile_State_Destroyed(void)
{
    RSDK_THIS(KleptoMobile);

    RSDK.ProcessAnimation(&entity->animator3);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(KleptoMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = entity->position.x + (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16);
            int y = entity->position.y + (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++entity->timer == 96) {
        Debris_FallFlickerAnimSetup(KleptoMobile->aniFrames, KleptoMobile->debrisInfo, 18);
    }
    else if (entity->timer == 144) {
        KleptoMobile->defeated = true;
        if (PhantomKing->defeated) {
            entity->position.y += 0x1000000;
            foreach_all(ERZOutro, outro)
            {
                outro->active = ACTIVE_NORMAL;
                foreach_break;
            }
        }
        entity->state = KleptoMobile_State_Explode;
    }
}

void KleptoMobile_State_Explode(void)
{
    RSDK_THIS(KleptoMobile);

    entity->velocity.y += 0x2800;
    entity->position.y += entity->velocity.y;

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(KleptoMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = entity->position.x + (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16);
            int y = entity->position.y + (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
    if (entity->position.y >= 0x2800000) {
        if (!PhantomKing->defeated) {
            KleptoMobile_SwitchToKing();
            entity->state = StateMachine_None;
        }
    }
}

void KleptoMobile_State_CutsceneExplode(void)
{
    RSDK_THIS(KleptoMobile);

    RSDK.ProcessAnimation(&entity->animator3);
    entity->field_70.x += entity->velocity.x;
    entity->field_70.y += entity->velocity.y;
    entity->position.x = entity->field_70.x;
    entity->position.y = entity->field_70.y;

    if (entity->explosionVolume > 0) {
        if (!(Zone->timer % 3)) {
            int channel = RSDK.PlaySfx(KleptoMobile->sfxExplosion, false, 255);
            RSDK.SetChannelAttributes(channel, entity->explosionVolume * (1 / 512.0f), 0.0, 1.0);
            if ((Zone->timer & 4) != 0) {
                int x = entity->position.x + (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16);
                int y = entity->position.y + (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
        if (entity->timer >= 60)
            entity->explosionVolume -= 2;
        entity->timer++;
    }
}

#if RETRO_INCLUDE_EDITOR
void KleptoMobile_EditorDraw(void)
{
    RSDK_THIS(KleptoMobile);
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &entity->animator2, false, 1);
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &entity->animator3, false, 0);
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &entity->animator4, false, 0);

    KleptoMobile_StateDraw_Unknown1();
}

void KleptoMobile_EditorLoad(void) { KleptoMobile->aniFrames = RSDK.LoadSpriteAnimation("Phantom/KleptoMobile.bin", SCOPE_STAGE); }
#endif

void KleptoMobile_Serialize(void) { RSDK_EDITABLE_VAR(KleptoMobile, VAR_ENUM, type); }
