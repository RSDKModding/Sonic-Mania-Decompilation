#include "SonicMania.h"

ObjectScrewMobile *ScrewMobile;

void ScrewMobile_Update(void)
{
    RSDK_THIS(ScrewMobile);
    StateMachine_Run(entity->state);
}

void ScrewMobile_LateUpdate(void) {}

void ScrewMobile_StaticUpdate(void)
{

    foreach_active(ScrewMobile, screwMobile)
    {
        if (screwMobile->state != ScrewMobile_State2_Unknown1) {
            RSDK.AddDrawListRef(Zone->playerDrawLow - 1, RSDK.GetEntityID(screwMobile));
        }
    }

    if (ScrewMobile->shouldPlayFanSfx) {
        if (!ScrewMobile->playingFanSfx) {
            RSDK.PlaySfx(ScrewMobile->sfxBigFan, 47208, 255);
            ScrewMobile->playingFanSfx = true;
        }
    }
    else if (ScrewMobile->playingFanSfx) {
        RSDK.StopSFX(ScrewMobile->sfxBigFan);
        ScrewMobile->playingFanSfx = false;
    }
    ScrewMobile->shouldPlayFanSfx = 0;
}

void ScrewMobile_Draw(void)
{
    RSDK_THIS(ScrewMobile);
    StateMachine_Run(entity->stateDraw);
}

void ScrewMobile_Create(void *data)
{
    RSDK_THIS(ScrewMobile);

    if (globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(entity);
    }
    else {
        entity->drawFX = FX_FLIP;
        if (!RSDK_sceneInfo->inEditor) {
            entity->visible = true;
            if (data) {
                if (voidToInt(data) == 1) {
                    entity->active        = ACTIVE_NORMAL;
                    entity->drawOrder     = Zone->hudDrawOrder - 2;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->field_74      = 0x3800;
                    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 6, &entity->animator1, true, 0);
                    entity->state     = ScrewMobile_State2_Unknown1;
                    entity->stateDraw = ScrewMobile_StateDraw2_Unknown;
                }
            }
            else {
                entity->active        = ACTIVE_BOUNDS;
                entity->drawOrder     = Zone->hudDrawOrder - 1;
                entity->updateRange.x = 0x800000;
                entity->updateRange.y = 0x800000;
                entity->startPos      = entity->position;
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 1, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &entity->animator3, true, 6);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 2, &entity->animator4, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 3, &entity->animator5, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 5, &entity->animator6, true, 0);
                entity->field_7C                 = 8;
                entity->animator2.animationSpeed = 0;
                entity->state                    = ScrewMobile_State_Unknown1;
                entity->stateDraw                = ScrewMobile_StateDraw1_Unknown;
            }
        }
    }
}

void ScrewMobile_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        ScrewMobile->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ScrewMobile.bin", SCOPE_STAGE);

    ScrewMobile->hitbox1.left   = -32;
    ScrewMobile->hitbox1.top    = -20;
    ScrewMobile->hitbox1.right  = 32;
    ScrewMobile->hitbox1.bottom = 20;

    ScrewMobile->hitbox2.left   = -8;
    ScrewMobile->hitbox2.top    = -4;
    ScrewMobile->hitbox2.right  = 8;
    ScrewMobile->hitbox2.bottom = 4;

    ScrewMobile->playingFanSfx    = false;
    ScrewMobile->shouldPlayFanSfx = 0;
    ScrewMobile->active           = ACTIVE_ALWAYS;
    ScrewMobile->sfxButton        = RSDK.GetSFX("Stage/Button2.wav");
    ScrewMobile->sfxEggMobile     = RSDK.GetSFX("HCZ/EggMobile.wav");
    ScrewMobile->sfxBigFan        = RSDK.GetSFX("HCZ/BigFan.wav");
    ScrewMobile->sfxImpact        = RSDK.GetSFX("Stage/Impact5.wav");
}

