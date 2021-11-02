#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectGigaMetal *GigaMetal;

void GigaMetal_Update(void)
{
    RSDK_THIS(GigaMetal);
    StateMachine_Run(entity->state);
}

void GigaMetal_LateUpdate(void) {}

void GigaMetal_StaticUpdate(void) {}

void GigaMetal_Draw(void)
{
    RSDK_THIS(GigaMetal);

    if (GigaMetal->field_8 <= 0) {
        if (GigaMetal->invincibleTimer & 1) {
            RSDK.CopyPalette(2, 32, 0, 32, 16);
            RSDK.CopyPalette(2, 160, 0, 160, 10);
            RSDK.CopyPalette(2, 240, 0, 240, 7);

            if (entity->stateDraw) {
                StateMachine_Run(entity->stateDraw);
            }
            else {
                RSDK.DrawSprite(&entity->animator, NULL, false);
            }

            RSDK.CopyPalette(1, 32, 0, 32, 16);
            RSDK.CopyPalette(1, 160, 0, 160, 10);
            RSDK.CopyPalette(1, 240, 0, 240, 7);
        }
        else {
            if (entity->stateDraw) {
                StateMachine_Run(entity->stateDraw);
            }
            else {
                RSDK.DrawSprite(&entity->animator, NULL, false);
            }
        }
    }
    else {
        RSDK.SetLimitedFade(0, 4, 5, GigaMetal->field_8, 0, 256);
        if (entity->stateDraw) {
            StateMachine_Run(entity->stateDraw);
        }
        else {
            RSDK.DrawSprite(&entity->animator, NULL, false);
        }
        RSDK.CopyPalette(4, 1, 0, 1, 255);
    }
}

void GigaMetal_Create(void *data)
{
    RSDK_THIS(GigaMetal);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->drawOrder     = Zone->drawOrderLow - 1;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            int slot              = RSDK.GetEntityID(entity);
            if (data)
                entity->aniID = voidToInt(data);

            switch (entity->aniID) {
                case GIGAMETAL_HEAD:
                    entity->body       = RSDK_GET_ENTITY(slot - 1, );
                    entity->stateDraw  = GigaMetal_StateDraw0_Unknown;
                    entity->field_8C.x = 0x140000;
                    entity->field_8C.y = -0x240000;
                    entity->frontArm   = RSDK_GET_ENTITY(slot + 3, );
                    entity->backArm    = RSDK_GET_ENTITY(slot - 2, );
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_HEAD, &entity->animator, true, 0);
                    break;
                case GIGAMETAL_SHOULDER:
                    entity->body       = RSDK_GET_ENTITY(slot - 5, );
                    entity->frontArm   = RSDK_GET_ENTITY(slot - 1, );
                    entity->stateDraw  = GigaMetal_StateDraw1_Unknown;
                    entity->field_8C.x = -0x240000;
                    entity->field_8C.y = -0x2C0000;
                    entity->drawOrder  = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_SHOULDER, &entity->animator, true, 0);
                    break;
                case GIGAMETAL_BODY:
                    entity->active   = ACTIVE_BOUNDS;
                    entity->head     = RSDK_GET_ENTITY(slot + 1, );
                    entity->ruby     = RSDK_GET_ENTITY(slot + 2, );
                    entity->cover    = RSDK_GET_ENTITY(slot + 3, );
                    entity->frontArm = RSDK_GET_ENTITY(slot + 4, );
                    entity->backArm  = RSDK_GET_ENTITY(slot - 1, );
                    entity->shoulder = RSDK_GET_ENTITY(slot + 5, );
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_BODY, &entity->animator, true, 0);
                    foreach_all(MetalSonic, metal) { entity->metalSonic = (Entity *)metal; }
                    entity->health = 8;
                    entity->state  = GigaMetal_State_SetupBounds;
                    break;
                case GIGAMETAL_COVER:
                    entity->body      = RSDK_GET_ENTITY(slot - 3, );
                    entity->stateDraw = GigaMetal_StateDraw3_Unknown;
                    entity->inkEffect = INK_ADD;
                    entity->alpha     = 255;
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_COVER, &entity->animator, true, 0);
                    break;
                case GIGAMETAL_ARMFRONT:
                    entity->body       = RSDK_GET_ENTITY(slot - 4, );
                    entity->stateDraw  = GigaMetal_StateDraw6_Unknown;
                    entity->field_8C.x = -0x1C0000;
                    entity->field_8C.y = -0x2C0000;
                    entity->drawOrder  = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMFRONT, &entity->animator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMFRONT, &entity->animator2, true, 1);
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMFRONT, &entity->animator3, true, 2);
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMFRONT, &entity->animator4, true, 3);
                    break;
                case GIGAMETAL_ARMBACK:
                    entity->body       = RSDK_GET_ENTITY(slot + 1, );
                    entity->stateDraw  = GigaMetal_StateDraw6_Unknown;
                    entity->field_8C.x = -0xC0000;
                    entity->field_8C.y = -0x2C0000;
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMBACK, &entity->animator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMBACK, &entity->animator2, true, 1);
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMBACK, &entity->animator3, true, 2);
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMBACK, &entity->animator4, true, 3);
                    break;
                case GIGAMETAL_LASEREDGE:
                    entity->stateDraw = GigaMetal_StateDraw8_Unknown;
                    entity->drawFX    = FX_ROTATE;
                    entity->field_10C = -1;
                    entity->angle     = 64;
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->scale.x   = 0x200;
                    entity->scale.y   = 0x200;
                    entity->state     = GigaMetal_State8_Unknown;
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_LASEREDGE, &entity->animator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_LASERBEAM, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_LASEREDGE, &entity->animator3, true, 1);
                    break;
                case GIGAMETAL_SHARD:
                    entity->scale.x    = RSDK.Rand(0x200, 0x400);
                    entity->scale.y    = RSDK.Rand(0x200, 0x400);
                    entity->field_100  = RSDK.Rand(0, 0x400);
                    entity->field_104  = RSDK.Rand(0, 0x400);
                    entity->field_108  = RSDK.Rand(0, 0x400);
                    entity->active     = ACTIVE_NORMAL;
                    entity->visible    = true;
                    entity->drawOrder  = Zone->drawOrderHigh;
                    entity->velocity.x = RSDK.Rand(-0x10000, 0x10000);
                    entity->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                    entity->groundVel  = RSDK.Rand(-16, 16);
                    entity->state      = GigaMetal_State10_Unknown;
                    entity->stateDraw  = GigaMetal_StateDraw10_Unknown;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void GigaMetal_StageLoad(void)
{
    GigaMetal->spriteIndex = RSDK.LoadSpriteAnimation("SSZ2/GigaMetal.bin", SCOPE_STAGE);

    *(int *)&GigaMetal->hitbox1.left  = -720908;
    *(int *)&GigaMetal->hitbox1.right = 786444;

    *(int *)&GigaMetal->hitbox2.left  = -589836;
    *(int *)&GigaMetal->hitbox2.right = 0xA000C;

    *(int *)&GigaMetal->hitbox3.left  = -983080;
    *(int *)&GigaMetal->hitbox3.right = 0x280028;

    *(int *)&GigaMetal->hitbox4.left  = -8060984;
    *(int *)&GigaMetal->hitbox4.right = -4194288;

    GigaMetal->sfxRoar   = RSDK.GetSFX("SSZ2/MSRoar.wav");
    GigaMetal->sfxImpact = RSDK.GetSFX("Stage/Impact6.wav");
    GigaMetal->sfxTarget = RSDK.GetSFX("SSZ2/MSTarget.wav");
    GigaMetal->sfxPimpom = RSDK.GetSFX("Stage/PimPom.wav");
    GigaMetal->sfxCannon = RSDK.GetSFX("SSZ2/MSCannon.wav");
    RSDK.CopyPalette(0, 1, 4, 1, 255);
    for (int i = 0; i < 256; ++i) RSDK.SetPaletteEntry(5, i, 0xF00080);
    GigaMetal->invincibleTimer = 0;
    GigaMetal->field_8         = 0;
    if (StarPost->postIDs[0]) {
        foreach_all(Music, music)
        {
            if (music->playOnLoad)
                music->trackID = TRACK_EGGMAN1;
        }
        Music->field_254 = TRACK_EGGMAN1;
    }
}

