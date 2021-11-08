#include "SonicMania.h"

ObjectEggPrison *EggPrison;

void EggPrison_Update(void)
{
    RSDK_THIS(EggPrison);
    StateMachine_Run(entity->state);

    if (entity->type == EGGPRISON_FLYING) {
        RSDK.ProcessAnimation(&entity->animator2);
        EggPrison_HandleMovement();
        if (!entity->activated) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionBox(player, entity, &entity->hitbox1) == C_TOP) {
                    player->position.x += entity->velocity.x;
                    player->position.y += 0x10000;
                }

                if (entity->state == EggPrison_Unknown2) {
                    if (Player_CheckCollisionBox(player, entity, &entity->hitbox2) == C_BOTTOM) {
                        entity->velocity.x = 0;
                        entity->active     = ACTIVE_NORMAL;
                        entity->state      = EggPrison_Activated;
                        entity->buttonPos  = -0x80000;
                    }
                    else {
                        if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                            if (entity->buttonPos < 0)
                                entity->buttonPos += 0x10000;
                        }
                        else {
                            Hitbox *playerHitbox = Player_GetHitbox(player);
                            entity->buttonPos    = ((playerHitbox->top - 48) << 16) - entity->position.y + player->position.y;
                            if (entity->buttonPos >= -0x80000) {
                                if (entity->buttonPos > 0)
                                    entity->buttonPos = 0;
                                entity->buttonPos &= 0xFFFF0000;
                            }
                            else {
                                entity->buttonPos = -0x80000;
                                entity->buttonPos &= 0xFFFF0000;
                            }
                        }
                    }
                }
                else {
                    Player_CheckCollisionBox(player, entity, &entity->hitbox2);
                }
            }
        }
    }
    else if (!entity->activated) {
        foreach_active(Player, player)
        {
            Player_CheckCollisionBox(player, entity, &entity->hitbox1);
            if (entity->state == EggPrison_Unknown2) {
                if (Player_CheckCollisionBox(player, entity, &entity->hitbox2) == C_TOP) {
                    entity->buttonPos = 0x80000;
                    if (entity->type < EGGPRISON_DUD)
                        RSDK_sceneInfo->timeEnabled = false;
                    if (entity->type == EGGPRISON_ANIMALS) {
                        int32 anim = player->playerAnimator.animationID;
                        if (anim == ANI_WALK || (anim > ANI_AIRWALK && anim <= ANI_DASH))
                            player->storedAnim = player->playerAnimator.animationID;
                        else
                            player->storedAnim = ANI_WALK;
                        player->state      = Player_State_Air;
                        player->onGround   = false;
                        player->velocity.y = -0xA0000;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                        RSDK.PlaySfx(EggPrison->sfxSpring, 0, 255);
                    }
                    else {
                        entity->active = ACTIVE_NORMAL;
                        entity->state  = EggPrison_Activated;
                    }
                }
                else {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                        Hitbox *playerHitbox = Player_GetHitbox(player);
                        entity->buttonPos    = ((playerHitbox->bottom + 48) << 16) - entity->position.y + player->position.y;
                        if (entity->buttonPos <= 0x80000) {
                            if (entity->buttonPos < 0)
                                entity->buttonPos = 0;
                        }
                        else {
                            entity->buttonPos = 0x80000;
                        }
                        entity->buttonPos &= 0xFFFF0000;
                    }
                    else {
                        if (entity->buttonPos > 0)
                            entity->buttonPos -= 0x10000;
                    }
                }
            }
            else {
                Player_CheckCollisionBox(player, entity, &entity->hitbox2);
            }
        }
    }
}

void EggPrison_LateUpdate(void) {}

void EggPrison_StaticUpdate(void) {}