void ScrewMobile_State_Unknown1(void)
{
    RSDK_THIS(ScrewMobile);

    Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 32;
    Zone->screenBoundsR2[0]     = Zone->screenBoundsR1[0] << 16;
    Zone->playerBoundActiveR[0] = true;

    if (Player->playerCount > 1) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (Player_CheckCollisionTouch(player2, entity, &ScrewMobile->hitbox1)) {
            player2->direction      = FLIP_X;
            player2->state          = Player_State_None;
            player2->velocity.x     = 0;
            player2->velocity.y     = 0;
            player2->groundVel      = 0;
            player2->position.x     = entity->position.x;
            player2->position.y     = entity->position.y - 0x100000;
            player2->tileCollisions = true;
            RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimator, true, 0);
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (Player_CheckCollisionTouch(player1, entity, &ScrewMobile->hitbox1)) {
        Zone->screenBoundsT1[0]     = (entity->position.y >> 16) - 40;
        Zone->screenBoundsR2[0]     = 0x7FFF;
        Zone->screenBoundsR1[0]     = 0x7FFF;
        Zone->playerBoundActiveR[0] = false;
        player1->state              = Player_State_None;
        player1->velocity.x         = 0;
        player1->velocity.y         = 0;
        player1->groundVel          = 0;
        player1->direction          = FLIP_X;
        player1->position.x         = entity->position.x;
        player1->position.y         = entity->position.y - 0x100000;
        player1->tileCollisions     = true;
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, true, 0);

        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        RSDK.PlaySfx(ScrewMobile->sfxEggMobile, false, 255);
        DCEvent->finished = true;
        entity->state     = ScrewMobile_State_Unknown2;
    }
}

