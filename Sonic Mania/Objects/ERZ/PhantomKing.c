#include "SonicMania.h"

ObjectPhantomKing *PhantomKing;

void PhantomKing_Update(void)
{
    RSDK_THIS(PhantomKing);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    StateMachine_Run(entity->state);
}

void PhantomKing_LateUpdate(void) {}

void PhantomKing_StaticUpdate(void) {}

void PhantomKing_Draw(void)
{
    RSDK_THIS(PhantomKing);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void PhantomKing_Create(void *data)
{
    RSDK_THIS(PhantomKing);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x1000000;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->drawFX        = FX_ROTATE | FX_FLIP;
        entity->type          = voidToInt(data);

        switch (entity->type) {
            case 0: // main
                entity->hitbox.left   = -24;
                entity->hitbox.top    = -24;
                entity->hitbox.right  = 24;
                entity->hitbox.bottom = 24;
                entity->active        = ACTIVE_BOUNDS;
                entity->visible       = false;
                entity->direction     = FLIP_X;
                entity->health        = 8;
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 0, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 1, &entity->animator3, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 2, &entity->animator4, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 7, &entity->animator5, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &entity->animatorRuby, true, 0);
                entity->posUnknown = entity->position;
                entity->state      = PhantomKing_State_Unknown1;
                break;
            case 1: // body
            case 2: // body
                entity->active  = ACTIVE_NORMAL;
                entity->visible = true;
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 3, &entity->animator8, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 4, &entity->animator9, true, 0);
                if (entity->type == 1) {
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 6, &entity->animator10, true, 0);
                }
                else {
                    entity->drawOrder = Zone->playerDrawLow;
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 5, &entity->animator10, true, 0);
                }
                entity->stateDraw = PhantomKing_StateDraw_Arm;
                entity->state     = PhantomKing_StateArm1_Unknown2;
                break;
        }
    }
}

void PhantomKing_StageLoad(void)
{
    PhantomKing->aniFrames    = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);
    PhantomKing->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    PhantomKing->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
}

void PhantomKing_CheckPlayerCollisions(void)
{
    RSDK_THIS(PhantomKing);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
            PhantomKing_Hit();
        }
    }
}

void PhantomKing_Hit(void)
{
    RSDK_THIS(PhantomKing);

    foreach_all(PhantomRuby, ruby)
    {
        ruby->position.x = entity->rubyPos.x;
        ruby->position.y = entity->rubyPos.y;
        ruby->velocity.x = -0x20000;
        ruby->velocity.y = -0x40000;
        ruby->state      = PhantomRuby_Unknown6;
    }

    entity->drawRuby = false;
    if (--entity->health <= 0) {
        entity->posUnknown.x = entity->position.x;
        entity->posUnknown.y = entity->position.y;
        entity->state        = PhantomKing_State_Explode;
        entity->velocity.y   = -0x10000;
        entity->timer        = 0;
        foreach_active(PKingAttack, attack)
        {
            if (attack->state == PKingAttack_Unknown3)
                destroyEntity(attack);
        }

        if (KleptoMobile->defeated)
            RSDK_sceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        entity->invincibilityTimer = 48;
        RSDK.PlaySfx(PhantomKing->sfxHit, false, 255);
        entity->state = PhantomKing_State_HitFall;
    }
}