void GigaMetal_StateDraw3_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    entity->position.x = entity->body->position.x + entity->field_8C.x;
    entity->position.y = entity->body->position.y + entity->field_8C.y;

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void GigaMetal_StateDraw0_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    entity->position.x = entity->body->position.x + entity->field_8C.x;
    entity->position.y = entity->body->position.y + entity->field_8C.y;

    RSDK.DrawSprite(&entity->animator, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void GigaMetal_StateDraw1_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    entity->position.x = entity->body->position.x + entity->field_8C.x;
    entity->position.y = entity->body->position.y + entity->field_8C.y;

    int x = 3 * (entity->position.x >> 2) + ((entity->frontArm->position.x - 0xC0000) >> 2);
    if (x > entity->position.x)
        x = entity->position.x;
    entity->position.x = x;

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void GigaMetal_StateDraw6_Unknown(void)
{
    RSDK_THIS(GigaMetal);
    Vector2 drawPos;

    entity->position.x = entity->body->position.x + entity->field_8C.x;
    entity->position.y = entity->body->position.y + entity->field_8C.y;

    int angle          = entity->field_100 >> 6;
    drawPos.x          = (RSDK.Sin1024(angle) << 11) + entity->position.x;
    drawPos.y          = (RSDK.Cos1024(angle) << 11) + entity->position.y;
    entity->position.x = drawPos.x;
    entity->position.y = drawPos.y;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    angle += (entity->field_104 >> 6);
    drawPos.x += 0x600 * RSDK.Sin1024(angle);
    drawPos.y += 0x600 * RSDK.Cos1024(angle);

    if (entity->animator.animationID == 5) {
        entity->drawFX   = FX_ROTATE;
        entity->rotation = -(angle >> 1);
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->drawFX = FX_NONE;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->drawFX   = FX_ROTATE;
        entity->rotation = -(angle >> 1);
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->drawFX = FX_NONE;
    }

    drawPos.x += RSDK.Sin1024(angle) << 9;
    drawPos.y += RSDK.Cos1024(angle) << 9;

    angle += 0x100;
    drawPos.x += 0x1500 * RSDK.Sin1024(angle);
    drawPos.y += 0x1500 * RSDK.Cos1024(angle);
    entity->drawFX   = FX_ROTATE;
    entity->rotation = -((angle + ((entity->field_108 >> 6) - 256)) >> 1);
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    entity->drawFX = FX_NONE;
}

void GigaMetal_StateDraw8_Unknown(void)
{
    RSDK_THIS(GigaMetal);
    Vector2 drawPos;

    entity->position.x = entity->body->position.x;
    entity->position.y = entity->body->position.y;

    int angle        = entity->angle - entity->body->rotation;
    entity->rotation = entity->angle + entity->body->rotation;
    drawPos.x        = (RSDK.Sin512(angle + 8) << 12) + entity->position.x;
    drawPos.y        = (RSDK.Cos512(angle + 8) << 12) + entity->position.y;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
    int moveX = (RSDK.Sin512(angle) << 12) & 0xFFFF0000;
    int moveY = ((RSDK.Cos512(angle)) << 12) & 0xFFFF0000;

    for (int i = 0; i < entity->field_10C; ++i) {
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        drawPos.x += moveX;
        drawPos.y += moveY;
    }
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}

void GigaMetal_StateDraw10_Unknown(void)
{
    RSDK_THIS(GigaMetal);
    Vector2 vertices[3];

    int x = RSDK_screens[RSDK_sceneInfo->currentScreenID].position.x << 16;
    int y = RSDK_screens[RSDK_sceneInfo->currentScreenID].position.y << 16;

    int angle     = entity->angle + entity->field_100;
    vertices[0].x = entity->scale.x * RSDK.Sin1024(angle) - x + entity->position.x;
    vertices[0].y = entity->scale.y * RSDK.Cos1024(angle) + entity->position.y - y;

    angle         = entity->angle + entity->field_104;
    vertices[1].x = entity->scale.x * RSDK.Sin1024(angle) - x + entity->position.x;
    vertices[1].y = entity->scale.y * RSDK.Cos1024(angle) + entity->position.y - y;

    angle         = entity->angle + entity->field_108;
    vertices[2].x = entity->scale.x * RSDK.Sin1024(angle) - x + entity->position.x;
    vertices[2].y = entity->scale.y * RSDK.Cos1024(angle) + entity->position.y - y;
    RSDK.DrawQuad(vertices, 3, 240, 0, 128, 256, INK_ADD);
}

void GigaMetal_HandleCameraMovement(void)
{
    Zone->screenBoundsL1[0] += 2;
    Zone->screenBoundsR1[0] += 2;
    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    camera->boundsL      = Zone->screenBoundsL1[0];
    camera->boundsR      = Zone->screenBoundsR1[0];
    camera->position.x   = (Zone->screenBoundsR1[0] + camera->boundsL) << 15;
    // MetalSonic_HandleStageWrap();
}

void GigaMetal_CheckPlayerCollisions(void)
{
    RSDK_THIS(GigaMetal);

    if (GigaMetal->invincibleTimer > 0)
        GigaMetal->invincibleTimer--;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &GigaMetal->hitbox4) && player->velocity.x < 0 && Player_CheckBossHit(player, entity)) {
            player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
            RSDK.PlaySfx(GigaMetal->sfxPimpom, false, 255);
        }
    }

    foreach_active(Player, playerPtr)
    {
        if (!GigaMetal->invincibleTimer && Player_CheckBadnikTouch(player, entity, &GigaMetal->hitbox3) && Player_CheckBossHit(player, entity)) {
            player->velocity.x += Zone->autoScrollSpeed;
            GigaMetal_Hit();
        }
    }
}