void ScrewMobile_State_Unknown2(void)
{
    RSDK_THIS(ScrewMobile);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);
    RSDK.ProcessAnimation(&entity->animator6);

    entity->position.y = BadnikHelpers_Oscillate(entity->startPos.y, 2, 10);
    entity->position.x += entity->velocity.x;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (Player_CheckValidState(player1)) {
        player1->position.x = entity->position.x;
        player1->position.y = entity->position.y;
        player1->position.y -= 0x100000;
        player1->velocity.x     = 0;
        player1->velocity.y     = 0;
        player1->outtaHereTimer = 0;
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, true, 0);

        if (player1->left || player1->right) {
            if (player1->left) {
                entity->velocity.x -= 0xC00;
                if (entity->velocity.x > 0)
                    entity->velocity.x -= 0x1200;
                else if (entity->velocity.x < -0x20000)
                    entity->velocity.x = -0x20000;
            }
            else if (player1->right) {
                entity->velocity.x += 0xC00;
                if (entity->velocity.x < 0)
                    entity->velocity.x += 0x1200;
                else if (entity->velocity.x > 0x20000)
                    entity->velocity.x = 0x20000;
            }
        }
        else {
            if (entity->velocity.x) {
                if (entity->velocity.x >= 0) {
                    entity->velocity.x -= 0xC00;
                    if (entity->velocity.x < 0)
                        entity->velocity.x = 0;
                }
                else {
                    entity->velocity.x += 0xC00;
                    if (entity->velocity.x > 0)
                        entity->velocity.x = 0;
                }
            }
        }

        if (entity->velocity.x) {
            if (entity->velocity.x <= 0) {
                entity->direction = FLIP_NONE;
                if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x200000, 0, true)) {
                    if (entity->velocity.x <= -0x20000)
                        RSDK.PlaySfx(ScrewMobile->sfxImpact, false, 255);
                    entity->velocity.x = 0;
                }
            }
            else {
                entity->direction = FLIP_X;
                if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x200000, 0, true)) {
                    if (entity->velocity.x >= 0x20000)
                        RSDK.PlaySfx(ScrewMobile->sfxImpact, false, 255);
                    entity->velocity.x = 0;
                }
            }
        }

        player1->direction = entity->direction ^ FLIP_X;
    }

    if (Player->playerCount >= 2) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (Player_CheckValidState(player2)) {
            if (player2->state == Player_State_None) {
                player2->position.x     = entity->position.x;
                player2->position.y     = entity->position.y - 0x100000;
                player2->velocity.x     = 0;
                player2->velocity.y     = 0;
                player2->outtaHereTimer = 0;
                player2->direction      = entity->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimator, true, 0);
            }
            else if (Player_CheckCollisionTouch(player2, entity, &ScrewMobile->hitbox1)) {
                player2->state          = Player_State_None;
                player2->velocity.x     = 0;
                player2->velocity.y     = 0;
                player2->groundVel      = 0;
                player2->direction      = FLIP_X;
                player2->position.x     = entity->position.x;
                player2->position.y     = entity->position.y - 0x100000;
                player2->drawOrder      = Zone->playerDrawLow;
                player2->tileCollisions = true;
                player2->direction      = entity->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimator, true, 0);
            }
        }
    }

    if (Player_CheckValidState(player1)) {
        if (player1->jumpPress) {
            if (!Zone->screenBoundsT1[0]) {
                Player_StartJump(player1);
                entity->state = ScrewMobile_State_Idle;
                return;
            }
            if (!entity->whirlPoolTimer && ScrewMobile->playingFanSfx == false) {
                entity->animator2.animationSpeed = 64;
                RSDK.PlaySfx(ScrewMobile->sfxButton, false, 255);
            }
        }
        if (!player1->jumpHold || entity->whirlPoolTimer) {
            EntityWhirlpool *whirlpool = (EntityWhirlpool *)entity->whirlpool;
            if (whirlpool) {
                if (whirlpool->objectID == Whirlpool->objectID)
                    whirlpool->activePlayers = -3;
                entity->whirlpool = NULL;
            }

            if (entity->animator2.animationSpeed)
                entity->animator2.animationSpeed -= 2;

            if (entity->animator2.animationSpeed > 0xC0)
                ++ScrewMobile->shouldPlayFanSfx;
        }
        else {
            if (entity->field_74 > 0) {
                EntityWhirlpool *whirlpool = (EntityWhirlpool *)entity->whirlpool;
                if (whirlpool) {
                    if (whirlpool->objectID == Whirlpool->objectID)
                        whirlpool->position.x = entity->position.x;
                    else
                        entity->whirlpool = NULL;
                }
                else {
                    Vector2 pos;
                    pos.x                    = 0x1000000;
                    pos.y                    = 0x1000000;
                    whirlpool                = CREATE_ENTITY(Whirlpool, &pos, entity->position.x, entity->startPos.y + 0xF00000);
                    whirlpool->activePlayers = -1;
                    whirlpool->angVel        = 10;
                    whirlpool->alpha         = 0;
                    whirlpool->isPermanent   = true;
                    entity->whirlpool        = (Entity *)whirlpool;
                }
            }

            if (entity->animator2.animationSpeed < 0x100)
                entity->animator2.animationSpeed += 2;

            if (entity->animator2.animationSpeed > 0x40)
                ++ScrewMobile->shouldPlayFanSfx;
        }
    }

    if (entity->animator2.animationSpeed < 0xF0) {
        if (entity->field_74 > 0) {
            entity->field_74--;
        }
    }
    else if (entity->field_74 < 86) {
        entity->field_74++;
    }

    if (player1->down) {
        if (!entity->bombPress && !entity->timer && entity->animator3.frameID == 6) {
            RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &entity->animator3, true, 0);
            entity->bombPress = true;
        }
    }
    else {
        entity->bombPress = false;
    }

    if (entity->animator3.frameID == 2 && !entity->timer) {
        entity->timer            = 60;
        EntityScrewMobile *child = CREATE_ENTITY(ScrewMobile, intToVoid(1), entity->position.x, entity->position.y + 0x130000);
        if (entity->direction)
            child->position.x -= 0x350000;
        else
            child->position.x += 0x350000;
        child->direction  = entity->direction;
        child->velocity.y = -0x7000;
    }

    if (entity->whirlPoolTimer > (player1->jumpHold == true))
        entity->whirlPoolTimer--;

    if (entity->timer)
        entity->timer--;

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;
}

