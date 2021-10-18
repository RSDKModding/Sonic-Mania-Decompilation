#include "SonicMania.h"

ObjectAmoebaDroid *AmoebaDroid;

void AmoebaDroid_Update(void)
{
    RSDK_THIS(AmoebaDroid);
    StateMachine_Run(entity->state);
}

void AmoebaDroid_LateUpdate(void) {}

void AmoebaDroid_StaticUpdate(void) {}

void AmoebaDroid_Draw(void)
{
    RSDK_THIS(AmoebaDroid);

    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void AmoebaDroid_Create(void *data)
{
    RSDK_THIS(AmoebaDroid);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            entity->type          = voidToInt(data);
            switch (entity->type) {
                case 0:
                    entity->visible       = false;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->drawFX        = FX_FLIP;
                    entity->health        = 6;
                    entity->hitbox.left   = -22;
                    entity->hitbox.top    = -22;
                    entity->hitbox.right  = 22;
                    entity->hitbox.bottom = 22;
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 1, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 2, &entity->animator3, true, 0);
                    entity->stateDraw = AmoebaDroid_StateDrawMain_Unknown1;
                    entity->state     = AmoebaDroid_StateMain_Setup;
                    break;
                case 1:
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderHigh;
                    entity->hitbox.left   = -40;
                    entity->hitbox.top    = -40;
                    entity->hitbox.right  = 40;
                    entity->hitbox.bottom = 40;
                    entity->drawFX        = FX_SCALE;
                    entity->inkEffect     = INK_ALPHA;
                    entity->scale.y       = 0x200;
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 3, &entity->animator1, true, 0);
                    entity->stateDraw = AmoebaDroid_StateDraw1_Unknown1;
                    entity->state     = AmoebaDroid_State1_Unknown1;
                    break;
                case 2:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderHigh;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 8;
                    entity->updateRange.x = 0x200000;
                    entity->updateRange.y = 0x200000;
                    entity->inkEffect     = INK_ALPHA;
                    entity->state         = AmoebaDroid_State2_Unknown1;
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 4, &entity->animator1, true, 0);
                    break;
                case 3:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->state     = AmoebaDroid_State3_Unknown1;
                    entity->drawOrder = Zone->hudDrawOrder - 2;
                    RSDK.SetSpriteAnimation(AmoebaDroid->waterFrames, 1, &entity->animator1, true, 0);
                    break;
                case 4:
                    entity->active    = ACTIVE_NORMAL;
                    entity->state     = AmoebaDroid_State4_Unknown1;
                    entity->drawOrder = Zone->hudDrawOrder - 2;
                    RSDK.SetSpriteAnimation(AmoebaDroid->waterFrames, 1, &entity->animator1, true, 0);
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void AmoebaDroid_StageLoad(void)
{
    AmoebaDroid->aniFrames    = RSDK.LoadSpriteAnimation("CPZ/AmoebaDroid.bin", SCOPE_STAGE);
    AmoebaDroid->waterFrames  = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);
    AmoebaDroid->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    AmoebaDroid->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    AmoebaDroid->sfxGather    = RSDK.GetSFX("CPZ/DroidGather.wav");
    AmoebaDroid->sfxBounce    = RSDK.GetSFX("CPZ/DroidBounce.wav");
    AmoebaDroid->sfxRelease   = RSDK.GetSFX("CPZ/DroidRelease.wav");
}

void AmoebaDroid_HandleDropletMovement(void)
{
    RSDK_THIS(AmoebaDroid);
    int32 angle       = entity->partAngle2;
    entity->partPos = (entity->partOffset * RSDK.Cos256(entity->partAngle)) >> 8;
    for (int32 i = 0; i < 8; ++i) {
        EntityAmoebaDroid *droidPart = (EntityAmoebaDroid *)entity->parts[i];
        droidPart->velocity          = droidPart->position;
        droidPart->position.x        = entity->partOffset * RSDK.Cos256(angle) + entity->position.x;
        droidPart->velocity.x        = droidPart->position.x - droidPart->velocity.x;
        droidPart->position.y        = entity->partPos * RSDK.Sin256(angle) + entity->position.y;
        droidPart->velocity.y        = (entity->partPos * RSDK.Sin256(angle) + entity->position.y) - droidPart->velocity.y;
        if ((entity->partAngle2 & 0x7F) && angle <= 0x80)
            droidPart->drawOrder = Zone->drawOrderLow - 1;
        else
            droidPart->drawOrder = Zone->drawOrderLow;
        angle = (angle + 32) & 0xFF;
    }
}

