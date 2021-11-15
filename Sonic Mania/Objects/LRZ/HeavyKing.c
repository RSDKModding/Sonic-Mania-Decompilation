#include "SonicMania.h"

ObjectHeavyKing *HeavyKing;

void HeavyKing_Update(void)
{
    RSDK_THIS(HeavyKing);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    StateMachine_Run(entity->state);
}

void HeavyKing_LateUpdate(void) {}

void HeavyKing_StaticUpdate(void) {}

void HeavyKing_Draw(void)
{
    RSDK_THIS(HeavyKing);

    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 16, 0, 16, 16);
        RSDK.CopyPalette(2, 192, 0, 192, 16);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.CopyPalette(1, 16, 0, 16, 16);
        RSDK.CopyPalette(1, 192, 0, 192, 16);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void HeavyKing_Create(void *data)
{
    RSDK_THIS(HeavyKing);

    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        else {
            entity->drawOrder     = Zone->drawOrderLow;
            entity->drawFX        = FX_FLIP;
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0xC00000;
            entity->updateRange.y = 0x800000;
            entity->health        = 8;
            entity->state         = HeavyKing_State_SetupArena;
            RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 7, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 16, &entity->animator2, true, 0);
        }
    }
}

void HeavyKing_StageLoad(void)
{
    HeavyKing->aniFrames      = RSDK.LoadSpriteAnimation("LRZ3/HeavyKing.bin", SCOPE_STAGE);
    HeavyKing->cutsceneFrames = RSDK.LoadSpriteAnimation("Players/KnuxCutsceneHPZ.bin", SCOPE_STAGE);
    HeavyKing->attackPatternPos = 0;
    HeavyKing->sfxHit         = RSDK.GetSFX("Stage/BossHit.wav");
    HeavyKing->sfxImpact2     = RSDK.GetSFX("Stage/Impact2.wav");
    HeavyKing->sfxImpact5     = RSDK.GetSFX("Stage/Impact5.wav");
    HeavyKing->sfxExplosion   = RSDK.GetSFX("Stage/Explosion2.wav");
    HeavyKing->sfxFreeze      = RSDK.GetSFX("PSZ/Freeze.wav");
    HeavyKing->sfxCharge      = RSDK.GetSFX("LRZ/KingCharge.wav");
    HeavyKing->sfxImpact6     = RSDK.GetSFX("Stage/Impact6.wav");
    HeavyKing->sfxRodPlant    = RSDK.GetSFX("LRZ/RodPlant.wav");
    HeavyKing->sfxRodShine    = RSDK.GetSFX("LRZ/RodShine.wav");
    HeavyKing->sfxElecOn      = RSDK.GetSFX("LRZ/ElecOn.wav");
    HeavyKing->sfxTwinCharge  = RSDK.GetSFX("LRZ/TwinCharge.wav");

    HeavyKing->hitbox1.left   = -24;
    HeavyKing->hitbox1.top    = 40;
    HeavyKing->hitbox1.right  = 24;
    HeavyKing->hitbox1.bottom = 44;

    HeavyKing->hitbox2.left   = -20;
    HeavyKing->hitbox2.top    = 20;
    HeavyKing->hitbox2.right  = 20;
    HeavyKing->hitbox2.bottom = 16;

    HeavyKing->hitbox3.left   = -32;
    HeavyKing->hitbox3.top    = -40;
    HeavyKing->hitbox3.right  = 32;
    HeavyKing->hitbox3.bottom = 40;
}

void HeavyKing_CheckPlayerCollisions(void)
{
    RSDK_THIS(HeavyKing);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &HeavyKing->hitbox2) && Player_CheckBossHit(player, entity)) {
            HeavyKing_Hit();
        }
    }
}

void HeavyKing_CheckPlayerCollisions_Charging(void)
{
    RSDK_THIS(HeavyKing);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer && Player_CheckCollisionTouch(player, entity, &HeavyKing->hitbox3)) {
            Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
        }
    }
}

void HeavyKing_Hit(void)
{
    RSDK_THIS(HeavyKing);

    if (--entity->health <= 0) {
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 6, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 15, &entity->animator2, false, 0);
        entity->state      = HeavyKing_State_Destroyed;
        entity->velocity.y = -0x40000;
        entity->onGround   = false;
        entity->timer      = 0;
        RSDK.StopSFX(HeavyKing->sfxTwinCharge);
        foreach_active(KingAttack, attack) { destroyEntity(attack); }
        RSDK_sceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        if (entity->animator1.animationID == 5) {
            entity->invincibilityTimer = 48;
        }
        else {
            entity->invincibilityTimer = 96;
            entity->stateStore     = entity->state;
            entity->animator1Store = entity->animator1;
            entity->animator2Store = entity->animator2;
            RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 6, &entity->animator1, false, 0);
            RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 15, &entity->animator2, false, 0);
            entity->state = HeavyKing_State_HitRecoil;
        }
        RSDK.PlaySfx(HeavyKing->sfxHit, false, 255);
    }
}