void ScrewMobile_State_BossFinished(void)
{
    RSDK_THIS(ScrewMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->up           = false;
    player1->down         = false;
    player1->left         = false;
    player1->right        = true;
    player1->jumpPress    = false;
    player1->jumpHold     = false;
    ScrewMobile_State_Unknown2();
    Zone->screenBoundsL1[0] = RSDK_screens->position.x;
    Zone->screenBoundsL2[0] = Zone->screenBoundsL1[0] << 16;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x200000, 0, true)) {
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        Zone->screenBoundsL1[0] += 2;
        RSDK_screens->position.x = Zone->screenBoundsL1[0];
        Zone->screenBoundsL2[0]  = Zone->screenBoundsL1[0] << 16;
        camera->position.x       = Zone->screenBoundsL1[0] << 16;
        if (Zone->screenBoundsL1[0] >= (entity->position.x >> 16) - 32)
            entity->state = ScrewMobile_State_Idle;
    }
}

void ScrewMobile_State_Idle(void)
{
    RSDK_THIS(ScrewMobile);

    entity->position.y = BadnikHelpers_Oscillate(entity->startPos.y, 2, 10);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->state == Player_State_None) {
        if (player1->jumpPress) {
            Player_StartJump(player1);
        }
        else {
            player1->position.x = entity->position.x;
            player1->position.y = entity->position.y;
            player1->position.y -= 0x100000;
            player1->velocity.x     = 0;
            player1->velocity.y     = 0;
            player1->outtaHereTimer = 0;
            player1->direction      = entity->direction ^ FLIP_X;
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, true, 0);
        }
    }
    else if (player1->velocity.y >= 0 && Player_CheckCollisionTouch(player1, entity, &ScrewMobile->hitbox1)) {
        player1->state      = Player_State_None;
        player1->velocity.x = 0;
        player1->velocity.y = 0;
        player1->groundVel  = 0;
        player1->direction  = FLIP_X;
        player1->position.x = entity->position.x;
        player1->position.y = entity->position.y;
        player1->position.y -= 0x100000;
        player1->drawOrder      = Zone->playerDrawLow;
        player1->tileCollisions = true;
        player1->direction      = entity->direction ^ FLIP_X;
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, true, 0);
    }

    if (Player->playerCount >= 2) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (player2->state == Player_State_None) {
            if (player2->jumpPress) {
                Player_StartJump(player2);
            }
            else {
                player2->position.x = entity->position.x;
                player2->position.y = entity->position.y;
                player2->position.y -= 0x100000;
                player2->velocity.x     = 0;
                player2->velocity.y     = 0;
                player2->outtaHereTimer = 0;
                player2->direction      = entity->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimator, true, 0);
            }
        }
        else if (player2->velocity.y >= 0) {
            if (Player_CheckCollisionTouch(player2, entity, &ScrewMobile->hitbox1)) {
                player2->state      = Player_State_None;
                player2->velocity.x = 0;
                player2->velocity.y = 0;
                player2->groundVel  = 0;
                player2->direction  = FLIP_X;
                player2->position.x = entity->position.x;
                player2->position.y = entity->position.y;
                player2->position.y -= 0x100000;
                player2->drawOrder      = Zone->playerDrawLow;
                player2->tileCollisions = true;
                player2->direction      = entity->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimator, true, 0);
            }
        }
    }
}

