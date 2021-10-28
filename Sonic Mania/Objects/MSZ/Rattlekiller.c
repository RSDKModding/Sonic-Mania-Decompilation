#include "SonicMania.h"

ObjectRattlekiller *Rattlekiller;

void Rattlekiller_Update(void)
{
    RSDK_THIS(Rattlekiller);

    if (entity->timer) {
        entity->timer--;
        if (entity->bodyStates[0] == 4) {
            foreach_active(Player, player)
            {
                if (abs(entity->bodyPositions[0].x - player->position.x) < 0x600000) {
                    if (abs(entity->bodyPositions[0].y - player->position.y) < 0x600000) {
                        entity->playerPos.x = player->position.x;
                        entity->playerPos.y = player->position.y;
                        foreach_break;
                    }
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (entity->bodyPositions[0].x <= entity->startPos.x) {
                if (player->position.x >= entity->startPos.x || player->position.x - entity->startPos.x <= -0x600000)
                    continue;
            }
            else if (player->position.x <= entity->startPos.x || player->position.x - entity->startPos.x >= 0x600000) {
                continue;
            }

            if (abs(entity->bodyPositions[0].y - player->position.y) < 0x600000) {
                int delay = 30;
                for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
                    entity->bodyDelays[i] = 30;
                    entity->field_DC[i].x = entity->bodyPositions[i].x;
                    entity->field_DC[i].y = entity->bodyPositions[0].y;
                    entity->bodyStates[i]   = 4;

                    delay += 2;
                }

                entity->playerPos.x = player->position.x;
                entity->playerPos.y = player->position.y;
                entity->timer       = 480;
                foreach_break;
            }
        }
    }

    foreach_active(Player, player)
    {
        for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
            switch (entity->bodyStates[i]) {
                case 0:
                    if (--entity->bodyDelays[i] <= 0)
                        entity->bodyStates[i] = (entity->field_12C[i].y < 0) + 2;
                    break;
                case 1:
                    entity->bodyAngles[i] = (entity->bodyAngles[i] + 2) & 0xFF;
                    entity->bodyPositions[i].y += entity->field_12C[i].y;
                    entity->bodyPositions[i].x = entity->startPos.x - 0x1800 * RSDK.Sin256(entity->bodyAngles[i]);
                    entity->field_1A4[i]       = RSDK.Cos256(entity->bodyAngles[i]);
                    if (entity->timer < 320) {
                        if (--entity->bodyDelays[i] <= 0)
                            entity->bodyStates[i] = (entity->field_12C[i].y < 0) + 2;
                    }
                    break;
                case 2:
                    entity->bodyAngles[i] = (entity->bodyAngles[i] + 2) & 0xFF;
                    entity->field_12C[i].y += 0x400;
                    if (entity->field_12C[i].y > 0x8000)
                        entity->field_12C[i].y = 0x8000;

                    entity->bodyPositions[i].y += entity->field_12C[i].y;
                    entity->bodyPositions[i].x = entity->startPos.x - 0x1800 * RSDK.Sin256(entity->bodyAngles[i]);
                    entity->field_1A4[i]       = RSDK.Cos256(entity->bodyAngles[i]);
                    if (entity->bodyPositions[i].y >= entity->startPos2.y)
                        entity->bodyStates[i] = 3;
                    break;
                case 3:
                    entity->bodyAngles[i] = (entity->bodyAngles[i] + 2) & 0xFF;
                    entity->field_12C[i].y -= 0x400;
                    if (entity->field_12C[i].y < -0x8000)
                        entity->field_12C[i].y = -0x8000;

                    entity->bodyPositions[i].y += entity->field_12C[i].y;
                    entity->bodyPositions[i].x = entity->startPos.x - 0x1800 * RSDK.Sin256(entity->bodyAngles[i]);
                    entity->field_1A4[i]       = RSDK.Cos256(entity->bodyAngles[i]);
                    if (entity->bodyPositions[i].y <= entity->startPos.y)
                        entity->bodyStates[i] = 2;
                    break;
                case 4:
                    if (--entity->bodyDelays[i] > 0) {
                        if (entity->bodyDelays[i] < 15 && i > 4) {
                            entity->bodyAngles[i]          = (entity->bodyAngles[i] + 4) & 0xFF;
                            entity->bodyPositions[i].x = entity->startPos.x - 0x1800 * RSDK.Sin256(entity->bodyAngles[i]);
                            entity->field_1A4[i]       = RSDK.Cos256(entity->bodyAngles[i]);
                        }
                    }
                    else if (i) {
                        entity->bodyStates[i] = 5;
                    }
                    else {
                        entity->bodyStates[0] = 6;
                        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, (entity->bodyPositions[0].x > entity->startPos.x) + 2, entity->animators,
                                                true, 0);
                        RSDK.PlaySfx(Rattlekiller->sfxRocketJet, false, 255);
                    }
                    break;
                case 5:
                    if (entity->bodyStates[0] < 6) {
                        entity->bodyPositions[i].y += (entity->bodyPositions[0].y - entity->bodyPositions[i].y) >> 3;
                        if (entity->bodyStates[i - 1] == 1) {
                            if (abs(entity->bodyPositions[0].y - entity->bodyPositions[i].y) < 0x20000) {
                                entity->bodyPositions[i].y = entity->bodyPositions[0].y;
                                entity->field_12C[i].x     = 0;
                                entity->field_12C[i].y     = 0;
                                entity->bodyStates[i]      = 1;
                                entity->bodyDelays[i]      = 8 * i + entity->bodyDelays[0];
                                entity->bodyAngles[i]      = entity->bodyAngles[i - 1] - 16;
                            }
                        }
                    }
                    else if (i < Rattlekiller_SegmentCount - 1) {
                        entity->bodyPositions[i].x = (entity->bodyPositions[i - 1].x >> 1) + (entity->bodyPositions[i + 1].x >> 1);
                        entity->bodyPositions[i].y = (entity->bodyPositions[i - 1].y >> 1) + (entity->bodyPositions[i + 1].y >> 1);
                    }
                    break;
                case 6: {
                    int rx = (entity->playerPos.x - entity->bodyPositions[i].x) >> 16;
                    int ry = (entity->playerPos.y - entity->bodyPositions[i].y) >> 16;
                    if (rx * rx + ry * ry >= 64) {
                        int angle              = RSDK.ATan2(rx, ry);
                        entity->field_12C[i].x = RSDK.Cos256(angle) << 10;
                        entity->bodyPositions[i].x += entity->field_12C[i].x;
                        entity->bodyPositions[i].y += RSDK.Sin256(angle) << 10;
                        entity->field_12C[i].y = RSDK.Sin256(angle) << 10;
                        RSDK.ProcessAnimation(entity->animators);
                    }
                    else {
                        entity->bodyStates[i] = 7;
                        RSDK.ProcessAnimation(entity->animators);
                    }
                    break;
                }
                case 7: {
                    int rx = (entity->field_DC[i].x - entity->bodyPositions[i].x) >> 16;
                    int ry = (entity->field_DC[Rattlekiller_SegmentCount - 1].y - entity->bodyPositions[i].y) >> 16;
                    if (rx * rx + ry * ry >= 64) {
                        int angle              = RSDK.ATan2(rx, ry);
                        entity->field_12C[i].x = RSDK.Cos256(angle) << 10;
                        entity->bodyPositions[i].x += entity->field_12C[i].x;
                        entity->bodyPositions[i].y += RSDK.Sin256(angle) << 10;
                        entity->field_12C[i].y = RSDK.Sin256(angle) << 10;
                        RSDK.ProcessAnimation(entity->animators);
                    }
                    else {
                        entity->bodyStates[i] = 1;
                        if (i > 0)
                            entity->bodyAngles[i] = entity->bodyAngles[i - 1] - 16;
                        entity->bodyPositions[i].x = entity->field_DC[i].x;
                        entity->bodyPositions[i].y = entity->field_DC[Rattlekiller_SegmentCount - 1].y;
                        entity->bodyDelays[i]      = 8 * i;
                        entity->field_12C[i].x     = 0;
                        entity->field_12C[i].y     = 0;
                        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, entity->animators, true, 0);
                    }
                    break;
                }
                default: break;
            }
        }

        entity->position.x = entity->bodyPositions[0].x;
        entity->position.y = entity->bodyPositions[0].y;
        if (Player_CheckBadnikTouch(player, entity, &Rattlekiller->hitbox) && Player_CheckBadnikBreak(entity, player, false)) {
            for (int i = 1; i < Rattlekiller_SegmentCount; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->bodyPositions[i].x, entity->bodyPositions[i].y);
                RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, entity->bodyAnimators[i]->animationID, &debris->animator, true,
                                        entity->bodyAnimators[i]->frameID);
                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y    = RSDK.Rand(-0x20000, -0x10000);
                debris->gravity       = 0x4800;
                debris->drawOrder     = Zone->drawOrderHigh;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }

            destroyEntity(entity);
            entity->active = ACTIVE_NEVER2;
            foreach_break;
        }
        else {
            for (int i = 1; i < Rattlekiller_SegmentCount; ++i) {
                if (entity->field_1A4[i] > 0) {
                    entity->position.x = entity->bodyPositions[i].x;
                    entity->position.y = entity->bodyPositions[i].y;
                    if (Player_CheckCollisionTouch(player, entity, &Rattlekiller->hitbox)) {
#if RETRO_USE_PLUS
                        if (!Player_CheckMightyUnspin(512, player, 2, &player->uncurlTimer))
#endif
                            Player_CheckHit(player, entity);
                    }
                }
            }
        }
    }

    entity->position.x = entity->bodyPositions[0].x;
    entity->position.y = entity->bodyPositions[0].y;
    Rattlekiller_HandleSorting();
}