void EggPrison_Draw(void)
{
    RSDK_THIS(EggPrison);
    Vector2 drawPos;
    if (entity->type == EGGPRISON_FLYING) {
        entity->direction = FLIP_Y;
        drawPos.x         = entity->position.x;
        drawPos.y         = entity->position.y;
        drawPos.y += entity->buttonPos;
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);
        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->direction = FLIP_NONE;
    }
    else {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.y += entity->buttonPos;
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void EggPrison_Create(void *data)
{
    RSDK_THIS(EggPrison);
    if (globals->gameMode < MODE_TIMEATTACK || entity->type >= EGGPRISON_FLYING) {
        entity->drawFX = FX_FLIP;
        if (!RSDK_sceneInfo->inEditor) {
            if (data)
                entity->type = voidToInt(data);
            if (entity->type == EGGPRISON_FLYING) {
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 0, &entity->animator1, true, 1);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 1, &entity->animator4, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &entity->animator3, true, 1);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 3, &entity->animator2, true, 0);
                entity->hitbox2.left   = -16;
                entity->hitbox2.top    = 24;
                entity->hitbox2.right  = 16;
                entity->hitbox2.bottom = 38;

                entity->hitbox3.left   = -15;
                entity->hitbox3.top    = 24;
                entity->hitbox3.right  = 15;
                entity->hitbox3.bottom = 48;
                entity->velocity.x     = 0x10000;
                entity->velocity.y     = 0x4000;
            }
            else {
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 1, &entity->animator4, true, 0);
                RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &entity->animator3, true, 0);
                entity->hitbox2.left   = -16;
                entity->hitbox2.top    = -38;
                entity->hitbox2.right  = 16;
                entity->hitbox2.bottom = -24;

                entity->hitbox3.left  = -15;
                entity->hitbox3.top   = -47;
                entity->hitbox3.right = 15;
                entity->hitbox3.top   = -24;
            }
            entity->hitbox1.left   = -32;
            entity->hitbox1.top    = -24;
            entity->hitbox1.right  = 32;
            entity->hitbox1.bottom = 32;
            entity->state          = EggPrison_Unknown1;
            entity->active         = ACTIVE_BOUNDS;
            entity->updateRange.x  = 0x800000;
            entity->updateRange.y  = 0x800000;
            entity->visible        = true;
            entity->drawOrder      = Zone->drawOrderLow;
        }
    }
    else {
        destroyEntity(entity);
    }
}

void EggPrison_StageLoad(void)
{
    EggPrison->spriteIndex = RSDK.LoadSpriteAnimation("Global/EggPrison.bin", SCOPE_STAGE);
    EggPrison->sfxDestroy  = RSDK.GetSFX("Global/Destroy.wav");
    EggPrison->sfxSpring   = RSDK.GetSFX("Global/Spring.wav");
}

void EggPrison_HandleMovement(void)
{
    RSDK_THIS(EggPrison);

    bool32 flag = false;
    if (entity->field_70) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x300000, 0x900000, false)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x300000, 0x900000, false)) {
            entity->originY -= entity->velocity.y;
            flag = true;
        }
    }

    if (!flag) {
        if (entity->originY < (RSDK_screens->position.y + 64) << 16)
            entity->originY += entity->velocity.y;
    }

    if (entity->state != EggPrison_Unknown5) {
        if (entity->velocity.x > 0) {
            if (entity->position.x <= (RSDK_screens->position.x + RSDK_screens->width - 48) << 16) {
                if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x400000, 0, true))
                    entity->velocity.x = -entity->velocity.x;
            }
            else {
                entity->velocity.x = -entity->velocity.x;
            }
        }
        else if (entity->position.x < (RSDK_screens->position.x + 48) << 16) {
            entity->velocity.x = -entity->velocity.x;
        }
        else {
            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x400000, 0, true))
                entity->velocity.x = -entity->velocity.x;
        }
    }
    entity->position.x += entity->velocity.x;
    entity->position.y = (RSDK.Sin256(entity->angle) << 10) + entity->originY;
    entity->angle += 4;
    entity->angle &= 0xFF;
}

