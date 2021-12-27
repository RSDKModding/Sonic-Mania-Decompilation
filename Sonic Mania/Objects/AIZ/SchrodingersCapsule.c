// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SchrodingersCapsule Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectSchrodingersCapsule *SchrodingersCapsule;

void SchrodingersCapsule_Update(void)
{
    RSDK_THIS(SchrodingersCapsule);
    StateMachine_Run(self->state);

    if (self->mainAnimator.frameID != 1) {
        bool32 stoodSolid = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionBox(player, self, &self->hitboxSolid) == C_TOP)
                stoodSolid = true;

            if (self->state == SchrodingersCapsule_Unknown2) {
                if (Player_CheckCollisionBox(player, self, &self->hitboxButton) == C_TOP) {
                    self->buttonPos   = 0x80000;
                    player->velocity.x = 0;
                    player->groundVel  = 0;
                    CutsceneSeq_LockPlayerControl(player);
                    stoodSolid               = false;
                    player->state      = Player_State_Ground;
                    player->stateInput = StateMachine_None;
                    self->active     = ACTIVE_NORMAL;
                    self->state      = SchrodingersCapsule_Unknown3;
                }
                else {
                    if (Player_CheckCollisionTouch(player, self, &self->hitboxButtonTrigger)) {
                        Hitbox *playerHitbox = Player_GetHitbox(player);
                        self->buttonPos     = ((playerHitbox->bottom + 48) << 16) - self->position.y + player->position.y;
                        if (self->buttonPos <= 0x80000) {
                            if (self->buttonPos < 0)
                                self->buttonPos = 0;
                            self->buttonPos &= 0xFFFF0000;
                        }
                        else {
                            self->buttonPos = 0x80000;
                            self->buttonPos &= 0xFFFF0000;
                        }
                    }
                    else {
                        if (self->buttonPos > 0)
                            self->buttonPos -= 0x10000;
                    }

                    if (stoodSolid) {
                        EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
                        if (ruby->velocity.y > 0)
                            ruby->velocity.y = -ruby->velocity.y;
                        ruby->state = EncoreIntro_PhantomRuby_CapsuleRiseUp;
                    }
                }
            }
            else {
                stoodSolid = false;
                Player_CheckCollisionBox(player, self, &self->hitboxButton);
            }
        }
    }
}

void SchrodingersCapsule_LateUpdate(void) {}

void SchrodingersCapsule_StaticUpdate(void) {}

void SchrodingersCapsule_Draw(void)
{
    RSDK_THIS(SchrodingersCapsule);
    Vector2 drawPos;

    if (!self->mainAnimator.frameID) {
        drawPos = self->position;
        drawPos.y += self->buttonPos;
        RSDK.DrawSprite(&self->buttonAnimator, &drawPos, false);

        self->mainAnimator.frameID = 2;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        if ((Zone->timer & 8)) {
            self->mainAnimator.frameID = 3;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
        }

        drawPos = self->position;
        drawPos.x += 0xE0000;
        RSDK.DrawSprite(&self->mightyAnimator, &drawPos, false);

        drawPos.x -= 0x1C0000;
        RSDK.DrawSprite(&self->rayAnimator, &drawPos, false);

        self->mainAnimator.frameID = 0;
    }
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (!self->mainAnimator.frameID) {
        self->inkEffect = INK_ADD;
        self->alpha     = 0x80;
        RSDK.DrawSprite(&self->glassAnimator, NULL, false);

        self->inkEffect = INK_NONE;
    }
}

void SchrodingersCapsule_Create(void *data)
{
    RSDK_THIS(SchrodingersCapsule);
    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 1, &self->buttonAnimator, true, 0);
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 2, &self->glassAnimator, true, 0);
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 3, &self->mightyAnimator, true, 0);
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 4, &self->rayAnimator, true, 0);
        self->hitboxSolid.left           = -40;
        self->hitboxSolid.top            = -40;
        self->hitboxSolid.right          = 40;
        self->hitboxSolid.bottom         = 40;

        self->hitboxButton.left          = -16;
        self->hitboxButton.top           = -44;
        self->hitboxButton.right         = 16;
        self->hitboxButton.bottom        = -30;

        self->hitboxButtonTrigger.left   = -15;
        self->hitboxButtonTrigger.top    = -60;
        self->hitboxButtonTrigger.right  = 15;
        self->hitboxButtonTrigger.bottom = -30;
        self->state          = SchrodingersCapsule_Unknown1;
        self->active         = ACTIVE_BOUNDS;
        self->updateRange.x  = 0x800000;
        self->updateRange.y  = 0x800000;
        self->visible        = true;
        self->drawOrder      = Zone->drawOrderLow;
    }
}