void Rattlekiller_LateUpdate(void) {}

void Rattlekiller_StaticUpdate(void)
{
    foreach_active(Rattlekiller, rattlekiller) { RSDK.AddDrawListRef(Zone->drawOrderLow - 1, RSDK.GetEntityID(rattlekiller)); }
}

void Rattlekiller_Draw(void)
{
    RSDK_THIS(Rattlekiller);

    if (RSDK_sceneInfo->currentDrawGroup == entity->drawOrder) {
        for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
            int id = entity->bodyIDs[i];
            if (entity->field_1A4[id] > 0) {
                Animator *animator = entity->bodyAnimators[id];
                if (animator->animationID < 2)
                    animator->frameID = entity->bodyAngles[id] >> 4;
                RSDK.DrawSprite(animator, &entity->bodyPositions[id], false);
            }
        }
    }
    else {
        for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
            int id = entity->bodyIDs[i];
            if (entity->field_1A4[id] <= 0) {
                Animator *animator = entity->bodyAnimators[id];
                if (animator->animationID < 2)
                    animator->frameID = entity->bodyAngles[id] >> 4;
                RSDK.DrawSprite(animator, &entity->bodyPositions[id], false);
            }
        }
    }
}

void Rattlekiller_Create(void *data)
{
    RSDK_THIS(Rattlekiller);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->startPos      = entity->position;
        entity->startPos2     = entity->position;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (data)
            entity->length = voidToInt(data);
        else if (!entity->length)
            entity->length = 128;

        entity->startPos.y -= entity->length << 15;
        entity->startPos2.y += entity->length << 15;

        int delay = 0;
        for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
            entity->bodyIDs[i]       = i;
            entity->bodyAnimators[i] = &entity->animators[1];
            entity->bodyDelays[i]    = delay;
            entity->bodyPositions[i] = entity->startPos;

            delay += 8;
        }

        entity->bodyAnimators[0] = &entity->animators[0];
        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 1, &entity->animators[1], true, 0);
        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 4, &entity->animators[2], true, 0);
    }
}

