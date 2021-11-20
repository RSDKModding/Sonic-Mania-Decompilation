#include "SonicMania.h"

ObjectHatterkiller *Hatterkiller;

void Hatterkiller_Update(void)
{
    RSDK_THIS(Hatterkiller);

    for (int s = 0; s < Hatterkiller_SegmentCount; ++s) {
        if (self->delays[s] <= 0) {
            self->velocities[s].y += 0x3800;
            self->positions[s].x += self->velocities[s].x + TornadoPath->moveVel.x;
            self->positions[s].y += self->velocities[s].y + TornadoPath->moveVel.y;
        }
        else {
            self->delays[s]--;
            self->positions[s].x += TornadoPath->moveVel.x;
            self->positions[s].y += TornadoPath->moveVel.y;
        }
    }

    if (HeavyMystic->curtainLinePos) {
        if (!(Zone->timer & 3))
            HeavyMystic_Unknown1(self->position.x, self->position.y);
        if (++self->field_58 == 40) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            RSDK.PlaySfx(Hatterkiller->sfxTransform2, false, 255);
            RSDK.PlaySfx(Hatterkiller->sfxPowerup, false, 255);
            CREATE_ENTITY(FXSpinRay, NULL, self->position.x, self->position.y)->parent = (Entity *)self;
            EntityFXFade *fade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
            fade->speedIn      = 8;
            fade->speedOut     = 8;
        }
    }
    else {

        foreach_active(Player, player)
        {
            self->position.x = self->positions[0].x;
            self->position.y = self->positions[0].y;
            if (Player_CheckBadnikTouch(player, self, &Hatterkiller->hitbox) && Player_CheckBadnikBreak(self, player, false)) {
                for (int s = 1; s < Hatterkiller_SegmentCount; ++s) {
                    Vector2 *pos       = &self->positions[s];
                    Animator *animator = self->animators[s];

                    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, pos->x, pos->y);
                    RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, animator->animationID, &debris->animator, true, animator->frameID);
                    debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                    debris->velocity.y    = RSDK.Rand(-0x20000, -0x10000);
                    debris->gravity       = 0x4800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;
                }

                destroyEntity(self);
                self->active = ACTIVE_NEVER2;
                foreach_break;
            }

            for (int s = 1; s < Hatterkiller_SegmentCount; ++s) {
                self->position.x = self->positions[s].x;
                self->position.y = self->positions[s].y;

                if (Player_CheckCollisionTouch(player, self, &Hatterkiller->hitbox)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x200, player, 2, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, self);
                }
            }
        }
    }

    if (HeavyMystic->curtainLinePos) {
        self->position = self->positions[9];
        if (!RSDK.CheckOnScreen(self, NULL)) {
            if (HeavyMystic->curtainLinePos == 1 && !RSDK.CheckOnScreen(self, NULL)) {
                foreach_active(ParallaxSprite, sprite) { sprite->visible = false; }
                ++HeavyMystic->curtainLinePos;
            }
            CREATE_ENTITY(UberCaterkiller, NULL, self->positions[Hatterkiller_SegmentCount - 1].x,
                          (ScreenInfo->position.y + 64 + ScreenInfo->height) << 16);
            destroyEntity(self);
        }
    }
    else if (!RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }

    self->position.x = self->positions[0].x;
    self->position.y = self->positions[0].y;
}

void Hatterkiller_LateUpdate(void) {}

void Hatterkiller_StaticUpdate(void) {}

void Hatterkiller_Draw(void)
{
    RSDK_THIS(Hatterkiller);

    for (int s = Hatterkiller_SegmentCount - 1; s >= 0; --s) {
        RSDK.DrawSprite(self->animators[s], &self->positions[s], false);
    }
}

void Hatterkiller_Create(void *data)
{
    RSDK_THIS(Hatterkiller);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->active        = ACTIVE_NORMAL;
        self->drawOrder     = Zone->drawOrderLow + 1;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;

        int delay = 0;
        for (int s = 0; s < Hatterkiller_SegmentCount; ++s) {
            self->velocities[s].x = voidToInt(data);
            self->velocities[s].y = -0x40000;
            self->animators[s]    = &self->animator2;
            self->delays[s]       = delay;
            self->positions[s].x  = self->position.x;
            self->positions[s].y  = self->position.y;
            delay += 4;
        }
        self->animators[0] = &self->animator1;

        if (voidToInt(data) >= 0)
            RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 3, &self->animator1, true, 0);
        else
            RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 2, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(Hatterkiller->aniFrames, 4, &self->animator3, true, 0);
    }
}

void Hatterkiller_StageLoad(void)
{
    Hatterkiller->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/RattleKiller.bin", SCOPE_STAGE);
    Hatterkiller->hitbox.left   = -8;
    Hatterkiller->hitbox.top    = -8;
    Hatterkiller->hitbox.right  = 8;
    Hatterkiller->hitbox.bottom = 8;
    Hatterkiller->sfxRocketJet  = RSDK.GetSFX("Stage/RocketJet.wav");
    Hatterkiller->sfxTransform2 = RSDK.GetSFX("Stage/Transform2.wav");
    Hatterkiller->sfxPowerup    = RSDK.GetSFX("Stage/PowerUp.wav");
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
    CREATE_ENTITY(Hatterkiller, intToVoid(-0x20000), self->position.x, self->position.y);
}

#if RETRO_INCLUDE_EDITOR
void Hatterkiller_EditorDraw(void) {}

void Hatterkiller_EditorLoad(void) {}
#endif

void Hatterkiller_Serialize(void) {}