void PhantomKing_HandleFrames(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->state != PhantomKing_State_Unknown7) {
        int max = entity->velocity.x >> 15;
        if (entity->rotation >= max) {
            if (entity->rotation > max) {
                entity->rotation--;
                if (entity->rotation < max)
                    entity->rotation = entity->velocity.x >> 15;
            }
        }
        else {
            entity->rotation++;
            if (entity->rotation > max)
                entity->rotation = max;
        }
    }

    int negAng     = -entity->rotation;
    entity->angle2 = (entity->angle2 + 12) & 0x3FF;

    int x = 0x1C00 * RSDK.Sin512(negAng) + entity->position.x;
    int y = 0x1C00 * RSDK.Cos512(negAng) + entity->position.y;

    int angle = entity->angle2;

    for (int i = 0; i < 10; i += 2) {
        entity->framePositions[i].x = x + 2 * RSDK.Cos512(entity->rotation) * RSDK.Cos1024(angle);
        entity->framePositions[i].y = y + 2 * RSDK.Sin512(entity->rotation) * RSDK.Cos1024(angle);
        entity->frameIDs[i]         = angle & 0x3FF;

        angle += 512;

        entity->framePositions[i + 1].x = x + 2 * RSDK.Cos512(entity->rotation) * RSDK.Cos1024(angle);
        entity->framePositions[i + 1].y = y + 2 * RSDK.Sin512(entity->rotation) * RSDK.Cos1024(angle);
        entity->frameIDs[i + 1]         = angle & 0x3FF;

        x += RSDK.Sin512(negAng) << 10;
        y += RSDK.Cos512(negAng) << 10;
        angle += 0x240;
    }

    entity->rubyPos.x = entity->position.x;
    entity->rubyPos.y = entity->position.y;
    entity->rubyPos.x -= 0x1400 * RSDK.Sin512(negAng);
    entity->rubyPos.y -= 0x1400 * RSDK.Cos512(negAng);
    if (entity->direction) {
        entity->rubyPos.x -= 0x180 * RSDK.Cos512(negAng);
        entity->rubyPos.y -= 0x180 * RSDK.Sin512(negAng);
    }
    else {
        entity->rubyPos.x += 0x180 * RSDK.Cos512(negAng);
        entity->rubyPos.y += 0x180 * RSDK.Sin512(negAng);
    }
}

void PhantomKing_HandleAttacks(void)
{
    RSDK_THIS(PhantomKing);

    EntityPlayer *player = Player_GetNearestPlayer();
    bool32 flag          = false;

    foreach_active(PKingAttack, attack)
    {
        if (attack->state == PKingAttack_Unknown3) {
            PhantomRuby_PlaySFX(RSDK.Rand(RUBYSFX_ATTACK1, RUBYSFX_REDCUBE));
            attack->type = 2;
            int angle    = RSDK.ATan2((player->position.x - attack->position.x) >> 16, (player->position.y - attack->position.y) >> 16);

            attack->field_70.x = 0x600 * RSDK.Cos256(angle);
            attack->field_70.y = 0x600 * RSDK.Sin256(angle);
            attack->drawOrder  = Zone->drawOrderLow;
            attack->state      = PKingAttack_Unknown4;
            flag               = true;
            foreach_break;
        }
    }

    if (!flag) {
        for (int i = 0; i < 0x3FC; i += 0xAA) {
            EntityPKingAttack *attack = CREATE_ENTITY(PKingAttack, intToVoid(1), entity->position.x, entity->position.y);
            attack->angle             = i;
            attack->target            = (Entity *)entity;
        }
        entity->timer = -90;
    }
}

void PhantomKing_SwitchToEggman(void)
{
    RSDK_THIS(PhantomKing);

    EntityKleptoMobile *eggmanPtr = NULL;

    foreach_active(KleptoMobile, eggman)
    {
        if (!eggman->type) {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            eggman->direction     = FLIP_X;
            if (RSDK.Rand(0, 2))
                eggman->position.x = player1->position.x + 0x1000000;
            else
                eggman->position.x = player1->position.x - 0x1000000;
            eggman->position.y = player1->position.y + 0x800000;
            eggman->field_70.y = player1->position.y + 0x800000;
            eggman->field_120  = 1;
            eggman->velocity.x = 0;
            eggman->velocity.y = 0;
            eggman->state      = KleptoMobile_State_Unknown2;
            eggmanPtr          = eggman;
            foreach_break;
        }
    }

    foreach_active(KleptoMobile, eggmanArm)
    {
        if (eggmanArm->type) {
            eggmanArm->position.x = eggmanPtr->position.x;
            eggmanArm->position.y = eggmanPtr->position.y;
            for (int i = 0; i < 10; ++i) eggmanArm->framePositions[i] = eggmanPtr->position;
            eggmanArm->velocity.x = 0;
            eggmanArm->velocity.y = 0;
        }
    }
}