void HeavyKing_HandleClawMovement(void)
{
    RSDK_THIS(HeavyKing);

    EntityKingClaw *claw = (EntityKingClaw *)entity->claw;

    if (claw) {
        entity->position.x = claw->position.x;
        entity->position.y = claw->position.y;
        if (entity->direction == FLIP_NONE)
            entity->position.x += 0x300000;
        else
            entity->position.x -= 0x300000;
        entity->position.y += 0x9C0000;
    }
}

void HeavyKing_HandleAnimators(void)
{
    RSDK_THIS(HeavyKing);

    entity->animator1.animationSpeed = 0;
    entity->animator2.animationSpeed = 0;
    if (entity->velocity.y >= 0) {
        if (entity->velocity.y <= 0x10000) {
            entity->animator1.frameID = 7;
            entity->animator2.frameID = 7;
        }
        else {
            entity->animator1.frameID = 8;
            entity->animator2.frameID = 8;
        }
    }
    else if (entity->velocity.y >= -0x10000) {
        entity->animator1.frameID = 6;
        entity->animator2.frameID = 6;
    }
    else {
        entity->animator1.frameID = 5;
        entity->animator2.frameID = 5;
    }
}

void HeavyKing_Unknown6(void)
{
    RSDK_THIS(HeavyKing);

    EntityPlayer *player = Player_GetNearestPlayerX();

    int bounds = HeavyKing->boundsL;
    if (player->position.x >= bounds) {
        bounds = player->position.x;
        if (player->position.x > HeavyKing->boundsR)
            bounds = HeavyKing->boundsR;
    }

    int angle = RSDK.ATan2((bounds - entity->position.x) >> 16, (player->position.y - entity->position.y) >> 16);
    int rot   = 2 * angle;

    if (abs(rot) >= abs(rot - 0x200)) {
        if (abs(rot - 0x200) < abs(rot + 0x200)) {
            entity->rotation += ((rot - 0x200) >> 5);
        }
        else {
            entity->rotation += ((rot + 0x200) >> 5);
        }
    }
    else {
        if (abs(rot) < abs(rot + 0x200)) {
            entity->rotation += (rot >> 5);
        }
        else {
            entity->rotation += ((rot + 0x200) >> 5);
        }
    }
    entity->rotation &= 0x1FF;

    entity->velocity.x += 8 * RSDK.Cos512(entity->rotation);
    int angle2         = (entity->position.x - HeavyKing->boundsM) >> 16;
    entity->velocity.x = clampVal(entity->velocity.x, -0x20000, 0x20000);
    entity->position.x += entity->velocity.x;
    entity->originPos.y = HeavyKing->value10 + 96 * abs(angle2 * angle2);
    entity->direction   = entity->position.x <= player->position.x;
}

void HeavyKing_FindTargetEmerald(void)
{
    RSDK_THIS(HeavyKing);

    EntityHPZEmerald *emeraldPtr = NULL;
    int emeraldDist              = 0x7FFFFFFF;

    if (entity->onGround) {
        if (entity->direction == FLIP_X) {
            foreach_active(HPZEmerald, emerald)
            {
                if (emerald->type != HPZEMERALD_MASTER) {
                    int dist = emerald->position.x - entity->position.x;
                    if (dist < emeraldDist && dist >= 0 && entity->targetEmerald != (Entity*)emerald) {
                        emeraldDist = dist;
                        emeraldPtr  = emerald;
                    }
                }
            }
        }
        else {
            foreach_active(HPZEmerald, emerald)
            {
                if (emerald->type != HPZEMERALD_MASTER) {
                    int dist = entity->position.x - emerald->position.x;
                    if (dist < emeraldDist && dist >= 0 && entity->targetEmerald != (Entity *)emerald) {
                        emeraldDist = dist;
                        emeraldPtr  = emerald;
                    }
                }
            }
        }

        if (emeraldPtr)
            entity->targetEmerald = (Entity *)emeraldPtr;
        else
            entity->direction ^= FLIP_X;
    }
    else {
        foreach_active(HPZEmerald, emerald)
        {
            if (emerald->type != HPZEMERALD_MASTER) {
                int dist = abs(entity->position.x - emerald->position.x);
                if (dist < emeraldDist) {
                    emeraldDist = dist;
                    emeraldPtr  = emerald;
                }
            }
        }

        if (emeraldPtr)
            entity->targetEmerald = (Entity *)emeraldPtr;
    }
}

