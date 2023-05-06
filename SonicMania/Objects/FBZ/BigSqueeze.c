// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BigSqueeze Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBigSqueeze *BigSqueeze;

void BigSqueeze_Update(void)
{
    RSDK_THIS(BigSqueeze);

    StateMachine_Run(self->state);
}

void BigSqueeze_LateUpdate(void) {}

void BigSqueeze_StaticUpdate(void) {}

void BigSqueeze_Draw(void)
{
    RSDK_THIS(BigSqueeze);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

void BigSqueeze_Create(void *data)
{
    RSDK_THIS(BigSqueeze);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->active = ACTIVE_BOUNDS;

            switch (self->type) {
                case BIGSQUEEZE_MANAGER:
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->visible       = false;

                    self->hitbox.left   = -256;
                    self->hitbox.top    = 0;
                    self->hitbox.right  = 256;
                    self->hitbox.bottom = 32;

                    self->state = BigSqueeze_StateManager_SetupIntro;
                    break;

                case BIGSQUEEZE_BOSS:
                    self->updateRange.x = 0x8000000;
                    self->updateRange.y = 0x1000000;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[1];

                    self->hitbox.left   = -32;
                    self->hitbox.top    = -16;
                    self->hitbox.right  = 32;
                    self->hitbox.bottom = 16;

                    self->timer = 6;
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 1, &self->animator, true, 0);
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 2, &self->domeAnimator, true, 0);
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 3, &self->prongsAnimator, true, 0);
                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 4, &self->wheelAnimator, true, 0);

                    self->stateDraw = BigSqueeze_Draw_Boss;
                    break;

                case BIGSQUEEZE_CRUSHER_L:
                case BIGSQUEEZE_CRUSHER_R:
                    if (self->type == BIGSQUEEZE_CRUSHER_R) {
                        self->direction = FLIP_X;
                        self->drawFX    = FX_FLIP;
                    }
                    self->updateRange.x = 0x8000000;
                    self->updateRange.y = 0x1000000;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[1];

                    RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 0, &self->animator, true, 0);

                    self->hitbox.left   = -24;
                    self->hitbox.top    = -120;
                    self->hitbox.right  = 24;
                    self->hitbox.bottom = 120;

                    self->stateDraw = BigSqueeze_Draw_Crusher;
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void BigSqueeze_StageLoad(void)
{
    BigSqueeze->active = ACTIVE_ALWAYS;

    BigSqueeze->aniFrames = RSDK.LoadSpriteAnimation("FBZ/BigSqueeze.bin", SCOPE_STAGE);

    BigSqueeze->sfxBossHit    = RSDK.GetSfx("Stage/BossHit.wav");
    BigSqueeze->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
    BigSqueeze->sfxMagnet     = RSDK.GetSfx("FBZ/Magnet.wav");
    BigSqueeze->sfxOrbinaut   = RSDK.GetSfx("FBZ/Orbinaut.wav");

    BigSqueeze->crushTimer = 0;
    BigSqueeze->isCrushing = false;

    Soundboard_LoadSfx("Stage/Rumble.wav", true, BigSqueeze_SfxCheck_Rumble, StateMachine_None);
}

bool32 BigSqueeze_SfxCheck_Rumble(void) { return BigSqueeze->isCrushing && SceneInfo->state == ENGINESTATE_REGULAR; }

void BigSqueeze_HandleWallCollisions(void)
{
    RSDK_THIS(BigSqueeze);

    foreach_active(Player, player)
    {
        int32 side = Player_CheckCollisionBox(player, self, &self->hitbox);

        if (side == C_RIGHT)
            player->collisionFlagH |= 2;
        else if (side == C_LEFT)
            player->collisionFlagH |= 1;
    }

    foreach_active(SignPost, signPost)
    {
        if (signPost->state == SignPost_State_Falling) {
            if (signPost->velocity.x >= 0) {
                if (signPost->position.x > self->position.x + ((self->hitbox.left - 24) << 16) && signPost->position.x < self->position.x)
                    signPost->velocity.x = -signPost->velocity.x;
            }
            else {
                if (signPost->position.x < self->position.x + ((self->hitbox.right + 24) << 16) && signPost->position.x > self->position.x)
                    signPost->velocity.x = -signPost->velocity.x;
            }
        }
    }
}

