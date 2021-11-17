#include "SonicMania.h"

ObjectTubinaut *Tubinaut;

void Tubinaut_Update(void)
{
    RSDK_THIS(Tubinaut);
    StateMachine_Run(entity->state);
}

void Tubinaut_LateUpdate(void) {}

void Tubinaut_StaticUpdate(void) {}

void Tubinaut_Draw(void)
{
    RSDK_THIS(Tubinaut);

    if (entity->timer2 < 256 || entity->orbCount <= 1) {
        for (int i = 0; i < Tubinaut_OrbCount; ++i) {
            if (entity->field_C4[i]) {
                RSDK.DrawSprite(&entity->animators[i], &entity->orbPositions[i], false);
            }
        }
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }

    entity->drawFX = FX_FLIP;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->drawFX = FX_NONE;
}

void Tubinaut_Create(void *data)
{
    RSDK_THIS(Tubinaut);

    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    if (data) {
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, voidToInt(data), &entity->animator1, true, 0);
        entity->state = Tubinaut_State2_Unknown;
    }
    else {
        entity->startPos      = entity->position;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 1, entity->animators, true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 2, &entity->animators[1], true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 3, &entity->animators[2], true, 0);
        entity->orbAngles[0] = 0;
        entity->orbAngles[1] = 0x500;
        entity->orbAngles[2] = 0xA00;
        entity->field_C4[0]  = true;
        entity->field_C4[1]  = true;
        entity->field_C4[2]  = true;
        entity->activeOrbs   = 1 | 2 | 4;
        entity->orbCount     = 3;
        entity->timer2       = 64;
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 7, &entity->animator2, true, 0);
        entity->state    = Tubinaut_State_Setup;
        entity->stateOrb = Tubinaut_StateOrb_Unknown1;
    }
}

void Tubinaut_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Tubinaut.bin", SCOPE_STAGE);
    else
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Tubinaut.bin", SCOPE_STAGE);
    Tubinaut->hitbox1.left   = -10;
    Tubinaut->hitbox1.top    = -10;
    Tubinaut->hitbox1.right  = 10;
    Tubinaut->hitbox1.bottom = 10;
    Tubinaut->hitbox2.left   = -7;
    Tubinaut->hitbox2.top    = -7;
    Tubinaut->hitbox2.right  = 7;
    Tubinaut->hitbox2.bottom = 7;
    Tubinaut->hitbox3.left   = -20;
    Tubinaut->hitbox3.top    = -20;
    Tubinaut->hitbox3.right  = 20;
    Tubinaut->hitbox3.bottom = 20;
    DEBUGMODE_ADD_OBJ(Tubinaut);
    Tubinaut->sfxPowerdown = RSDK.GetSFX("Stage/PowerDown.wav");
    Tubinaut->sfxRepel     = RSDK.GetSFX("Stage/Repel.wav");
}

void Tubinaut_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityTubinaut *tubinaut = CREATE_ENTITY(Tubinaut, NULL, entity->position.x, entity->position.y);
    tubinaut->distanceRemain = 256;
    tubinaut->distance       = 256;
}