void ScrewMobile_StateDraw1_Unknown(void)
{
    RSDK_THIS(ScrewMobile);
    Vector2 drawPos;

    if (RSDK_sceneInfo->currentDrawGroup == Zone->playerDrawLow - 1) {
        entity->animator1.frameID = 1;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
    else {
        if (entity->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

        entity->animator1.frameID = 0;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.SetPaletteEntry(0, 128, 0x0000);
        RSDK.DrawSprite(&entity->animator3, NULL, false);

        int storeDir = entity->direction;

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        if (entity->animator2.animationSpeed >= 0x40) {
            drawPos.x         = entity->position.x;
            drawPos.y         = entity->position.y;
            entity->inkEffect = INK_ALPHA;
            if (entity->field_74 <= 0) {
                entity->alpha = entity->animator2.animationSpeed;
            }
            else {
                entity->alpha = 0xD0;
                drawPos.y     = Water->waterLevel - 0x100000;

                SpriteFrame *frame = RSDK.GetFrame(ScrewMobile->aniFrames, 2, entity->animator4.frameID);

                frame->height = 32;
                frame->sprY   = 33 * entity->animator4.frameID + 181;
                frame->pivotY = -16;

                int count = entity->field_74 >> 5;
                for (int i = 0; i < count; ++i) {
                    RSDK.DrawSprite(&entity->animator4, &drawPos, false);
                    drawPos.y -= 0x200000;
                }

                frame->height = entity->field_74 % 32;
                frame->sprY += 31 - frame->height;
                frame->pivotY = 16 - frame->height;
                RSDK.DrawSprite(&entity->animator4, &drawPos, false);
                drawPos.y = Water->waterLevel;
                RSDK.DrawSprite(&entity->animator6, &drawPos, false);
                entity->alpha = 0x100;
            }
            drawPos.y = Water->waterLevel - (entity->field_74 << 16);
            RSDK.DrawSprite(&entity->animator5, &drawPos, false);
            entity->inkEffect = INK_NONE;
        }
        entity->direction = storeDir;
    }
}

void ScrewMobile_State2_Unknown1(void)
{
    RSDK_THIS(ScrewMobile);

    if (RSDK.CheckOnScreen(entity, NULL)) {
        entity->position.y += entity->velocity.y;
        if (entity->field_74 == 0x3800 && entity->position.y >= Water->waterLevel) {
            entity->velocity.y = entity->velocity.y >> 2;
            entity->field_74   = 0;
            CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), entity->position.x, Water->waterLevel);
            RSDK.PlaySfx(Water->sfx_Splash, false, 255);
        }
        entity->velocity.y += entity->field_74;

        foreach_active(DiveEggman, eggman)
        {
            if (eggman->state == DiveEggman_State2_Unknown1 || eggman->state == DiveEggman_State2_Unknown2 || eggman->state == DiveEggman_State2_Unknown3
                || eggman->state == DiveEggman_State2_Unknown4) {
                if (RSDK.CheckObjectCollisionTouchBox(entity, &ScrewMobile->hitbox2, eggman, &DiveEggman->hitbox2)) {
                    CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), eggman->position.x, eggman->position.y)->drawOrder = Zone->drawOrderHigh;
                    RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);
                    EntityWater *water = CREATE_ENTITY(Water, intToVoid(WATER_BUBBLE), eggman->position.x, eggman->position.y);

                    water->velocity.y = -0x8800;
                    water->angle      = 2 * RSDK.Rand(0, 256);
                    water->field_68   = water->position.x;
                    water->childPtr   = 0;
                    RSDK.SetSpriteAnimation(Water->spriteIndex, 3, &water->animator, true, 0);
                    destroyEntity(eggman);
                    destroyEntity(entity);
                    foreach_break;
                }
            }
            else if (RSDK.CheckObjectCollisionTouchBox(entity, &ScrewMobile->hitbox2, eggman, &DiveEggman->hitbox3)) {
                RSDK.PlaySfx(DiveEggman->sfxRockemSockem, false, 255);
                entity->velocity.x = RSDK.Rand(-4, 5) << 15;
                entity->velocity.y = -0x20000;
                entity->state      = ScrewMobile_State2_Unknown2;
                foreach_break;
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void ScrewMobile_State2_Unknown2(void)
{
    RSDK_THIS(ScrewMobile);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;
    entity->visible ^= FLIP_X;
    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void ScrewMobile_StateDraw2_Unknown(void)
{
    RSDK_THIS(ScrewMobile);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void ScrewMobile_EditorDraw(void)
{
    RSDK_THIS(ScrewMobile);

    entity->startPos = entity->position;
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &entity->animator3, true, 6);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 2, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 3, &entity->animator5, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 5, &entity->animator6, true, 0);
    entity->field_7C                 = 8;
    entity->animator2.animationSpeed = 0;

    ScrewMobile_StateDraw1_Unknown();
}

void ScrewMobile_EditorLoad(void) { ScrewMobile->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ScrewMobile.bin", SCOPE_STAGE); }
#endif

void ScrewMobile_Serialize(void) {}