void BigSqueeze_CheckPlayerCollisions_Vulnerable(void)
{
    RSDK_THIS(BigSqueeze);

    if (!self->invincible) {
        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self))
                BigSqueeze_Hit();
        }
    }
}

void BigSqueeze_CheckPlayerCollisions_Electrified(void)
{
    RSDK_THIS(BigSqueeze);

    if (!self->invincible) {
        if (self->invincibilityTimer > 0)
            self->invincibilityTimer--;

        foreach_active(Player, player)
        {
            if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox)) {
                if (player->invincibleTimer || player->blinkTimer > 0 || player->shield == SHIELD_LIGHTNING) {
                    if (Player_CheckBossHit(player, self))
                        BigSqueeze_Hit();
                }
                else {
                    Player_Hurt(player, self);
                }
            }
        }
    }
}

void BigSqueeze_Hit(void)
{
    RSDK_THIS(BigSqueeze);

    if (--self->timer <= 0) {
        foreach_active(BigSqueeze, boss)
        {
            if (boss->type == BIGSQUEEZE_CRUSHER_L || boss->type == BIGSQUEEZE_CRUSHER_R)
                boss->state = BigSqueeze_HandleWallCollisions;
        }

        RSDK.SetSpriteAnimation(-1, 0, &self->electricAnimator, true, 0);

        self->state            = BigSqueeze_StateBoss_Destroyed;
        self->setupTimer       = 0;
        SceneInfo->timeEnabled = false;
        EntityPlayer *player   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        Player_GiveScore(player, 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(BigSqueeze->sfxBossHit, false, 255);
    }
}

void BigSqueeze_Explode(void)
{
    RSDK_THIS(BigSqueeze);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(BigSqueeze->sfxExplosion2, false, 0xFF);

        if (Zone->timer & 4) {
            int32 x = (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16) + self->position.x;
            int32 y = (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16) + self->position.y;
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void BigSqueeze_HandleBossMovement(void)
{
    RSDK_THIS(BigSqueeze);

    if (BigSqueeze->crushTimer < 10)
        self->position.x += self->velocity.x;

    foreach_active(BigSqueeze, boss)
    {
        if (boss->type == BIGSQUEEZE_CRUSHER_L) {
            if (self->position.x < boss->position.x + 0x340000 && self->velocity.x < 0)
                self->velocity.x = -self->velocity.x;
        }
        else if (boss->type == BIGSQUEEZE_CRUSHER_R) {
            if (self->position.x > boss->position.x - 0x340000 && self->velocity.x > 0)
                self->velocity.x = -self->velocity.x;
        }
    }

    RSDK.ProcessAnimation(&self->wheelAnimator);
}

// debrisInfo format:
// - count
// <for count entries>
// - frameID
// - velocityX
// - velocityY

void BigSqueeze_SpawnDebris(int32 *debrisInfo)
{
    RSDK_THIS(BigSqueeze);
    if (debrisInfo) {
        int32 count = debrisInfo[0];
        debrisInfo++;

        for (int32 i = 0; i < count; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);

            RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 6, &debris->animator, true, debrisInfo[0]);
            debris->velocity.x      = debrisInfo[1];
            debris->velocity.y      = debrisInfo[2];
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x800000;
            debris->updateRange.y   = 0x800000;

            debrisInfo += 3;
        }
    }
}

void BigSqueeze_Draw_Boss(void)
{
    RSDK_THIS(BigSqueeze);
    Vector2 drawPos;

    if (self->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 156, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 33, 0xE0E0E0);
    }

    RSDK.DrawSprite(&self->animator, NULL, false);
    RSDK.DrawSprite(&self->domeAnimator, NULL, false);
    RSDK.DrawSprite(&self->prongsAnimator, NULL, false);
    RSDK.DrawSprite(&self->electricAnimator, NULL, false);

    RSDK.SetPaletteEntry(0, 156, 0x202020);
    RSDK.SetPaletteEntry(0, 33, 0x303840);
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    drawPos.x -= 0x140000;
    RSDK.DrawSprite(&self->wheelAnimator, &drawPos, false);

    drawPos.x += 0x280000;
    RSDK.DrawSprite(&self->wheelAnimator, &drawPos, false);
}

