#include "SonicMania.h"

ObjectTuesday *Tuesday;

void Tuesday_Update(void)
{
    RSDK_THIS(Tuesday);
    StateMachine_Run(entity->state);
}

void Tuesday_LateUpdate(void) {}

void Tuesday_StaticUpdate(void)
{
    foreach_all(Tuesday, tuesday) {
        if (!tuesday->velocity.y) {
            RSDK.AddDrawListRef(Zone->drawOrderLow, RSDK.GetEntityID(tuesday));
        }
    }
}

void Tuesday_Draw(void)
{
    RSDK_THIS(Tuesday);
    if (RSDK_sceneInfo->currentDrawGroup == RSDK_sceneInfo->entity->drawOrder) {
        RSDK.DrawSprite(&entity->animator1, &entity->drawPos, false);
        if (!entity->type)
            RSDK.DrawSprite(&entity->animator2, &entity->drawPos, false);
    }
    else {
        Tuesday_DrawElectricity();
    }
}

void Tuesday_Create(void *data)
{
    RSDK_THIS(Tuesday);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow + 1;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 3, &entity->animator1, true, 0);
        entity->active = ACTIVE_NORMAL;
        entity->state  = Tuesday_State_Debris;
    }
    else {
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 0, &entity->animator2, true, 0);
        entity->health = 1;
        if (!entity->type)
            entity->state = Tuesday_Unknown2;
        else
            entity->state = Tuesday_Unknown3;
        entity->linkPtrs[0] = 0;
        entity->linkPtrs[1] = 0;
        entity->linkPtrs[2] = 0;
        entity->linkPtrs[3] = 0;
        entity->linkPtrs[4] = 0;
        entity->linkPtrs[5] = 0;
        entity->linkPtrs[6] = 0;
        entity->linkPtrs[7] = 0;
        if (!entity->type)
            entity->field_88 = (Entity *)entity;
        entity->field_8C = 0xFF;
        int32 minX         = 0x7FFFFFFF;
        int32 minY         = 0x7FFFFFFF;
        int32 maxX         = 0;
        int32 maxY         = 0;
        entity->drawPos  = entity->position;
        if (entity->type && entity->field_88) {
            entity->position    = entity->field_88->position;
            entity->updateRange = entity->field_88->updateRange;
        }

        int32 slotID           = RSDK.GetEntityID(entity->field_88);
        EntityTuesday *child = RSDK_GET_ENTITY(slotID, Tuesday);
        while (child->objectID == Tuesday->objectID || child->objectID == Platform->objectID) {
            if (child != entity && child->objectID == Tuesday->objectID) {
                ++entity->field_94;
                if (!entity->type) {
                    child->field_88 = (Entity *)entity;
                    if (child->position.x < minX)
                        minX = child->position.x;
                    if (child->position.x > maxX)
                        maxX = child->position.x;

                    if (child->position.y < minY)
                        minY = child->position.y;
                    if (child->position.y > maxY)
                        maxY = child->position.y;
                }
                if (!child->drawPos.x) {
                    child->drawPos.x = child->position.x;
                    child->drawPos.y = child->position.y;
                }
                int32 distX = (child->drawPos.x - entity->drawPos.x) >> 16;
                int32 distY = (child->drawPos.y - entity->drawPos.y) >> 16;

                if (abs(distX) <= 256 && abs(distY) <= 256) {
                    int32 dir = 0;
                    if (!distX)
                        dir |= 1;
                    else if (distX >= 0)
                        dir |= 2;

                    if (!distY)
                        dir |= 4;
                    else if (distY >= 0)
                        dir |= 8;

                    switch (dir) {
                        case 0:
                            if ((entity->links & 0x80) && abs(distX) == abs(distY)) {
                                EntityTuesday *tuesday = (EntityTuesday *)entity->linkPtrs[7];
                                if (!tuesday || child->drawPos.y > tuesday->drawPos.y) {
                                    entity->linkPtrs[7] = (Entity*)child;
                                    if (entity->field_8C)
                                        child->field_8C |= 0x08;
                                    else if (child->field_8C)
                                        entity->field_8C = 0x80;
                                }
                            }
                            break;
                        case 1:
                            if (entity->links & 0x40) {
                                EntityTuesday *tuesday = (EntityTuesday *)entity->linkPtrs[6];
                                if (!tuesday || child->drawPos.y > tuesday->drawPos.y) {
                                    entity->linkPtrs[6] = (Entity *)child;
                                    if (entity->field_8C)
                                        child->field_8C |= 0x04;
                                    else if (child->field_8C)
                                        entity->field_8C = 0x40;
                                }
                            }
                            break;
                        case 2:
                            if ((entity->links & 0x20) && abs(distX) == abs(distY)) {
                                EntityTuesday *tuesday = (EntityTuesday *)entity->linkPtrs[5];
                                if (!tuesday || child->drawPos.x < tuesday->drawPos.x) {
                                    entity->linkPtrs[5] = (Entity *)child;
                                    if (entity->field_8C)
                                        child->field_8C |= 0x02;
                                    else if (child->field_8C)
                                        entity->field_8C = 0x20;
                                }
                            }
                            break;
                        case 4:
                            if (entity->links & 1) {
                                EntityTuesday *tuesday = (EntityTuesday *)entity->linkPtrs[0];
                                if (!tuesday || child->drawPos.x > tuesday->drawPos.x) {
                                    entity->linkPtrs[0] = (Entity *)child;
                                    if (entity->field_8C)
                                        child->field_8C |= 0x10;
                                    else if (child->field_8C)
                                        entity->field_8C = 1;
                                }
                            }
                            break;
                        case 6:
                            if (entity->links & 0x10) {
                                EntityTuesday *tuesday = (EntityTuesday *)entity->linkPtrs[4];
                                if (!tuesday || child->drawPos.x < tuesday->drawPos.x) {
                                    entity->linkPtrs[4] = (Entity *)child;
                                    if (entity->field_8C)
                                        child->field_8C |= 1;
                                    else if (child->field_8C)
                                        entity->field_8C = 0x10;
                                }
                            }
                            break;
                        case 8:
                            if ((entity->links & 2) && abs(distX) == abs(distY)) {
                                EntityTuesday *tuesday = (EntityTuesday *)entity->linkPtrs[1];
                                if (!tuesday || child->drawPos.x > tuesday->drawPos.x) {
                                    entity->linkPtrs[1] = (Entity *)child;
                                    if (entity->field_8C)
                                        child->field_8C |= 0x20;
                                    else if (child->field_8C)
                                        entity->field_8C = 0x02;
                                }
                            }
                            break;
                        case 9:
                            if (entity->links & 4) {
                                EntityTuesday *tuesday = (EntityTuesday *)entity->linkPtrs[2];
                                if (!tuesday || child->drawPos.y < tuesday->drawPos.y) {
                                    entity->linkPtrs[2] = (Entity *)child;
                                    if (entity->field_8C)
                                        child->field_8C |= 0x40;
                                    else if (child->field_8C)
                                        entity->field_8C = 0x04;
                                }
                            }
                            break;
                        case 10:
                            if ((entity->links & 8) && abs(distX) == abs(distY)) {
                                EntityTuesday *tuesday = (EntityTuesday *)entity->linkPtrs[3];
                                if (!tuesday || child->drawPos.y < tuesday->drawPos.y) {
                                    entity->linkPtrs[3] = (Entity *)child;
                                    if (entity->field_8C)
                                        child->field_8C |= 0x80;
                                    else if (child->field_8C)
                                        entity->field_8C = 0x08;
                                }
                            }
                            break;
                        default: break;
                    }
                }
            }
            child = RSDK_GET_ENTITY(++slotID, Tuesday);
        }

        if (!entity->type) {
            entity->position.x    = ((maxX - minX) >> 1) + minX;
            entity->position.y    = ((maxY - minY) >> 1) + minY;
            entity->updateRange.x = 0x800000 + ((maxX - minX) >> 1);
            entity->updateRange.y = 0x800000 + ((maxY - minY) >> 1);
        }
    }
}