void GigaMetal_Hit(void)
{
    RSDK_THIS(GigaMetal);

    if (--entity->health <= 0) {
        EntityGigaMetal *head     = (EntityGigaMetal *)entity->head;
        EntityGigaMetal *frontArm = (EntityGigaMetal *)entity->frontArm;
        EntityGigaMetal *backArm  = (EntityGigaMetal *)entity->backArm;
        head->state               = StateMachine_None;
        frontArm->state           = StateMachine_None;
        backArm->state            = StateMachine_None;

        entity->timer               = 0;
        entity->state               = GigaMetal_State_Destroyed;
        RSDK_sceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        GigaMetal->invincibleTimer = 40;
        RSDK.PlaySfx(MetalSonic->sfxHit, false, 255);

        EntityGigaMetal *cover = (EntityGigaMetal *)entity->cover;
        ++cover->animator.frameID;
    }
}

void GigaMetal_State_SetupBounds(void)
{
    RSDK_THIS(GigaMetal);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.y < entity->position.y && !player1->collisionPlane) {
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX + 32;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX + 32;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 16;
        Zone->deathBoundary[0]      = (entity->position.y >> 16) + 16;

        EntityMetalSonic *metal = (EntityMetalSonic *)entity->metalSonic;
        if (metal) {
            if (!metal->activeScreens) {
                metal->position.x = entity->position.x - 0xC00000;
                metal->position.y = entity->position.y + 0x400000;
            }
            metal->angle        = 0;
            metal->posUnknown.x = entity->position.x - 0x400000;
            metal->posUnknown.y = entity->position.y - 0x500000;
            metal->timer        = 0;
            metal->visible      = true;
            metal->active       = ACTIVE_NORMAL;
            // metal->state        = MetalSonic_Unknown32;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &metal->animator, false, 0);
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &metal->animator2, false, 0);
        }
        entity->position.x -= 0x500000;
        entity->position.y -= 0x380000;
        entity->active = ACTIVE_NORMAL;
        entity->state  = GigaMetal_State_Unknown2;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
    }
}