void Tubinaut_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Tubinaut_CheckPlayerCollisions(void)
{
    RSDK_THIS(Tubinaut);

    int storeX   = entity->position.x;
    int storeY   = entity->position.y;
    int playerID = 0;

    foreach_active(Player, player)
    {
        if (entity->playerTimers[playerID]) {
            --entity->playerTimers[playerID];
        }
        else if (entity->stateOrb == Tubinaut_StateOrb_Unknown3 && entity->orbCount > 1) {
            if (Player_CheckCollisionTouch(player, entity, &Tubinaut->hitbox3)
                && (entity->orbCount == 2 || entity->orbCount == 3 && !Player_CheckHit(player, entity))) {
                Tubinaut_HandleRepel(player, playerID);
            }
        }
        else {
            bool32 flag = false;
            for (int i = 0; i < Tubinaut_OrbCount; ++i) {
                if (entity->field_C4[i]) {
                    entity->position.x = entity->orbPositions[i].x;
                    entity->position.y = entity->orbPositions[i].y;

                    if (Player_CheckCollisionTouch(player, entity, &Tubinaut->hitbox2)) {
                        Tubinaut_Unknown5(player, i);
#if RETRO_USE_PLUS
                        if (player->state != Player_State_MightyHammerDrop) {
#endif
                            flag                           = true;
                            entity->playerTimers[playerID] = 15;
#if RETRO_USE_PLUS
                        }
#endif

                        if (entity->orbCount) {
                            if (entity->orbCount == 1) {
                                entity->animator1.frameID = 2;
                            }
                            else if (entity->orbCount == 2) {
                                switch (entity->activeOrbs) {
                                    case 3: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 4, &entity->animator2, true, 0); break;
                                    case 5: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 6, &entity->animator2, true, 0); break;
                                    case 6: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 5, &entity->animator2, true, 0); break;
                                }
                                entity->animator1.frameID = 0;
                            }
                        }
                        else {
                            entity->animator1.frameID = 3;
                        }
                    }
                }
            }

            entity->position.x = storeX;
            entity->position.y = storeY;
            if (!flag && Player_CheckBadnikTouch(player, entity, &Tubinaut->hitbox1) && Player_CheckBadnikBreak(entity, player, false)) {
                for (int i = 0; i < Tubinaut_OrbCount; ++i) {
                    if (entity->field_C4[i]) {
                        EntityTubinaut *orb = CREATE_ENTITY(Tubinaut, intToVoid(i + 1), entity->orbPositions[i].x, entity->orbPositions[i].y);
                        orb->velocity.x     = 0x380 * RSDK.Cos256(entity->orbAngles[i] >> 4);
                        orb->velocity.y     = 0x380 * RSDK.Sin256(entity->orbAngles[i] >> 4);
                    }
                }

                destroyEntity(entity);
                entity->active = ACTIVE_NEVER2;
            }
        }
        ++playerID;
    }
}

bool32 Tubinaut_CheckAttacking(EntityPlayer *player)
{
    RSDK_THIS(Tubinaut);

    bool32 flag = Player_CheckAttacking(player, entity);

#if RETRO_USE_PLUS
    if (!flag && player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_CROUCH) {
        if (!player->uncurlTimer) {
            RSDK.PlaySfx(Player->sfxPimPom, false, 255);
            player->uncurlTimer = 30;
            if (entity->position.x > player->position.x)
                player->groundVel = -0x10000;
            else
                player->groundVel = 0x10000;
            flag = true;
        }
    }
#endif

    if (!flag)
        Player_CheckHit(player, entity);

    return flag;
}

void Tubinaut_Unknown5(EntityPlayer *player, int orbID)
{
    RSDK_THIS(Tubinaut);

    if (Tubinaut_CheckAttacking(player)) {
        EntityDust *dust = CREATE_ENTITY(Dust, entity, entity->position.x, entity->position.y);
        dust->state      = Dust_State_DropDash;
        dust->drawOrder  = player->drawOrder;
        RSDK.PlaySfx(Tubinaut->sfxPowerdown, false, 255);
#if RETRO_USE_PLUS
        if (player->characterID != ID_MIGHTY || player->playerAnimator.animationID != ANI_DROPDASH) {
#endif
            int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);

            int velX = 0, velY = 0;
#if RETRO_USE_PLUS
            if (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_CROUCH) {
                velX = player->velocity.x;
                velY = player->velocity.y - 0x10000;
            }
            else {
#endif
                velX = 0x540 * RSDK.Cos256(angle);
                velY = 0x540 * RSDK.Sin256(angle);
#if RETRO_USE_PLUS
            }
#endif

            if (player->characterID != ID_KNUCKLES || player->playerAnimator.animationID != ANI_FLY) {
                player->velocity.x  = velX;
                player->groundVel   = velX;
                player->jumpAbility = 0;
            }
            player->velocity.y     = velY;
            player->onGround       = false;
            player->tileCollisions = true;
#if RETRO_USE_PLUS
        }
#endif

        entity->field_C4[orbID] = 0;
        --entity->orbCount;
        entity->activeOrbs &= ~(1 << orbID);

        if (entity->orbCount == 2) {
            switch (orbID) {
                case 0:
                    entity->field_BC[1] = -24;
                    entity->field_BC[2] = 24;
                    break;
                case 1:
                    entity->field_BC[2] = -24;
                    entity->field_BC[0] = 24;
                    break;
                case 2:
                    entity->field_BC[0] = -24;
                    entity->field_BC[1] = 24;
                    break;
            }
        }
    }
}