void EggPrison_Activated(void)
{
    RSDK_THIS(EggPrison);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
    entity->state = EggPrison_Unknown3;

    switch (entity->type) {
        default:
        case EGGPRISON_NORMAL:
        case EGGPRISON_FLYING:
        case EGGPRISON_DUD:
            for (int32 i = 0; i < 10; ++i) {
                EntityAnimals *animals =
                    (EntityAnimals *)RSDK.CreateEntity(Animals->objectID, intToVoid(Animals->animalTypes[i & 1] + 1),
                                                       (((RSDK.Rand(0, 48) & -4) - 24) << 16) + entity->position.x, entity->position.y + 0x40000);
                animals->delay     = 4 * i;
                animals->state     = Animals_State_BounceAround;
                animals->behaviour = 1;
                animals->direction = (i ^ (i >> 1)) & 1;
            }
            break;
        case EGGPRISON_RINGS: {
            int32 angle = 144;
            for (int32 r = 0; r < 6; ++r) {
                EntityRing *ring = (EntityRing *)RSDK.CreateEntity(
                    Ring->objectID, entity, (((RSDK.Rand(0, 48) & 0xFFFFFFFC) - 24) << 16) + entity->position.x, entity->position.y);
                ring->velocity.x              = RSDK.Cos256(angle) << 9;
                ring->velocity.y              = RSDK.Sin256(angle) << 9;
                ring->animator.animationSpeed = 512;
                ring->collisionPlane          = 0;
                ring->inkEffect               = INK_ALPHA;
                if (angle != 176)
                    angle += 8;
                else
                    angle = 208;
                ring->alpha     = 0x100;
                ring->state     = Ring_State_Bounce;
                ring->stateDraw = Ring_StateDraw_Normal;
            }
            break;
        }
        case EGGPRISON_TRAP: {
            EntityTechnosqueek *technosqueek = CREATE_ENTITY(Technosqueek, NULL, entity->position.x - 0x80000, entity->position.y);
            technosqueek->velocity.x         = -0x30000;
            technosqueek->velocity.y         = -0x40000;
            technosqueek->active             = ACTIVE_NORMAL;
            technosqueek->state              = Technosqueek_State_Unknown6;

            technosqueek             = CREATE_ENTITY(Technosqueek, NULL, entity->position.x + 0x80000, entity->position.y);
            technosqueek->velocity.x = 0x30000;
            technosqueek->velocity.y = -0x40000;
            technosqueek->active     = ACTIVE_NORMAL;
            technosqueek->direction  = 1;
            technosqueek->state      = Technosqueek_State_Unknown6;

            EntityBlaster *blaster = CREATE_ENTITY(Blaster, NULL, entity->position.x - 0x180000, entity->position.y);
            blaster->velocity.x    = -0x30000;
            blaster->velocity.y    = -0x30000;
            blaster->active        = ACTIVE_NORMAL;
            blaster->state         = Blaster_State_Unknown7;

            blaster             = CREATE_ENTITY(Blaster, NULL, entity->position.x + 0x180000, entity->position.y);
            blaster->velocity.x = 0x30000;
            blaster->velocity.y = -0x30000;
            blaster->active     = ACTIVE_NORMAL;
            blaster->direction  = 1;
            blaster->state      = Blaster_State_Unknown7;
            break;
        }
        case EGGPRISON_ANIMALS:
            for (int32 a = 0; a < 10; ++a) {
                EntityAnimals *animals = (EntityAnimals *)RSDK.CreateEntity(Animals->objectID, intToVoid(Animals->animalTypes[a & 1] + 1),
                                                                            (((RSDK.Rand(0, 48) & 0xFFFFFFFC) - 24) << 16) + entity->position.x,
                                                                            entity->position.y + 0x40000);
                animals->delay         = 4 * a;
                animals->state         = Animals_State_BounceAround;
                animals->behaviour     = 1;
                animals->direction     = (a ^ (a >> 1)) & 1;
            }
            break;
    }

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x160000, entity->position.y);
    RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &debris->animator, true, 2);
    debris->velocity.x    = -0x20000;
    debris->velocity.y    = -0x20000;
    debris->gravity       = 0x3800;
    debris->rotSpeed      = -4;
    debris->drawFX        = FX_ROTATE;
    debris->drawOrder     = Zone->drawOrderHigh;
    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;

    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
    RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &debris->animator, true, 3);
    debris->velocity.x    = 0;
    debris->velocity.y    = -0x30000;
    debris->gravity       = 0x3800;
    debris->rotSpeed      = 8;
    debris->drawFX        = FX_ROTATE;
    debris->drawOrder     = Zone->drawOrderHigh;
    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;

    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x160000, entity->position.y);
    RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &debris->animator, true, 4);
    debris->velocity.x    = 0x20000;
    debris->velocity.y    = -0x20000;
    debris->gravity       = 0x3800;
    debris->rotSpeed      = 4;
    debris->drawFX        = FX_ROTATE;
    debris->drawOrder     = Zone->drawOrderHigh;
    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;
}

