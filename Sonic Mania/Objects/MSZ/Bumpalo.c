#include "SonicMania.h"

ObjectBumpalo *Bumpalo;

void Bumpalo_Update(void)
{
    RSDK_THIS(Bumpalo);
    RSDK.ProcessAnimation(&entity->animator2);
    StateMachine_Run(entity->state);
}

void Bumpalo_LateUpdate(void) {}

void Bumpalo_StaticUpdate(void) {}

void Bumpalo_Draw(void)
{
    RSDK_THIS(Bumpalo);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    if (entity->state == Bumpalo_State_Unknown3)
        RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void Bumpalo_Create(void *data)
{
    RSDK_THIS(Bumpalo);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow + 1;
    entity->drawFX        = FX_FLIP;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &entity->animator1, true, 0);
    entity->state = Bumpalo_State_Setup;
}

void Bumpalo_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        Bumpalo->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Bumpalo.bin", SCOPE_STAGE);
    Bumpalo->hitboxBadnik.left   = -20;
    Bumpalo->hitboxBadnik.top    = 0;
    Bumpalo->hitboxBadnik.right  = 20;
    Bumpalo->hitboxBadnik.bottom = 16;

    Bumpalo->hitboxBumper.left   = -10;
    Bumpalo->hitboxBumper.top    = -16;
    Bumpalo->hitboxBumper.right  = 12;
    Bumpalo->hitboxBumper.bottom = 0;

    Bumpalo->hitboxClack.left   = -128;
    Bumpalo->hitboxClack.top    = -64;
    Bumpalo->hitboxClack.right  = -16;
    Bumpalo->hitboxClack.bottom = 96;

    Bumpalo->hitboxCharge.left   = -17;
    Bumpalo->hitboxCharge.top    = -14;
    Bumpalo->hitboxCharge.right  = 10;
    Bumpalo->hitboxCharge.bottom = 0;

    Bumpalo->hitbox5.left   = -24;
    Bumpalo->hitbox5.top    = -10;
    Bumpalo->hitbox5.right  = -8;
    Bumpalo->hitbox5.bottom = 0;

    Bumpalo->sfxBumper = RSDK.GetSFX("Stage/Bumper.wav");
    Bumpalo->sfxHuff   = RSDK.GetSFX("Stage/Huff.wav");
    Bumpalo->sfxClack  = RSDK.GetSFX("Stage/Clack.wav");
    Bumpalo->sfxImpact = RSDK.GetSFX("Stage/Impact5.wav");
    DEBUGMODE_ADD_OBJ(Bumpalo);
}

void Bumpalo_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityBumpalo *bumpalo = CREATE_ENTITY(Bumpalo, NULL, entity->position.x, entity->position.y);
    bumpalo->direction     = entity->direction;
    bumpalo->startDir      = entity->direction;
}

void Bumpalo_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Bumpalo_CheckOnScreen(void)
{
    RSDK_THIS(Bumpalo);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Bumpalo_Create(NULL);
    }
}

void Bumpalo_BumpPlayer(void *p)
{
    RSDK_THIS(Bumpalo);
    EntityPlayer *player = (EntityPlayer *)p;

    RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
    int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
    int velX  = 0x700 * RSDK.Cos256(angle);
    int velY  = 0x700 * RSDK.Sin256(angle);

    if (player->state == Player_State_FlyCarried)
        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

    int anim = player->playerAnimator.animationID;
    if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED) {
        if (player->state != Player_State_TailsFlight) {
            if (player->state != Player_State_DropDash)
                player->state = Player_State_Air;
            if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                player->playerAnimator.animationID = ANI_WALK;
        }
    }
    if (player->playerAnimator.animationID != ANI_FLY) {
        player->velocity.x  = velX;
        player->groundVel   = velX;
        player->jumpAbility = 0;
    }
    player->velocity.y     = velY;
    player->onGround       = false;
    player->tileCollisions = true;
    if (entity->animator1.animationID == 3)
        entity->velocity.x >>= 1;
    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &entity->animator1, true, 0);

    if (entity->state != Bumpalo_State_Unknown6 && entity->state != Bumpalo_State_Unknown5)
        entity->velocity.x = 0;
    entity->state = Bumpalo_State_Unknown5;
}