void HeavyKing_Unknown8(int frame)
{
    RSDK_THIS(HeavyKing);

    RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 3, &entity->animator1, true, frame);
    RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 12, &entity->animator2, true, frame);
    if (RSDK.GetEntityCount(KingAttack->objectID, true)) {
        HeavyKing_FindTargetEmerald();
        entity->velocity.x   = (entity->targetEmerald->position.x - entity->position.x) / 40;
        EntityPlayer *player = Player_GetNearestPlayer();

        foreach_active(KingAttack, attack)
        {
            if (attack->state == KingAttack_State2_Unknown2) {
                int angle          = RSDK.ATan2((player->position.x - attack->position.x) >> 16, (player->position.y - attack->position.y) >> 16);
                attack->field_70.x = 0x600 * RSDK.Cos256(angle);
                attack->field_70.y = 0x600 * RSDK.Sin256(angle);
                attack->drawOrder  = Zone->drawOrderLow;
                attack->state      = KingAttack_State_Unknown;
                RSDK.PlaySfx(KingAttack->sfxElecPulse, false, 255);
                foreach_break;
            }
        }
    }
    else {
        entity->field_70 = 0;
        if (entity->position.x - HeavyKing->boundsM > 0)
            entity->velocity.x = -0x30000;
        else
            entity->velocity.x = 0x30000;
    }
    entity->velocity.y = -0x60000;
    entity->onGround   = false;
    entity->state      = HeavyKing_State_Unknown1;
}

void HeavyKing_Unknown9(void)
{
    RSDK_THIS(HeavyKing);

    HeavyKing_FindTargetEmerald();
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    entity->onGround   = false;
    entity->state      = HeavyKing_State_Unknown13;
    RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 3, &entity->animator1, false, 6);
    RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 12, &entity->animator2, false, 6);
}

void HeavyKing_CreateSpinRay(void)
{
    RSDK_THIS(HeavyKing);

    EntityFXSpinRay *ray = NULL;
    if (entity->direction) {
        ray             = CREATE_ENTITY(FXSpinRay, NULL, entity->position.x - 0x240000, entity->position.y - 0x300000);
        ray->field_78.x = -0x240000;
    }
    else {
        ray             = CREATE_ENTITY(FXSpinRay, NULL, entity->position.x + 0x240000, entity->position.y - 0x300000);
        ray->field_78.x = 0x240000;
    }
    ray->parent     = (Entity *)entity;
    ray->field_78.y = -0x300000;
}

void HeavyKing_CreateExpandRing(void)
{
    RSDK_THIS(HeavyKing);

    EntityFXExpandRing *expandRing = CREATE_ENTITY(FXExpandRing, NULL, entity->position.x + 0x240000, entity->position.y - 0x300000);
    expandRing->parent             = (Entity *)entity;
    expandRing->drawFX             = FX_FLIP;
    expandRing->posOffset.x        = 0x240000;
    expandRing->posOffset.y        = -0x300000;
}

void HeavyKing_CreateAttack(void)
{
    RSDK_THIS(HeavyKing);

    EntityKingAttack *attack = CREATE_ENTITY(KingAttack, intToVoid(0), entity->position.x, HeavyKing->value9);
    attack->parent           = (Entity *)entity;
    if (entity->direction) {
        attack->position.x -= 0xC00000;
        attack->velocity.x = 0x100000;
    }
    else {
        attack->position.x += 0xC00000;
        attack->velocity.x = -0x100000;
    }
}

void HeavyKing_CreateAttackCircle(void)
{
    RSDK_THIS(HeavyKing);

    for (int angle = 0; angle < 1020; angle += 170) {
        EntityKingAttack *attack = CREATE_ENTITY(KingAttack, intToVoid(2), entity->position.x, entity->position.y);
        attack->angle            = angle;
        attack->parent           = (Entity *)entity;
    }
}

void HeavyKing_State_SetupArena(void)
{
    RSDK_THIS(HeavyKing);

    if (++entity->timer >= 8) {
        entity->timer = 0;
        foreach_active(KingClaw, claw) { entity->claw = (Entity *)claw; }
        EntityKingClaw *kingClaw = (EntityKingClaw *)entity->claw;

        if (kingClaw) {
            HeavyKing->boundsL          = kingClaw->position.x - 0xA00000;
            HeavyKing->boundsM          = kingClaw->position.x;
            HeavyKing->boundsR          = kingClaw->position.x + 0x600000;
            Zone->playerBoundActiveL[0] = true;
            Zone->screenBoundsL1[0]     = (kingClaw->position.x >> 16) - 320;
            Zone->playerBoundActiveR[0] = true;
            Zone->screenBoundsR1[0]     = (kingClaw->position.x >> 16) + 320;
            Zone->screenBoundsT1[0]     = (kingClaw->position.y >> 16);
            HeavyKing->value9           = kingClaw->position.y + 0x2400000;
            entity->active              = ACTIVE_NORMAL;
            HeavyKing_HandleClawMovement();
            entity->state = HeavyKing_State_HandleCutsceneSetup;
        }
    }
}