void Tubinaut_HandleRepel(EntityPlayer *player, int playerID)
{
    RSDK_THIS(Tubinaut);

#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_CROUCH) {
        entity->playerTimers[playerID] = 15;
        RSDK.PlaySfx(Tubinaut->sfxRepel, false, 255);

        if (entity->position.x > player->position.x)
            player->groundVel = -0x20000;
        else
            player->groundVel = 0x20000;
    }
    else {
#endif
        if (Tubinaut_CheckAttacking(player)) {
            int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
            int velX  = 0x700 * RSDK.Cos256(angle);
            int velY  = 0x700 * RSDK.Sin256(angle);
            if (player->characterID != ID_KNUCKLES || player->playerAnimator.animationID != ANI_FLY) {
                player->velocity.x  = velX;
                player->groundVel   = velX;
                player->jumpAbility = 0;
            }
            player->velocity.y             = velY;
            player->onGround               = false;
            player->tileCollisions         = true;
            entity->playerTimers[playerID] = 15;
            RSDK.PlaySfx(Tubinaut->sfxRepel, false, 255);
        }
#if RETRO_USE_PLUS
    }
#endif
}

void Tubinaut_HandleOrbs(void)
{
    RSDK_THIS(Tubinaut);

    entity->position.y = (RSDK.Sin256(RSDK_sceneInfo->entity->angle) << 10) + entity->startPos.y;
    entity->angle      = (entity->angle + 4) & 0xFF;

    for (int i = 0; i < Tubinaut_OrbCount; ++i) {
        entity->orbPositions[i].x = 0x1400 * RSDK.Cos256(entity->orbAngles[i] >> 4) + entity->position.x;
        entity->orbPositions[i].y = 0x1400 * RSDK.Sin256(entity->orbAngles[i] >> 4) + entity->position.y;
        entity->orbAngles[i] -= entity->timer2;

        if (entity->field_BC[i]) {
            if (entity->field_BC[i] <= 0) {
                entity->field_BC[i]++;
                entity->orbAngles[i] -= 16;
            }
            else {
                entity->field_BC[i]--;
                entity->orbAngles[i] += 16;
            }
        }
        entity->orbAngles[i] &= 0xFFF;

        if (entity->stateOrb == Tubinaut_StateOrb_Unknown4 || entity->orbCount <= 1 || entity->timer2 < 192) {
            entity->animators[i].frameID        = (2 * (7 - ((((entity->orbAngles[i] >> 4) + 112) >> 5) & 7)));
            entity->animators[i].animationTimer = (entity->animators[i].animationTimer + 1) & 3;
            entity->animators[i].frameID += (entity->animators[i].animationTimer >> 1);
        }
        else {
            int val                             = ((entity->animators[i].animationTimer + 1) & 7) >> 1;
            entity->animators[i].animationTimer = (entity->animators[i].animationTimer + 1) & 7;
            switch (val) {
                case 0:
                case 2: entity->animators[i].frameID = (val + 2 * (7 - ((((entity->orbAngles[i] >> 4) + 112) >> 5) & 7))); break;
                case 1: entity->animators[i].frameID = 17; break;
                case 3: entity->animators[i].frameID = 16; break;
                default: break;
            }
        }
    }
}

void Tubinaut_CheckOnScreen(void)
{
    RSDK_THIS(Tubinaut);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->visible    = 0;
        entity->state      = Tubinaut_State_Setup;
        entity->active     = ACTIVE_BOUNDS;
        if (entity->orbCount > 1) {
            entity->timer2   = 64;
            entity->stateOrb = Tubinaut_StateOrb_Unknown1;
        }
    }
}

void Tubinaut_State_Setup(void)
{
    RSDK_THIS(Tubinaut);

    entity->timer          = 0;
    entity->distanceRemain = entity->distance;
    entity->direction      = FLIP_NONE;
    entity->velocity.x     = -0x8000;
    entity->visible        = true;
    entity->active         = ACTIVE_NORMAL;
    entity->state          = Tubinaut_State_Move;
    Tubinaut_State_Move();
}