void GigaMetal_State_Unknown2(void)
{
    RSDK_THIS(GigaMetal);

    EntityMetalSonic *metal = (EntityMetalSonic *)entity->metalSonic;
    if (metal->active == ACTIVE_NEVER) {
        EntityGigaMetal *head = (EntityGigaMetal *)entity->head;
        head->visible         = true;
        head->active          = ACTIVE_NORMAL;

        EntityPhantomRuby *ruby = (EntityPhantomRuby *)entity->ruby;
        ruby->startPos.x        = entity->position.x + 0x200000;
        ruby->startPos.y        = entity->position.y + 0x100000;
        ruby->drawOrder         = entity->drawOrder;
        ruby->position.x        = ruby->startPos.x;
        ruby->position.y        = ruby->startPos.y;

        EntityGigaMetal *cover = (EntityGigaMetal *)entity->cover;
        cover->visible         = true;
        cover->active          = ACTIVE_NORMAL;

        EntityGigaMetal *shoulder = (EntityGigaMetal *)entity->shoulder;
        shoulder->visible         = true;
        shoulder->active          = ACTIVE_NORMAL;

        EntityGigaMetal *frontArm = (EntityGigaMetal *)entity->frontArm;
        frontArm->visible         = true;
        frontArm->active          = ACTIVE_NORMAL;
        frontArm->state           = GigaMetal_StateArm_Unknown1;

        EntityGigaMetal *backArm = (EntityGigaMetal *)entity->backArm;
        backArm->visible         = true;
        backArm->active          = ACTIVE_NORMAL;
        backArm->state           = GigaMetal_StateArm_Unknown1;

        entity->visible = true;
        entity->state   = GigaMetal_State_Unknown3;
        RSDK.CopyTileLayer(Zone->fgHigh, 0, 34, Zone->fgHigh, 0, 26, 256, 6);

        foreach_active(Player, player)
        {
            if (player->position.x < entity->position.x + 0xE00000)
                player->position.x = entity->position.x + 0xE00000;
            player->position.y = ruby->position.y + 0x80000;
            player->groundVel  = 0;
            player->velocity.x = 0;
        }
    }
}

void GigaMetal_State_Unknown3(void)
{
    RSDK_THIS(GigaMetal);

    if (++entity->timer == 120) {
        EntityGigaMetal *head = (EntityGigaMetal *)entity->head;
        head->state           = GigaMetal_StateHead_Unknown1;

        EntityGigaMetal *frontArm = (EntityGigaMetal *)entity->frontArm;
        frontArm->state           = GigaMetal_StateArm_Unknown2;

        EntityGigaMetal *backArm = (EntityGigaMetal *)entity->backArm;
        backArm->state           = GigaMetal_StateArm_Unknown2;
    }
}

void GigaMetal_State_Unknown4(void)
{
    RSDK_THIS(GigaMetal);

    ++entity->timer;
    if (!(entity->timer & 7))
        Camera_ShakeScreen(0, 0, 4);
    if (entity->timer == 120) {
        EntityGigaMetal *head = (EntityGigaMetal *)entity->head;
        head->state           = GigaMetal_StateHead_Unknown7;

        EntityGigaMetal *frontArm = (EntityGigaMetal *)entity->frontArm;
        frontArm->timer           = 48;
        frontArm->state           = GigaMetal_StateArm_Unknown3;

        EntityGigaMetal *backArm = (EntityGigaMetal *)entity->backArm;
        backArm->timer           = 48;
        backArm->angle           = 128;
        backArm->state           = GigaMetal_StateArm_Unknown3;

        entity->field_94.x = entity->position.x;
        entity->field_94.y = entity->position.y;
        entity->field_94.y += 0x80000;
        entity->timer         = 320;
        entity->velocity.x    = 0x20000;
        entity->state         = GigaMetal_State_Unknown5;
        Zone->autoScrollSpeed = 0x20000;
    }
}

void GigaMetal_State_Unknown5(void)
{
    RSDK_THIS(GigaMetal);

    entity->position.x += entity->velocity.x;

    if ((RSDK.Sin256(entity->angle) & 0x200000) != 0)
        entity->position.y = entity->field_94.y - -(RSDK.Sin256(entity->angle) << 10);
    else
        entity->position.y = entity->field_94.y - (RSDK.Sin256(entity->angle) << 10);
    entity->angle = (entity->angle + 2) & 0xFF;

    EntityPhantomRuby *ruby = (EntityPhantomRuby *)entity->ruby;
    ruby->position.x        = entity->position.x + 0x200000;
    ruby->startPos.y        = entity->position.y + 0x100000;

    EntityGigaMetal *head = (EntityGigaMetal *)entity->head;
    if (!(entity->angle & 0x7F)) {
        if (!head->state) {
            head->field_94.x = head->field_8C.x;
            head->field_94.y = head->field_8C.y;
            head->velocity.y = 0x10000;
            head->state      = GigaMetal_StateHead_Unknown3;
            RSDK.PlaySfx(GigaMetal->sfxImpact, false, 255);
        }

        EntityGigaMetal *shoulder = (EntityGigaMetal *)entity->shoulder;
        shoulder->field_94.x      = shoulder->field_8C.x;
        shoulder->field_94.y      = shoulder->field_8C.y;
        shoulder->velocity.y      = -0x12000;
        shoulder->state           = GigaMetal_StateShoulder_Unknown1;
        Camera_ShakeScreen(0, 0, 4);
    }

    if (!head->state || head->state == GigaMetal_StateHead_Unknown3)
        --entity->timer;
    if (entity->timer <= 0) {
        EntityGigaMetal *frontArm = (EntityGigaMetal *)entity->frontArm;
        EntityGigaMetal *backArm  = (EntityGigaMetal *)entity->backArm;

        entity->timer = RSDK.Rand(240, 480);
        if (RSDK.Rand(0, 1024) <= 512) {
            if ((entity->field_88 & 0xFFFFFF00) != 0)
                entity->field_88 = 0;
            else
                entity->field_88++;

            if ((entity->field_88 & 0xFF) > 2)
                entity->field_88 = 256;
        }
        else {
            if ((entity->field_88 & 0xFFFFFF00) == 256)
                ++entity->field_88;
            else
                entity->field_88 = 256;

            if ((entity->field_88 & 0xFF) > 2)
                entity->field_88 = 0;
        }

        if ((entity->field_88 & 0xFFFFFF00) == 256)
            head->state = GigaMetal_StateHead_Unknown8;
        else
            head->state = GigaMetal_StateHead_Unknown4;

        frontArm->state = GigaMetal_StateArm_Unknown2;
        backArm->state  = GigaMetal_StateArm_Unknown2;
    }
    GigaMetal_HandleCameraMovement();
    GigaMetal_CheckPlayerCollisions();

    if ((Zone->timer & 7) == 2) {
        int endY = (RSDK_screens->height + RSDK_screens->position.y) >> 4;
        int endX = ((RSDK_screens->width + RSDK_screens->position.x) >> 4) + 1;

        RSDK.CopyTileLayer(Zone->fgHigh, endX, endY - 5, Zone->fgHigh, endX, endY - 5 + 8, 1, 6);
        if (endX > 224)
            RSDK.CopyTileLayer(Zone->fgHigh, endX - 224, endY - 5, Zone->fgHigh, endX - 224, endY - 5 + 8, 1, 6);

        int delay  = 4;
        int tileX  = (RSDK_screens->position.x >> 4) + 8;
        int spawnY = (endY << 20) + 0x80000;

        for (int delay = 4; delay < 40; delay += 6) {
            uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, tileX, endY);

            if (tile != 0xFFFF) {
                EntityBreakableWall *block = CREATE_ENTITY(BreakableWall, intToVoid(2), (tileX << 20) + 0x80000, spawnY);
                block->drawOrder           = Zone->drawOrderHigh;
                block->layerID             = Zone->fgHigh;
                block->tileInfo            = tile;
                block->tilePos.x           = tileX;
                block->tilePos.y           = endY;
                block->timer               = delay;
                block->active              = ACTIVE_NORMAL;
                if (tileX > 224) {
                    block                = CREATE_ENTITY(BreakableWall, intToVoid(2), (tileX << 20) - 0xDF80000, spawnY);
                    block->drawOrder     = Zone->drawOrderHigh;
                    block->layerID       = Zone->fgHigh;
                    block->tileInfo      = tile;
                    block->tilePos.y     = endY;
                    block->tilePos.x     = tileX - 224;
                    block->timer         = delay;
                    block->updateRange.x = 0x10000000;
                    block->active        = ACTIVE_NORMAL;
                }
            }

            --endY;
            spawnY -= 0x100000;
        }
    }
}

