#include "SonicMania.h"

ObjectFlasherMKII *FlasherMKII;

void FlasherMKII_Update(void)
{
    RSDK_THIS(FlasherMKII);
    RSDK.ProcessAnimation(&entity->animator);
    StateMachine_Run(entity->state);
}

void FlasherMKII_LateUpdate(void) {}

void FlasherMKII_StaticUpdate(void) {}

void FlasherMKII_Draw(void)
{
    RSDK_THIS(FlasherMKII);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void FlasherMKII_Create(void *data)
{
    RSDK_THIS(FlasherMKII);
    entity->drawFX |= FX_FLIP;

    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->startPos      = entity->position;
        switch (entity->orientation) {
            case 0:
                entity->type      = 0;
                entity->direction = FLIP_NONE;
                break;
            case 1:
                entity->type      = 0;
                entity->direction = FLIP_Y;
                break;
            case 2:
                entity->direction = FLIP_NONE;
                entity->type      = 4;
                break;
            case 3:
                entity->direction = FLIP_X;
                entity->type      = 4;
                break;
            default: break;
        }
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type, &entity->animator, true, 0);
        entity->state = FlasherMKII_State_Unknown1;
    }
}

void FlasherMKII_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        FlasherMKII->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/FlasherMKII.bin", SCOPE_STAGE);

    DEBUGMODE_ADD_OBJ(FlasherMKII);

    FlasherMKII->sfxFlop = RSDK.GetSFX("TMZ1/FlasherFlop.wav");
    FlasherMKII->sfxZap  = RSDK.GetSFX("TMZ1/FlasherZap.wav");
}

void FlasherMKII_DebugSpawn(void)
{
    RSDK_THIS(FlasherMKII);
    CREATE_ENTITY(FlasherMKII, NULL, entity->position.x, entity->position.y);
}

void FlasherMKII_DebugDraw(void)
{
    RSDK_THIS(FlasherMKII);
    RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void FlasherMKII_CheckOnScreen(void)
{
    RSDK_THIS(FlasherMKII);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange))
        FlasherMKII_Create(NULL);
}

void FlasherMKII_HandlePlayerCollisions(void)
{
    RSDK_THIS(FlasherMKII);

    foreach_active(Player, player)
    {
        Hitbox *hitbox = RSDK.GetHitbox(&entity->animator, 0);
        if (Player_CheckBadnikTouch(player, entity, hitbox)) {
            if ((entity->animator.animationID & 3) == 3) {
                if (player->shield == SHIELD_LIGHTNING || player->invincibleTimer || player->blinkTimer) {
                    if (Player_CheckBadnikBreak(entity, player, true))
                        RSDK.StopSFX(FlasherMKII->sfxZap);
                }
                else {
                    Player_CheckHit(player, entity);
                }
            }
            else {
                Player_CheckBadnikBreak(entity, player, true);
            }
        }
    }
}

void FlasherMKII_HandleHarmPlayerCollisions(void)
{
    RSDK_THIS(FlasherMKII);

    foreach_active(Player, player)
    {
        if (player->shield == SHIELD_LIGHTNING || player->invincibleTimer || player->blinkTimer) {
            Hitbox *hitbox = RSDK.GetHitbox(&entity->animator, 0);
            if (Player_CheckBadnikTouch(player, entity, hitbox) && Player_CheckBadnikBreak(entity, player, true))
                RSDK.StopSFX(FlasherMKII->sfxZap);
        }
        else {
            Hitbox *hitbox = RSDK.GetHitbox(&entity->animator, 1);
            if (Player_CheckCollisionTouch(player, entity, hitbox))
                Player_CheckHit(player, entity);
        }
    }
}