void Tuesday_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        Tuesday->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Tuesday.bin", SCOPE_STAGE);

    Tuesday->hitbox1.left   = -16;
    Tuesday->hitbox1.top    = -16;
    Tuesday->hitbox1.right  = 16;
    Tuesday->hitbox1.bottom = 16;
    Tuesday->hitbox2.left   = -40;
    Tuesday->hitbox2.top    = -8;
    Tuesday->hitbox2.right  = 40;
    Tuesday->hitbox2.bottom = 80;

    Tuesday->sfxElecCharge = RSDK.GetSFX("Stage/ElecCharge.wav");
    Tuesday->sfxZap        = RSDK.GetSFX("Stage/Zap.wav");
    Tuesday->sfxBossHit    = RSDK.GetSFX("Stage/BossHit.wav");
    Tuesday->sfxExplosion  = RSDK.GetSFX("Stage/Explosion2.wav");
    Tuesday->sfxDrop       = RSDK.GetSFX("Stage/Drop.wav");
}

void Tuesday_DrawElectricity(void)
{
    RSDK_THIS(Tuesday);
    Vector2 drawPos;

    if (entity->timer) {
        int32 frameID = 0;
        if (entity->timer <= 60)
            frameID = (Zone->timer >> 1) & 3;
        else
            frameID = 3 * ((Zone->timer >> 1) & 1);
        entity->drawFX |= FX_FLIP;

        Animator animator;
        for (int32 i = 0; i < 8; ++i) {
            if (((1 << i) & entity->field_8E) && entity->linkPtrs[i]) {
                drawPos.x           = entity->drawPos.x;
                drawPos.y           = entity->drawPos.y;
                EntityTuesday *link = (EntityTuesday *)entity->linkPtrs[i];
                switch (i) {
                    case 0:
                        drawPos.x -= 0x200000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 5, &animator, true, frameID);
                        for (int32 p = (entity->drawPos.x - link->drawPos.x - 0x400000) >> 21; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x -= 0x100000;
                        }
                        break;
                    case 1:
                        drawPos.y += 0x100000;
                        drawPos.x -= 0x100000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 6, &animator, true, frameID);
                        for (int32 p = ((entity->drawPos.x - link->drawPos.x - 0x180000) >> 16) / 19; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x -= 0xC0000;
                            drawPos.y += 0xC0000;
                        }
                        break;
                    case 2:
                        drawPos.y += 0x200000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 4, &animator, true, frameID);
                        for (int32 p = (link->drawPos.y - entity->drawPos.y - 0x400000) >> 21; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.y += 0x100000;
                        }
                        break;
                    case 3:
                        entity->direction = FLIP_X;
                        drawPos.x += 0x100000;
                        drawPos.y += 0x100000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 6, &animator, true, frameID);
                        for (int32 p = ((link->drawPos.x - entity->drawPos.x - 1572864) >> 16) / 19; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x += 0xC0000;
                            drawPos.y += 0xC0000;
                        }
                        entity->direction = FLIP_NONE;
                        break;
                    case 4:
                        drawPos.x += 0x200000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 5, &animator, true, frameID);
                        for (int32 p = (link->drawPos.x - entity->drawPos.x - 0x400000) >> 21; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x += 0x100000;
                        }
                        break;
                    case 5:
                        drawPos.y -= 0x100000;
                        drawPos.x += 0x100000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 6, &animator, true, frameID);
                        for (int32 p = ((link->drawPos.x - entity->drawPos.x - 1572864) >> 16) / 19; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x += 0xC0000;
                            drawPos.y -= 0xC0000;
                        }
                        break;
                    case 6:
                        drawPos.y -= 0x200000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 4, &animator, true, frameID);
                        for (int32 p = (entity->drawPos.y - link->drawPos.y - 0x400000) >> 21; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.y -= 0x100000;
                        }
                        break;
                    case 7:
                        entity->direction = FLIP_X;
                        drawPos.x -= 0x100000;
                        drawPos.y -= 0x100000;
                        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 6, &animator, true, frameID);
                        for (int32 p = ((entity->drawPos.x - link->drawPos.x - 0x180000) >> 16) / 19; p >= 0; --p) {
                            RSDK.DrawSprite(&animator, &drawPos, false);
                            drawPos.x -= 0xC0000;
                            drawPos.y -= 0xC0000;
                        }
                        entity->direction = FLIP_NONE;
                        break;
                    default: break;
                }
            }
        }
        entity->drawFX &= ~FX_FLIP;
    }
}