void GigaMetal_State_Destroyed(void)
{
    RSDK_THIS(GigaMetal);

    entity->velocity.y += 0x1000;
    entity->position.y += entity->velocity.y;
    if (entity->position.y >= entity->field_94.y) {
        EntityGigaMetal *head = (EntityGigaMetal *)entity->head;
        entity->velocity.y    = -0xC000;
        head->velocity.y      = 0x18000;
        head->field_94        = head->field_8C;
        head->state           = GigaMetal_StateHead_Unknown3;

        EntityGigaMetal *shoulder = (EntityGigaMetal *)entity->shoulder;
        shoulder->velocity.y      = -0x18000;
        shoulder->field_94        = shoulder->field_8C;
        shoulder->state           = GigaMetal_StateShoulder_Unknown1;
        Camera_ShakeScreen(0, 0, 4);
    }

    EntityPhantomRuby *ruby = (EntityPhantomRuby *)entity->ruby;
    ruby->position.x        = entity->position.x + 0x200000;
    ruby->startPos.y        = entity->position.y + 0x100000;

    if (!(Zone->timer & 7)) {
        int x = entity->position.x + RSDK.Rand(-0x600000, 0x600000);
        int y = entity->position.y + RSDK.Rand(-0x600000, 0x600000);
        CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
    }
    if (!(Zone->timer % 6))
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);

    if ((Zone->timer & 0x3F) == 32)
        RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);

    foreach_active(Player, player)
    {
        if (player->position.x < entity->position.x + 0x400000) {
            player->velocity.x = 0x40000;
            if (player->velocity.y >= 0) {
                player->velocity.y = -0x40000;
                player->onGround   = false;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
            }
        }
    }

    if (GigaMetal->field_8 >= 240) {
        GigaMetal->field_8      = 0;
        EntityMetalSonic *metal = (EntityMetalSonic *)entity->metalSonic;

        metal->position.x = entity->position.x - 0x100000;
        metal->position.y = entity->position.y - 0x200000;
        metal->active     = ACTIVE_NORMAL;
        // metal->state           = MetalSonic_State_Defeated;
        metal->velocity.x = 0;
        metal->velocity.y = -0x40000;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 10, &metal->animator, false, 0);

        foreach_active(GigaMetal, part) { destroyEntity(part); }

        for (int i = 0; i < 0x40; ++i) {
            int x = metal->position.x + RSDK.Rand(-0x600000, 0x400000);
            int y = metal->position.y + RSDK.Rand(-0x600000, 0x600000);
            CREATE_ENTITY(GigaMetal, intToVoid(GIGAMETAL_SHARD), x, y);
        }

        EntityFXFade *fxFade  = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn       = 256;
        fxFade->speedOut      = 64;
        Zone->autoScrollSpeed = 0;
        RSDK.PlaySfx(MetalSonic->sfxExplosion3, false, 255);

        EntityDango *dango = CREATE_ENTITY(Dango, NULL, (RSDK_screens->position.x - 64) << 16, (RSDK_screens->position.y + 200) << 16);
        dango->timer       = 0;
        dango->direction   = FLIP_X;
        dango->state       = Dango_StateTaunt_Unknown1;

        int tileX = RSDK_screens->position.x >> 4;
        int tileY = ((RSDK_screens->height + RSDK_screens->position.y) >> 4) - 5;

        int spawnX = (tileX << 20) + 0x80000;
        for (int x = 0; x < 32; ++x) {
            int spawnY = (tileY << 20) + 0x80000;
            for (int y = 0; y < 6; ++y) {
                uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, tileX, tileY);
                if (tile != 0xFFFF) {
                    RSDK.SetTileInfo(Zone->fgHigh, tileX, tileY, 0xFFFF);
                    EntityBreakableWall *block = CREATE_ENTITY(BreakableWall, intToVoid(1), spawnX, spawnY);
                    block->drawOrder           = Zone->drawOrderHigh;
                    block->visible             = true;
                    block->tileInfo            = tile;
                    block->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                    block->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                    block->drawFX              = FX_ROTATE | FX_FLIP;
                }
                ++tileY;
                spawnY += 0x100000;
            }
            spawnX += 0x100000;
            tileY -= 6;
            ++tileX;
        }

        RSDK.CopyTileLayer(Zone->fgLow, 0, 30, Zone->fgLow, 0, 174, 256, 2);
        RSDK.CopyTileLayer(Zone->fgHigh, 0, 26, Zone->fgLow, 0, 0, 256, 6);
    }
    else {
        GigaMetal->field_8++;
    }
}