void BigSqueeze_Draw_Crusher(void)
{
    RSDK_THIS(BigSqueeze);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void BigSqueeze_StateManager_SetupIntro(void)
{
    RSDK_THIS(BigSqueeze);

    if (++self->setupTimer >= 8) {
        self->setupTimer = 0;
        foreach_all(CollapsingPlatform, platform)
        {
            platform->collapseDelay = 1;
            platform->active        = ACTIVE_NEVER;
        }

        foreach_active(Eggman, eggman) { eggman->direction = FLIP_X; }

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - WIDE_SCR_XCENTER;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + 308;
        self->state                 = BigSqueeze_StateManager_SetupEggman;
    }
}

void BigSqueeze_StateManager_SetupEggman(void)
{
    RSDK_THIS(BigSqueeze);

    if (self->setupTimer) {
        self->setupTimer++;
        EntityEggman *eggmanPtr = self->eggman;

        if (self->setupTimer == 104)
            RSDK.SetSpriteAnimation(Eggman->aniFrames, 5, &eggmanPtr->animator, true, 0);

        if (self->setupTimer == 120) {
            self->setupTimer      = 0;
            self->state           = BigSqueeze_StateManager_SetupArena;
            eggmanPtr->onGround   = false;
            eggmanPtr->velocity.x = -0x10000;
            eggmanPtr->velocity.y = -0x40000;
            eggmanPtr->state      = Eggman_State_ProcessAirThenSet;
            eggmanPtr->animID     = 2;
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++self->setupTimer;

            foreach_active(LightBarrier, barrier) { barrier->enabled = true; }

            foreach_active(Eggman, eggman)
            {
                self->eggman = eggman;
                RSDK.SetSpriteAnimation(Eggman->aniFrames, 2, &eggman->animator, true, 0);
                eggman->state  = Eggman_State_ProcessThenSet;
                eggman->animID = 0;
            }
        }
    }
}

void BigSqueeze_StateManager_SetupArena(void)
{
    RSDK_THIS(BigSqueeze);

    EntityEggman *eggman = self->eggman;
    if (eggman->state == Eggman_State_ProcessAnimation) {
        foreach_all(CollapsingPlatform, platform) { platform->active = ACTIVE_BOUNDS; }

        Zone->cameraBoundsR[0] = (self->position.x >> 16) + WIDE_SCR_XCENTER;
        Zone->cameraBoundsB[0] = 1792;
        Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - ScreenInfo->size.y;

        BigSqueeze->boundsB = (Zone->cameraBoundsB[0] - 16) << 16;
        eggman->state       = Eggman_State_ProcessThenSet;
        eggman->animID      = 0;
        self->state         = BigSqueeze_StateManager_SetupBoss;
    }
}

void BigSqueeze_StateManager_SetupBoss(void)
{
    RSDK_THIS(BigSqueeze);

    if (++self->setupTimer == 48) {
        foreach_active(BigSqueeze, boss)
        {
            switch (boss->type) {
                default: break;

                case BIGSQUEEZE_BOSS:
                    boss->invincible = true;
                    boss->state      = BigSqueeze_StateBoss_Idle;
                    boss->velocity.x = -0x10000;
                    break;

                case BIGSQUEEZE_CRUSHER_L:
                    boss->state      = BigSqueeze_StateCrusher_BeginCrushing;
                    boss->velocity.x = 0x8000;
                    boss->hitbox.top = -0x7FFF;
                    break;

                case BIGSQUEEZE_CRUSHER_R:
                    boss->state      = BigSqueeze_StateCrusher_BeginCrushing;
                    boss->velocity.x = -0x8000;
                    boss->hitbox.top = -0x7FFF;
                    break;
            }
        }

        self->state = BigSqueeze_StateManager_HandleBoss;
    }
}

void BigSqueeze_StateBoss_Idle(void)
{
    RSDK_THIS(BigSqueeze);

    if (self->setupTimer > 30)
        BigSqueeze_HandleBossMovement();

    if (++self->setupTimer == 180) {
        self->invincible = false;
        self->setupTimer = 0;
        RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 5, &self->electricAnimator, true, 0);
        self->state = BigSqueeze_StateBoss_Electrified;
    }

    BigSqueeze_CheckPlayerCollisions_Vulnerable();
}