void Tuesday_Unknown2(void)
{
    RSDK_THIS(Tuesday);
    if (++entity->field_92 == 240) {
        RSDK.PlaySfx(Tuesday->sfxElecCharge, false, 255);
        entity->timer          = 120;
        int32 slotID             = RSDK_sceneInfo->entitySlot + 1;
        EntityTuesday *tuesday = RSDK_GET_ENTITY(slotID, Tuesday);
        int32 count              = entity->field_94;
        for (int32 i = 0; i < count;) {
            if (tuesday->objectID == Tuesday->objectID) {
                ++i;
                tuesday->field_8D = 0;
            }
            tuesday = RSDK_GET_ENTITY(++slotID, Tuesday);
        }

        switch (entity->field_8F) {
            case 0:
                entity->field_8D = 0xAA;
                entity->field_8E = 0xAA;
                entity->field_8F = 1;
                break;
            case 1:
                entity->field_8D = 0x11;
                entity->field_8E = 0x11;
                entity->field_8F = 2;
                break;
            case 2:
                entity->field_8D = 0x44;
                entity->field_8E = 0x44;
                entity->field_8F = 0;
                break;
        }

        entity->field_92 = 0;
    }
    if (entity->timer == 60)
        RSDK.PlaySfx(Tuesday->sfxZap, false, 255);
    Tuesday_Unknown3();
}

