#include "SonicMania.h"

ObjectERZKing *ERZKing;

void ERZKing_Update(void)
{
    RSDK_THIS(ERZKing);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    StateMachine_Run(entity->state);
}

void ERZKing_LateUpdate(void) {}

void ERZKing_StaticUpdate(void) {}

void ERZKing_Draw(void)
{
    RSDK_THIS(ERZKing);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else
        RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void ERZKing_Create(void *data)
{
    RSDK_THIS(ERZKing);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
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
                entity->visible       = 0;
                entity->direction     = FLIP_X;
                entity->health        = 8;
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 0, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 1, &entity->animator3, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 2, &entity->animator4, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 7, &entity->animator5, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 8, &entity->animator6, true, 0);
                entity->posUnknown = entity->position;
                entity->state      = ERZKing_State_Unknown1;
                break;
            case 1: // body
            case 2: // body
                entity->visible = true;
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 3, &entity->animator8, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 4, &entity->animator9, true, 0);
                if (entity->type == 1) {
                    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 6, &entity->animator10, true, 0);
                }
                else {
                    entity->drawOrder = Zone->playerDrawLow;
                    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 5, &entity->animator10, true, 0);
                }
                entity->stateDraw = ERZKing_StateDraw_Arm;
                entity->state     = ERZKing_State_Arm;
                break;
        }
    }
}

void ERZKing_StageLoad(void)
{
    ERZKing->aniFrames     = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);
    ERZKing->sfxHit        = RSDK.GetSFX("Stage/BossHit.wav");
    ERZKing->sfxExplosion2 = RSDK.GetSFX("Stage/Explosion2.wav");
}

void ERZKing_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZKing);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
            if (--entity->health <= 0) {
                entity->posUnknown.x        = entity->position.x;
                entity->posUnknown.y        = entity->position.y;
                entity->state               = ERZKing_State_Explode;
                entity->velocity.y          = -0x10000;
                entity->timer               = 0;
                RSDK_sceneInfo->timeEnabled = false;
            }
            else {
                entity->invincibilityTimer = 48;
                RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
            }
        }
    }
}

void ERZKing_HandleFrames(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&entity->animator3);

    entity->rotation = RSDK.Sin512(2 * Zone->timer) >> 6;
    int negAng       = -entity->rotation;

    entity->angle2 = (entity->angle2 + 12) & 0x3FF;

    int x = 0x1C00 * RSDK.Sin512(negAng) + entity->position.x;
    int y = 0x1C00 * RSDK.Cos512(negAng) + entity->position.y;

    int angle = entity->angle2;

    for (int i = 0; i < 10; i += 2) {
        entity->framePositions[0].x = x + 2 * RSDK.Cos512(entity->rotation) * RSDK.Cos1024(angle);
        entity->framePositions[0].y = y + 2 * RSDK.Sin512(entity->rotation) * RSDK.Cos1024(angle);
        entity->frameIDs[0]         = angle & 0x3FF;

        angle += 512;

        entity->framePositions[1].x = x + 2 * RSDK.Cos512(entity->rotation) * RSDK.Cos1024(angle);
        entity->framePositions[1].y = y + 2 * RSDK.Sin512(entity->rotation) * RSDK.Cos1024(angle);
        entity->frameIDs[1]         = angle & 0x3FF;

        x += RSDK.Sin512(negAng) << 10;
        y += RSDK.Cos512(negAng) << 10;
        angle += 0x240;
    }

    entity->drawPos.x = entity->position.x;
    entity->drawPos.y = entity->position.y;
    entity->drawPos.x -= 0x1400 * RSDK.Sin512(negAng);
    entity->drawPos.y -= 0x1400 * RSDK.Cos512(negAng);
    if (entity->direction) {
        entity->drawPos.x -= 0x180 * RSDK.Cos512(negAng);
        entity->drawPos.y -= 0x180 * RSDK.Sin512(negAng);
    }
    else {
        entity->drawPos.x += 0x180 * RSDK.Cos512(negAng);
        entity->drawPos.y += 0x180 * RSDK.Sin512(negAng);
    }
}