void SchrodingersCapsule_StageLoad(void)
{
    SchrodingersCapsule->aniFrames     = RSDK.LoadSpriteAnimation("AIZ/SchrodingersCapsule.bin", SCOPE_STAGE);
    SchrodingersCapsule->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
    SchrodingersCapsule->sfxExplosion3 = RSDK.GetSfx("Stage/Explosion3.wav");
}

void SchrodingersCapsule_Unknown1(void)
{
    RSDK_THIS(SchrodingersCapsule);
    self->state = SchrodingersCapsule_Unknown2;
}

void SchrodingersCapsule_Unknown2(void)
{
    RSDK_THIS(SchrodingersCapsule);
    RSDK.ProcessAnimation(&self->glassAnimator);
    RSDK.ProcessAnimation(&self->mightyAnimator);
    RSDK.ProcessAnimation(&self->rayAnimator);

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (!player->sidekick) {
            if (abs(self->position.x - player->position.x) < 0x1000000) {
                if (abs(self->position.y - player->position.y) < 0x1000000 && self->position.x - (Zone->cameraBoundsR[p] << 16) < 0x1000000) {
                    Zone->playerBoundActiveL[p] = true;
                    Zone->playerBoundActiveR[p] = true;
                    Zone->cameraBoundsL[p]     = (self->position.x >> 16) - ScreenInfo[p].centerX;
                    Zone->cameraBoundsR[p]     = (self->position.x >> 16) + ScreenInfo[p].centerX;
                }
            }
        }
    }
}

void SchrodingersCapsule_Unknown3(void)
{
    RSDK_THIS(SchrodingersCapsule);
    RSDK.ProcessAnimation(&self->glassAnimator);
    RSDK.ProcessAnimation(&self->mightyAnimator);
    RSDK.ProcessAnimation(&self->rayAnimator);
    RSDK.SetSpriteAnimation(-1, 0, &self->glassAnimator, true, 0);
    self->state               = SchrodingersCapsule_Unknown4;
    SceneInfo->timeEnabled = false;
}