void Tuesday_Unknown3(void)
{
    RSDK_THIS(Tuesday);
    EntityPlatform *platform = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, Platform);
    if (platform->objectID == Platform->objectID && platform->childCount > 0) {
        entity->position.x -= platform->collisionOffset.x;
        entity->position.y -= platform->collisionOffset.y;
        entity->drawPos.x += platform->collisionOffset.x;
        entity->drawPos.y += platform->collisionOffset.y;
    }

    if (entity->timer) {
        if (entity->timer <= 60)
            RSDK.SetSpriteAnimation(Tuesday->aniFrames, 2, &entity->animator1, false, 0);
        else
            RSDK.SetSpriteAnimation(Tuesday->aniFrames, 1, &entity->animator1, false, 0);
        RSDK.ProcessAnimation(&entity->animator1);
    }
    else {
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 1, &entity->animator1, true, 0);
    }

    int32 storeX       = entity->position.x;
    int32 storeY       = entity->position.y;
    entity->position = entity->drawPos;
    Hitbox hitbox;

    foreach_active(Player, player)
    {
        if (!entity->invincibleTimer) {
            bool32 hit = false;
            if (entity->type)
                hit = Player_CheckBadnikTouch(player, entity, &Tuesday->hitbox1);
            else
                hit = Player_CheckBadnikTouch(player, entity, &Tuesday->hitbox2);
            if (hit) {
                if (!player->invincibleTimer && player->shield != SHIELD_LIGHTNING && player->blinkTimer <= 0 && entity->timer && entity->timer <= 60
                    && (entity->type || Player_CheckCollisionTouch(player, entity, &Tuesday->hitbox1))) {
                    Player_CheckHit(player, entity);
                }
                else if (Player_CheckBossHit(player, entity)) {
                    if (!--entity->health) {
                        entity->state = Tuesday_State_Destroyed;
                        if (!entity->type)
                            entity->invincibleTimer = 48;
                        else
                            entity->invincibleTimer = 16;
                    }
                    else {
                        entity->invincibleTimer = 30;
                        RSDK.PlaySfx(Tuesday->sfxBossHit, false, 255);
                    }
                }
            }
            else {
                if (entity->timer < 60) {
                    if (entity->field_8E == 17) {
                        EntityTuesday *child = (EntityTuesday *)entity->linkPtrs[0];
                        if (child) {
                            hitbox.top    = -2;
                            hitbox.bottom = 2;
                            hitbox.left   = (child->drawPos.x - entity->position.x) >> 16;
                            hitbox.right  = 0;
                            if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                                Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                            }
                        }

                        child = (EntityTuesday *)entity->linkPtrs[4];
                        if (child) {
                            hitbox.left   = -0;
                            hitbox.top    = -2;
                            hitbox.bottom = 2;
                            hitbox.right  = (child->drawPos.x - entity->position.x) >> 16;
                            if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                                Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                            }
                        }
                    }
                    else {
                        if (entity->field_8E != 68) {
                            if (entity->field_8E == 170) {
                                EntityTuesday *child = (EntityTuesday *)entity->linkPtrs[1];
                                if (child) {
                                    hitbox.top    = 0;
                                    hitbox.bottom = (child->drawPos.y - entity->position.y) >> 16;
                                    hitbox.left   = (child->drawPos.x - entity->position.x) >> 16;
                                    hitbox.right  = 0;
                                    if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                                        int32 distX = abs((player->position.x - entity->position.x) >> 16);
                                        int32 distY = abs((player->position.y - entity->position.y) >> 16);
                                        if (abs(distX - distY) < 13) {
                                            Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                                        }
                                    }
                                }
                                else {
                                    child = (EntityTuesday *)entity->linkPtrs[3];
                                    if (child) {
                                        hitbox.bottom = (child->drawPos.y - entity->position.y) >> 16;
                                        hitbox.left   = 0;
                                        hitbox.top    = 0;
                                        hitbox.right  = (child->drawPos.x - entity->position.x) >> 16;
                                        if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                                            int32 distX = abs((player->position.x - entity->position.x) >> 16);
                                            int32 distY = abs((player->position.y - entity->position.y) >> 16);
                                            if (abs(distX - distY) < 13) {
                                                Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                                            }
                                        }
                                    }
                                    else {
                                        child = (EntityTuesday *)entity->linkPtrs[5];
                                        if (child) {
                                            hitbox.top    = (child->drawPos.y - entity->position.y) >> 16;
                                            hitbox.bottom = 0;
                                            hitbox.left   = 0;
                                            hitbox.right  = (child->drawPos.x - entity->position.x) >> 16;
                                            if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                                                int32 distX = abs((player->position.x - entity->position.x) >> 16);
                                                int32 distY = abs((player->position.y - entity->position.y) >> 16);
                                                if (abs(distX - distY) < 13) {
                                                    Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                                                }
                                            }
                                        }
                                        else {
                                            child = (EntityTuesday *)entity->linkPtrs[7];
                                            if (child) {
                                                hitbox.top    = (child->drawPos.y - entity->position.y) >> 16;
                                                hitbox.bottom = 0;
                                                hitbox.left   = (child->drawPos.x - entity->position.x) >> 16;
                                                hitbox.right  = 0;
                                                if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                                                    int32 distX = abs((player->position.x - entity->position.x) >> 16);
                                                    int32 distY = abs((player->position.y - entity->position.y) >> 16);
                                                    if (abs(distX - distY) < 13) {
                                                        Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            EntityTuesday *child = (EntityTuesday *)entity->linkPtrs[2];
                            if (child) {
                                hitbox.left   = 2;
                                hitbox.top    = -2;
                                hitbox.right  = 2;
                                hitbox.bottom = (child->drawPos.y - entity->position.y) >> 16;
                                if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                                    Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                                }
                            }

                            child = (EntityTuesday *)entity->linkPtrs[6];
                            if (child) {
                                hitbox.left   = -2;
                                hitbox.right  = 2;
                                hitbox.top    = (child->drawPos.y - entity->position.y) >> 16;
                                hitbox.bottom = 0;
                                if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                                    Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
    if (entity->field_8D != 0xFF && entity->field_8D) {
        for (int32 i = 0; i < 8; ++i) {
            EntityTuesday *child = (EntityTuesday *)entity->linkPtrs[i];
            if (child) {
                if (!child->field_8D)
                    child->field_8D = entity->field_8D;

                if (entity->field_8D == 17) {
                    if (!i || i == 4) {
                        child->timer     = 120;
                        entity->timer    = 120;
                        child->field_8E  = 0x11;
                        entity->field_8E = 0x11;
                    }
                }
                else if (entity->field_8D == 68) {
                    if (i == 2 || i == 6) {
                        child->timer     = 120;
                        entity->timer    = 120;
                        child->field_8E  = 0x44;
                        entity->field_8E = 0x44;
                    }
                }
                else if (entity->field_8D == 170 && (i == 1 || i == 3 || i == 5 || i == 7)) {
                    child->timer     = 120;
                    entity->timer    = 120;
                    child->field_8E  = 0xAA;
                    entity->field_8E = 0xAA;
                }
            }
        }
        entity->field_8D = 0xFF;
    }

    if (entity->timer) {
        entity->timer--;
        if (!entity->timer)
            entity->field_8E = 0;
    }
}

void Tuesday_State_Destroyed(void)
{
    RSDK_THIS(Tuesday);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(Tuesday->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 data = ((RSDK.Rand(0, 256) > 192) + 2);

            int32 val    = 0;
            int32 minVal = 0, maxVal = 0;
            if (entity->type) {
                val    = RSDK.Rand(-12, 12);
                maxVal = 12;
                minVal = -12;
            }
            else {
                val    = RSDK.Rand(-8, 73);
                maxVal = 33;
                minVal = -32;
            }
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid(data), (RSDK.Rand(minVal, maxVal) << 16) + entity->drawPos.x, (val << 16) + entity->drawPos.y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (!--entity->invincibleTimer) {
        --((EntityTuesday *)entity->field_88)->field_94;

        EntityTuesday *child = (EntityTuesday *)entity->linkPtrs[0];
        if (child) {
            child->linkPtrs[4] = NULL;
            child->field_8C &= ~0x10;
        }

        child = (EntityTuesday *)entity->linkPtrs[1];
        if (child) {
            child->linkPtrs[5] = NULL;
            child->field_8C &= ~0x20;
        }

        child = (EntityTuesday *)entity->linkPtrs[2];
        if (child) {
            child->linkPtrs[6] = NULL;
            child->field_8C &= ~0x40;
        }

        child = (EntityTuesday *)entity->linkPtrs[3];
        if (child) {
            child->linkPtrs[7] = NULL;
            child->field_8C &= ~0x80;
        }

        child = (EntityTuesday *)entity->linkPtrs[4];
        if (child) {
            child->linkPtrs[0] = NULL;
            child->field_8C &= ~1;
        }

        child = (EntityTuesday *)entity->linkPtrs[5];
        if (child) {
            child->linkPtrs[1] = NULL;
            child->field_8C &= ~2;
        }

        child = (EntityTuesday *)entity->linkPtrs[6];
        if (child) {
            child->linkPtrs[2] = NULL;
            child->field_8C &= ~4;
        }

        child = (EntityTuesday *)entity->linkPtrs[7];
        if (child) {
            child->linkPtrs[3] = NULL;
            child->field_8C &= ~8;
        }

        child                    = CREATE_ENTITY(Tuesday, intToVoid(true), entity->drawPos.x, entity->drawPos.y);
        child->drawPos.x         = child->position.x;
        child->drawPos.y         = child->position.y;
        child->animator1.frameID = 0;
        child->velocity.x        = -0x20000;
        child->velocity.y        = -0x40000;
        child->type              = 1;

        child                    = CREATE_ENTITY(Tuesday, intToVoid(true), entity->drawPos.x, entity->drawPos.y);
        child->drawPos.x         = child->position.x;
        child->drawPos.y         = child->position.y;
        child->animator1.frameID = 1;
        child->velocity.x        = 0x20000;
        child->velocity.y        = -0x40000;
        child->type              = 1;

        child                    = CREATE_ENTITY(Tuesday, intToVoid(true), entity->drawPos.x, entity->drawPos.y);
        child->drawPos.x         = child->position.x;
        child->drawPos.y         = child->position.y;
        child->animator1.frameID = 2;
        child->velocity.x        = -0x10000;
        child->velocity.y        = -0x20000;
        child->type              = 1;

        child                    = CREATE_ENTITY(Tuesday, intToVoid(true), entity->drawPos.x, entity->drawPos.y);
        child->drawPos.x         = child->position.x;
        child->drawPos.y         = child->position.y;
        child->animator1.frameID = 3;
        child->velocity.x        = 0x10000;
        child->velocity.y        = -0x20000;
        child->type              = 1;

        if (!entity->type) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
            child = CREATE_ENTITY(Tuesday, intToVoid(true), entity->drawPos.x, entity->drawPos.y);
            RSDK.SetSpriteAnimation(Tuesday->aniFrames, 0, &child->animator1, true, 0);
            child->drawPos.x  = child->position.x;
            child->drawPos.y  = child->position.y;
            child->velocity.y = -0x30000;
            child->state      = Tuesday_State_Debris;
        }

        entity->linkPtrs[4] = NULL;
        entity->linkPtrs[5] = NULL;
        entity->linkPtrs[6] = NULL;
        entity->linkPtrs[7] = NULL;
        entity->linkPtrs[0] = NULL;
        entity->linkPtrs[1] = NULL;
        entity->linkPtrs[2] = NULL;
        entity->linkPtrs[3] = NULL;
        RSDK.SetSpriteAnimation(Tuesday->aniFrames, 3, &entity->animator1, true, 4);
        entity->state = StateMachine_None;
    }
}

void Tuesday_State_Debris(void)
{
    RSDK_THIS(Tuesday);
    entity->drawPos.x += entity->velocity.x;
    entity->drawPos.y += entity->velocity.y;
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    entity->velocity.y += 0x3800;

    if (RSDK.CheckOnScreen(entity, NULL)) {
        if (entity->type) {
            entity->visible ^= 1;
        }
        else if (entity->velocity.y == 0x8000) {
            RSDK.PlaySfx(Tuesday->sfxDrop, false, 255);
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Tuesday_EditorDraw(void) {}

void Tuesday_EditorLoad(void) {}

void Tuesday_Serialize(void)
{
    RSDK_EDITABLE_VAR(Tuesday, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Tuesday, VAR_UINT8, links);
}