void Bumpalo_CheckPlayerCollisions(void)
{
    RSDK_THIS(Bumpalo);

    foreach_active(Player, player)
    {
        if (entity->animator1.animationID == 3) {
            if (abs(entity->velocity.x) <= 0x10000) {
                if (!Player_CheckBadnikTouch(player, entity, &Bumpalo->hitboxBadnik)) {
                    if (!Player_CheckBadnikTouch(player, entity, &Bumpalo->hitboxBumper)) {
                        if (Player_CheckCollisionTouch(player, entity, &Bumpalo->hitboxCharge)) {
#if RETRO_USE_PLUS
                            if (Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)) {
                                RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &entity->animator1, true, 0);
                                entity->state      = Bumpalo_State_Unknown5;
                                entity->velocity.y = -0x20000;
                                if (entity->direction == FLIP_NONE)
                                    entity->velocity.x = 0x10000;
                                else
                                    entity->velocity.x = -0x10000;
                            }
                            else
#endif
                                Player_CheckHit(player, entity);
                        }
                    }
                    else {
#if RETRO_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop) {
                            RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                            Player_CheckBadnikBreak(entity, player, true);
                        }
                        else
#endif
                            Bumpalo_BumpPlayer(player);
                    }
                }
                else if (Player_CheckBadnikTouch(player, entity, &Bumpalo->hitboxBadnik)) {
                    Player_CheckBadnikBreak(entity, player, true);
                }
            }
            else if (Player_CheckBadnikTouch(player, entity, &Bumpalo->hitboxBumper)) {
#if RETRO_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                    Player_CheckBadnikBreak(entity, player, true);
                }
                else
#endif
                    Bumpalo_BumpPlayer(player);
            }
            else if (Player_CheckCollisionTouch(player, entity, &Bumpalo->hitboxCharge)) {
#if RETRO_USE_PLUS
                if (Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)) {
                    RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &entity->animator1, true, 0);
                    entity->state      = Bumpalo_State_Unknown5;
                    entity->velocity.y = -0x20000;
                    if (entity->direction == FLIP_NONE)
                        entity->velocity.x = 0x10000;
                    else
                        entity->velocity.x = -0x10000;
                }
                else
#endif
                    Player_CheckHit(player, entity);
            }
            else if (Player_CheckBadnikTouch(player, entity, &Bumpalo->hitboxBadnik)) {
                Player_CheckBadnikBreak(entity, player, true);
            }
        }
        else {
            if (!Player_CheckBadnikTouch(player, entity, &Bumpalo->hitboxBadnik)) {
                if (!Player_CheckBadnikTouch(player, entity, &Bumpalo->hitboxBumper)) {
                    if (Player_CheckCollisionTouch(player, entity, &Bumpalo->hitboxCharge)) {
#if RETRO_USE_PLUS
                        if (Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)) {
                            RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &entity->animator1, true, 0);
                            entity->state      = Bumpalo_State_Unknown5;
                            entity->velocity.y = -0x20000;
                            if (entity->direction == FLIP_NONE)
                                entity->velocity.x = 0x10000;
                            else
                                entity->velocity.x = -0x10000;
                        }
                        else
#endif
                            Player_CheckHit(player, entity);
                    }
                }
                else {
#if RETRO_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop) {
                        RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                        Player_CheckBadnikBreak(entity, player, true);
                    }
                    else
#endif
                        Bumpalo_BumpPlayer(player);
                }
            }
            else if (Player_CheckBadnikTouch(player, entity, &Bumpalo->hitboxBadnik)) {
                Player_CheckBadnikBreak(entity, player, true);
            }
        }
    }
}

