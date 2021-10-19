#include "SonicMania.h"

ObjectCaterkillerJr *CaterkillerJr;

void CaterkillerJr_Update(void)
{
    RSDK_THIS(CaterkillerJr);
    entity->active = ACTIVE_NORMAL;
    StateMachine_Run(entity->state);
    if (entity->objectID == CaterkillerJr->objectID && !RSDK.CheckOnScreen(entity, NULL)
        && !RSDK.CheckPosOnScreen(&entity->headStartPos, &entity->updateRange)) {
        entity->position = entity->headStartPos;
        CaterkillerJr_SetupPositions();
    }
}

void CaterkillerJr_LateUpdate(void) {}

void CaterkillerJr_StaticUpdate(void) {}

void CaterkillerJr_Draw(void)
{
    RSDK_THIS(CaterkillerJr);
    int32 storeDir = entity->direction;

    for (int32 i = 6; i >= 0; --i) {
        entity->direction = entity->bodyDirection[i];
        RSDK.DrawSprite(&entity->bodyAnimators[i], &entity->bodyPosition[i], false);
    }

    entity->direction = entity->bodyDirection[3];
    RSDK.DrawSprite(&entity->animator2, &entity->bodyPosition[3], false);

    entity->direction = entity->bodyDirection[2];
    RSDK.DrawSprite(&entity->animator1, &entity->bodyPosition[2], false);

    entity->direction = entity->bodyDirection[1];
    RSDK.DrawSprite(&entity->bodyAnimators[7], &entity->bodyPosition[1], false);

    entity->direction = storeDir;
}

void CaterkillerJr_Create(void *data)
{
    RSDK_THIS(CaterkillerJr);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible           = true;
        entity->drawOrder         = Zone->drawOrderHigh;
        entity->headStartPos.x    = entity->position.x;
        entity->headStartPos.y    = entity->position.y;
        entity->drawFX            = FX_FLIP;
        entity->updateRange.x     = 0x800000;
        entity->updateRange.y     = 0x800000;
        entity->bodyVelocity[0].x = (2 * (entity->direction != FLIP_NONE) - 1) << 16;
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 0, &entity->bodyAnimators[0], true, 6);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &entity->bodyAnimators[1], true, 6);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &entity->bodyAnimators[2], true, 6);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &entity->bodyAnimators[3], true, 6);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 2, &entity->bodyAnimators[4], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &entity->bodyAnimators[5], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &entity->bodyAnimators[6], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &entity->bodyAnimators[7], true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &entity->animator2, true, 0);
        CaterkillerJr_SetupPositions();
    }
}

void CaterkillerJr_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CaterkillerJr.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("AIZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("AIZ/CaterkillerJr.bin", SCOPE_STAGE);

    CaterkillerJr->hitbox.left   = -16;
    CaterkillerJr->hitbox.top    = 8;
    CaterkillerJr->hitbox.right  = 0;
    CaterkillerJr->hitbox.bottom = 8;

    DEBUGMODE_ADD_OBJ(CaterkillerJr);
}

void CaterkillerJr_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void CaterkillerJr_DebugSpawn(void)
{
    RSDK_THIS(CaterkillerJr);
    CREATE_ENTITY(CaterkillerJr, NULL, entity->position.x, entity->position.y);
}

void CaterkillerJr_SetupPositions(void)
{
    RSDK_THIS(CaterkillerJr);
    for (int32 i = 0; i < 7; ++i) {
        entity->bodyPosition[i].x = entity->position.x;
        entity->bodyPosition[i].y = entity->position.y;
        entity->bodyVelocity[i].x = 0;
        entity->bodyVelocity[i].y = 0;
        entity->bodyDirection[i]  = entity->direction;
        entity->bodyTimer[i]      = 0;
    }

    if (entity->direction) {
        entity->boundL = entity->position.x - 0x3C0000;
        entity->boundR = entity->position.x + 0xBC0000;
    }
    else {
        entity->boundL = entity->position.x - 0xBC0000;
        entity->boundR = entity->position.x + 0x3C0000;
    }

    entity->bodyAnimators[7].animationTimer = 14;
    entity->animator1.animationTimer        = 10;
    entity->animator2.animationTimer        = 6;
    entity->timer                           = 0;
    entity->bodyAnimators[7].frameID        = 8;
    entity->animator1.frameID               = 8;
    entity->animator2.frameID               = 8;
    entity->bodyVelocity[0].x               = (2 * (entity->direction != FLIP_NONE) - 1) << 16;
    entity->active                          = ACTIVE_BOUNDS;
    entity->state                           = CaterkillerJr_State1;
}