void EggPrison_Unknown1(void)
{
    RSDK_THIS(EggPrison);
    entity->originY = entity->position.y;
    entity->state   = EggPrison_Unknown2;
}

void EggPrison_Unknown2(void)
{
    RSDK_THIS(EggPrison);
    for (int32 p = 0; p < Player->playerCount && entity->type < EGGPRISON_DUD; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (!player->sidekick) {
            if (abs(entity->position.x - player->position.x) < 0x1000000) {
                if (abs(entity->position.y - player->position.y) < 0x1000000 && entity->position.x - (Zone->screenBoundsR1[p] << 16) < 0x1000000) {
                    Zone->playerBoundActiveL[p] = true;
                    Zone->playerBoundActiveR[p] = true;
                    if (entity->type == EGGPRISON_NORMAL) {
                        Zone->screenBoundsL1[p] = (entity->position.x >> 0x10) - RSDK_screens[p].centerX;
                        Zone->screenBoundsR1[p] = (entity->position.x >> 0x10) + RSDK_screens[p].centerX;
                    }
                }
            }
        }
    }
}

void EggPrison_Unknown3(void)
{
    RSDK_THIS(EggPrison);
    if (!(entity->timer % 3)) {
        Entity *explosion    = RSDK.CreateEntity(Explosion->objectID, intToVoid(2 * (RSDK.Rand(0, 256) > 192) + 1),
                                              (RSDK.Rand(-24, 24) << 16) + entity->position.x, (RSDK.Rand(-24, 24) << 16) + entity->position.y);
        explosion->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(EggPrison->sfxDestroy, 0, 255);
    }
    if (++entity->timer == 20) {
        entity->timer = 0;
        if (entity->type < EGGPRISON_DUD) {
            entity->state = EggPrison_Unknown4;
            Music_FadeOut(0.025);
        }
        else {
            entity->state = StateMachine_None;
        }
    }
}

void EggPrison_Unknown4(void)
{
    RSDK_THIS(EggPrison);
    if (++entity->timer == 60) {
        entity->timer                 = 0;
        entity->state                 = StateMachine_None;
        Zone->forcePlayerOnScreenFlag = false;
        Music_PlayTrack(TRACK_ACTCLEAR);
        RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->objectID, 0);
    }
}

void EggPrison_Unknown5(void)
{
    RSDK_THIS(EggPrison);
    if (entity->velocity.x > -0x30000)
        entity->velocity.x -= 0x1000;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void EggPrison_EditorDraw(void)
{
    RSDK_THIS(EggPrison);
    Vector2 drawPos;

    if (entity->type == EGGPRISON_FLYING) {
        RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 0, &entity->animator1, true, 1);
        RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 1, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &entity->animator3, true, 1);
        RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 3, &entity->animator2, true, 0);

        entity->direction = FLIP_Y;
        drawPos.x         = entity->position.x;
        drawPos.y         = entity->position.y;
        drawPos.y += entity->buttonPos;
        RSDK.DrawSprite(&entity->animator4, &drawPos, 0);
        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->direction = FLIP_NONE;
    }
    else {
        RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 1, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(EggPrison->spriteIndex, 2, &entity->animator3, true, 0);

        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.y += entity->buttonPos;
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void EggPrison_EditorLoad(void)
{
    EggPrison->spriteIndex = RSDK.LoadSpriteAnimation("Global/EggPrison.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(EggPrison, type);
    RSDK_ENUM_VAR(EGGPRISON_NORMAL);
    RSDK_ENUM_VAR(EGGPRISON_FLYING);
    RSDK_ENUM_VAR(EGGPRISON_DUD);
    RSDK_ENUM_VAR(EGGPRISON_RINGS);
    RSDK_ENUM_VAR(EGGPRISON_TRAP);
    RSDK_ENUM_VAR(EGGPRISON_ANIMALS);
}
#endif

void EggPrison_Serialize(void) { RSDK_EDITABLE_VAR(EggPrison, VAR_ENUM, type); }