void FlasherMKII_State_Unknown1(void)
{
    RSDK_THIS(FlasherMKII);

    entity->active = ACTIVE_NORMAL;
    if (entity->timer > 0)
        entity->timer--;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        int rx = (entity->position.x - player->position.x) >> 16;
        int ry = (entity->position.y - player->position.y) >> 16;
        if (!entity->timer && rx * rx + ry * ry < 0x1000) {
            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 2, &entity->animator, false, 0);
            RSDK.PlaySfx(FlasherMKII->sfxZap, false, 255);
            entity->state = FlasherMKII_State_Unknown3;
        }
        else {
            switch (entity->orientation) {
                case 0:
                    if (player->position.x >= entity->position.x) {
                        if ((uint32)(rx + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x600000, 0xC0000, 2)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 1, &entity->animator, false, 0);
                            entity->direction  = FLIP_X;
                            entity->velocity.x = 0x220000;
                            entity->state      = FlasherMKII_State_Unknown2;
                            break;
                        }
                    }
                    else if ((rx - 65) <= 0xBE && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x600000, 0xC0000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 1, &entity->animator, false, 0);
                        entity->direction  = FLIP_NONE;
                        entity->velocity.x = -0x220000;
                        entity->state      = FlasherMKII_State_Unknown2;
                        break;
                    }
                    break;
                case 1:
                    if (player->position.x >= entity->position.x) {
                        if ((uint32)(rx + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_ROOF, 0, 0x600000, -0xC0000, 2)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 1, &entity->animator, false, 0);
                            entity->velocity.x = 0x220000;
                            entity->direction  = FLIP_XY;
                            entity->state      = FlasherMKII_State_Unknown2;
                            break;
                        }
                    }
                    else if ((rx - 65) <= 0xBE && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_ROOF, 0, -0x600000, -0xC0000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 1, &entity->animator, false, 0);
                        entity->direction  = FLIP_Y;
                        entity->velocity.x = -0x220000;
                        entity->state      = FlasherMKII_State_Unknown2;
                        break;
                    }
                    break;
                case 2:
                    if (player->position.y >= entity->position.y) {
                        if ((uint32)(ry + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_RWALL, 0, -0xC0000, 0x600000, 2u)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 1, &entity->animator, false, 0);
                            entity->direction  = FLIP_Y;
                            entity->velocity.y = 0x220000;
                            entity->state      = FlasherMKII_State_Unknown2;
                            break;
                        }
                    }
                    else if ((ry - 65) <= 0xBE && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_RWALL, 0, -0xC0000, -0x600000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 1, &entity->animator, false, 0);
                        entity->direction  = FLIP_NONE;
                        entity->velocity.y = -0x220000;
                        entity->state      = FlasherMKII_State_Unknown2;
                        break;
                    }
                    break;
                case 3:
                    if (player->position.y >= entity->position.y) {
                        if ((uint32)(ry + 0xFF) <= 0xBE && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_LWALL, 0, 0xC0000, 0x600000, 2u)) {
                            RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 1, &entity->animator, false, 0);
                            entity->velocity.y = 0x220000;
                            entity->direction  = FLIP_XY;
                            entity->state      = FlasherMKII_State_Unknown2;
                            break;
                        }
                    }
                    else if ((uint32)(ry - 65) <= 0xBE && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_LWALL, 0, 0xC0000, -0x600000, 2)) {
                        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 1, &entity->animator, false, 0);
                        entity->direction  = FLIP_X;
                        entity->velocity.y = -0x220000;
                        entity->state      = FlasherMKII_State_Unknown2;
                        break;
                    }
                    break;
                default: break;
            }
        }
    }

    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

void FlasherMKII_State_Unknown2(void)
{
    RSDK_THIS(FlasherMKII);

    if ((entity->animator.frameID == 5 || entity->animator.frameID == 12) && entity->animator.animationTimer == 1) {
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        if (entity->activeScreens == 1)
            RSDK.PlaySfx(FlasherMKII->sfxFlop, false, 255);
    }
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type, &entity->animator, false, 0);
        entity->state = FlasherMKII_State_Unknown1;
    }
    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

void FlasherMKII_State_Unknown3(void)
{
    RSDK_THIS(FlasherMKII);

    if (++entity->timer == 60) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 3, &entity->animator, false, 0);
        entity->state = FlasherMKII_State_Unknown4;
    }
    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

void FlasherMKII_State_Unknown4(void)
{
    RSDK_THIS(FlasherMKII);

    if (++entity->timer == 120) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type + 2, &entity->animator, false, 0);
        entity->state = FlasherMKII_State_Unknown5;
    }
    FlasherMKII_HandleHarmPlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

void FlasherMKII_State_Unknown5(void)
{
    RSDK_THIS(FlasherMKII);

    if (++entity->timer == 30) {
        RSDK.SetSpriteAnimation(FlasherMKII->aniFrames, entity->type, &entity->animator, false, 0);
        entity->state = FlasherMKII_State_Unknown1;
    }
    FlasherMKII_HandlePlayerCollisions();
    FlasherMKII_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void FlasherMKII_EditorDraw(void) {}

void FlasherMKII_EditorLoad(void) {}
#endif

void FlasherMKII_Serialize(void) { RSDK_EDITABLE_VAR(FlasherMKII, VAR_UINT8, orientation); }