void PhantomKing_SetupKing(EntityPhantomKing *king)
{
    king->posUnknown     = king->position;
    king->velocity.x     = 0;
    king->drawRuby      = true;
    PhantomKing->boundsM = king->position.x;
    PhantomKing->boundsL = PhantomKing->boundsM - 0x800000;
    PhantomKing->boundsR = PhantomKing->boundsM + 0x800000;
    PhantomKing->boundsT = (Zone->screenBoundsT1[0] + 48) << 16;
    PhantomKing->boundsB = (Zone->screenBoundsB1[0] - 96) << 16;

    int slot                                      = RSDK.GetEntityID(king);
    RSDK_GET_ENTITY(slot - 1, PhantomKing)->state = PhantomKing_StateArm1_Unknown2;
    RSDK_GET_ENTITY(slot + 1, PhantomKing)->state = PhantomKing_StateArm1_Unknown2;
    king->state                                   = PhantomKing_State_Unknown8;
}

void PhantomKing_StateDraw_Body(void)
{
    RSDK_THIS(PhantomKing);

    if (entity->typeChangeTimer <= 0) {
        if (entity->invincibilityTimer & 1)
            RSDK.CopyPalette(2, 128, 0, 128, 128);
    }
    else {
        RSDK.SetLimitedFade(0, 1, 4, entity->typeChangeTimer, 0, 48);
        RSDK.SetLimitedFade(0, 1, 4, entity->typeChangeTimer, 128, 256);
    }
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    for (int i = 0; i < 10; ++i) {
        if (entity->frameIDs[i] < 0x200) {
            entity->animator5.frameID = entity->frameIDs[i] / 42 % 6;
            RSDK.DrawSprite(&entity->animator5, &entity->framePositions[i], false);
        }
    }

    entity->drawFX = entity->storeDrawFX | FX_ROTATE;
    RSDK.DrawSprite(&entity->animator4, NULL, false);
    entity->drawFX = entity->storeDrawFX | FX_ROTATE | FX_FLIP;

    for (int i = 0; i < 10; ++i) {
        if (entity->frameIDs[i] >= 0x200) {
            entity->animator5.frameID = entity->frameIDs[i] / 42 % 6;
            RSDK.DrawSprite(&entity->animator5, &entity->framePositions[i], false);
        }
    }

    if (entity->drawRuby)
        RSDK.DrawSprite(&entity->animatorRuby, &entity->rubyPos, false);

    if (entity->typeChangeTimer <= 0) {
        if (entity->invincibilityTimer & 1)
            RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomKing_StateDraw_Arm(void)
{
    RSDK_THIS(PhantomKing);
    EntityPhantomKing *parent = (EntityPhantomKing *)entity->parent;

    if (parent->typeChangeTimer > 0) {
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 0, 48);
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 128, 256);
    }

    for (int i = 0; i < 6; ++i) RSDK.DrawSprite(&entity->animator8, &entity->framePositions[i], false);
    RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);
    RSDK.DrawSprite(&entity->animator10, &entity->framePositions[6], false);

    if (parent->typeChangeTimer > 0) {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomKing_State_Unknown1(void)
{
    RSDK_THIS(PhantomKing);

    if (++entity->timer >= 8) {
        entity->position.y += 0x1000000;
        entity->timer  = 0;
        entity->active = ACTIVE_NORMAL;
        entity->state  = StateMachine_None;
    }
}

void PhantomKing_State_Unknown2(void)
{
    RSDK_THIS(PhantomKing);

    entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < entity->position.x;
    if (++entity->timer == 30) {
        EntityPhantomKing *arm1 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, PhantomKing);
        RSDK.ResetEntityPtr(arm1, PhantomKing->objectID, intToVoid(1));
        arm1->position.x = entity->position.x;
        arm1->position.y = entity->position.y;
        arm1->parent     = (Entity *)entity;
        arm1->field_11C  = 0x2800;
        arm1->field_120  = 96;
        arm1->field_124  = 32;

        EntityPhantomKing *arm2 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, PhantomKing);
        RSDK.ResetEntityPtr(arm2, PhantomKing->objectID, intToVoid(2));
        arm2->position.x = entity->position.x;
        arm2->position.y = entity->position.y;
        arm2->parent     = (Entity *)entity;
        arm2->field_11C  = 0x2800;
        arm2->field_120  = 96;
        arm2->field_124  = 32;

        entity->timer     = 0;
        entity->visible   = true;
        entity->stateDraw = PhantomKing_StateDraw_Body;
        entity->state     = PhantomKing_State_Unknown3;
    }
}

