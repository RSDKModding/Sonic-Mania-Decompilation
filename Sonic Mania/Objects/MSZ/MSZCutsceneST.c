#include "SonicMania.h"

ObjectMSZCutsceneST *MSZCutsceneST;

void MSZCutsceneST_Update(void)
{
    RSDK_THIS(MSZCutsceneST);
    if (!entity->activated) {
        if (entity->nodePtr) {
            MSZCutsceneST->nodePtr = entity->nodePtr;
            MSZCutsceneST_SetupCutscene();
            entity->activated = true;
        }
    }
}

void MSZCutsceneST_LateUpdate(void) {}

void MSZCutsceneST_StaticUpdate(void)
{
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID == PauseMenu->objectID || !MSZCutsceneST->isMayday) {
        if (MSZCutsceneST->playingMaydaySfx) {
            RSDK.StopSFX(MSZCutsceneST->sfxMayday);
            MSZCutsceneST->playingMaydaySfx = false;
        }
    }
    else if (!MSZCutsceneST->playingMaydaySfx) {
        RSDK.PlaySfx(MSZCutsceneST->sfxMayday, true, 255);
        MSZCutsceneST->playingMaydaySfx = true;
    }
}

void MSZCutsceneST_Draw(void) {}

void MSZCutsceneST_Create(void *data)
{
    RSDK_THIS(MSZCutsceneST);

    INIT_ENTITY(entity);
    CutsceneRules_DrawCutsceneBounds(entity, &entity->size);
    entity->active = ACTIVE_NEVER;
}

void MSZCutsceneST_StageLoad(void)
{
    MSZCutsceneST->actFinishFlag = false;

    foreach_all(SignPost, signPost)
    {
        MSZCutsceneST->signPost = (Entity *)signPost;
        foreach_break;
    }

    foreach_all(CutsceneHBH, hbh)
    {
        if (hbh->characterID == HBH_MYSTIC)
            MSZCutsceneST->mystic = hbh;
        if (hbh->characterID == HBH_ROUGESA) {
            MSZCutsceneST->rouges[0]         = hbh;
            MSZCutsceneST->rougePlatforms[0] = RSDK_GET_ENTITY(RSDK.GetEntityID(hbh) + 1, Armadiloid);
        }
        if (hbh->characterID == HBH_ROUGESB) {
            MSZCutsceneST->rouges[1]         = hbh;
            MSZCutsceneST->rougePlatforms[1] = RSDK_GET_ENTITY(RSDK.GetEntityID(hbh) + 1, Armadiloid);
        }
        if (hbh->characterID == HBH_ROUGESC) {
            MSZCutsceneST->rouges[2]         = hbh;
            MSZCutsceneST->rougePlatforms[2] = RSDK_GET_ENTITY(RSDK.GetEntityID(hbh) + 1, Armadiloid);
        }
    }

    foreach_all(Tornado, tornado)
    {
        MSZCutsceneST->tornado = (Entity *)tornado;
        foreach_break;
    }

    MSZCutsceneST->sfxExplosion3 = RSDK.GetSFX("Stage/Explosion3.wav");
    MSZCutsceneST->sfxMayday     = RSDK.GetSFX("MSZ/Mayday.wav");
    MSZCutsceneST->sfxLedgeBreak = RSDK.GetSFX("Stage/LedgeBreak.wav");
    MSZCutsceneST->active        = ACTIVE_ALWAYS;
}

void MSZCutsceneST_SetupCutscene(void)
{
    RSDK_THIS(MSZCutsceneST);
    void *states[] = { MSZCutsceneST_CutsceneState_Unknown1, MSZCutsceneST_CutsceneState_Unknown2,
                       MSZCutsceneST_CutsceneState_Unknown3, MSZCutsceneST_CutsceneState_Unknown4,
                       MSZCutsceneST_CutsceneState_Unknown5, MSZCutsceneST_CutsceneState_Unknown6,
                       MSZCutsceneST_CutsceneState_Unknown7, MSZCutsceneST_CutsceneState_Unknown8,
                       MSZCutsceneST_CutsceneState_Unknown9, NULL };
    CutsceneSeq_StartSequence((Entity *)entity, states);
}

