#include "SonicMania.h"

ObjectTornado *Tornado;

void Tornado_Update(void)
{
    RSDK_THIS(Tornado);
    entity->prevPosY = entity->position.y;
    StateMachine_Run(entity->state);
    entity->animatorTornado.frameID = entity->turnAngle >> 4;
    RSDK.ProcessAnimation(&entity->animatorPropeller);
    RSDK.ProcessAnimation(&entity->animatorPilot);
    RSDK.ProcessAnimation(&entity->animatorFlame);
    RSDK.ProcessAnimation(&entity->animatorKnux);
}

void Tornado_LateUpdate(void) {}

void Tornado_StaticUpdate(void) {}

void Tornado_Draw(void)
{
    RSDK_THIS(Tornado);
    Vector2 drawPos;

    RSDK.DrawSprite(&entity->animatorPilot, NULL, false);
    RSDK.DrawSprite(&entity->animatorPropeller, NULL, false);
    RSDK.DrawSprite(&entity->animatorTornado, NULL, false);

    if (entity->showFlame) {
        drawPos = entity->position;
        drawPos.y += Tornado->flameOffsets[entity->animatorTornado.frameID];
        RSDK.DrawSprite(&entity->animatorFlame, &drawPos, false);
    }

    if (!MSZSetup->flag) {
        drawPos = entity->position;
        drawPos.x += entity->knuxOffset.x;
        drawPos.y += entity->knuxOffset.y;
        RSDK.DrawSprite(&entity->animatorKnux, &drawPos, false);
    }
}

void Tornado_Create(void *data)
{
    RSDK_THIS(Tornado);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        entity->drawFX        = FX_FLIP;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->movePos       = entity->position;
        entity->turnAngle     = 48;
        entity->isStood       = true;
        entity->offsetX       = 0x80000;
        entity->active        = ACTIVE_BOUNDS;
        entity->state         = Tornado_State_Setup;
        entity->knuxOffset.x  = -0x140000;
        entity->knuxOffset.y  = -0x160000;
        RSDK.SetSpriteAnimation(Tornado->aniFrames, 0, &entity->animatorTornado, true, 0);
        RSDK.SetSpriteAnimation(Tornado->aniFrames, 1, &entity->animatorPropeller, true, 0);
        RSDK.SetSpriteAnimation(Tornado->aniFrames, 2, &entity->animatorFlame, true, 0);
        if ((globals->playerID & 0xFF) == ID_TAILS
#if RETRO_USE_PLUS
            || (globals->playerID & 0xFF) == ID_MIGHTY || (globals->playerID & 0xFF) == ID_RAY
#endif
        ) {
            RSDK.SetSpriteAnimation(Tornado->aniFrames, 3, &entity->animatorPilot, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(Tornado->aniFrames, 6, &entity->animatorPilot, true, 0);
        }
        if ((globals->playerID & 0xFF) != ID_KNUCKLES && !StarPost->postIDs[0])
            RSDK.SetSpriteAnimation(Tornado->knuxFrames, 6, &entity->animatorKnux, false, 0);
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (player2->objectID == Player->objectID)
            player2->state = MSZSetup_Player_State_Pilot;
    }
}

void Tornado_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ") || RSDK.CheckStageFolder("MSZCutscene")) {
        Tornado->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Tornado.bin", SCOPE_STAGE);
        if ((globals->playerID & 0xFF) != ID_KNUCKLES)
            Tornado->knuxFrames = RSDK.LoadSpriteAnimation("Players/KnuxCutsceneAIZ.bin", SCOPE_STAGE);
    }
    Tornado->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    Tornado->sfxImpact    = RSDK.GetSFX("Stage/Impact5.wav");
}

void Tornado_State_Setup(void)
{
    RSDK_THIS(Tornado);
    entity->active = ACTIVE_NORMAL;
    if (RSDK.CheckStageFolder("MSZ"))
        entity->state = Tornado_State_SetupPlayers;
    else
        entity->state = Tornado_HandlePlayerCollisions;
    StateMachine_Run(entity->state);
}

void Tornado_State_SetupPlayers(void)
{
    RSDK_THIS(Tornado);
    Tornado_HandlePlayerCollisions();
    if (StarPost->postIDs[0]) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animatorKnux, false, 0);
        entity->state = Tornado_Unknown6;
    }
    else {
        foreach_all(Player, player)
        {
            player->stateInput = StateMachine_None;
            player->up         = false;
            player->down       = false;
            player->left       = false;
            player->right      = false;
            player->jumpPress  = false;
            player->jumpHold   = false;
            player->groundVel  = 0;
            player->velocity.x = 0;
            player->velocity.y = 0;
        }
        entity->state = Tornado_Unknown3;
    }
}

void Tornado_Unknown3(void)
{
    RSDK_THIS(Tornado);
    Tornado_HandlePlayerCollisions();
    if (++entity->timer == 180) {
        entity->timer = 0;
        RSDK.PlaySfx(Tornado->sfxImpact, false, 255);
        entity->knuxVel.x = -0x20000;
        entity->knuxVel.y = -0x40000;
        RSDK.SetSpriteAnimation(Tornado->knuxFrames, 4, &entity->animatorKnux, false, 0);
        entity->state = Tornado_Unknown4;
        foreach_active(Player, player) { player->stateInput = Player_ProcessP1Input; }
    }
}