void PhantomKing_State_Unknown3(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&entity->animator4);
    entity->velocity.y -= 0x800;

    if (entity->position.y <= entity->posUnknown.y) {
        entity->posUnknown.x                                                = entity->position.x;
        entity->posUnknown.y                                                = entity->posUnknown.y;
        entity->position.y                                                  = entity->posUnknown.y;
        RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, PhantomKing)->state = PhantomKing_StateArm1_Unknown1;
        entity->velocity.y                                                  = 0;
        entity->state                                                       = PhantomKing_State_Unknown4;
    }
    else {
        entity->position.y += entity->velocity.y;
    }
    PhantomKing_HandleFrames();
}

void PhantomKing_State_Unknown4(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&entity->animator4);

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown.y;

    PhantomKing_HandleFrames();
    if (++entity->timer >= 120) {
        entity->timer = 0;
        entity->state = PhantomKing_State_Unknown5;
    }
}

void PhantomKing_State_Unknown5(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&entity->animator4);

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown.y;

    if (entity->velocity.x < 0x40000)
        entity->velocity.x += 0x1800;
    entity->position.x += entity->velocity.x;

    PhantomKing_HandleFrames();
    if (++entity->timer >= 180) {
        RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, PhantomKing)->state = PhantomKing_StateArm1_Unknown2;
        entity->velocity.x                                                  = 0;
        entity->velocity.y                                                  = 0;
        entity->state                                                       = PhantomKing_State_Unknown6;
        entity->field_224                                                   = true;
    }
}

void PhantomKing_State_Unknown6(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&entity->animator4);

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown.y;

    PhantomKing_HandleFrames();
}

void PhantomKing_State_Unknown7(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&entity->animator4);

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown.y;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    PhantomKing_HandleFrames();
}

void PhantomKing_State_Unknown8(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&entity->animator4);

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown.y;

    PhantomKing_CheckPlayerCollisions();
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    int angle = RSDK.ATan2(entity->position.x - player1->position.x, entity->posUnknown.y - player1->position.y);
    int x     = (RSDK.Cos256(angle) << 15) + player1->position.x;
    int y     = (RSDK.Sin256(angle) << 15) + player1->position.y;

    if (x <= entity->position.x) {
        if (entity->velocity.x > -0x20000)
            entity->velocity.x -= 0x800;
    }
    else {
        if (entity->velocity.x < 0x20000)
            entity->velocity.x += 0x800;
    }

    int bottom = Zone->screenBoundsB1[0] << 16;
    if (y > bottom - 0x400000)
        y = bottom - 0x800000;

    if (y <= entity->posUnknown.y) {
        if (entity->velocity.y > -0x20000)
            entity->velocity.y -= 0x800;
    }
    else {
        if (entity->velocity.y < 0x20000)
            entity->velocity.y += 0x800;
    }

    entity->posUnknown.y += entity->velocity.y;
    entity->position.x += entity->velocity.x;
    entity->direction = player1->position.x <= entity->position.x;

    if (entity->field_114) {
        if (++entity->timer >= 60) {
            entity->timer = 0;
            PhantomKing_HandleAttacks();
        }
    }
    else if (entity->activeScreens == 1) {
        entity->field_114 = true;
    }
    PhantomKing_HandleFrames();
}

void PhantomKing_State_Switch(void)
{
    RSDK_THIS(PhantomKing);

    if (++entity->timer >= 120) {
        entity->timer = 0;
        PhantomKing_SwitchToEggman();
        entity->state = StateMachine_None;
    }
}

void PhantomKing_State_HitFall(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&entity->animator4);
    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x3800;

    if (entity->position.y < -0x1000000) {
        foreach_active(PKingAttack, attack)
        {
            if (attack->state == PKingAttack_Unknown3)
                destroyEntity(attack);
        }
        entity->field_114  = false;
        entity->velocity.y = 0;
        entity->timer      = 0;
        entity->state      = PhantomKing_State_Switch;
    }
    PhantomKing_HandleFrames();
}