void Bumpalo_HandlePlatformCollisions(void *p)
{
    RSDK_THIS(Bumpalo);
    EntityPlatform *platform = (EntityPlatform *)p;

    if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;

        bool32 flag = false;
        if (platform->collision) {
            if (platform->collision != PLATFORM_C_1) {
                if (platform->collision == PLATFORM_C_2
                    && RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, entity, &Bumpalo->hitboxBadnik)) {
                    if ((entity->collisionLayers & Zone->moveID) != 0) {
                        TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
                        move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                        move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
                    }
                    if (entity->velocity.y >= 0)
                        entity->onGround |= true;
                }
                flag = true;
            }
        }
        else {
            Hitbox *solid = RSDK.GetHitbox(&platform->animator, 0);
            entity->onGround |= RSDK.CheckObjectCollisionPlatform(platform, solid, entity, &Bumpalo->hitboxBadnik, true);
        }

        if (!flag) {
            Hitbox *platformHitbox = RSDK.GetHitbox(&platform->animator, 1);
            int velX               = entity->velocity.x;
            int side               = RSDK.CheckObjectCollisionBox(platform, platformHitbox, entity, &Bumpalo->hitboxBadnik, true);

            switch (side) {
                default: break;
                case C_TOP:
                    if (entity->velocity.y > 0)
                        entity->onGround |= true;
                    break;
                case C_LEFT:
                    if (velX > 0)
                        entity->flag |= 1;
                    break;
                case C_RIGHT:
                    if (velX < 0)
                        entity->flag |= 1;
                    break;
            }
        }
        platform->position.x = platform->centerPos.x;
        platform->position.y = platform->centerPos.y;
    }
}

void Bumpalo_HandleObjectCollisions(void)
{
    RSDK_THIS(Bumpalo);

    foreach_all(Platform, platform) { Bumpalo_HandlePlatformCollisions(platform); }

    foreach_all(Spikes, spikes)
    {
        int velX = entity->velocity.x;
        int side = RSDK.CheckObjectCollisionBox(platform, &platform->hitbox, entity, &Bumpalo->hitboxBadnik, true);

        switch (side) {
            default: break;
            case C_TOP:
                if (entity->velocity.y > 0)
                    entity->onGround |= true;
                break;
            case C_LEFT:
                if (velX > 0)
                    entity->flag |= 1;
                break;
            case C_RIGHT:
                if (velX < 0)
                    entity->flag |= 1;
                break;
        }
    }

    foreach_all(BreakableWall, wall)
    {
        int velX = entity->velocity.x;
        int side = RSDK.CheckObjectCollisionBox(wall, &wall->hitbox, entity, &Bumpalo->hitboxBadnik, true);

        switch (side) {
            default: break;
            case C_TOP:
                if (entity->velocity.y > 0)
                    entity->onGround |= true;
                break;
            case C_LEFT:
                if (velX > 0)
                    entity->flag |= 1;
                break;
            case C_RIGHT:
                if (velX < 0)
                    entity->flag |= 1;
                break;
        }
    }

    if (entity->direction)
        entity->flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x140000, 0, true);
    else
        entity->flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x140000, 0, true);
}

void Bumpalo_State_Setup(void)
{
    RSDK_THIS(Bumpalo);

    entity->active = ACTIVE_NORMAL;
    if (entity->direction == FLIP_NONE)
        entity->velocity.x = -0x10000;
    else
        entity->velocity.x = 0x10000;
    entity->state = Bumpalo_State_Unknown1;
    Bumpalo_State_Unknown1();
}