void Tubinaut_State_Move(void)
{
    RSDK_THIS(Tubinaut);

    if (entity->distance) {
        entity->position.x += entity->velocity.x;
        if (!--entity->distanceRemain) {
            entity->distanceRemain = entity->distance;
            entity->direction ^= FLIP_X;
            entity->velocity.x = -entity->velocity.x;
        }
    }
    else {
        EntityPlayer *playerPtr = NULL;
        foreach_active(Player, player)
        {
            if (playerPtr) {
                if (abs(player->position.x - entity->position.x) < abs(playerPtr->position.x - entity->position.x))
                    playerPtr = player;
            }
            else {
                playerPtr = player;
            }
        }

        if (playerPtr)
            entity->direction = playerPtr->position.x >= entity->position.x;
    }

    Tubinaut_HandleOrbs();
    if (!entity->orbCount)
        RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->stateOrb();
    Tubinaut_CheckPlayerCollisions();
    Tubinaut_CheckOnScreen();
}

void Tubinaut_StateOrb_Unknown1(void)
{
    RSDK_THIS(Tubinaut);

    if (++entity->timer > 120) {
        entity->stateOrb = Tubinaut_StateOrb_Unknown2;
        if (entity->orbCount > 2)
            entity->animator1.frameID = 1;
        entity->timer = 0;
    }
}

void Tubinaut_StateOrb_Unknown2(void)
{
    RSDK_THIS(Tubinaut);

    if (entity->timer2 >= 256) {
        if (entity->orbCount > 1)
            entity->stateOrb = Tubinaut_StateOrb_Unknown3;
    }
    else {
        entity->timer2++;
    }
}

void Tubinaut_StateOrb_Unknown3(void)
{
    RSDK_THIS(Tubinaut);

    if (++entity->timer > 120) {
        entity->stateOrb = Tubinaut_StateOrb_Unknown4;
        switch (entity->orbCount) {
            case 0: entity->animator1.frameID = 3; break;
            case 1: entity->animator1.frameID = 2; break;
            case 2:
            case 3: entity->animator1.frameID = 0; break;
            default: break;
        }
        entity->timer = 0;
    }
}

void Tubinaut_StateOrb_Unknown4(void)
{
    RSDK_THIS(Tubinaut);

    if (entity->orbCount > 1) {
        if (--entity->timer2 <= 64)
            entity->stateOrb = Tubinaut_StateOrb_Unknown1;
    }
    else {
        entity->stateOrb = Tubinaut_StateOrb_Unknown2;
    }
}

void Tubinaut_State2_Unknown(void)
{
    RSDK_THIS(Tubinaut);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void Tubinaut_EditorDraw(void)
{
    RSDK_THIS(Tubinaut);

    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 1, entity->animators, true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 2, &entity->animators[1], true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 3, &entity->animators[2], true, 0);
    entity->orbAngles[0] = 0;
    entity->orbAngles[1] = 0x500;
    entity->orbAngles[2] = 0xA00;
    entity->field_C4[0]  = true;
    entity->field_C4[1]  = true;
    entity->field_C4[2]  = true;
    entity->activeOrbs   = 1 | 2 | 4;
    entity->orbCount     = 3;
    entity->timer2       = 64;
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 7, &entity->animator2, true, 0);
    entity->state    = Tubinaut_State_Setup;
    entity->stateOrb = Tubinaut_StateOrb_Unknown1;

    for (int i = 0; i < Tubinaut_OrbCount; ++i) {
        entity->orbPositions[i].x = 0x1400 * RSDK.Cos256(entity->orbAngles[i] >> 4) + entity->position.x;
        entity->orbPositions[i].y = 0x1400 * RSDK.Sin256(entity->orbAngles[i] >> 4) + entity->position.y;
    }

    Tubinaut_Draw();

    if (showGizmos()) {
        int x = entity->position.x;

        entity->position.x += entity->distance * -0x8000;

        for (int i = 0; i < Tubinaut_OrbCount; ++i) {
            entity->orbPositions[i].x = 0x1400 * RSDK.Cos256(entity->orbAngles[i] >> 4) + entity->position.x;
            entity->orbPositions[i].y = 0x1400 * RSDK.Sin256(entity->orbAngles[i] >> 4) + entity->position.y;
        }

        entity->inkEffect = INK_BLEND;
        Tubinaut_Draw();

        RSDK.DrawLine(x, entity->position.y, entity->position.x, entity->position.y, 0x00FF00, 0xFF, INK_NONE, false);

        entity->position.x = x;
    }
}

void Tubinaut_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Tubinaut.bin", SCOPE_STAGE);
    else
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Tubinaut.bin", SCOPE_STAGE);
}
#endif

void Tubinaut_Serialize(void) { RSDK_EDITABLE_VAR(Tubinaut, VAR_UINT16, distance); }