void SchrodingersCapsule_Unknown4(void)
{
    RSDK_THIS(SchrodingersCapsule);
    RSDK.ProcessAnimation(&self->glassAnimator);
    RSDK.ProcessAnimation(&self->mightyAnimator);
    RSDK.ProcessAnimation(&self->rayAnimator);
    if (!(self->timer % 3)) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((2 * (RSDK.Rand(0, 256) > 192) + EXPLOSION_ENEMY)),
                                                   (RSDK.Rand(-24, 24) << 16) + self->position.x, (RSDK.Rand(-24, 24) << 16) + self->position.y);
        explosion->drawOrder = Zone->drawOrderHigh;

        RSDK.PlaySfx(SchrodingersCapsule->sfxExplosion2, 0, 255);
    }

    if (++self->timer == 60) {
        self->timer             = 0;
        self->state             = StateMachine_None;
        self->mainAnimator.frameID = 1;
        RSDK.SetSpriteAnimation(-1, -1, &self->mightyAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, -1, &self->rayAnimator, true, 0);

        EntityPlayer *buddy1 = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
        buddy1->objectID     = Player->objectID;
        Player_ChangeCharacter(buddy1, ID_MIGHTY);
        buddy1->position.x      = self->position.x + 0xE0000;
        buddy1->position.y      = self->position.y;
        buddy1->playerID        = RSDK.GetEntityID(buddy1);
        buddy1->active          = ACTIVE_NORMAL;
        buddy1->tileCollisions  = true;
        buddy1->interaction     = true;
        buddy1->visible         = true;
        buddy1->controllerID    = buddy1->playerID + 1;
        buddy1->drawOrder       = Zone->playerDrawLow;
        buddy1->scale.x         = 0x200;
        buddy1->scale.y         = 0x200;
        buddy1->state           = Player_State_Ground;
        buddy1->collisionLayers = Zone->fgLayers;
        buddy1->drawFX          = FX_ROTATE | FX_FLIP;
        buddy1->velocity.x      = 0x1C000;
        buddy1->velocity.y      = -0x40000;
        buddy1->direction       = FLIP_X;
        RSDK.SetSpriteAnimation(buddy1->aniFrames, ANI_HURT, &buddy1->animator, true, 0);

        EntityPlayer *buddy2 = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
        buddy2->objectID     = Player->objectID;
        Player_ChangeCharacter(buddy2, ID_RAY);
        buddy2->position.x      = self->position.x - 0xE0000;
        buddy2->position.y      = self->position.y;
        buddy2->playerID        = RSDK.GetEntityID(buddy2);
        buddy2->active          = ACTIVE_NORMAL;
        buddy2->tileCollisions  = true;
        buddy2->interaction     = true;
        buddy2->visible         = true;
        buddy2->controllerID    = buddy2->playerID + 1;
        buddy2->drawOrder       = Zone->playerDrawLow;
        buddy2->scale.x         = 0x200;
        buddy2->scale.y         = 0x200;
        buddy2->state           = Player_State_Ground;
        buddy2->collisionLayers = Zone->fgLayers;
        buddy2->drawFX          = FX_ROTATE | FX_FLIP;
        buddy2->velocity.x      = -0x24000;
        buddy2->velocity.y      = -0x40000;
        RSDK.SetSpriteAnimation(buddy2->aniFrames, ANI_HURT, &buddy2->animator, true, 0);

        Music_FadeOut(0.025);
        RSDK.PlaySfx(SchrodingersCapsule->sfxExplosion3, 0, 255);

        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player->position.x   = self->position.x;
        Player_ChangeCharacter(player, player->characterID);
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, true, 0);
        player->velocity.x = 0;
        player->velocity.y = 0;
        player->groundVel  = 0;
        player->stateInput = StateMachine_None;
        player->left       = false;
        player->right      = false;
        player->jumpPress  = false;
        player->jumpHold   = false;

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->position.x += 0x100000;

        self->timer = 0;
        self->state = SchrodingersCapsule_Unknown5;

        EntityFXFade *fade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fade->speedIn      = 256;
        fade->speedOut     = 32;
    }
}

void SchrodingersCapsule_Unknown5(void)
{
    RSDK_THIS(SchrodingersCapsule);
    if (++self->timer == 90) {
        self->timer = 0;
        self->state = StateMachine_None;

        EntityPlayer *buddy1 = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
        buddy1->state        = Player_State_Victory;
        RSDK.SetSpriteAnimation(buddy1->aniFrames, ANI_VICTORY, &buddy1->animator, true, 0);

        EntityPlayer *buddy2 = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
        buddy2->state        = Player_State_Victory;
        RSDK.SetSpriteAnimation(buddy2->aniFrames, ANI_VICTORY, &buddy2->animator, true, 0);

        ActClear->displayedActID = 1;
        ActClear->forceNoSave    = true;
        Music_PlayTrack(TRACK_ACTCLEAR);
        RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->objectID, NULL);
    }
}

#if RETRO_INCLUDE_EDITOR
void SchrodingersCapsule_EditorDraw(void)
{
    RSDK_THIS(SchrodingersCapsule);

    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 0, &self->mainAnimator, true, 0);
    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 1, &self->buttonAnimator, true, 0);
    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 2, &self->glassAnimator, true, 0);
    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 3, &self->mightyAnimator, true, 0);
    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 4, &self->rayAnimator, true, 0);

    SchrodingersCapsule_Draw();
}

void SchrodingersCapsule_EditorLoad(void) { SchrodingersCapsule->aniFrames = RSDK.LoadSpriteAnimation("AIZ/SchrodingersCapsule.bin", SCOPE_STAGE); }
#endif

void SchrodingersCapsule_Serialize(void) {}
#endif
