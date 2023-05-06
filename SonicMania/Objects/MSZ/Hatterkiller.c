// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Hatterkiller Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHatterkiller *Hatterkiller;

void Hatterkiller_Update(void)
{
    RSDK_THIS(Hatterkiller);

    for (int32 s = 0; s < HATTERKILLER_SEGMENT_COUNT; ++s) {
        if (self->bodyDelays[s] <= 0) {
            self->bodyVelocities[s].y += 0x3800;
            self->bodyPositions[s].x += self->bodyVelocities[s].x + TornadoPath->moveVel.x;
            self->bodyPositions[s].y += self->bodyVelocities[s].y + TornadoPath->moveVel.y;
        }
        else {
            self->bodyDelays[s]--;
            self->bodyPositions[s].x += TornadoPath->moveVel.x;
            self->bodyPositions[s].y += TornadoPath->moveVel.y;
        }
    }

    if (HeavyMystic->curtainLinePos) {
        if (!(Zone->timer & 3))
            HeavyMystic_SpawnParticleFX(self->position.x, self->position.y);

        if (++self->timer == 40) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            RSDK.PlaySfx(Hatterkiller->sfxTransform2, false, 255);
            RSDK.PlaySfx(Hatterkiller->sfxPowerup, false, 255);

            CREATE_ENTITY(FXSpinRay, NULL, self->position.x, self->position.y)->parent = (Entity *)self;

            EntityFXFade *fade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
            fade->speedIn      = 8;
            fade->speedOut     = 8;
        }
    }
    else {

        foreach_active(Player, player)
        {
            self->position.x = self->bodyPositions[0].x;
            self->position.y = self->bodyPositions[0].y;
            if (Player_CheckBadnikTouch(player, self, &Hatterkiller->hitboxSegment) && Player_CheckBadnikBreak(player, self, false)) {
                for (int32 s = 1; s < HATTERKILLER_SEGMENT_COUNT; ++s) {
                    Vector2 *position    = &self->bodyPositions[s];
                    Animator *animator   = self->bodyAnimators[s];
                    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, position->x, position->y);

                    RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, animator->animationID, &debris->animator, true, animator->frameID);
                    debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
                    debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
                    debris->gravityStrength = 0x4800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;
                }

                destroyEntity(self);
                self->active = ACTIVE_DISABLED;
                foreach_break;
            }

            for (int32 s = 1; s < HATTERKILLER_SEGMENT_COUNT; ++s) {
                self->position.x = self->bodyPositions[s].x;
                self->position.y = self->bodyPositions[s].y;

                if (Player_CheckCollisionTouch(player, self, &Hatterkiller->hitboxSegment)) {
#if MANIA_USE_PLUS
                    if (!Player_CheckMightyUnspin(player, 0x200, 2, &player->uncurlTimer))
#endif
                        Player_Hurt(player, self);
                }
            }
        }
    }

    if (HeavyMystic->curtainLinePos) {
        self->position = self->bodyPositions[9];

        if (!RSDK.CheckOnScreen(self, NULL)) {
            if (HeavyMystic->curtainLinePos == 1 && !RSDK.CheckOnScreen(self, NULL)) {
                foreach_active(ParallaxSprite, sprite) { sprite->visible = false; }
                ++HeavyMystic->curtainLinePos;
            }

            CREATE_ENTITY(UberCaterkiller, NULL, self->bodyPositions[HATTERKILLER_SEGMENT_COUNT - 1].x,
                          (ScreenInfo->position.y + 64 + ScreenInfo->size.y) << 16);

            destroyEntity(self);
        }
    }
    else if (!RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }

    self->position.x = self->bodyPositions[0].x;
    self->position.y = self->bodyPositions[0].y;
}

void Hatterkiller_LateUpdate(void) {}

void Hatterkiller_StaticUpdate(void) {}

void Hatterkiller_Draw(void)
{
    RSDK_THIS(Hatterkiller);

    for (int32 s = HATTERKILLER_SEGMENT_COUNT - 1; s >= 0; --s) RSDK.DrawSprite(self->bodyAnimators[s], &self->bodyPositions[s], false);
}

void Hatterkiller_Create(void *data)
{
    RSDK_THIS(Hatterkiller);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->active        = ACTIVE_NORMAL;
        self->drawGroup     = Zone->objectDrawGroup[0] + 1;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;

        int32 delay = 0;
        for (int32 s = 0; s < HATTERKILLER_SEGMENT_COUNT; ++s) {
            self->bodyVelocities[s].x = VOID_TO_INT(data);
            self->bodyVelocities[s].y = -0x40000;
            self->bodyAnimators[s]    = &self->bodyAnimator;
            self->bodyDelays[s]       = delay;
            self->bodyPositions[s]    = self->position;
            delay += 4;
        }

        self->bodyAnimators[0] = &self->headAnimator;

        if (VOID_TO_INT(data) >= 0)
            RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 3, &self->headAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 2, &self->headAnimator, true, 0);

        RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 1, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 4, &self->tailAnimator, true, 0);
    }
}

void Hatterkiller_StageLoad(void)
{
    Hatterkiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/RattleKiller.bin", SCOPE_STAGE);

    Hatterkiller->hitboxSegment.left   = -8;
    Hatterkiller->hitboxSegment.top    = -8;
    Hatterkiller->hitboxSegment.right  = 8;
    Hatterkiller->hitboxSegment.bottom = 8;

    Hatterkiller->sfxRocketJet  = RSDK.GetSfx("Stage/RocketJet.wav");
    Hatterkiller->sfxTransform2 = RSDK.GetSfx("Stage/Transform2.wav");
    Hatterkiller->sfxPowerup    = RSDK.GetSfx("Stage/PowerUp.wav");

    DEBUGMODE_ADD_OBJ(Hatterkiller);
}

void Hatterkiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Hatterkiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Hatterkiller, INT_TO_VOID(-0x20000), self->position.x, self->position.y);
}

#if GAME_INCLUDE_EDITOR
void Hatterkiller_EditorDraw(void)
{
    RSDK_THIS(Hatterkiller);

    self->visible       = true;
    self->active        = ACTIVE_NORMAL;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    int32 delay = 0;
    for (int32 s = 0; s < HATTERKILLER_SEGMENT_COUNT; ++s) {
        self->bodyAnimators[s] = &self->bodyAnimator;
        self->bodyDelays[s]    = delay;
        self->bodyPositions[s] = self->position;
        delay += 4;
    }

    self->bodyAnimators[0] = &self->headAnimator;

    RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 2, &self->headAnimator, true, 0);
    RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 1, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 4, &self->tailAnimator, true, 0);

    Hatterkiller_Draw();
}

void Hatterkiller_EditorLoad(void) { Hatterkiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/RattleKiller.bin", SCOPE_STAGE); }
#endif

void Hatterkiller_Serialize(void) {}