void Bumpalo_State_Unknown1(void)
{
    RSDK_THIS(Bumpalo);

    entity->position.x += entity->velocity.x;
    entity->onGround = false;
    entity->flag     = false;
    Bumpalo_HandleObjectCollisions();

    entity->onGround |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8);
    if (!entity->onGround) {
        if (entity->ignoreCliffs) {
            entity->state = Bumpalo_State_Unknown6;
        }
        else {
            if (entity->animator1.animationID == 3)
                entity->velocity.x >>= 1;
            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 0, &entity->animator1, true, 0);
            entity->timer   = 0;
            bool32 onGround = entity->direction ? RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x40000, 0xF0000, 8)
                                                : RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x40000, 0xF0000, 8);
            entity->onGround = onGround;
            if (!entity->onGround) {
                entity->state = Bumpalo_State_Unknown6;
            }
            else {
                entity->position.x -= entity->velocity.x;
                entity->state = Bumpalo_State_Unknown2;
            }
        }
    }
    else if (entity->animator1.animationID == 3) {
        if (entity->flag) {
            RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &entity->animator1, true, 0);
            entity->state      = Bumpalo_State_Unknown5;
            entity->velocity.y = -0x20000;
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = 0x10000;
            else
                entity->velocity.x = -0x10000;
        }
    }
    else if (entity->flag) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 0, &entity->animator1, true, 0);
        entity->timer = 0;
        entity->state = Bumpalo_State_Unknown2;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Bumpalo->hitboxClack)) {
                entity->timer = 92;
                RSDK.PlaySfx(Bumpalo->sfxClack, false, 255);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 2, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 6, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 7, &entity->animator3, true, 0);
                entity->state = Bumpalo_State_Unknown3;
            }
        }
    }
    RSDK.ProcessAnimation(&entity->animator1);
    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOnScreen();
}

void Bumpalo_State_Unknown2(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&entity->animator1);
    if (++entity->timer == 48) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 5, &entity->animator1, true, 0);
        entity->state = Bumpalo_State_Unknown4;
    }
    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOnScreen();
}

void Bumpalo_State_Unknown3(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator3);

    entity->timer--;
    if (entity->timer == 83) {
        RSDK.PlaySfx(Bumpalo->sfxHuff, false, 255);
    }
    else if (entity->timer <= 0) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 3, &entity->animator1, true, 0);
        entity->state = Bumpalo_State_Unknown1;
        entity->velocity.x *= 2;
    }
    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOnScreen();
}

void Bumpalo_State_Unknown4(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &entity->animator1, true, 0);
        entity->state = Bumpalo_State_Unknown1;
        entity->direction ^= 1;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x10000;
        else
            entity->velocity.x = 0x10000;
        Bumpalo_State_Unknown1();
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOnScreen();
    }
}

void Bumpalo_State_Unknown5(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.x;
    entity->velocity.y += 0x3800;
    entity->onGround = false;
    entity->flag     = false;
    Bumpalo_HandleObjectCollisions();
    entity->onGround |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, true);

    if (entity->velocity.y >= 0 && entity->onGround) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
    }
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &entity->animator1, true, 0);
        if (entity->onGround) {
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = -0x10000;
            else
                entity->velocity.x = 0x10000;
            if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
                entity->state = Bumpalo_State_Unknown1;
                Bumpalo_State_Unknown1();
            }
            else {
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 5, &entity->animator1, true, 0);
                entity->state = Bumpalo_State_Unknown4;
                Bumpalo_State_Unknown4();
            }
        }
        else {
            entity->state = Bumpalo_State_Unknown6;
            Bumpalo_State_Unknown6();
        }
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOnScreen();
    }
}

void Bumpalo_State_Unknown6(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->onGround = false;
    entity->flag     = false;
    Bumpalo_HandleObjectCollisions();
    entity->onGround |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, true);
    if (entity->onGround == true) {
        entity->velocity.y = 0;
        RSDK.PlaySfx(Player->sfx_Land, false, 255);
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &entity->animator1, true, 0);
        entity->state = Bumpalo_State_Unknown5;
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOnScreen();
    }
}

void Bumpalo_EditorDraw(void)
{
    RSDK_THIS(Bumpalo);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Bumpalo_EditorLoad(void) { Bumpalo->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Bumpalo.bin", SCOPE_STAGE); }

void Bumpalo_Serialize(void)
{
    RSDK_EDITABLE_VAR(Bumpalo, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Bumpalo, VAR_BOOL, ignoreCliffs);
}