void ERZKing_StateDraw_Body(void)
{
    RSDK_THIS(ERZKing);

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

    RSDK.DrawSprite(&entity->animator6, &entity->drawPos, false);
    if (entity->typeChangeTimer <= 0) {
        if (entity->invincibilityTimer & 1)
            RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void ERZKing_StateDraw_Arm(void)
{
    RSDK_THIS(ERZKing);
    EntityERZKing *parent = (EntityERZKing *)entity->parent;

    if (parent->typeChangeTimer > 0) {
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 0, 48);
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 128, 256);
    }

    for (int i = 0; i < 6; ++i) {
        RSDK.DrawSprite(&entity->animator8, &entity->framePositions[i], false);
    }

    RSDK.DrawSprite(&entity->animator9, &entity->framePositions[6], false);
    RSDK.DrawSprite(&entity->animator10, &entity->framePositions[6], false);

    if (parent->typeChangeTimer > 0) {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void ERZKing_State_Unknown1(void)
{
    RSDK_THIS(ERZKing);

    if (++entity->timer >= 8) {
        entity->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - 320;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 320;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - RSDK_screens->height;
        ERZKing->boundsL            = (Zone->screenBoundsL1[0] + 64) << 16;
        ERZKing->boundsR            = (Zone->screenBoundsR1[0] - 64) << 16;
        ERZKing->boundsM            = entity->position.x;
        ERZKing->boundsT            = (Zone->screenBoundsT1[0] + 48) << 16;
        ERZKing->boundsB            = (Zone->screenBoundsB1[0] - 96) << 16;
        entity->position.y += 0x1000000;
        entity->active = ACTIVE_NORMAL;
        entity->state  = ERZKing_State_Unknown2;
    }
}

void ERZKing_State_Unknown2(void)
{
    RSDK_THIS(ERZKing);

    if (entity->timer) {
        entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < entity->position.x;
        if (++entity->timer == 30) {
            EntityERZKing *leftArm = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, ERZKing);
            RSDK.ResetEntityPtr(leftArm, ERZKing->objectID, intToVoid(1));
            leftArm->position.x = entity->position.x;
            leftArm->position.y = entity->position.y;
            leftArm->parent     = (Entity *)entity;

            EntityERZKing *rightArm = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, ERZKing);
            RSDK.ResetEntityPtr(rightArm, ERZKing->objectID, intToVoid(1));
            rightArm->position.x = entity->position.x;
            rightArm->position.y = entity->position.y;
            rightArm->parent     = (Entity *)entity;

            entity->timer     = 0;
            entity->visible   = true;
            entity->stateDraw = ERZKing_StateDraw_Body;
            entity->state     = ERZKing_State_Unknown3;
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x)
            ++entity->timer;
    }
}

void ERZKing_State_Unknown3(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&entity->animator4);
    entity->velocity.y -= 0x1800;
    if (entity->position.y <= entity->posUnknown.y - 0x200000) {
        entity->posUnknown.x = entity->position.x;
        entity->posUnknown.y = entity->position.y;
        entity->state        = ERZKing_State_Unknown4;
    }
    else {
        entity->position.y += entity->velocity.y;
    }
    ERZKing_HandleFrames();
}

void ERZKing_State_Unknown4(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&entity->animator4);

    entity->angle      = (entity->angle + 3) & 0xFF;
    entity->position.y = (RSDK.Sin256(entity->angle) << 11) + entity->posUnknown.y;
    ERZKing_CheckPlayerCollisions();

    if (entity->direction) {
        if (entity->velocity.x > -0x20000)
            entity->velocity.x -= 0x800;
        if (entity->position.x < ERZKing->boundsL)
            entity->direction = FLIP_NONE;
    }
    else {
        if (entity->velocity.x < 0x20000)
            entity->velocity.x += 0x800;
        if (entity->position.x > ERZKing->boundsR)
            entity->direction = FLIP_X;
    }
    entity->position.x += entity->velocity.x;
    ++entity->timer;

    if (entity->timer > 240) {
        if (abs(entity->position.x - ERZKing->boundsM) < 0x200000) {
            entity->timer                                                                               = 0;
            entity->scale.x                                                                             = 0x200;
            entity->scale.y                                                                             = 0x200;
            entity->storeDrawFX                                                                         = FX_SCALE;
            entity->state                                                                               = ERZKing_State_ChangeHBH;
            CREATE_ENTITY(FXRuby, FXRuby_Unknown9, entity->position.x, entity->position.y)->radiusSpeed = 0x80000;
        }
    }
    ERZKing_HandleFrames();
}