void Rattlekiller_StageLoad(void)
{
    Rattlekiller->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/Rattlekiller.bin", SCOPE_STAGE);
    Rattlekiller->hitbox.left   = -8;
    Rattlekiller->hitbox.top    = -8;
    Rattlekiller->hitbox.right  = 8;
    Rattlekiller->hitbox.bottom = 8;
    Rattlekiller->sfxRocketJet  = RSDK.GetSFX("Stage/RocketJet.wav");
    DEBUGMODE_ADD_OBJ(Rattlekiller);
}

void Rattlekiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityRattlekiller *rattlekiller = CREATE_ENTITY(Rattlekiller, intToVoid(0x80), entity->position.x, entity->position.y);
    rattlekiller->direction          = FLIP_NONE;
}

void Rattlekiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Rattlekiller_HandleSorting(void)
{
    RSDK_THIS(Rattlekiller);

    for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
        for (int i2 = Rattlekiller_SegmentCount - 1; i2 > i; --i2) {
            int idA = entity->bodyIDs[i2 - 1];
            int idB = entity->bodyIDs[i2];
            if (entity->field_1A4[idA] > entity->field_1A4[idB]) {
                entity->bodyIDs[i2 - 1] = idB;
                entity->bodyIDs[i2]     = idA;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Rattlekiller_EditorDraw(void)
{
    RSDK_THIS(Rattlekiller);
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &entity->animators[0], false, 0);
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 1, &entity->animators[1], false, 0);
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 4, &entity->animators[2], false, 0);

    entity->startPos      = entity->position;
    entity->startPos2     = entity->position;

    entity->startPos.y -= entity->length << 15;
    entity->startPos2.y += entity->length << 15;

    int delay = 0;
    for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
        entity->bodyIDs[i]       = i;
        entity->bodyAnimators[i] = &entity->animators[1];
        entity->bodyDelays[i]    = delay;
        entity->bodyPositions[i] = entity->startPos;

        delay += 8;
    }

    
    Rattlekiller_HandleSorting();
    Rattlekiller_Draw();
}

void Rattlekiller_EditorLoad(void) { Rattlekiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Rattlekiller.bin", SCOPE_STAGE); }
#endif

void Rattlekiller_Serialize(void)
{
    RSDK_EDITABLE_VAR(Rattlekiller, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Rattlekiller, VAR_ENUM, length);
}