void PhantomKing_StateArm1_Unknown2(void)
{
    RSDK_THIS(PhantomKing);
    EntityPhantomKing *parent = (EntityPhantomKing *)entity->parent;

    int parentX = parent->position.x;
    int parentY = parent->position.y;

    int moveX = 0;
    int moveY = ((RSDK.Sin256(2 * (Zone->timer + (entity->type << 6)) - 128) + 512) << 12) + parentY;

    entity->direction = parent->direction;
    int negAngle      = -parent->rotation;

    int x = 0;
    int y = 0;

    int x2 = 0;
    int y2 = 0;

    if (entity->direction) {
        moveX = parentX - 0x300000;
        x     = 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle) + parent->position.x;
        y     = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);
        if (entity->type == 1) {
            x += -0x1800 * RSDK.Cos512(negAngle);
            y += 0x1800 * RSDK.Sin512(negAngle);
            moveX -= 0x300000;
        }
        x2 = ((entity->position.x + x) >> 1) + 0x200000;
        y2 = ((entity->position.y + y) >> 1) + 0x200000;
    }
    else {
        moveX = parentX + 0x300000;
        x     = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y     = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;
        if (entity->type == 1) {
            x += 0x1800 * RSDK.Cos512(negAngle);
            y += -0x1800 * RSDK.Sin512(negAngle);
            moveX += 0x300000;
        }
        x2 = ((entity->position.x + x) >> 1) - 0x100000;
        y2 = ((entity->position.y + y) >> 1) + 0x100000;
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

void PhantomKing_StateArm1_Unknown3(void)
{
    RSDK_THIS(PhantomKing);
    EntityPhantomKing *parent = (EntityPhantomKing *)entity->parent;

    entity->direction = parent->direction;
    int negAngle      = -parent->rotation;

    int x = 0;
    int y = 0;

    if (entity->direction) {
        x = parent->position.x + 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle);
        y = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);

        if (entity->type == 1) {
            x += -0x1800 * RSDK.Cos512(negAngle);
            y += 0x1800 * RSDK.Sin512(negAngle);
        }
    }
    else {
        x = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;

        if (entity->type == 1) {
            x += 0x1800 * RSDK.Cos512(negAngle);
            y += -0x1800 * RSDK.Sin512(negAngle);
        }
    }

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        entity->framePositions[i] = MathHelpers_Unknown5(percent, x, y, entity->field_F8, entity->field_FC, entity->field_F8, entity->field_FC,
                                                         entity->position.x, entity->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&entity->animator9);
    RSDK.ProcessAnimation(&entity->animator10);
}

void PhantomKing_Unknown21(void)
{
    RSDK_THIS(PhantomKing);
    EntityPhantomKing *parent = (EntityPhantomKing *)entity->parent;

    RSDK.Sin256(2 * (Zone->timer + (entity->type << 6)) - 128);
    ++entity->timer;
    entity->direction = parent->direction;
    int negAngle      = -parent->rotation;

    int x = 0;
    int y = 0;

    int x2 = 0;
    int y2 = 0;

    int x3 = 0;
    int y3 = 0;

    if (entity->direction) {
        x = parent->position.x + 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle);
        y = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);
        if (entity->type == 1) {
            x += -0x1800 * RSDK.Cos512(negAngle);
            y += 0x1800 * RSDK.Sin512(negAngle);
        }
        x2 = x + entity->field_11C * RSDK.Cos256(entity->field_120);
        y2 = y + entity->field_11C * RSDK.Sin256(entity->field_120);
        x3 = x2 + entity->field_11C * RSDK.Cos256(entity->field_124 + entity->field_120);
        y3 = y2 + entity->field_11C * RSDK.Sin256(entity->field_124 + entity->field_120);
    }
    else {
        x = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;
        if (entity->type == 1) {
            x += 0x1800 * RSDK.Cos512(negAngle);
            y += -0x1800 * RSDK.Sin512(negAngle);
        }
        x2 = x + entity->field_11C * RSDK.Cos256(entity->field_120);
        y2 = y + entity->field_11C * RSDK.Sin256(entity->field_120);
        x3 = x2 + entity->field_11C * RSDK.Cos256(entity->field_124 + entity->field_120);
        y3 = y2 + entity->field_11C * RSDK.Sin256(entity->field_124 + entity->field_120);
    }

    entity->velocity.x += ((x3 - entity->position.x) >> 5) - (entity->velocity.x >> 3);
    entity->velocity.y += ((y3 - entity->position.y) >> 5) - (entity->velocity.y >> 3);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        entity->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, x3, y3);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&entity->animator9);
    RSDK.ProcessAnimation(&entity->animator10);
}