void ERZKing_State_ChangeHBH(void)
{
    RSDK_THIS(ERZKing);

    entity->typeChangeTimer += 16;
    entity->scale.x -= (entity->scale.x >> 4);
    entity->scale.y = entity->scale.x;
    if (entity->typeChangeTimer == 0x400) {
        entity->typeChangeTimer = 0;
        foreach_all(ERZKing, king) { king->active = ACTIVE_NEVER; }

        switch (entity->nextType) {
            case 0:
                CREATE_ENTITY(ERZGunner, NULL, entity->position.x, entity->position.y);

                entity->storeDrawFX = FX_NONE;
                entity->state       = ERZKing_State_Unknown4;
                entity->nextType--;
                entity->nextType &= 1;
                break;
            case 1:
                CREATE_ENTITY(ERZMystic, NULL, entity->position.x, entity->position.y);
                entity->storeDrawFX = FX_NONE;
                entity->state       = ERZKing_State_Unknown4;
                entity->nextType--;
                entity->nextType &= 1;
                break;
        }
    }
}

void ERZKing_State_Arm(void)
{
    RSDK_THIS(ERZKing);

    EntityERZKing *parent = (EntityERZKing *)entity->parent;

    int moveX = 0;
    int moveY = ((RSDK.Sin256(2 * (Zone->timer + (entity->type << 6)) - 128) + 512) << 12) + parent->position.y;

    entity->direction = parent->direction;
    int negAngle      = -parent->rotation;

    int x = 0, y = 0;
    int x2 = 0, y2 = 0;
    if (parent->direction) {
        moveX = parent->position.x - 0x300000;
        x     = (0xD00 * RSDK.Cos512(negAngle)) + 0x300 * RSDK.Sin512(negAngle) + parent->position.x;
        y     = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);
        if (entity->type == 1) {
            x += -0x1800 * RSDK.Cos512(parent->rotation);
            y += 0x1800 * RSDK.Sin512(parent->rotation);
            moveX -= 0x300000;
        }
        x2 = ((entity->position.x + x) >> 1) + 0x200000;
        y2 = ((entity->position.y + y) >> 1) + 0x200000;
    }
    else {
        moveX = parent->position.x + 0x300000;
        x     = 0x300 * RSDK.Sin512(negAngle) - (0xD00 * RSDK.Cos512(negAngle)) + parent->position.x;
        y     = (0xD00 * RSDK.Sin512(negAngle)) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;
        if (entity->type == 1) {
            x += 0x1800 * RSDK.Cos512(parent->rotation);
            y += -0x1800 * RSDK.Sin512(parent->rotation);
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

void ERZKing_State_Explode(void)
{
    RSDK_THIS(ERZKing);

    entity->velocity.y += 0x2800;
    entity->position.y += entity->velocity.y;
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(ERZKing->sfxExplosion2, false, 255);
        if (Zone->timer & 4) {
            int x = RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16;
            int y = RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16;
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x + entity->position.x, y + entity->position.y)
                ->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        GameProgress_GiveEnding(2);
        API_UnlockAchievement("ACH_GAME_CLEARED");
        RSDK.SetScene("Presentation", "Credits");
        Zone_StartFadeOut(10, 0x000000);
        Music_FadeOut(0.025);
        destroyEntity(entity);
    }
}

void ERZKing_EditorDraw(void) {}

void ERZKing_EditorLoad(void) {}

void ERZKing_Serialize(void) { RSDK_EDITABLE_VAR(ERZKing, VAR_ENUM, type); }