void GigaMetal_StateShoulder_Unknown1(void)
{
    RSDK_THIS(GigaMetal);

    entity->velocity.y += 0x3800;
    entity->field_8C.y += entity->velocity.y;
    if (entity->velocity.y > 0) {
        if (entity->field_8C.y >= entity->field_94.y) {
            entity->field_8C.y = entity->field_94.y;
            entity->velocity.y = -(entity->velocity.y >> 2);
            if (entity->velocity.y > -0x800) {
                entity->velocity.y = 0;
                entity->state      = StateMachine_None;
            }
        }
    }
}

void GigaMetal_StateHead_Unknown3(void)
{
    RSDK_THIS(GigaMetal);

    entity->velocity.y -= 0x3800;
    entity->field_8C.y += entity->velocity.y;
    if (entity->velocity.y < 0) {
        if (entity->field_8C.y <= entity->field_94.y) {
            entity->field_8C.y = entity->field_94.y;
            entity->velocity.y = -(entity->velocity.y >> 2);
            if (entity->velocity.y < 0x4000) {
                entity->velocity.y = 0;
                entity->state      = StateMachine_None;
            }
        }
    }
}

void GigaMetal_State8_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&entity->animator2);
    if (!(Zone->timer & 1)) {
        if (entity->field_10C < 9)
            entity->field_10C++;
    }

    entity->position.x = entity->body->position.x;
    entity->position.y = entity->body->position.y;
    int angle          = entity->angle - entity->body->rotation;
    int x              = (RSDK.Sin512(angle + 8) << 12) + entity->position.x;
    int y              = (RSDK.Cos512(angle + 8) << 12) + entity->position.y;
    int moveX          = (RSDK.Sin512(angle) << 12) & 0xFFFF0000;
    int moveY          = (RSDK.Cos512(angle) << 12) & 0xFFFF0000;

    for (int c = 0; c < entity->field_10C; ++c) {
        entity->position.x = x;
        entity->position.y = y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &GigaMetal->hitbox1) && Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING)) {
                player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
            }
        }

        if (!(Zone->timer & 3)) {
            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x20000, true))
                CREATE_ENTITY(Explosion, NULL, entity->position.x, entity->position.y);
        }

        x += moveX;
        y += moveY;
    }
}

void GigaMetal_StateHead_Unknown10(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&entity->animator2);

    if (entity->scale.y <= 8)
        destroyEntity(entity);
    else
        entity->scale.y -= 8;
}

void GigaMetal_State10_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->angle += entity->groundVel;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void GigaMetal_StateHead_Unknown1(void)
{
    RSDK_THIS(GigaMetal);

    entity->drawFX = FX_ROTATE;
    if (entity->rotation <= -32) {
        entity->state = GigaMetal_StateHead_Unknown2;
        RSDK.PlaySfx(GigaMetal->sfxRoar, false, 255);
    }
    else {
        entity->rotation -= 2;
    }
}

void GigaMetal_StateHead_Unknown2(void)
{
    RSDK_THIS(GigaMetal);

    entity->rotation = RSDK.Rand(-34, -30);
}

void GigaMetal_StateHead_Unknown7(void)
{
    RSDK_THIS(GigaMetal);

    if (entity->rotation >= 0) {
        entity->rotation = 0;
        entity->drawFX   = FX_NONE;
        entity->state    = StateMachine_None;
    }
    else {
        entity->rotation += 2;
    }
}

void GigaMetal_StateHead_Unknown4(void)
{
    RSDK_THIS(GigaMetal);

    entity->drawFX = FX_ROTATE;
    if (entity->rotation <= -64) {
        RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_LASERCHARGE, &entity->animator3, false, 0);
        entity->state = GigaMetal_StateHead_Unknown5;
        RSDK.PlaySfx(GigaMetal->sfxRoar, false, 255);
    }
    else {
        entity->rotation = -2;
    }
}

void GigaMetal_StateHead_Unknown5(void)
{
    RSDK_THIS(GigaMetal);

    entity->rotation = RSDK.Rand(-66, -62);
    RSDK.ProcessAnimation(&entity->animator3);
    if (++entity->timer == 120) {
        entity->timer                                                                                          = 0;
        entity->state                                                                                          = GigaMetal_StateHead_Unknown6;
        CREATE_ENTITY(GigaMetal, intToVoid(GIGAMETAL_LASEREDGE), entity->position.x, entity->position.y)->body = (Entity *)entity;
        RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 255);
    }
}