void PhantomKing_StateArm1_Unknown1(void)
{
    RSDK_THIS(PhantomKing);

    PhantomKing_Unknown21();

    uint8 flags = 0;

    if (entity->field_120 >= 192)
        flags = 1;
    else
        entity->field_120 += 3;

    if (entity->field_124 >= 96)
        ++flags;
    else
        entity->field_124 += 3;

    if (flags == 2)
        entity->state = PhantomKing_StateArm1_Unknown4;
}

void PhantomKing_StateArm1_Unknown4(void)
{
    RSDK_THIS(PhantomKing);

    PhantomKing_Unknown21();

    if (entity->field_120 <= 128)
        entity->field_120 = 128;
    else
        entity->field_120 -= 16;

    if (entity->field_124 <= 0)
        entity->field_124 = 0;
    else
        entity->field_124 -= 16;
}

void PhantomKing_DestroyEntity(void)
{
    RSDK_THIS(PhantomKing);

    int slot                = RSDK.GetEntityID(entity);
    EntityPhantomKing *arm1 = RSDK_GET_ENTITY(slot - 1, PhantomKing);
    EntityPhantomKing *arm2 = RSDK_GET_ENTITY(slot + 1, PhantomKing);

    foreach_all(PKingAttack, attack) { destroyEntity(attack); }
    destroyEntity(arm1);
    destroyEntity(arm2);
    destroyEntity(entity);
}

void PhantomKing_State_Explode(void)
{
    RSDK_THIS(PhantomKing);

    if (!(Zone->timer % 7)) {
        RSDK.PlaySfx(PhantomKing->sfxExplosion, false, 255);
        if (Zone->timer & 8) {
            int x = entity->position.x + (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16);
            int y = entity->position.y + (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
    if (++entity->timer == 60)
        entity->state = PhantomKing_State_Destroyed;
}

void PhantomKing_State_Destroyed(void)
{
    RSDK_THIS(PhantomKing);

    entity->velocity.y += 0x2800;
    entity->position.y += entity->velocity.y;
    if (!(Zone->timer % 7)) {
        RSDK.PlaySfx(PhantomKing->sfxExplosion, false, 255);
        if (Zone->timer & 8) {
            int x = entity->position.x + (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16);
            int y = entity->position.y + (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (entity->position.y >= 0x2800000) {
        PhantomKing->defeated = true;
        if (!KleptoMobile->defeated) {
            PhantomKing_SwitchToEggman();
        }
        else {
            foreach_all(ERZOutro, outro)
            {
                outro->active = ACTIVE_NORMAL;
                foreach_break;
            }
        }
        PhantomKing_DestroyEntity();
    }
    PhantomKing_HandleFrames();
}

#if RETRO_INCLUDE_EDITOR
void PhantomKing_EditorDraw(void)
{
    RSDK_THIS(PhantomKing);

    entity->posUnknown               = entity->position;
    entity->rotation                 = 0;
    entity->angle2                   = 0;
    entity->animator3.animationSpeed = 0;
    PhantomKing_HandleFrames();

    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 0, &entity->animator2, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 1, &entity->animator3, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 2, &entity->animator4, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 7, &entity->animator5, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &entity->animatorRuby, false, 0);

    PhantomKing_StateDraw_Body();
}

void PhantomKing_EditorLoad(void) { PhantomKing->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE); }
#endif

void PhantomKing_Serialize(void) { RSDK_EDITABLE_VAR(PhantomKing, VAR_ENUM, type); }