void BigSqueeze_StateBoss_Electrified(void)
{
    RSDK_THIS(BigSqueeze);

    RSDK.ProcessAnimation(&self->domeAnimator);

    // Uncomment this line if you want the prongs to animate when the boss is attacking
    // This is all that's needed to get them animating with no issues, so I assume this line being absent was prolly an oversight?
    // RSDK.ProcessAnimation(&self->prongsAnimator);

    RSDK.ProcessAnimation(&self->electricAnimator);

    if (!self->setupTimer)
        RSDK.PlaySfx(BigSqueeze->sfxOrbinaut, false, 255);

    if (++self->setupTimer == 16)
        FBZTrash_SummonOrbinaut(self->position.x, self->position.y + 0x300000);

    if (self->setupTimer == 120) {
        self->setupTimer = 0;
        RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 2, &self->domeAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->electricAnimator, true, 0);
        self->state = BigSqueeze_StateBoss_Idle;
    }

    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(BigSqueeze->sfxMagnet, false, 255);

    BigSqueeze_CheckPlayerCollisions_Electrified();
}

void BigSqueeze_StateBoss_Destroyed(void)
{
    RSDK_THIS(BigSqueeze);

    BigSqueeze_Explode();

    ++self->setupTimer;
    if (self->type == BIGSQUEEZE_BOSS) {
        switch (self->setupTimer) {
            case 20:
                BigSqueeze_SpawnDebris(BigSqueeze->prongDebrisInfo);
                RSDK.SetSpriteAnimation(-1, 0, &self->prongsAnimator, true, 0);
                break;

            case 40:
                BigSqueeze_SpawnDebris(BigSqueeze->domeDebrisInfo);
                RSDK.SetSpriteAnimation(-1, 0, &self->domeAnimator, true, 0);
                break;

            case 60:
                BigSqueeze_SpawnDebris(BigSqueeze->baseDebrisInfo);
                RSDK.SetSpriteAnimation(-1, 0, &self->animator, true, 0);
                RSDK.SetSpriteAnimation(-1, 0, &self->wheelAnimator, true, 0);
                break;

            case 80:
                Music_TransitionTrack(TRACK_STAGE, 0.0125);
                self->setupTimer = 0;
                self->visible    = false;
                self->state      = BigSqueeze_StateBoss_DropSignPost;
                foreach_active(FBZTrash, trash)
                {
                    if (trash->state != FBZTrash_State_LooseTrash) {
                        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), trash->position.x, trash->position.y)->drawGroup =
                            Zone->objectDrawGroup[1];
                        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
                        destroyEntity(trash);
                    }
                }
                break;
        }
    }
    else if (self->timer == 80) {
        destroyEntity(self);
    }
}

void BigSqueeze_StateBoss_DropSignPost(void)
{
    RSDK_THIS(BigSqueeze);

    BigSqueeze->isCrushing = false;
    if (++self->setupTimer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->active     = ACTIVE_NORMAL;
            signPost->state      = SignPost_State_Falling;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }

        self->state = StateMachine_None;
    }
}

void BigSqueeze_StateManager_HandleOutro(void)
{
    RSDK_THIS(BigSqueeze);

    self->position.y = BigSqueeze->boundsB;
}

void BigSqueeze_StateManager_HandleBoss(void)
{
    RSDK_THIS(BigSqueeze);

    self->position.y = BigSqueeze->boundsB;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, &self->hitbox)) {
            if (abs(player->groundVel) > 0x20000 && !(Zone->timer & 7)) {
                int32 x              = player->position.x + RSDK.Rand(-0x40000, 0x40000);
                int32 y              = player->position.y + 0x40000 + RSDK.Rand(-0x40000, 0x40000);
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, x, y);

                RSDK.SetSpriteAnimation(FBZTrash->aniFrames, RSDK.Rand(0, 2) + 9, &debris->animator, false, 0);
                debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y      = -0x20000;
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = Zone->objectDrawGroup[0];
                debris->updateRange.x   = 0x200000;
                debris->updateRange.y   = 0x200000;
            }
        }
    }

    foreach_active(FBZSinkTrash, sinkTrash)
    {
        sinkTrash->position.y = BigSqueeze->boundsB + (sinkTrash->size.y >> 1);
        sinkTrash->size.x     = BigSqueeze->crusherX[BIGSQUEEZE_CRUSHER_R] - BigSqueeze->crusherX[BIGSQUEEZE_CRUSHER_L];
    }
}