void CaterkillerJr_State1(void)
{
    RSDK_THIS(CaterkillerJr);

    int32 pos = 2 * (entity->direction != FLIP_NONE) - 1;
    switch (++entity->timer) {
        case 12: entity->bodyVelocity[1].x = pos << 16; break;
        case 24: entity->bodyVelocity[2].x = pos << 16; break;
        case 36: entity->bodyVelocity[3].x = pos << 16; break;
        case 48: entity->bodyVelocity[4].x = pos << 16; break;
        case 56: entity->bodyVelocity[5].x = pos << 16; break;
        case 64:
            entity->timer             = 0;
            entity->bodyVelocity[6].x = pos << 16;
            entity->state             = CaterkillerJr_State2;
            break;
    }
    CaterkillerJr_State2();
}

void CaterkillerJr_State2(void)
{
    RSDK_THIS(CaterkillerJr);
    for (int32 i = 0; i < 7; ++i) {
        RSDK.ProcessAnimation(&entity->bodyAnimators[i]);
        entity->bodyPosition[i].x += entity->bodyVelocity[i].x;
        if (entity->bodyDirection[i] && entity->bodyPosition[i].x >= entity->boundR - 0x80000) {
            entity->bodyDirection[i] ^= 1;
            entity->bodyAnimators[i].animationTimer = 0;
            entity->bodyAnimators[i].frameID        = 0;
        }
        else if (!entity->bodyDirection[i] && entity->bodyPosition[i].x <= entity->boundL + 0x80000) {
            entity->bodyDirection[i] ^= 1;
            entity->bodyAnimators[i].animationTimer = 0;
            entity->bodyAnimators[i].frameID        = 0;
        }

        if (entity->bodyVelocity[i].x <= 0 && entity->bodyPosition[i].x <= entity->boundL) {
            entity->bodyVelocity[i].x = -entity->bodyVelocity[i].x;
            entity->bodyTimer[i]      = 0xA00;
        }
        else if (entity->bodyVelocity[i].x >= 0 && entity->bodyPosition[i].x >= entity->boundR) {
            entity->bodyVelocity[i].x = -entity->bodyVelocity[i].x;
            entity->bodyTimer[i]      = 0xA00;
        }

        if (entity->bodyVelocity[i].x) {
            int32 val = 0;
            if (entity->bodyTimer[i] >= 2048) {
                entity->bodyTimer[i] += 8;
                val = RSDK.Sin1024(entity->bodyTimer[i]) << 10;
            }
            else {
                entity->bodyTimer[i] += 17;
                val = RSDK.Sin1024(entity->bodyTimer[i]) << 8;
            }
            entity->bodyPosition[i].y = val + entity->headStartPos.y;
            entity->bodyTimer[i] %= 3072;
        }
    }

    RSDK.ProcessAnimation(&entity->bodyAnimators[7]);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    foreach_active(Player, player)
    {
        entity->position = entity->bodyPosition[0];
        if (Player_CheckBadnikTouch(player, entity, &CaterkillerJr->hitbox) && Player_CheckBadnikBreak(entity, player, false)) {
            for (int32 i = 0; i < 6; ++i) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->bodyPosition[i + 1].x, entity->bodyPosition[i + 1].y);
                debris->animator      = entity->bodyAnimators[i + 1];
                debris->velocity.x    = RSDK.Rand(-16, 16) << 14;
                debris->velocity.y    = RSDK.Rand(-8, 8) << 14;
                debris->gravity       = 0x3800;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }

            destroyEntity(entity);
            entity->active = -1;
            foreach_break;
        }

        if (entity->objectID != TYPE_BLANK) {
            for (int32 i = 0; i < 6; ++i) {
                entity->position.x = entity->bodyPosition[i + 1].x;
                entity->position.y = entity->bodyPosition[i + 1].y;
                if (Player_CheckCollisionTouch(player, entity, &CaterkillerJr->hitbox)) {
                    Player_CheckHit(player, entity);
                }
            }
            entity->position.x = entity->bodyPosition[0].x;
            entity->position.y = entity->bodyPosition[0].y;
        }
    }
}

void CaterkillerJr_EditorDraw(void)
{
    RSDK_THIS(CaterkillerJr);
    entity->visible           = true;
    entity->drawFX            = FX_FLIP;
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 0, &entity->bodyAnimators[0], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &entity->bodyAnimators[1], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &entity->bodyAnimators[2], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 1, &entity->bodyAnimators[3], true, 6);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 2, &entity->bodyAnimators[4], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &entity->bodyAnimators[5], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 3, &entity->bodyAnimators[6], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &entity->bodyAnimators[7], true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(CaterkillerJr->aniFrames, 4, &entity->animator2, true, 0);
    CaterkillerJr_SetupPositions();

    CaterkillerJr_Draw();
}

void CaterkillerJr_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CaterkillerJr.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("AIZ"))
        CaterkillerJr->aniFrames = RSDK.LoadSpriteAnimation("AIZ/CaterkillerJr.bin", SCOPE_STAGE);
}

void CaterkillerJr_Serialize(void) { RSDK_EDITABLE_VAR(CaterkillerJr, VAR_UINT8, direction); }