void HeavyKing_State_HandleCutsceneSetup(void)
{
    RSDK_THIS(HeavyKing);
    EntityKingClaw *claw = (EntityKingClaw *)entity->claw;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > entity->position.x - 0x1800000) {
        CutsceneSeq_LockPlayerControl(player1);
        player1->stateInput       = StateMachine_None;
        player1->jumpPress        = false;
        player1->jumpHold         = false;
        player1->right            = true;
        player1->pushing          = false;
        player1->state            = Player_State_Ground;
        Player->disableP2KeyCheck = true;
        if (player1->velocity.x > 0x20000) {
            player1->groundVel  = 0x20000;
            player1->velocity.x = 0x20000;
        }
    }

    if (player1->position.x > entity->position.x - 0xE00000) {
        player1->groundVel  = 0;
        player1->velocity.x = 0;
        player1->right      = false;
        player1->state      = Player_State_None;
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_LOOKUP, &player1->playerAnimator, false, 0);

        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (player2->objectID == Player->objectID) {
            player2->groundVel  = 0;
            player2->velocity.x = 0;
            player2->direction  = FLIP_NONE;
            CutsceneSeq_LockPlayerControl(player2);
            player2->stateInput = StateMachine_None;
            player2->up         = true;
        }
        Music_TransitionTrack(TRACK_HBHMISCHIEF, 0.0125);
        claw->state     = KingClaw_Unknown2;
        entity->visible = true;
        entity->state   = HeavyKing_State_CutsceneUnknown1;
        Camera_ShakeScreen(6, 0, 6);
        RSDK.PlaySfx(HeavyKing->sfxImpact6, false, 255);
    }
}

void HeavyKing_State_CutsceneUnknown1(void)
{
    RSDK_THIS(HeavyKing);

    ++entity->timer;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->playerAnimator.frameID == 5)
        player1->playerAnimator.animationSpeed = 0;
    if (entity->timer == 30) {
        entity->timer = 0;
        foreach_all(HPZEmerald, emerald)
        {
            emerald->active = ACTIVE_NORMAL;
            if (emerald->type == HPZEMERALD_MASTER) {
                entity->masterEmerald = (Entity *)emerald;
                Camera_SetupLerp(2, 0, emerald->position.x - 0x500000, emerald->position.y - 0x800000, 3);
            }
        }
        entity->state = HeavyKing_State_CutsceneUnknown2;
    }
    HeavyKing_HandleClawMovement();
}

void HeavyKing_State_CutsceneUnknown2(void)
{
    RSDK_THIS(HeavyKing);

    if (++entity->timer == 180) {
        entity->timer        = 0;
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        Camera_SetupLerp(3, 0, camera->position.x, camera->startLerpPos.y, 4);
        entity->state = HeavyKing_State_CutsceneUnknown3;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
        if (player1->characterID == ID_KNUCKLES)
#endif
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, false, 0);
    }
    HeavyKing_HandleClawMovement();
}

void HeavyKing_State_CutsceneUnknown3(void)
{
    RSDK_THIS(HeavyKing);

    if (!RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->state) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
        if (player1->characterID == ID_KNUCKLES) {
#endif
            RSDK.SetSpriteAnimation(HeavyKing->cutsceneFrames, 0, &player1->playerAnimator, false, 0);
            CREATE_ENTITY(ThoughtBubble, NULL, player1->position.x - 0x300000, player1->position.y - 0x500000);
#if RETRO_USE_PLUS
        }
#endif
        entity->state = HeavyKing_State_CutsceneUnknown4;
    }
    HeavyKing_HandleClawMovement();
}

void HeavyKing_State_CutsceneUnknown4(void)
{
    RSDK_THIS(HeavyKing);
    EntityKingClaw *claw = (EntityKingClaw *)entity->claw;

    HeavyKing_HandleClawMovement();
    if (++entity->timer == 180) {
        claw->state = KingClaw_Unknown3;
    }
    if (entity->timer == 240) {
        entity->timer = 0;
        Camera_ShakeScreen(6, 0, 6);
        RSDK.PlaySfx(HeavyKing->sfxImpact6, false, 255);
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
        if (player1->characterID == ID_KNUCKLES)
#endif
            RSDK.SetSpriteAnimation(HeavyKing->cutsceneFrames, 1, &player1->playerAnimator, false, 0);
        entity->state = HeavyKing_State_CutsceneUnknown5;
    }
}