void GigaMetal_StateHead_Unknown6(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->rotation >= 0) {
        if (++entity->timer == 2) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, false, 0);

            foreach_active(GigaMetal, arm)
            {
                if (arm->state == GigaMetal_State8_Unknown) {
                    arm->drawFX |= FX_SCALE;
                    arm->alpha = 0xA0;
                    arm->state = GigaMetal_StateHead_Unknown10;
                }
            }
        }
        else if (entity->timer == 30) {
            entity->timer = 0;
            foreach_active(GigaMetal, arm)
            {
                if (arm->state == GigaMetal_StateArm_Unknown2) {
                    arm->angle = arm->body->angle;
                    if (arm->animator.animationID == 6)
                        arm->angle += 128;
                    arm->timer = 48;
                    arm->state = GigaMetal_StateArm_Unknown3;
                }
            }
            entity->rotation = 0;
            entity->drawFX   = FX_NONE;
            entity->state    = StateMachine_None;
        }
    }
    else {
        entity->rotation++;
    }
}

void GigaMetal_StateHead_Unknown8(void)
{
    RSDK_THIS(GigaMetal);

    ++entity->timer;
    RSDK.ProcessAnimation(&entity->animator2);

    if (entity->timer == 1) {
        RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_EYE, &entity->animator2, false, 3);
    }
    else if (entity->timer == 88) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, false, 0);
        entity->field_94 = entity->field_8C;
        entity->timer    = 0;
        entity->state    = GigaMetal_StateHead_Unknown9;
    }

    bool32 flag = false;
    if (entity->timer >= 72)
        flag = (entity->timer & 7) == 1;
    else
        flag = (entity->timer % 20) == 1;

    if (flag)
        RSDK.PlaySfx(GigaMetal->sfxTarget, false, 255);
}

void GigaMetal_StateHead_Unknown9(void)
{
    RSDK_THIS(GigaMetal);

    entity->timer++;
    if ((entity->timer % 90) == 1) {
        entity->velocity.x = -0x8000;
        EntityMSBomb *bomb = CREATE_ENTITY(MSBomb, NULL, entity->position.x + 0x100000, entity->position.y + 0xC0000);
        bomb->velocity.x   = 0x40000;
        bomb->velocity.y   = 0x40000;
        RSDK.PlaySfx(GigaMetal->sfxCannon, false, 255);
    }

    entity->field_8C.x += entity->velocity.x;
    entity->field_8C.y += entity->velocity.x;
    entity->velocity.x += 0x3800;
    if (entity->field_8C.x > entity->field_94.x) {
        entity->field_8C.x = entity->field_94.x;
        entity->field_8C.y = entity->field_94.y;
        entity->velocity.x = 0;
    }

    if (entity->timer == 224) {
        EntityGigaMetal *frontArm = (EntityGigaMetal *)entity->frontArm;
        entity->timer             = 0;
        frontArm->timer           = 48;
        frontArm->angle           = 0;
        frontArm->state           = GigaMetal_StateArm_Unknown3;

        EntityGigaMetal *backArm = (EntityGigaMetal *)entity->backArm;
        backArm->timer           = 48;
        backArm->angle           = 128;
        backArm->state           = GigaMetal_StateArm_Unknown3;
        entity->state            = 0;
    }
}

void GigaMetal_HandleArmCollisions(void)
{
    RSDK_THIS(GigaMetal);

    if (entity->animator.animationID != 6) {
        int angle          = entity->field_100 >> 6;
        entity->position.x = entity->body->position.x;
        entity->position.y = entity->body->position.y;
        entity->position.x += entity->field_8C.x;
        entity->position.y += entity->field_8C.y;
        entity->position.x += RSDK.Sin1024(angle) << 11;
        entity->position.y += RSDK.Cos1024(angle) << 11;

        angle += entity->field_104 >> 6;
        entity->position.x += 0x600 * RSDK.Sin1024(angle);
        entity->position.y += 0x600 * RSDK.Cos1024(angle);
        entity->position.x += RSDK.Sin1024(angle) << 9;

        angle += 256;
        entity->position.y += RSDK.Cos1024(angle) << 9;
        entity->position.x += 0x1500 * RSDK.Sin1024(angle);
        entity->position.y += 0x1500 * RSDK.Cos1024(angle);

        angle += entity->field_108 >> 6;
        entity->position.x += 0x600 * RSDK.Sin1024(angle);
        entity->position.y += 0x600 * RSDK.Cos1024(angle);

        foreach_active(Player, player)
        {
            if (!GigaMetal->invincibleTimer) {
                if (Player_CheckCollisionTouch(player, entity, &GigaMetal->hitbox2)
                    && !Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer) && Player_CheckHit(player, entity)) {
                    player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
                }
            }
        }
    }
}

void GigaMetal_StateArm_Unknown1(void)
{
    RSDK_THIS(GigaMetal);

    entity->field_100 = RSDK.Sin256(entity->timer) - 0x800;
    entity->field_104 = RSDK.Sin256(entity->timer) - 0x200;
    entity->field_108 = RSDK.Sin256(entity->timer) + 0xC00;
    entity->timer     = (entity->timer + 2) & 0xFF;
    GigaMetal_HandleArmCollisions();
}

void GigaMetal_StateArm_Unknown2(void)
{
    RSDK_THIS(GigaMetal);

    if (entity->field_100 >= -0x1F00) {
        entity->field_100 += ((-0x2000 - entity->field_100) >> 3);
        entity->field_104 += (-0x1000 - entity->field_104) >> 3;
        entity->field_108 += ((0x1000 - entity->field_108) >> 3);
    }
    else {
        entity->field_104 = RSDK.Rand(-0x1200, -0xE00);
    }
    GigaMetal_HandleArmCollisions();
}