void BigSqueeze_StateCrusher_BeginCrushing(void)
{
    RSDK_THIS(BigSqueeze);

    BigSqueeze_HandleWallCollisions();

    BigSqueeze->crusherX[self->type] = self->position.x;
    if (++self->setupTimer == 8) {
        BigSqueeze->isCrushing = false;
    }

    if (self->setupTimer == 300) {
        self->setupTimer       = 0;
        self->state            = BigSqueeze_StateCrusher_Crushing;
        BigSqueeze->isCrushing = true;
        if (self->velocity.x > 0)
            ++BigSqueeze->crushTimer;
    }
}

void BigSqueeze_StateCrusher_Crushing(void)
{
    RSDK_THIS(BigSqueeze);

    BigSqueeze_HandleWallCollisions();

    self->position.x += self->velocity.x;
    BigSqueeze->crusherX[self->type] = self->position.x;

    if (!(Zone->timer & 3))
        Camera_ShakeScreen(0, 0, 4);

    if (self->type == BIGSQUEEZE_CRUSHER_L)
        BigSqueeze->boundsB -= 0x4000;

    if (++self->setupTimer == 32 && BigSqueeze->crushTimer < 10) {
        self->setupTimer = 0;
        self->state      = BigSqueeze_StateCrusher_BeginCrushing;
    }
}

#if GAME_INCLUDE_EDITOR
void BigSqueeze_EditorDraw(void)
{
    RSDK_THIS(BigSqueeze);

    switch (self->type) {
        case BIGSQUEEZE_MANAGER:
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 4, &self->wheelAnimator, true, 0);
            RSDK.DrawSprite(&self->wheelAnimator, NULL, false); // an odd sprite, but it beats no sprite
            break;

        case BIGSQUEEZE_BOSS:
            self->updateRange.x = 0x8000000;
            self->updateRange.y = 0x1000000;
            self->drawGroup     = Zone->objectDrawGroup[1];
            RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 1, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 2, &self->domeAnimator, true, 0);
            RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 3, &self->prongsAnimator, true, 0);
            RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 4, &self->wheelAnimator, true, 0);
            BigSqueeze_Draw_Boss();
            break;

        case BIGSQUEEZE_CRUSHER_L:
        case BIGSQUEEZE_CRUSHER_R:
            if (self->type == BIGSQUEEZE_CRUSHER_R) {
                self->direction = FLIP_X;
                self->drawFX    = FX_FLIP;
            }
            self->updateRange.x = 0x8000000;
            self->updateRange.y = 0x1000000;
            self->drawGroup     = Zone->objectDrawGroup[1];
            RSDK.SetSpriteAnimation(BigSqueeze->aniFrames, 0, &self->animator, true, 0);
            BigSqueeze_Draw_Crusher();
            break;

        default: break;
    }
}

void BigSqueeze_EditorLoad(void)
{
    BigSqueeze->aniFrames = RSDK.LoadSpriteAnimation("FBZ/BigSqueeze.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BigSqueeze, type);
    RSDK_ENUM_VAR("Manager", BIGSQUEEZE_MANAGER);
    RSDK_ENUM_VAR("Boss", BIGSQUEEZE_BOSS);
    RSDK_ENUM_VAR("Left Crusher", BIGSQUEEZE_CRUSHER_L);
    RSDK_ENUM_VAR("Right Crusher", BIGSQUEEZE_CRUSHER_R);
}
#endif

void BigSqueeze_Serialize(void) { RSDK_EDITABLE_VAR(BigSqueeze, VAR_ENUM, type); }