void HeavyKing_State_CutsceneUnknown5(void)
{
    RSDK_THIS(HeavyKing);

    HeavyKing_HandleClawMovement();

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
    if (player1->characterID == ID_KNUCKLES) {
#endif
        if (player1->playerAnimator.frameID == player1->playerAnimator.frameCount - 1) {
            RSDK.SetSpriteAnimation(HeavyKing->cutsceneFrames, 2, &player1->playerAnimator, false, 0);
            entity->state = HeavyKing_State_CutsceneUnknown6;
        }
#if RETRO_USE_PLUS
    }
    else {
        entity->state = HeavyKing_State_CutsceneUnknown6;
    }
#endif
}

void HeavyKing_State_CutsceneUnknown6(void)
{
    RSDK_THIS(HeavyKing);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *camera  = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    HeavyKing_HandleClawMovement();
    if (player1->onGround)
        player1->groundVel = 0;

    switch (++entity->timer) {
        case 30:
#if RETRO_USE_PLUS
            if (player1->characterID == ID_KNUCKLES)
#endif
                RSDK.SetSpriteAnimation(HeavyKing->cutsceneFrames, 4, &player1->playerAnimator, false, 0);
            break;
        case 90:
            RSDK.PlaySfx(Player->sfx_Jump, false, 255);
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_JUMP, &player1->playerAnimator, false, 0);
            player1->velocity.x  = 0x2C000;
            player1->velocity.y  = -0x50000;
            player1->jumpAbility = 0;
            player1->onGround    = false;
            player1->state       = Player_State_Air;
            break;
        case 124:
            RSDK.PlaySfx(Player->sfx_Jump, false, 255);
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_JUMP, &player1->playerAnimator, false, 0);
            player1->velocity.x  = -0x1C000;
            player1->velocity.y  = -0x66000;
            player1->jumpAbility = 0;
            player1->onGround    = false;
            player1->state       = Player_State_Air;
            Camera_SetupLerp(3, 0, camera->startLerpPos.x, camera->startLerpPos.y + 0x200000, 4);
            break;
        default:
            if (entity->timer > 144 && player1->onGround) {
#if RETRO_USE_PLUS
                if (player1->characterID == ID_KNUCKLES)
#endif
                    RSDK.SetSpriteAnimation(HeavyKing->cutsceneFrames, 3, &player1->playerAnimator, false, 0);
#if RETRO_USE_PLUS
                else
                    RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, false, 0);
#endif
                player1->position.y -= player1->cameraOffset;
                player1->state           = Player_State_None;
                player1->nextGroundState = StateMachine_None;
                entity->timer            = 0;
                entity->state            = HeavyKing_State_CutsceneUnknown7;
            }
            break;
    }
}

void HeavyKing_State_CutsceneUnknown7(void)
{
    RSDK_THIS(HeavyKing);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    HeavyKing_HandleClawMovement();
    if (++entity->timer == 90) {
        entity->timer  = 0;
        player1->down  = true;
        player1->state = Player_State_Ground;
        entity->state  = HeavyKing_State_CutsceneUnknown8;
    }
}

void HeavyKing_State_CutsceneUnknown8(void)
{
    RSDK_THIS(HeavyKing);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    HeavyKing_HandleClawMovement();
    entity->timer++;
    player1->jumpPress = entity->timer > 24 && !(entity->timer & 7);
    if (entity->timer == 56) {
        entity->timer = 0;
        player1->down = false;
        entity->state = HeavyKing_State_CutsceneUnknown9;
        Music_TransitionTrack(TRACK_EGGMAN2, 0.0125);
    }
}

void HeavyKing_State_CutsceneUnknown9(void)
{
    RSDK_THIS(HeavyKing);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    HeavyKing_HandleClawMovement();
    if (++entity->timer == 2) {
        player1->jumpPress   = true;
        player1->jumpHold    = true;
        player1->scrollDelay = 160;
        if (entity->masterEmerald)
            Camera_SetupLerp(2, 0, entity->masterEmerald->position.x, entity->masterEmerald->position.y + 0x300000, 4);
    }
    else {
        player1->jumpPress = false;
    }

    if (entity->timer == 14) {
        entity->timer      = 0;
        entity->velocity.y = -0x20000;
        entity->velocity.x = 0x10000;
        entity->state      = HeavyKing_State_CutsceneUnknown10;
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 6, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 15, &entity->animator2, false, 0);
        player1->velocity.x = -0x24000;
        player1->velocity.y = -0x20000;

        EntityKingClaw *claw = (EntityKingClaw *)entity->claw;
        claw->state          = KingClaw_Unknown5;
        claw->timer          = 128;
        if (claw->masterEmerald)
            claw->masterEmerald->onGround = false;
        RSDK.PlaySfx(HeavyKing->sfxImpact5, false, 255);
    }
}