void GigaMetal_StateArm_Unknown3(void)
{
    RSDK_THIS(GigaMetal);

    int val1 = 16 * RSDK.Sin256(entity->angle);
    int val2 = 4 * RSDK.Sin256(entity->angle);
    int val3 = 4 * RSDK.Sin256(entity->angle - 64);
    if (entity->timer > 0) {
        val2 = entity->field_104 + ((val2 - entity->field_104) >> 2);
        val1 = entity->field_100 + ((val1 - entity->field_100) >> 2);
        val3 = entity->field_108 + ((val3 - entity->field_108) >> 2);
        --entity->timer;
    }

    entity->field_100 = val1;
    entity->field_104 = val2;
    entity->field_108 = val3;
    entity->angle     = (entity->angle + 2) & 0xFF;
    GigaMetal_HandleArmCollisions();
}

#if RETRO_INCLUDE_EDITOR
void GigaMetal_EditorDraw(void)
{
    RSDK_THIS(GigaMetal);

    switch (entity->aniID) {
        case GIGAMETAL_HEAD:
            entity->stateDraw  = GigaMetal_StateDraw0_Unknown;
            entity->field_8C.x = 0x140000;
            entity->field_8C.y = -0x240000;
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_HEAD, &entity->animator, true, 0);

            RSDK.DrawSprite(&entity->animator, NULL, false);
            break;
        case GIGAMETAL_SHOULDER:
            entity->stateDraw  = GigaMetal_StateDraw1_Unknown;
            entity->field_8C.x = -0x240000;
            entity->field_8C.y = -0x2C0000;
            entity->drawOrder  = Zone->drawOrderHigh;
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_SHOULDER, &entity->animator, true, 0);

            RSDK.DrawSprite(&entity->animator, NULL, false);
            break;
        case GIGAMETAL_BODY:
            entity->active   = ACTIVE_BOUNDS;
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_BODY, &entity->animator, true, 0);
            entity->health = 8;
            entity->state  = GigaMetal_State_SetupBounds;

            RSDK.DrawSprite(&entity->animator, NULL, false);
            break;
        case GIGAMETAL_COVER:
            entity->inkEffect = INK_ADD;
            entity->alpha     = 255;
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_COVER, &entity->animator, true, 0);

            RSDK.DrawSprite(&entity->animator, NULL, false);
            break;
        case GIGAMETAL_ARMFRONT:
            entity->stateDraw  = GigaMetal_StateDraw6_Unknown;
            entity->field_8C.x = -0x1C0000;
            entity->field_8C.y = -0x2C0000;
            entity->drawOrder  = Zone->drawOrderHigh;
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMFRONT, &entity->animator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMFRONT, &entity->animator2, true, 1);
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMFRONT, &entity->animator3, true, 2);
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMFRONT, &entity->animator4, true, 3);

            entity->body = (Entity *)entity;
            StateMachine_Run(entity->stateDraw);
            break;
        case GIGAMETAL_ARMBACK:
            entity->stateDraw  = GigaMetal_StateDraw6_Unknown;
            entity->field_8C.x = -0xC0000;
            entity->field_8C.y = -0x2C0000;
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMBACK, &entity->animator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMBACK, &entity->animator2, true, 1);
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMBACK, &entity->animator3, true, 2);
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_ARMBACK, &entity->animator4, true, 3);

            entity->body     = (Entity *)entity;
            StateMachine_Run(entity->stateDraw);
            break;
        case GIGAMETAL_LASEREDGE:
            entity->stateDraw = GigaMetal_StateDraw8_Unknown;
            entity->drawFX    = FX_ROTATE;
            entity->field_10C = -1;
            entity->angle     = 64;
            entity->visible   = true;
            entity->scale.x   = 0x200;
            entity->scale.y   = 0x200;
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_LASEREDGE, &entity->animator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_LASERBEAM, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->spriteIndex, GIGAMETAL_LASEREDGE, &entity->animator3, true, 1);

            entity->body = (Entity *)entity;
            StateMachine_Run(entity->stateDraw);
            break;
        case GIGAMETAL_SHARD:
            break;
        default: break;
    }
}

void GigaMetal_EditorLoad(void)
{
    GigaMetal->spriteIndex = RSDK.LoadSpriteAnimation("SSZ2/GigaMetal.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(GigaMetal, aniID);
    RSDK_ENUM_VAR(GIGAMETAL_HEAD);
    RSDK_ENUM_VAR(GIGAMETAL_SHOULDER);
    RSDK_ENUM_VAR(GIGAMETAL_BODY);
    RSDK_ENUM_VAR(GIGAMETAL_COVER);
    RSDK_ENUM_VAR(GIGAMETAL_EYE);
    RSDK_ENUM_VAR(GIGAMETAL_ARMFRONT);
    RSDK_ENUM_VAR(GIGAMETAL_ARMBACK);
    RSDK_ENUM_VAR(GIGAMETAL_LASERCHARGE);
    RSDK_ENUM_VAR(GIGAMETAL_LASEREDGE);
    RSDK_ENUM_VAR(GIGAMETAL_LASERBEAM);
    // RSDK_ENUM_VAR(GIGAMETAL_SHARD);
}
#endif

void GigaMetal_Serialize(void)
{
    RSDK_EDITABLE_VAR(GigaMetal, VAR_UINT8, aniID);
    RSDK_EDITABLE_VAR(GigaMetal, VAR_UINT8, frameID);
}
#endif