void AmoebaDroid_HandleDropletRelease(bool32 interact)
{
    RSDK_THIS(AmoebaDroid);

    for (int32 i = 0; i < 8; ++i) {
        EntityAmoebaDroid *part = (EntityAmoebaDroid *)entity->parts[i];
        if (part) {
            part->velocity.y -= 0x20000;
            part->onGround    = true;
            part->interaction = interact;
            entity->parts[i]  = NULL;
        }
    }
}

void AmoebaDroid_CheckHit(void)
{
    RSDK_THIS(AmoebaDroid);

    if (entity->invincibleTimer > 0)
        entity->invincibleTimer--;

    foreach_active(Player, player)
    {
        if (!entity->invincibleTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
            if (--entity->health <= 0) {
                AmoebaDroid_HandleDropletRelease(false);
                entity->state               = AmoebaDroid_StateMain_Death;
                entity->timer               = 0;
                RSDK_sceneInfo->timeEnabled = false;
                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
            }
            else {
                entity->invincibleTimer = 48;
                RSDK.PlaySfx(AmoebaDroid->sfxHit, false, 255);
            }
        }
    }
}

void AmoebaDroid_CheckPlayerHit(void)
{
    RSDK_THIS(AmoebaDroid);

    foreach_active(Player, player)
    {
        if (!entity->invincibleTimer) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void AmoebaDroid_StateDrawMain_Unknown1(void)
{
    RSDK_THIS(AmoebaDroid);
    if (entity->invincibleTimer & 1) {
        RSDK.CopyPalette(1, 16, 0, 128, 10);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        RSDK.CopyPalette(1, 0, 0, 128, 10);
    }
    else {
        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void AmoebaDroid_StateDraw1_Unknown1(void)
{
    RSDK_THIS(AmoebaDroid);
    Vector2 drawPos;

    drawPos            = entity->position;
    SpriteFrame *frame = RSDK.GetFrame(AmoebaDroid->aniFrames, 3, 0);
    int32 angle          = entity->angle;
    int32 sprY           = frame->sprY;

    int32 maxY      = (frame->sprY + 96) << 8;
    frame->height = 1;
    drawPos.y -= RSDK.Sin256(entity->angle) << 10;

    for (int32 y = sprY << 8; y < maxY; angle += 2) {
        entity->scale.x = (RSDK.Sin256(frame->sprY + 2 * angle) >> 2) + 512;
        frame->sprY     = y >> 8;

        if ((y >> 8) >= sprY)
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        drawPos.y += 0x10000;
        y += (RSDK.Sin256(angle) >> 1) + 256;
    }
    frame->sprY = sprY;
}

void AmoebaDroid_StateMain_Setup(void)
{
    RSDK_THIS(AmoebaDroid);
    if (++entity->timer >= 8) {
        entity->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = RSDK_screens->centerX + (entity->position.x >> 16);
        Zone->screenBoundsT1[0]     = (entity->position.y >> 16) - RSDK_screens->height;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16);
        AmoebaDroid->arenaLeft      = (Zone->screenBoundsL1[0] + 64) << 16;
        AmoebaDroid->arenaRight     = (Zone->screenBoundsR1[0] - 64) << 16;
        AmoebaDroid->startX         = entity->position.x;
        AmoebaDroid->arenaTop       = (Zone->screenBoundsT1[0] + 48) << 16;
        AmoebaDroid->arenaBottom    = (Zone->screenBoundsB1[0] - 8) << 16;
        entity->state               = AmoebaDroid_StateMain_SetupWaterLevel;
    }
}

void AmoebaDroid_StateMain_SetupWaterLevel(void)
{
    RSDK_THIS(AmoebaDroid);
    if (entity->timer) {
        entity->timer++;
        if (entity->timer == 120) {
            entity->timer   = 0;
            entity->visible = true;
            entity->position.y += -0x400000 - (RSDK_screens->height << 16);
            entity->state = AmoebaDroid_StateMain_Unknown1;
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++entity->timer;
        }
        Water->waterLevel       = 0x7FFFFFFF;
        Water->newWaterLevel    = 0x7FFFFFFF;
        Water->targetWaterLevel = 0x7FFFFFFF;
        Water->waterMoveSpeed   = 0;
        foreach_active(Water, water)
        {
            if (water->type == 3)
                destroyEntity(water);
        }
        RSDK.SetDrawLayerProperties(0, false, NULL);
        RSDK.SetDrawLayerProperties(Zone->hudDrawOrder, false, 0);
    }
}

void AmoebaDroid_StateMain_Unknown1(void)
{
    RSDK_THIS(AmoebaDroid);
    entity->position.y += 0x8000;
    if (++entity->timer == 90) {
        entity->timer      = 0;
        entity->velocity.y = 0x8000;
        entity->state      = AmoebaDroid_StateMain_Unknown2;
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_StateMain_Unknown2(void)
{
    RSDK_THIS(AmoebaDroid);
    entity->velocity.y += 0x1800;
    entity->position.y += entity->velocity.y;
    if (entity->position.y > AmoebaDroid->arenaBottom) {
        entity->velocity.y = entity->velocity.y >> 2;
        entity->state      = AmoebaDroid_StateMain_Unknown3;
        ChemicalPool_SetDeform(entity->position.x, 0x100000);
        CREATE_ENTITY(AmoebaDroid, intToVoid(3), entity->position.x, entity->position.y);
        RSDK.PlaySfx(Water->sfx_Splash, false, 255);
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_StateMain_Unknown3(void)
{
    RSDK_THIS(AmoebaDroid);

    entity->velocity.y -= 0x2800;
    entity->position.y += entity->velocity.y;
    if (entity->position.y <= AmoebaDroid->arenaBottom) {
        entity->position.y = AmoebaDroid->arenaBottom;
        entity->offsetPos  = entity->position;
        entity->velocity.y = 0;
        entity->state      = AmoebaDroid_StateMain_Unknown4;
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_StateMain_Unknown4(void)
{
    RSDK_THIS(AmoebaDroid);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->offsetPos.y) & 0xFFFF0000;
    if (++entity->timer == 48) {
        entity->timer = 3;
        if (RSDK.Rand(0, 256) > 128)
            entity->state = AmoebaDroid_StateMain_Unknown5;
        else
            entity->state = AmoebaDroid_StateMain_Unknown6;

        EntityAmoebaDroid *part = CREATE_ENTITY(AmoebaDroid, intToVoid(1), entity->position.x, entity->position.y);
        part->parent            = (Entity *)entity;
        entity->parts[0]        = (Entity *)part;
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_StateMain_Unknown5(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->offsetPos.y) & 0xFFFF0000;
    entity->velocity.x -= 0x2000;
    if (entity->velocity.x < -0x40000)
        entity->velocity.x = -0x40000;
    entity->position.x += entity->velocity.x;

    if (entity->timer <= 0) {
        if (entity->timer <= 0) {
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            entity->state      = AmoebaDroid_StateMain_Unknown7;
            ChemicalPool_SetDeform(entity->position.x, -0xC0000);
            CREATE_ENTITY(AmoebaDroid, intToVoid(4), entity->position.x, entity->position.y);
            RSDK.PlaySfx(Water->sfx_Splash, false, 255);
        }
    }
    else if (entity->position.x < AmoebaDroid->arenaLeft + 0x400000) {
        entity->state = AmoebaDroid_StateMain_Unknown6;
        entity->timer--;
    }
    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(AmoebaDroid->sfxGather, false, 255);
    ChemicalPool_SetDeform(entity->position.x, -0x8000);
}

void AmoebaDroid_StateMain_Unknown6(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->offsetPos.y) & 0xFFFF0000;
    entity->velocity.x += 0x2000;
    if (entity->velocity.x > 0x40000)
        entity->velocity.x = 0x40000;
    entity->position.x += entity->velocity.x;

    if (entity->timer <= 0) {
        if (entity->timer >= 0) {
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            entity->state      = AmoebaDroid_StateMain_Unknown7;
            ChemicalPool_SetDeform(entity->position.x, -0xC0000);
            CREATE_ENTITY(AmoebaDroid, intToVoid(4), entity->position.x, entity->position.y);
            RSDK.PlaySfx(Water->sfx_Splash, false, 255);
        }
    }
    else if (entity->position.x > AmoebaDroid->arenaRight - 0x400000) {
        entity->state = AmoebaDroid_StateMain_Unknown5;
        entity->timer--;
    }
    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(AmoebaDroid->sfxGather, false, 255);
    ChemicalPool_SetDeform(entity->position.x, -0x8000);
}

void AmoebaDroid_StateMain_Unknown7(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->velocity.y -= 0x1800;
    entity->position.y += entity->velocity.y;
    if (entity->position.y < AmoebaDroid->arenaTop + 0x400000) {
        entity->timer = 3;
        entity->state = AmoebaDroid_StateMain_Unknown8;
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_StateMain_Unknown8(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->velocity.y += 0x2000;
    entity->position.y += entity->velocity.y;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (entity->position.x < player1->position.x) {
        entity->position.x = entity->position.x + 0x10000;
    }
    else if (entity->position.x > player1->position.x) {
        entity->position.x = entity->position.x - 0x10000;
    }

    if (entity->velocity.y > 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, entity->collisionPlane, 0, 0x180000, true)) {
        if (--entity->timer <= 0) {
            EntityAmoebaDroid *part = (EntityAmoebaDroid *)entity->parts[0];
            RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 6, &part->animator1, false, 0);
            part->drawFX    = FX_NONE;
            part->stateDraw = StateMachine_None;
            part->state     = AmoebaDroid_State2_Unknown2;

            entity->state = StateMachine_None;
            for (int32 i = 0; i < 8; ++i) {
                entity->parts[i] = (Entity *)CREATE_ENTITY(AmoebaDroid, intToVoid(2), entity->position.x, entity->position.y);
            }

            entity->timer      = 0;
            entity->partAngle  = 196;
            entity->partOffset = 0x2800;
            entity->state      = AmoebaDroid_StateMain_Unknown9;
            RSDK.PlaySfx(AmoebaDroid->sfxRelease, false, 255);
        }
        else {
            entity->velocity.y = minVal(-entity->velocity.y, -0x20000);
            for (int32 i = 0; i < 8; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, entity->position.x + RSDK.Rand(-0x200000, 0x200000),
                                                     entity->position.y + RSDK.Rand(0x100000, 0x180000));
                RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 5, &debris->animator, false, 0);
                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y    = RSDK.Rand(-0x40000, -0x20000);
                debris->gravity       = 0x3800;
                debris->inkEffect     = INK_BLEND;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x200000;
                debris->updateRange.y = 0x200000;
            }
            RSDK.PlaySfx(AmoebaDroid->sfxBounce, false, 255);
        }
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_StateMain_Unknown9(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    AmoebaDroid_HandleDropletMovement();
    entity->partAngle2 = (entity->partAngle2 + 4) & 0xFF;

    if (entity->timer == 60) {
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        if (entity->position.y <= AmoebaDroid->arenaTop) {
            entity->offsetPos.x = entity->position.x;
            entity->timer       = 0;
            entity->angle       = 0;
            entity->offsetPos.y = entity->position.y;
            entity->position.y  = AmoebaDroid->arenaTop;
            entity->state       = AmoebaDroid_StateMain_Unknown10;
        }
    }
    else {
        entity->position.y -= 0x8000;
        entity->timer++;
        if (entity->timer == 60) {
            entity->velocity.x = (AmoebaDroid->startX - entity->position.x + RSDK.Rand(-0x800000, 0x800000)) >> 6;
            entity->velocity.y = (AmoebaDroid->arenaTop - entity->position.y) >> 6;
        }
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_StateMain_Unknown10(void)
{
    RSDK_THIS(AmoebaDroid);
    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->offsetPos.y) & 0xFFFF0000;
    if (entity->timer < 120) {
        AmoebaDroid_HandleDropletMovement();
        if (entity->offsetPos.x <= AmoebaDroid->startX)
            entity->partAngle -= 2;
        else
            entity->partAngle += 2;
        entity->partAngle &= 0xFF;
        entity->partAngle2 = (entity->partAngle2 + 4) & 0xFF;
        RSDK.ProcessAnimation(&entity->animator2);
        RSDK.ProcessAnimation(&entity->animator3);
    }
    AmoebaDroid_CheckHit();

    ++entity->timer;
    if (entity->timer == 120) {
        AmoebaDroid_HandleDropletRelease(true);
        RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 2, &entity->animator3, true, 0);
        RSDK.PlaySfx(AmoebaDroid->sfxRelease, false, 255);
    }
    else if (entity->timer == 240) {
        entity->timer      = 0;
        entity->angle      = 0;
        entity->velocity.y = -0x20000;
        entity->state      = AmoebaDroid_StateMain_Unknown2;
    }
}

void AmoebaDroid_State1_Unknown1(void)
{
    RSDK_THIS(AmoebaDroid);
    EntityAmoebaDroid *parent = (EntityAmoebaDroid *)entity->parent;
    entity->angle             = (entity->angle + 4) & 0xFF;
    if (parent->state == AmoebaDroid_StateMain_SpawnSignpost || parent->state == AmoebaDroid_StateMain_Death) {
        if (entity->alpha > 0) {
            entity->alpha -= 4;
            if (entity->alpha <= 0)
                destroyEntity(entity);
        }
    }
    else {
        AmoebaDroid_CheckPlayerHit();
        if (entity->alpha < 128) {
            entity->alpha++;
            entity->position.x = parent->position.x;
            entity->position.y = parent->position.y;
        }
    }
    entity->position.x = parent->position.x;
    entity->position.y = parent->position.y;
}

void AmoebaDroid_State2_Unknown1(void)
{
    RSDK_THIS(AmoebaDroid);
    if (entity->alpha < 192)
        entity->alpha += 4;
    if (entity->onGround) {
        entity->position.x += entity->velocity.x;
        entity->velocity.y += 0x2000;
        entity->position.y += entity->velocity.y;
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, entity->collisionPlane, 0, 0x60000, true)) {
            for (int32 i = 0; i < 4; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, entity->position.x + RSDK.Rand(-0x40000, 0x40000),
                                                     entity->position.y + RSDK.Rand(-0x40000, 0x40000));
                RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 5, &debris->animator, false, 0);
                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y    = -0x20000;
                debris->gravity       = 0x3800;
                debris->inkEffect     = INK_BLEND;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x200000;
                debris->updateRange.y = 0x200000;
            }
            destroyEntity(entity);
        }
        else {
            if (entity->interaction && entity->drawOrder == Zone->drawOrderLow)
                AmoebaDroid_CheckPlayerHit();

            if (!RSDK.CheckOnScreen(entity, NULL))
                destroyEntity(entity);
        }
    }
    else {
        if (entity->interaction && entity->drawOrder == Zone->drawOrderLow)
            AmoebaDroid_CheckPlayerHit();

        if (!RSDK.CheckOnScreen(entity, NULL))
            destroyEntity(entity);
    }
}

void AmoebaDroid_State2_Unknown2(void)
{
    RSDK_THIS(AmoebaDroid);
    EntityAmoebaDroid *parent = (EntityAmoebaDroid *)entity->parent;
    if (parent) {
        entity->position.x = parent->position.x;
        entity->position.y = parent->position.y;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void AmoebaDroid_State4_Unknown1(void)
{
    RSDK_THIS(AmoebaDroid);
    if (++entity->timer == 16) {
        entity->timer   = 0;
        entity->visible = true;
        entity->state   = AmoebaDroid_State3_Unknown1;
    }
}

void AmoebaDroid_State3_Unknown1(void)
{
    RSDK_THIS(AmoebaDroid);
    int32 pos = (entity->position.x + 0x80000) >> 20;
    foreach_active(ChemicalPool, pool)
    {
        if (pos >= pool->dword88 && pos < pool->dword8C)
            entity->position.y = pool->offsetY + ChemicalPool->table1[pos];
    }

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void AmoebaDroid_StateMain_Death(void)
{
    RSDK_THIS(AmoebaDroid);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(AmoebaDroid->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2),
                          (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16) + entity->position.x,
                          (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16) + entity->position.y)
                ->drawOrder = Zone->drawOrderHigh;
        }
    }

    ++entity->timer;
    if (entity->timer == 30) {
        Debris_FallFlickerAnimSetup(AmoebaDroid->aniFrames, AmoebaDroid->debrisInfo2, 7);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
    }
    else if (entity->timer == 60) {
        Debris_FallFlickerAnimSetup(AmoebaDroid->aniFrames, AmoebaDroid->debrisInfo1, 7);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator1, true, 0);
    }
    else if (entity->timer == 90) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        entity->timer   = 0;
        entity->visible = false;
        entity->state   = AmoebaDroid_StateMain_SpawnSignpost;
    }
}

void AmoebaDroid_StateMain_SpawnSignpost(void)
{
    RSDK_THIS(AmoebaDroid);
    if (++entity->timer == 60) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = entity->position.x;
            signPost->active     = ACTIVE_NORMAL;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfx_Twinkle, false, 255);
        }
        destroyEntity(entity);
    }
}

void AmoebaDroid_EditorDraw(void) {}

void AmoebaDroid_EditorLoad(void) {}

void AmoebaDroid_Serialize(void) { RSDK_EDITABLE_VAR(AmoebaDroid, VAR_ENUM, type); }