void HeavyKing_State_CutsceneUnknown10(void)
{
    RSDK_THIS(HeavyKing);

    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->onGround)
        player1->groundVel = 0;

    if (++entity->timer == 24) {
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 3, &entity->animator1, false, 6);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 12, &entity->animator2, false, 6);
    }

    if (entity->onGround) {
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 4, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 13, &entity->animator2, false, 0);
        HeavyKing_Unknown8(0);
        Player->disableP2KeyCheck = false;
        player1->stateInput       = Player_ProcessP1Input;

        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (player2->objectID == Player->objectID) {
            CutsceneSeq_LockPlayerControl(player2);
            player2->stateInput = Player_ProcessP2Input_AI;
        }
    }
}

void HeavyKing_State_Unknown13(void)
{
    RSDK_THIS(HeavyKing);

    HeavyKing_HandleAnimators();

    if (entity->onGround) {
        HeavyKing_Unknown8(4);
        if (!entity->field_70) {
            EntityHPZEmerald *masterEmerald = (EntityHPZEmerald *)entity->masterEmerald;
            if (abs(entity->position.x - HeavyKing->boundsM) < 0x100000) {
                masterEmerald->solid = true;
                entity->state       = HeavyKing_State_Unknown2;
            }
        }
    }
    else {
        EntityHPZEmerald *targetEmerald = (EntityHPZEmerald *)entity->targetEmerald;
        if (targetEmerald)
            entity->position.x += (targetEmerald->position.x - entity->position.x) >> 3;
        entity->velocity.y += 0x3800;
        entity->position.y += entity->velocity.y;
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown1(void)
{
    RSDK_THIS(HeavyKing);

    if (entity->animator1.frameID >= 5) {
        HeavyKing_HandleAnimators();
        RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, HeavyKing->hitbox1.bottom >> 15, true);
        if (!entity->onGround) {
            int vel = entity->velocity.y;
            entity->velocity.y += 0x3800;
            if (entity->velocity.y >= 0 && vel < 0)
                HeavyKing_FindTargetEmerald();

            entity->position.x += entity->velocity.x;

            EntityHPZEmerald *targetEmerald = (EntityHPZEmerald *)entity->targetEmerald;
            if (entity->velocity.y > 0) {
                if (entity->velocity.x <= 0) {
                    if (entity->velocity.x < 0) {
                        if (entity->position.x < targetEmerald->position.x) {
                            entity->velocity.x = 0;
                            entity->position.x = targetEmerald->position.x;
                        }
                    }
                }
                else {
                    if (entity->position.x > targetEmerald->position.x) {
                        entity->velocity.x = 0;
                        entity->position.x = targetEmerald->position.x;
                    }
                }
            }
            entity->position.y += entity->velocity.y;
        }
        else {
            EntityHPZEmerald *masterEmerald = (EntityHPZEmerald *)entity->masterEmerald;
            HeavyKing_Unknown8(4);
            if (!entity->field_70) {
                if (abs(entity->position.x - HeavyKing->boundsM) >= 0x200000) {
                    entity->direction = entity->position.x < masterEmerald->position.x;
                }
                else {
                    masterEmerald->solid = true;
                    entity->state       = HeavyKing_State_Unknown2;
                }
            }
        }
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown2(void)
{
    RSDK_THIS(HeavyKing);

    if (entity->animator1.frameID >= 5) {
        HeavyKing_HandleAnimators();
        EntityHPZEmerald *masterEmerald = (EntityHPZEmerald *)entity->masterEmerald;

        if (!entity->onGround) {
            entity->velocity.y += 0x3800;
            entity->position.y += entity->velocity.y;
            entity->position.x += (masterEmerald->position.x - entity->position.x) >> 2;
        }
        else {
            masterEmerald->solid = false;
            RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 4, &entity->animator1, false, 0);
            RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 13, &entity->animator2, false, 0);
            foreach_active(KingAttack, attack)
            {
                if (attack->state == KingAttack_State2_Unknown2)
                    destroyEntity(attack);
            }
            entity->state = HeavyKing_State_Unknown3;
        }
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown3(void)
{
    RSDK_THIS(HeavyKing);

    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 1, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 10, &entity->animator2, false, 0);
        entity->state = HeavyKing_State_Unknown4;
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown4(void)
{
    RSDK_THIS(HeavyKing);

    if (entity->animator2.frameID == 7 && entity->animator2.animationTimer == 1)
        RSDK.PlaySfx(HeavyKing->sfxRodPlant, false, 255);
    if (entity->animator1.frameID == 12) {
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 18, &entity->animator3, false, 0);
        entity->state    = HeavyKing_State_Unknown5;
        entity->field_70 = (entity->health >= 2) + 1;
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown5(void)
{
    RSDK_THIS(HeavyKing);

    RSDK.ProcessAnimation(&entity->animator3);
    ++entity->timer;

    EntityHPZEmerald *masterEmerald = (EntityHPZEmerald *)entity->masterEmerald;
    masterEmerald->alpha            = (RSDK.Sin256(8 * entity->timer) >> 1) + 128;
    if (entity->timer == 176) {
        entity->timer        = 0;
        masterEmerald->alpha = 0;
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 2, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 11, &entity->animator2, false, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, false, 0);
        entity->state = HeavyKing_State_Unknown6;
    }
    if (!(entity->timer & 7))
        RSDK.PlaySfx(HeavyKing->sfxCharge, false, 255);
    HeavyKing_CheckPlayerCollisions_Charging();
}

void HeavyKing_State_Unknown6(void)
{
    RSDK_THIS(HeavyKing);

    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.Rand(0, 3);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 3, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 12, &entity->animator2, false, 0);
        entity->state = HeavyKing_State_Unknown7;
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown7(void)
{
    RSDK_THIS(HeavyKing);

    if (entity->animator1.frameID == 5) {
        entity->position.y -= 0x60000;
        entity->originPos.x = entity->position.x;
        entity->originPos.y = entity->position.y;
        entity->velocity.x  = 0;
        entity->velocity.y  = -0x10000;
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 5, &entity->animator1, false, 0);
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 14, &entity->animator2, false, 0);
        entity->targetEmerald = NULL;
        entity->onGround      = false;
        entity->state         = HeavyKing_State_Unknown8;
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown8(void)
{
    RSDK_THIS(HeavyKing);

    entity->velocity.y -= 0x800;
    entity->originPos.y += entity->velocity.y;
    BadnikHelpers_Oscillate(entity->originPos.y, 2, 11);
    if (++entity->timer == 24) {
        HeavyKing->value10 = entity->originPos.y;
        entity->timer      = 0;
        entity->timer      = 0;
        entity->state      = HeavyKing_State_Unknown9;
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown9(void)
{
    RSDK_THIS(HeavyKing);

    HeavyKing_Unknown6();
    BadnikHelpers_Oscillate(entity->originPos.y, 2, 11);
    if (++entity->timer == 120) {
        entity->timer    = 0;
        entity->field_6C = HeavyKing->attackPattern[HeavyKing->attackPatternPos++];
        HeavyKing->attackPatternPos &= 0x1F;
        if (entity->field_70 > 0)
            entity->field_70--;

        switch (entity->field_6C) {
            case 0:
                HeavyKing_CreateSpinRay();
                RSDK.PlaySfx(HeavyKing->sfxRodShine, false, 255);
                entity->state = HeavyKing_State_Unknown10;
                break;
            case 1:
                HeavyKing_CreateExpandRing();
                RSDK.PlaySfx(HeavyKing->sfxElecOn, false, 255);
                entity->state = HeavyKing_State_Unknown11;
                break;
            case 2:
                --entity->field_70;
                CREATE_ENTITY(KingAttack, intToVoid(4), entity->position.x, entity->position.y)->parent = (Entity *)entity;
                RSDK.PlaySfx(HeavyKing->sfxTwinCharge, false, 255);
                entity->state = HeavyKing_State_Unknown12;
                break;
        }
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown10(void)
{
    RSDK_THIS(HeavyKing);

    BadnikHelpers_Oscillate(entity->originPos.y, 2, 11);
    if (++entity->timer == 60)
        HeavyKing_CreateAttack();
    if (entity->timer == 120) {
        entity->timer = 0;
        if (entity->field_70 <= 0)
            HeavyKing_Unknown9();
        else
            entity->state = HeavyKing_State_Unknown9;
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown11(void)
{
    RSDK_THIS(HeavyKing);

    HeavyKing_Unknown6();
    BadnikHelpers_Oscillate(entity->originPos.y, 2, 11);
    if (++entity->timer == 16)
        HeavyKing_CreateAttackCircle();
    if (entity->timer == 128) {
        entity->timer = 0;
        HeavyKing_Unknown9();
    }
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_Unknown12(void)
{
    RSDK_THIS(HeavyKing);

    BadnikHelpers_Oscillate(entity->originPos.y, 2, 11);
    if (!RSDK.GetEntityCount(KingAttack->objectID, true))
        HeavyKing_Unknown9();
    HeavyKing_CheckPlayerCollisions();
}

void HeavyKing_State_HitRecoil(void)
{
    RSDK_THIS(HeavyKing);

    if (entity->invincibilityTimer == 48) {
        entity->animator1 = entity->animator1Store;
        entity->animator2 = entity->animator2Store;
        entity->state     = entity->stateStore;
    }
}

void HeavyKing_State_Destroyed(void)
{
    RSDK_THIS(HeavyKing);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyKing->sfxExplosion, false, 255);
        if (!(Zone->timer & 0xF)) {
            int x = entity->position.x + RSDK.Rand(-0x180000, 0x180000);
            int y = entity->position.y + RSDK.Rand(-0x180000, 0x180000);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, HeavyKing->hitbox1.bottom >> 15, true);
    if (!entity->onGround) {
        entity->velocity.y += 0x3800;
        entity->position.y += entity->velocity.y;
    }
    if (++entity->timer == 60)
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 17, &entity->animator2, false, 0);

    if (entity->timer == 72) {
        RSDK.PlaySfx(HeavyKing->sfxFreeze, false, 255);
        int centerX = entity->position.x - 0x280000;
        if (entity->direction == FLIP_NONE)
            centerX = entity->position.x + 0x280000;

        for (int i = 0; i < 6; ++i) {
            int spawnY           = (entity->position.y - 0x300000) + RSDK.Rand(-0x40000, 0x40000);
            int spawnX           = centerX + RSDK.Rand(-0x40000, 0x40000);
            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, spawnX, spawnY);
            debris->state        = Debris_State_Fall;
            debris->gravity      = 0x3800;
            debris->velocity.x   = RSDK.Rand(0, 0x20000);
            if (debris->position.x < centerX)
                debris->velocity.x = -debris->velocity.x;
            debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
            debris->drawFX     = FX_FLIP;
            debris->direction  = i & 3;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->timer      = 120;
            RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 19, &debris->animator, true, RSDK.Rand(0, 3));
        }
    }
    if (entity->timer == 120) {
        EntityKingClaw *claw             = (EntityKingClaw *)entity->claw;
        entity->animator1.animationSpeed = 0;
        entity->animator2.animationSpeed = 0;
        entity->timer                    = 0;
        entity->state                    = HeavyKing_State_Escape;
        if (entity->direction == FLIP_NONE)
            claw->position.x = entity->position.x - 0x300000;
        else
            claw->position.x = entity->position.x + 0x300000;
        claw->state       = KingClaw_Unknown6;
        claw->velocity.y  = 0x10000;
        claw->flag        = true;
        claw->targetY     = entity->position.y - 0xA00000;
        claw->position.y  = claw->targetY - 0xE00000;
        claw->drawOrder   = Zone->drawOrderHigh;
        entity->drawOrder = Zone->drawOrderHigh;
        KingClaw->active  = ACTIVE_NEVER;
    }
}

void HeavyKing_State_Escape(void)
{
    RSDK_THIS(HeavyKing);

    if (!(Zone->timer & 0x1F)) {
        int x                                                                    = entity->position.x + RSDK.Rand(-0x180000, 0x180000);
        int y                                                                    = entity->position.y + RSDK.Rand(-0x180000, 0x180000);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), x, y)->drawOrder = Zone->drawOrderHigh;
    }

    if ((Zone->timer & 0xF) == 4) {
        int x = entity->position.x - 0x2C0000;
        if (entity->direction == FLIP_NONE)
            x = entity->position.x + 0x2C0000;
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), x, entity->position.y - 0x300000)->drawOrder = Zone->drawOrderHigh;
    }

    EntityKingClaw *claw = (EntityKingClaw *)entity->claw;
    if (!claw->state) {
        claw->state = KingClaw_Unknown7;
        RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 7, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        entity->state = HeavyKing_State_Finish;
    }
}

void HeavyKing_State_Finish(void)
{
    RSDK_THIS(HeavyKing);
    HeavyKing_HandleClawMovement();

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        Zone->screenBoundsR1[0] += 1024;
        destroyEntity(entity->claw);
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void HeavyKing_EditorDraw(void)
{
    RSDK_THIS(HeavyKing);
    RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 7, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(HeavyKing->aniFrames, 16, &entity->animator2, true, 0);

    HeavyKing_Draw();
}

void HeavyKing_EditorLoad(void) { HeavyKing->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/HeavyKing.bin", SCOPE_STAGE); }
#endif

void HeavyKing_Serialize(void) {}