void Tornado_Unknown4(void)
{
    RSDK_THIS(Tornado);
    Tornado_Unknown6();
    entity->knuxVel.y += 0x3800;
    entity->knuxOffset.x += entity->knuxVel.x;
    entity->knuxOffset.y += entity->knuxVel.y;
    if (++entity->timer == 120) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animatorKnux, false, 0);
        entity->state = Tornado_Unknown6;
    }
}

void Tornado_HandlePlayerCollisions(void)
{
    RSDK_THIS(Tornado);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Hitbox *hitbox        = RSDK.GetHitbox(&entity->animatorTornado, 0);
    entity->prevPosY      = entity->position.y;
    entity->turnAngle     = 32;
    player1->drawOrder    = entity->drawOrder + 1;
    entity->prevPosY &= 0xFFFF0000;
    entity->moveVelocityY = (entity->position.y & 0xFFFF0000) - entity->prevPosY;
    entity->position.y    = entity->prevPosY;
    entity->isStood       = false;
    int32 velY            = player1->velocity.y;
    if (Player_CheckCollisionPlatform(player1, entity, hitbox)) {
        player1->position.x += TornadoPath->moveVel.x;
        player1->position.y += entity->moveVelocityY;
        player1->flailing = false;
        entity->isStood   = true;
        if (velY > 0x10000) {
            entity->collideTimer = 0;
            entity->gravityForce = 0x20000;
            entity->collideMode  = 3;
        }
    }

    EntityCamera *camera = TornadoPath->cameraPtr;
    if (camera) {
        int32 screenX = camera->position.x - (RSDK_screens->centerX << 16) + 0xC0000;
        if (player1->position.x < screenX)
            player1->position.x = screenX;

        int32 screenY = ((RSDK_screens->centerX - 12) << 16) + camera->position.x;
        if (player1->position.x > screenY)
            player1->position.x = screenY;

        if (player1->objectID == Player->objectID) {
            int32 deathBounds = (camera->position.y + ((RSDK_screens[camera->screenID].centerY + 16) << 16));
            if (player1->position.y > deathBounds)
                player1->hurtFlag = 1;
        }
    }
}