bool32 MSZCutsceneST_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    EntitySignPost *signPost  = (EntitySignPost *)MSZCutsceneST->signPost;
    EntityCutsceneHBH *mystic = MSZCutsceneST->mystic;
    if (!host->timer) {
        RSDK.PlaySfx(SignPost->sfx_Twinkle, false, 255);
        signPost->state = SignPost_State_Fall;
        mystic->visible = true;
        foreach_all(ParallaxSprite, sprite) { sprite->visible = true; }
    }

    mystic->position.x = signPost->position.x - 0x30000;
    if (signPost->position.y >= MSZCutsceneST->signPostOffsets[mystic->animator.frameID] + mystic->position.y - 0x3D0000) {
        signPost->position.y = MSZCutsceneST->signPostOffsets[mystic->animator.frameID] + mystic->position.y - 0x3D0000;
        signPost->state      = SignPost_State_Land;
        RSDK.PlaySfx(SignPost->sfx_Slide, false, 255);
        signPost->spinCount  = 4;
        signPost->velocity.y = 0;
        Music_FadeOut(0.025);
        return true;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntitySignPost *signPost  = (EntitySignPost *)MSZCutsceneST->signPost;
    EntityCutsceneHBH *mystic = MSZCutsceneST->mystic;

    signPost->position.y = MSZCutsceneST->signPostOffsets[mystic->animator.frameID] + mystic->position.y - 0x3D0000;
    if (signPost->state == SignPost_State_Finish) {
        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        player1->right     = false;
        player1->left      = false;
        player1->down      = false;
        player1->up        = false;
        player1->jumpPress = false;
        player1->jumpHold  = false;
    }

    if (MSZCutsceneST->actFinishFlag) {
        mystic->oscillate = false;

        TileLayer *fgLow = RSDK.GetSceneLayer(Zone->fgLow);
        for (int32 i = 0; i < fgLow->scrollInfoCount; ++i) {
            fgLow->scrollInfo[i].parallaxFactor = 0x100;
            fgLow->scrollInfo[i].scrollSpeed    = 0;
        }

        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
            RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_NEXTSCENE;
        return true;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityTornado *tornado    = (EntityTornado *)MSZCutsceneST->tornado;
    EntitySignPost *signPost  = (EntitySignPost *)MSZCutsceneST->signPost;
    EntityCutsceneHBH *mystic = MSZCutsceneST->mystic;

    if (!host->timer) {
        Music_PlayTrack(TRACK_HBHMISCHIEF);
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_RIDE, &player1->playerAnimator, false, 0);
        player1->direction = FLIP_NONE;
    }

    if (!host->field_6C[0]) {
        int x = (tornado->position.x & 0xFFFF0000) + 0x200000;
        if (player1->position.x != x)
            player1->position.x += (x - player1->position.x) >> 1;

        if (abs(player1->position.x - x) < 0x10000) {
            player1->position.x   = x;
            MSZCutsceneST->value4 = tornado->position;
            host->field_68        = host->timer;
            host->field_6C[0]     = 1;
        }
    }
    tornado->drawOrder = Zone->playerDrawLow;
    player1->drawOrder = Zone->playerDrawHigh;
    mystic->velocity.y -= 0x800;
    mystic->position.y += mystic->velocity.y;
    mystic->drawOrder    = Zone->drawOrderLow;
    signPost->position.y = mystic->position.y - 0x3D0000 + MSZCutsceneST->signPostOffsets[mystic->animator.frameID];

    Vector2 range;
    range.x = RSDK_screens->width << 16;
    range.y = RSDK_screens->height << 16;
    if (!RSDK.CheckOnScreen(mystic, &range)) {
        host->field_6C[2] = 1;
        destroyEntity(mystic);
        return true;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityTornado *tornado     = (EntityTornado *)MSZCutsceneST->tornado;
    EntityTornadoPath *nodePtr = (EntityTornadoPath *)MSZCutsceneST->nodePtr;
    int x                      = nodePtr->position.x - 0x600000;
    int y                      = nodePtr->position.y + 0x300000;
    if (host->timer - host->field_68 == 120 && !host->field_6C[1]) {
        tornado->position.x = x;
        tornado->position.y = y;
        host->field_6C[0]   = 1;
        return true;
    }
    else {
        MathHelpers_Lerp4(&tornado->position, 255 * (host->timer - host->field_68) / 120, MSZCutsceneST->value4.x, MSZCutsceneST->value4.y, x, y);
        tornado->position.x &= 0xFFFF0000;
        tornado->position.y &= 0xFFFF0000;

        player1->position.x = (tornado->position.x & 0xFFFF0000) + 0x200000;
        if (player1->characterID == ID_TAILS)
            player1->position.y = (tornado->position.y & 0xFFFF0000) - 0x2C0000;
        else
            player1->position.y = (tornado->position.y & 0xFFFF0000) - 0x300000;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_Unknown5(EntityCutsceneSeq *host)
{
    bool32 (*rougeStates[3])(EntityCutsceneSeq *);
    rougeStates[0] = MSZCutsceneST_CutsceneState_ShowRougeA;
    rougeStates[1] = MSZCutsceneST_CutsceneState_ShowRougeB;
    rougeStates[2] = MSZCutsceneST_CutsceneState_ShowRougeC;

    if (!host->timer) {
        MSZCutsceneST->rougeID = RSDK.Rand(0, 8) % 3;

        for (int i = 0; i < 3; ++i) {
            if (i != MSZCutsceneST->rougeID) {
                destroyEntity(MSZCutsceneST->rouges[i]);
                destroyEntity(MSZCutsceneST->rougePlatforms[i]);
            }
        }
    }
    EntityArmadiloid *armadiloid         = MSZCutsceneST->rougePlatforms[MSZCutsceneST->rougeID];
    armadiloid->drawOrder                = Zone->drawOrderHigh;
    armadiloid->animator3.animationSpeed = 1;
    armadiloid->velocity.x               = 0;

    return rougeStates[MSZCutsceneST->rougeID](host);
}

bool32 MSZCutsceneST_CutsceneState_ShowRougeA(EntityCutsceneSeq *host)
{
    EntityTornado *tornado       = (EntityTornado *)MSZCutsceneST->tornado;
    EntityTornadoPath *nodePtr   = (EntityTornadoPath *)MSZCutsceneST->nodePtr;
    EntityCutsceneHBH *rouge     = MSZCutsceneST->rouges[MSZCutsceneST->rougeID];
    EntityArmadiloid *armadiloid = MSZCutsceneST->rougePlatforms[MSZCutsceneST->rougeID];
    if (!host->timer)
        armadiloid->state = Armadiloid_State_PlatformFlying;

    int x = nodePtr->position.x + 0x600000;
    int y = armadiloid->startPos.y;

    if (armadiloid->position.x > x) {
        MathHelpers_Lerp3(&armadiloid->position, 255 * host->timer / 180, armadiloid->startPos.x, armadiloid->startPos.y, x, y);
        armadiloid->position.x &= 0xFFFF0000;
        armadiloid->position.y &= 0xFFFF0000;
    }

    if (host->timer == 180) {
        armadiloid->position.x = x;
        armadiloid->position.y = y;
        host->field_6C[0]      = 1;
    }

    if (host->field_6C[0] && !host->field_6C[1]) {
        if (!rouge->animator.animationID)
            RSDK.SetSpriteAnimation(rouge->spriteIndex, 1, &rouge->animator, true, 0);
        if (rouge->animator.animationID == 1 && rouge->animator.frameID == rouge->animator.frameCount - 1 && rouge->animator.animationTimer == 12) {
            RSDK.SetSpriteAnimation(rouge->spriteIndex, 2, &rouge->animator, true, 0);
            rouge->velocity.y = -0x20000;
            RSDK.PlaySfx(HeavyMystic->sfxPon, false, 255);
            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, rouge->position.x, rouge->position.y - 0x40000);
            debris->position.x -= 0x180000;
            debris->velocity.x = -0x20000;
            debris->drawOrder  = Zone->playerDrawHigh;
            debris->state      = Debris_State_LightningSpark;
            debris->direction  = FLIP_X;
            debris->drawFX     = FX_FLIP;
            RSDK.SetSpriteAnimation(rouge->spriteIndex, 4, &debris->animator, true, 0);
            MSZCutsceneST->projectile = (Entity *)debris;
        }
        if (rouge->animator.animationID == 2) {
            rouge->position.y += rouge->velocity.y;
            rouge->velocity.y += 0x3800;
        }
    }

    rouge->position.x = armadiloid->position.x + 0x20000;
    if (rouge->position.y > armadiloid->position.y - 0x200000) {
        rouge->position.y = armadiloid->position.y - 0x200000;
        rouge->velocity.y = 0;
        if (rouge->animator.animationID == 2) {
            RSDK.SetSpriteAnimation(rouge->spriteIndex, 1, &rouge->animator, true, 2);
            rouge->animator.loopIndex = 3;
            host->field_6C[1]         = 1;
        }
    }

    if (MSZCutsceneST->projectile && MSZCutsceneST->projectile->position.x <= tornado->position.x + 0x300000) {
        destroyEntity(MSZCutsceneST->projectile);
        return true;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_ShowRougeB(EntityCutsceneSeq *host)
{
    EntityTornado *tornado       = (EntityTornado *)MSZCutsceneST->tornado;
    EntityTornadoPath *nodePtr   = (EntityTornadoPath *)MSZCutsceneST->nodePtr;
    EntityCutsceneHBH *rouge     = MSZCutsceneST->rouges[MSZCutsceneST->rougeID];
    EntityArmadiloid *armadiloid = MSZCutsceneST->rougePlatforms[MSZCutsceneST->rougeID];

    int x = nodePtr->position.x + 0x600000;
    int y = armadiloid->startPos.y;

    if (!host->timer)
        armadiloid->state = Armadiloid_State_PlatformFlying;

    if (armadiloid->position.x > x) {
        MathHelpers_Lerp3(&armadiloid->position, 255 * host->timer / 180, armadiloid->startPos.x, armadiloid->startPos.y, x, y);
        armadiloid->position.x &= 0xFFFF0000;
        armadiloid->position.y &= 0xFFFF0000;
    }

    if (host->timer == 180) {
        armadiloid->position.x = x;
        armadiloid->position.y = y;
        host->field_6C[0]      = 1;
    }

    if (host->field_6C[0] && !host->field_6C[1]) {
        if (rouge->animator.animationID == 5) {
            RSDK.SetSpriteAnimation(rouge->spriteIndex, 7, &rouge->animator, true, 0);
        }
        if (rouge->animator.animationID == 7) {
            if (rouge->animator.frameID == 2 && !MSZCutsceneST->projectile) {
                RSDK.PlaySfx(HeavyMystic->sfxDrop, false, 255);
                EntityDebris *debris = CREATE_ENTITY(Debris, NULL, rouge->position.x, rouge->position.y - 0x130000);
                debris->position.x += 0xB0000;
                debris->velocity.x = -0x48000;
                debris->velocity.y = -0x40000;
                debris->gravity    = 0x3800;
                debris->drawOrder  = Zone->playerDrawHigh;
                debris->state      = Debris_State_Fall;
                RSDK.SetSpriteAnimation(rouge->spriteIndex, 11, &debris->animator, true, 0);
                MSZCutsceneST->projectile = (Entity *)debris;
            }
            if (rouge->animator.animationID == 7 && rouge->animator.frameID == 5 && rouge->animator.animationTimer == 2) {
                RSDK.SetSpriteAnimation(rouge->spriteIndex, 5, &rouge->animator, true, 0);
                host->field_6C[1] = 1;
            }
        }
    }

    rouge->position.x = armadiloid->position.x + 0x40000;
    rouge->position.y = armadiloid->position.y - 0x200000;

    if (MSZCutsceneST->projectile && MSZCutsceneST->projectile->position.y >= tornado->position.y - 0x100000) {
        destroyEntity(MSZCutsceneST->projectile);
        return true;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_ShowRougeC(EntityCutsceneSeq *host)
{
    EntityTornado *tornado       = (EntityTornado *)MSZCutsceneST->tornado;
    EntityTornadoPath *nodePtr   = (EntityTornadoPath *)MSZCutsceneST->nodePtr;
    EntityCutsceneHBH *rouge     = MSZCutsceneST->rouges[MSZCutsceneST->rougeID];
    EntityArmadiloid *armadiloid = MSZCutsceneST->rougePlatforms[MSZCutsceneST->rougeID];

    int y = armadiloid->startPos.y;
    int x = (nodePtr->position.x + 0x600000);

    if (!host->timer)
        armadiloid->state = Armadiloid_State_PlatformFlying;

    if (armadiloid->position.x > x) {
        MathHelpers_Lerp3(&armadiloid->position, 255 * host->timer / 180, armadiloid->startPos.x, armadiloid->startPos.y, x, y);
        armadiloid->position.x &= 0xFFFF0000;
        armadiloid->position.y &= 0xFFFF0000;
    }

    if (host->timer == 180) {
        armadiloid->position.x = x;
        armadiloid->position.y = y;
        host->field_6C[0]      = 1;
    }

    if (host->field_6C[0] && !host->field_6C[1]) {
        if (rouge->animator.animationID == 12)
            RSDK.SetSpriteAnimation(rouge->spriteIndex, 14, &rouge->animator, true, 0);
        if (rouge->animator.animationID == 14) {
            if (rouge->animator.frameID == 3 && rouge->animator.animationTimer == 1) {
                RSDK.PlaySfx(HeavyMystic->sfxImpact2, false, 255);
                Camera_ShakeScreen(0, 0, 2);
                armadiloid->timer = 0;
                armadiloid->state = Armadiloid_State_PlatformShoot;
                RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 2, &armadiloid->animator2, true, 0);
            }
            if (rouge->animator.animationID == 14 && rouge->animator.frameID == 7 && rouge->animator.animationTimer == 2) {
                RSDK.SetSpriteAnimation(rouge->spriteIndex, 12, &rouge->animator, true, 0);
                host->field_6C[1] = 1;
            }
        }
    }

    if (!MSZCutsceneST->projectile) {
        foreach_active(Projectile, projectile)
        {
            MSZCutsceneST->projectile = (Entity *)projectile;
            projectile->drawOrder     = Zone->playerDrawHigh;
            foreach_break;
        }
    }

    rouge->position.x = armadiloid->position.x + 0x40000;
    rouge->position.y = armadiloid->position.y - 0x2C0000;

    if (MSZCutsceneST->projectile && MSZCutsceneST->projectile->position.x <= tornado->position.x + 0x300000) {
        destroyEntity(MSZCutsceneST->projectile);
        return true;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_Unknown6(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);
    EntityTornado *tornado     = (EntityTornado *)MSZCutsceneST->tornado;
    EntityTornadoPath *nodePtr = (EntityTornadoPath *)MSZCutsceneST->nodePtr;

    if (!host->timer) {
        RSDK.PlaySfx(MSZCutsceneST->sfxExplosion3, false, 255);
        host->fillTimerA        = 512;
        MSZCutsceneST->isMayday = true;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &tornado->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &tornado->animator3, true, 0);
        tornado->drawFX |= FX_ROTATE;
        tornado->state     = Tornado_Unknown7;
        tornado->rotation  = 48;
        player1->state     = Player_State_None;
        player1->direction = FLIP_X;
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_CLING, &player1->playerAnimator, true, 0);
        player1->rotation = tornado->rotation;
        tornado->active   = ACTIVE_NORMAL;
        nodePtr->state    = TornadoPath_Unknown3;
        destroyEntity(MSZCutsceneST->rouges[MSZCutsceneST->rougeID]);
        destroyEntity(MSZCutsceneST->rougePlatforms[MSZCutsceneST->rougeID]);

        TileLayer *bg1 = RSDK.GetSceneLayer(0);
        for (int32 i = 0; i < bg1->scrollInfoCount; ++i) {
            bg1->scrollInfo[i].scrollPos = 0;
        }

        TileLayer *bg2 = RSDK.GetSceneLayer(1);
        for (int32 i = 0; i < bg2->scrollInfoCount; ++i) {
            bg2->scrollInfo[i].scrollPos = 0;
        }

        foreach_active(ParallaxSprite, sprite) { sprite->scrollPos.x = 0; }

        MSZSetup_Unknown4(2048);
    }
    if (!host->field_6C[2]) {
        player1->position.x = tornado->position.x - 0x280000;
        if (player1->characterID == ID_TAILS)
            player1->position.y = tornado->position.y - 0x340000;
        else
            player1->position.y = tornado->position.y - 0x380000;
    }

    if (!host->field_6C[0]) {
        host->fillTimerA = clampVal(512 - ((host->timer - 45) << 9) / 60, 0, 512);
        if (!host->fillTimerA)
            host->field_6C[0] = 1;
    }

    if (host->timer == 150) {
        player1->onGround        = true;
        player1->velocity.x      = 0;
        player1->velocity.y      = 0;
        player1->groundVel       = 0;
        player1->nextAirState    = StateMachine_None;
        player1->nextGroundState = StateMachine_None;
        Player_StartJump(player1);
        player1->direction  = FLIP_NONE;
        player1->velocity.x = 0xE0000;
        host->field_6C[2]   = 1;
        if (player2->objectID == Player->objectID) {
            player2->state  = MSZSetup_Player_State_PostCrashJumpIn;
            player2->active = ACTIVE_NORMAL;
        }
    }
    if (TornadoPath->field_8.y > 0 && !(Zone->timer % 5))
        Camera_ShakeScreen(2, 0, 2);

    if (TornadoPath->field_8.y < 0 && !host->field_6C[3]) {
        MSZCutsceneST->isMayday = false;
        Camera_ShakeScreen(0, 0, 5);
        MSZSetup_Unknown4(512);
        host->field_6C[3] = 1;
        tornado->onGround = true;
        host->field_68    = host->timer;
        Music_FadeOut(0.02);
        RSDK.PlaySfx(MSZCutsceneST->sfxLedgeBreak, false, 255);
    }

    if (tornado->onGround && !((host->timer - host->field_68) % 16)) {
        EntityDebris *debris = CREATE_ENTITY(Debris, NULL, tornado->position.x, tornado->position.y);
        debris->position.x += 0x2C0000;
        debris->position.y += 0xC0000;
        debris->drawOrder = Zone->drawOrderLow;
        debris->state     = Debris_State_LightningSpark;
        debris->timer     = 53;
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &debris->animator, true, 0);
        MSZCutsceneST->projectile = (Entity *)debris;
    }

    if (TornadoPath->flag) {
        MSZSetup_Unknown4(0);
        tornado->state = StateMachine_None;
        return true;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_Unknown7(EntityCutsceneSeq *host)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (!host->timer) {
        player1->position.x = 0x3DA00000;
        player1->position.y = 0x5300000;
    }
    return host->timer == 90;
}

bool32 MSZCutsceneST_CutsceneState_Unknown8(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);

    if (!host->timer) {
        TornadoPath->cameraPtr = NULL;
        camera->boundsB        = 0x5A00000;
        Camera_SetupLerp(0, 0, player1->position.x + 0x340000, player1->position.y - 0x140000, 2);
        MSZSetup_Unknown4(1024);
    }
    if (camera->position.x == player1->position.x + 0x340000 && camera->position.y == player1->position.y - 0x140000) {
        MSZSetup_Unknown4(0);

        TileLayer *bg1 = RSDK.GetSceneLayer(0);
        for (int32 i = 0; i < bg1->scrollInfoCount; ++i) {
            bg1->scrollInfo[i].scrollPos &= 0xFFFF0000;
        }

        TileLayer *bg2 = RSDK.GetSceneLayer(1);
        for (int32 i = 0; i < bg2->scrollInfoCount; ++i) {
            bg2->scrollInfo[i].scrollPos &= 0xFFFF0000;
        }

        foreach_active(ParallaxSprite, sprite) { sprite->scrollPos.x &= 0xFFFF0000; }

        Zone->screenBoundsL1[0] = (player1->position.x >> 16) - 160;
        Zone->screenBoundsB1[0] = (player1->position.y >> 16) - 112;
        if (player2->objectID != Player->objectID || player2->onGround)
            return true;
    }
    return false;
}

bool32 MSZCutsceneST_CutsceneState_Unknown9(EntityCutsceneSeq *host)
{
    if (host->timer == 10) {
        MSZSetup_StoreMSZ1STScrollPos();
        return true;
    }
    return false;
}

#if RETRO_INCLUDE_EDITOR
void MSZCutsceneST_EditorDraw(void)
{
    RSDK_THIS(MSZCutsceneST);
    CutsceneRules_DrawCutsceneBounds(entity, &entity->size);
}

void MSZCutsceneST_EditorLoad(void) {}
#endif

void MSZCutsceneST_Serialize(void) { RSDK_EDITABLE_VAR(MSZCutsceneST, VAR_VECTOR2, size); }