void Tornado_Unknown6(void)
{
    RSDK_THIS(Tornado);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Hitbox *hitbox        = RSDK.GetHitbox(&entity->animatorTornado, TornadoPath->hitboxID);
    entity->prevPosY      = entity->position.y;
    if (entity->turnAngle < 32)
        player1->drawOrder = entity->drawOrder;
    else
        player1->drawOrder = entity->drawOrder + 1;

    if (player1->state == Player_State_Roll) {
        player1->groundVel  = clampVal(player1->groundVel, -entity->offsetX, entity->offsetX);
        player1->velocity.x = player1->groundVel;
    }

    switch (entity->collideMode) {
        case 0:
            if (player1->velocity.y < 0 && player1->jumpPress) {
                if (abs(TornadoPath->moveVel.x) > 0x40000) {
                    if (abs(player1->velocity.x) < abs(TornadoPath->moveVel.x) && !TornadoPath->hitboxID)
                        player1->velocity.x = abs(TornadoPath->moveVel.x);
                }

                ++entity->collideMode;
                entity->gravityForce = 0x20000;
                entity->storeY       = entity->position.y;
            }
            else {
                if (entity->velocity.y > 0) {
                    if (entity->turnAngle < 64)
                        entity->turnAngle += 4;
                }
                else if (entity->velocity.y >= 0) {
                    if (entity->turnAngle > 32)
                        entity->turnAngle -= 4;
                    else if (entity->turnAngle < 32)
                        entity->turnAngle += 4;
                }
                else if (entity->turnAngle > 0)
                    entity->turnAngle -= 4;
            }
            break;
        case 1:
            entity->gravityForce -= 0x2000;
            entity->position.y += entity->gravityForce;
            if (entity->gravityForce < 0)
                entity->collideMode = 2;
            break;
        case 2:
            entity->gravityForce -= 0x1000;
            entity->position.y += entity->gravityForce;
            if (entity->position.y <= entity->storeY) {
                entity->position.y   = entity->storeY;
                entity->collideMode  = 0;
                entity->gravityForce = 0;
            }
            break;
        case 3:
            entity->gravityForce -= 0x2000;
            entity->position.y += entity->gravityForce;
            if (++entity->collideTimer == 24) {
                entity->collideMode  = 0;
                entity->gravityForce = 0;
            }
            break;
        default: break;
    }

    if (entity->isStood) {
        entity->position.x += TornadoPath->moveVel.x;
        entity->position.y += TornadoPath->moveVel.y;
        if (player1->stateInput) {
            entity->velocity.y = 0;
            if (player1->up) {
                entity->velocity.y = -0x10000;
            }
            else if (player1->down) {
                entity->velocity.y = 0x10000;
            }
        }
    }
    else if (TornadoPath->hitboxID == 1) {
        entity->position.x += TornadoPath->moveVel.x;
        entity->position.y += TornadoPath->moveVel.y;
    }
    else {
        entity->velocity.y = 0;
    }

    if (entity->position.y < (RSDK_screens->position.y + 72) << 16)
        entity->position.y = (RSDK_screens->position.y + 72) << 16;
    if (entity->position.y > (RSDK_screens->height + RSDK_screens->position.y - 32) << 16)
        entity->position.y = (RSDK_screens->height + RSDK_screens->position.y - 32) << 16;

    int32 storeX    = entity->position.x;
    int32 storeY    = entity->position.y + entity->velocity.y;
    entity->isStood = false;
    entity->prevPosY &= 0xFFFF0000;
    entity->moveVelocityY = (storeY & 0xFFFF0000) - entity->prevPosY;
    entity->position.y    = entity->prevPosY;
    int32 velY            = player1->velocity.y;
    int32 posX            = entity->position.x;

    if (Player_CheckCollisionPlatform(player1, entity, hitbox)) {
        player1->position.x += TornadoPath->moveVel.x;
        player1->position.y += entity->moveVelocityY;
        player1->flailing = false;
        entity->isStood   = true;
        if (velY > 0x10000) {
            entity->collideTimer = 0;
            entity->gravityForce = 0x20000;
            entity->collideMode  = 3;
        }
    }
    else if (TornadoPath->hitboxID == 1) {
        player1->position.x += TornadoPath->moveVel.x;
    }

    int32 offsetX = 0;
    entity->position.x += 0x1E0000;
    if (abs(posX + 0x1E0000 - player1->position.x) > 0x100000) {
        offsetX = entity->offsetX;
        if (player1->position.x <= posX + 0x1E0000) {
            offsetX   = -offsetX;
            int32 pos = player1->position.x - (posX + 0x1E0000) + 0x100000;
            if (pos > offsetX)
                offsetX = pos;
        }
        else {
            int32 pos = player1->position.x - (posX + 0x1E0000) - 0x100000;
            if (pos < offsetX)
                offsetX = pos;
        }
    }
    entity->position.x = storeX;
    entity->position.x += offsetX;
    entity->position.y = storeY;

    EntityCamera *camera = TornadoPath->cameraPtr;
    if (camera) {
        int32 screenX = camera->position.x - (RSDK_screens->centerX << 16) + 0xC0000;
        if (player1->position.x < screenX)
            player1->position.x = screenX;

        int32 screenY = ((RSDK_screens->centerX - 12) << 16) + camera->position.x;
        if (player1->position.x > screenY)
            player1->position.x = screenY;

        if (player1->objectID == Player->objectID) {
            if (player1->position.y > (camera->position.y + ((RSDK_screens[camera->screenID].centerY + 16) << 16)))
                player1->hurtFlag = 1;
        }
    }
    if (player1->state == Player_State_TailsFlight) {
        if (player1->position.y < ((RSDK_screens->position.y + 20) << 16) && player1->velocity.y < 0) {
            player1->velocity.y   = 0;
            player1->abilitySpeed = 0x8000;
        }
    }
}

void Tornado_Unknown7(void)
{
    RSDK_THIS(Tornado);
    entity->position.x += TornadoPath->moveVel.x;
    entity->position.y += TornadoPath->moveVel.y;
    if (!(Zone->timer % 3)) {
        if (entity->onGround) {
            RSDK.PlaySfx(Tornado->sfxExplosion, false, 255);
        }
        if (Zone->timer & 4) {
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), (RSDK.Rand(-32, 32) << 16) + entity->position.x,
                              (RSDK.Rand(-16, 16) << 16) + entity->position.y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }
}

void Tornado_Unknown8(void)
{
    RSDK_THIS(Tornado);
    entity->position.x += 0x30000;
    entity->position.y -= 0x30000;
    entity->active = ACTIVE_BOUNDS;
}

void Tornado_Unknown9(void)
{
    RSDK_THIS(Tornado);
    entity->velocity.x -= 0x1800;
    entity->position.x += entity->velocity.x;
    entity->position.y -= 0x8000;
    entity->active = ACTIVE_BOUNDS;
}

void Tornado_EditorDraw(void)
{
    RSDK_THIS(Tornado);
    RSDK.SetSpriteAnimation(Tornado->aniFrames, 0, &entity->animatorTornado, true, 0);
    RSDK.SetSpriteAnimation(Tornado->aniFrames, 1, &entity->animatorPropeller, true, 0);
    RSDK.SetSpriteAnimation(Tornado->aniFrames, 2, &entity->animatorFlame, true, 0);

    RSDK.DrawSprite(&entity->animatorPilot, NULL, false);
    RSDK.DrawSprite(&entity->animatorPropeller, NULL, false);
    RSDK.DrawSprite(&entity->animatorTornado, NULL, false);
}

void Tornado_EditorLoad(void) { Tornado->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Tornado.bin", SCOPE_STAGE); }

void Tornado_Serialize(void) {}
