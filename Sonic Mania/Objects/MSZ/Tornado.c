// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Tornado Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTornado *Tornado;

void Tornado_Update(void)
{
    RSDK_THIS(Tornado);
    self->prevPosY = self->position.y;
    StateMachine_Run(self->state);

    self->animatorTornado.frameID = self->turnAngle >> 4;
    RSDK.ProcessAnimation(&self->animatorPropeller);
    RSDK.ProcessAnimation(&self->animatorPilot);
    RSDK.ProcessAnimation(&self->animatorFlame);
    RSDK.ProcessAnimation(&self->animatorKnux);
}

void Tornado_LateUpdate(void) {}

void Tornado_StaticUpdate(void) {}

void Tornado_Draw(void)
{
    RSDK_THIS(Tornado);
    Vector2 drawPos;

    RSDK.DrawSprite(&self->animatorPilot, NULL, false);
    RSDK.DrawSprite(&self->animatorPropeller, NULL, false);
    RSDK.DrawSprite(&self->animatorTornado, NULL, false);

    if (self->showFlame) {
        drawPos = self->position;
        drawPos.y += Tornado->flameOffsets[self->animatorTornado.frameID];
        RSDK.DrawSprite(&self->animatorFlame, &drawPos, false);
    }

    if (!MSZSetup->usingRegularPalette) {
        drawPos = self->position;
        drawPos.x += self->knuxPos.x;
        drawPos.y += self->knuxPos.y;
        RSDK.DrawSprite(&self->animatorKnux, &drawPos, false);
    }
}

void Tornado_Create(void *data)
{
    RSDK_THIS(Tornado);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->drawFX        = FX_FLIP;
        self->drawOrder     = Zone->objectDrawLow;
        self->movePos       = self->position;
        self->turnAngle     = 48;
        self->isStood       = true;
        self->offsetX       = 0x80000;
        self->active        = ACTIVE_BOUNDS;
        self->state         = Tornado_State_Setup;
        self->knuxPos.x  = -0x140000;
        self->knuxPos.y  = -0x160000;
        RSDK.SetSpriteAnimation(Tornado->aniFrames, 0, &self->animatorTornado, true, 0);
        RSDK.SetSpriteAnimation(Tornado->aniFrames, 1, &self->animatorPropeller, true, 0);
        RSDK.SetSpriteAnimation(Tornado->aniFrames, 2, &self->animatorFlame, true, 0);
        if (checkPlayerID(ID_TAILS, 1)
#if MANIA_USE_PLUS
            || checkPlayerID(ID_MIGHTY, 1) || checkPlayerID(ID_RAY, 1)
#endif
        ) {
            RSDK.SetSpriteAnimation(Tornado->aniFrames, 3, &self->animatorPilot, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(Tornado->aniFrames, 6, &self->animatorPilot, true, 0);
        }

        if ((globals->playerID & 0xFF) != ID_KNUCKLES && !StarPost->postIDs[0])
            RSDK.SetSpriteAnimation(Tornado->knuxFrames, 6, &self->animatorKnux, false, 0);

        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (player2->classID == Player->classID)
            player2->state = MSZSetup_PlayerState_Pilot;
    }
}

void Tornado_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ") || RSDK.CheckStageFolder("MSZCutscene")) {
        Tornado->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Tornado.bin", SCOPE_STAGE);
        if (!checkPlayerID(ID_KNUCKLES, 1))
            Tornado->knuxFrames = RSDK.LoadSpriteAnimation("Players/KnuxCutsceneAIZ.bin", SCOPE_STAGE);
    }

    Tornado->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    Tornado->sfxImpact    = RSDK.GetSfx("Stage/Impact5.wav");
}

void Tornado_State_Setup(void)
{
    RSDK_THIS(Tornado);
    self->active = ACTIVE_NORMAL;
    if (RSDK.CheckStageFolder("MSZ"))
        self->state = Tornado_State_SetupMSZ1Intro;
    else
        self->state = Tornado_HandlePlayerCollisions;

    StateMachine_Run(self->state);
}

void Tornado_State_SetupMSZ1Intro(void)
{
    RSDK_THIS(Tornado);

    Tornado_HandlePlayerCollisions();
    if (StarPost->postIDs[0]) {
        RSDK.SetSpriteAnimation(-1, 0, &self->animatorKnux, false, 0);
        self->state = Tornado_State_PlayerControlled;
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
        self->state = Tornado_State_MSZ1Intro;
    }
}

void Tornado_State_MSZ1Intro(void)
{
    RSDK_THIS(Tornado);
    Tornado_HandlePlayerCollisions();

    if (++self->timer == 180) {
        self->timer = 0;
        RSDK.PlaySfx(Tornado->sfxImpact, false, 255);
        self->knuxVel.x = -0x20000;
        self->knuxVel.y = -0x40000;
        RSDK.SetSpriteAnimation(Tornado->knuxFrames, 4, &self->animatorKnux, false, 0);
        self->state = Tornado_State_KnuxKnockedOff;
        foreach_active(Player, player) { player->stateInput = Player_ProcessP1Input; }
    }
}

void Tornado_State_KnuxKnockedOff(void)
{
    RSDK_THIS(Tornado);
    Tornado_State_PlayerControlled();

    self->knuxVel.y += 0x3800;
    self->knuxPos.x += self->knuxVel.x;
    self->knuxPos.y += self->knuxVel.y;

    if (++self->timer == 120) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(-1, 0, &self->animatorKnux, false, 0);
        self->state = Tornado_State_PlayerControlled;
    }
}

void Tornado_HandlePlayerCollisions(void)
{
    RSDK_THIS(Tornado);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Hitbox *hitbox        = RSDK.GetHitbox(&self->animatorTornado, 0);
    self->prevPosY      = self->position.y;
    self->turnAngle     = 32;
    player1->drawOrder    = self->drawOrder + 1;
    self->prevPosY &= 0xFFFF0000;
    self->moveVelocityY = (self->position.y & 0xFFFF0000) - self->prevPosY;
    self->position.y    = self->prevPosY;
    self->isStood       = false;
    int32 velY            = player1->velocity.y;
    if (Player_CheckCollisionPlatform(player1, self, hitbox)) {
        player1->position.x += TornadoPath->moveVel.x;
        player1->position.y += self->moveVelocityY;
        player1->flailing = false;
        self->isStood   = true;
        if (velY > 0x10000) {
            self->collideTimer = 0;
            self->gravityForce = 0x20000;
            self->mode  = TORNADO_MODE_LAND;
        }
    }

    EntityCamera *camera = TornadoPath->camera;
    if (camera) {
        int32 screenX = camera->position.x - (ScreenInfo->centerX << 16) + 0xC0000;
        if (player1->position.x < screenX)
            player1->position.x = screenX;

        int32 screenY = ((ScreenInfo->centerX - 12) << 16) + camera->position.x;
        if (player1->position.x > screenY)
            player1->position.x = screenY;

        if (player1->classID == Player->classID) {
            int32 deathBounds = (camera->position.y + ((ScreenInfo[camera->screenID].centerY + 16) << 16));
            if (player1->position.y > deathBounds)
                player1->deathType = PLAYER_DEATH_DIE_USESFX;
        }
    }
}

void Tornado_State_PlayerControlled(void)
{
    RSDK_THIS(Tornado);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Hitbox *hitbox        = RSDK.GetHitbox(&self->animatorTornado, TornadoPath->hitboxID);
    self->prevPosY      = self->position.y;
    if (self->turnAngle < 32)
        player1->drawOrder = self->drawOrder;
    else
        player1->drawOrder = self->drawOrder + 1;

    if (player1->state == Player_State_Roll) {
        player1->groundVel  = clampVal(player1->groundVel, -self->offsetX, self->offsetX);
        player1->velocity.x = player1->groundVel;
    }

    switch (self->mode) {
        case TORNADO_MODE_IDLE:
            if (player1->velocity.y < 0 && player1->jumpPress) {
                if (abs(TornadoPath->moveVel.x) > 0x40000) {
                    if (abs(player1->velocity.x) < abs(TornadoPath->moveVel.x) && !TornadoPath->hitboxID)
                        player1->velocity.x = abs(TornadoPath->moveVel.x);
                }

                ++self->mode;
                self->gravityForce = 0x20000;
                self->storeY       = self->position.y;
            }
            else {
                if (self->velocity.y > 0) {
                    if (self->turnAngle < 64)
                        self->turnAngle += 4;
                }
                else if (self->velocity.y >= 0) {
                    if (self->turnAngle > 32)
                        self->turnAngle -= 4;
                    else if (self->turnAngle < 32)
                        self->turnAngle += 4;
                }
                else if (self->turnAngle > 0)
                    self->turnAngle -= 4;
            }
            break;

        case TORNADO_MODE_JUMP_RECOIL: // jump force pushes tornado downwards
            self->gravityForce -= 0x2000;
            self->position.y += self->gravityForce;
            if (self->gravityForce < 0)
                self->mode = TORNADO_MODE_JUMP_REBOUND;
            break;

        case TORNADO_MODE_JUMP_REBOUND: // tornado returns to its original position
            self->gravityForce -= 0x1000;
            self->position.y += self->gravityForce;
            if (self->position.y <= self->storeY) {
                self->position.y   = self->storeY;
                self->mode  = TORNADO_MODE_IDLE;
                self->gravityForce = 0;
            }
            break;

        case TORNADO_MODE_LAND:
            self->gravityForce -= 0x2000;
            self->position.y += self->gravityForce;
            if (++self->collideTimer == 24) {
                self->mode  = TORNADO_MODE_IDLE;
                self->gravityForce = 0;
            }
            break;

        default: break;
    }

    if (self->isStood) {
        self->position.x += TornadoPath->moveVel.x;
        self->position.y += TornadoPath->moveVel.y;

        if (player1->stateInput) {
            self->velocity.y = 0;
            if (player1->up) 
                self->velocity.y = -0x10000;
            else if (player1->down) 
                self->velocity.y = 0x10000;
        }
    }
    else if (TornadoPath->hitboxID == 1) {
        self->position.x += TornadoPath->moveVel.x;
        self->position.y += TornadoPath->moveVel.y;
    }
    else {
        self->velocity.y = 0;
    }

    if (self->position.y < (ScreenInfo->position.y + 72) << 16)
        self->position.y = (ScreenInfo->position.y + 72) << 16;
    if (self->position.y > (ScreenInfo->height + ScreenInfo->position.y - 32) << 16)
        self->position.y = (ScreenInfo->height + ScreenInfo->position.y - 32) << 16;

    int32 storeX    = self->position.x;
    int32 storeY    = self->position.y + self->velocity.y;
    self->isStood = false;
    self->prevPosY &= 0xFFFF0000;
    self->moveVelocityY = (storeY & 0xFFFF0000) - self->prevPosY;
    self->position.y    = self->prevPosY;
    int32 velY            = player1->velocity.y;
    int32 posX            = self->position.x;

    if (Player_CheckCollisionPlatform(player1, self, hitbox)) {
        player1->position.x += TornadoPath->moveVel.x;
        player1->position.y += self->moveVelocityY;
        player1->flailing = false;
        self->isStood   = true;
        if (velY > 0x10000) {
            self->collideTimer = 0;
            self->gravityForce = 0x20000;
            self->mode  = TORNADO_MODE_LAND;
        }
    }
    else if (TornadoPath->hitboxID == 1) {
        player1->position.x += TornadoPath->moveVel.x;
    }

    int32 offsetX = 0;
    self->position.x += 0x1E0000;
    if (abs(posX + 0x1E0000 - player1->position.x) > 0x100000) {
        offsetX = self->offsetX;
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
    self->position.x = storeX;
    self->position.x += offsetX;
    self->position.y = storeY;

    EntityCamera *camera = TornadoPath->camera;
    if (camera) {
        int32 screenX = camera->position.x - (ScreenInfo->centerX << 16) + 0xC0000;
        if (player1->position.x < screenX)
            player1->position.x = screenX;

        int32 screenY = ((ScreenInfo->centerX - 12) << 16) + camera->position.x;
        if (player1->position.x > screenY)
            player1->position.x = screenY;

        if (player1->classID == Player->classID) {
            if (player1->position.y > (camera->position.y + ((ScreenInfo[camera->screenID].centerY + 16) << 16)))
                player1->deathType = PLAYER_DEATH_DIE_USESFX;
        }
    }
    if (player1->state == Player_State_TailsFlight) {
        if (player1->position.y < ((ScreenInfo->position.y + 20) << 16) && player1->velocity.y < 0) {
            player1->velocity.y   = 0;
            player1->abilitySpeed = 0x8000;
        }
    }
}

void Tornado_State_Mayday(void)
{
    RSDK_THIS(Tornado);

    self->position.x += TornadoPath->moveVel.x;
    self->position.y += TornadoPath->moveVel.y;

    if (!(Zone->timer % 3)) {
        if (self->onGround) 
            RSDK.PlaySfx(Tornado->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), (RSDK.Rand(-32, 32) << 16) + self->position.x,
                              (RSDK.Rand(-16, 16) << 16) + self->position.y);
            explosion->drawOrder = Zone->objectDrawHigh;
        }
    }
}

void Tornado_State_FlyAway_Right(void)
{
    RSDK_THIS(Tornado);

    self->position.x += 0x30000;
    self->position.y -= 0x30000;

    self->active = ACTIVE_BOUNDS;
}

void Tornado_State_FlyAway_Left(void)
{
    RSDK_THIS(Tornado);

    self->velocity.x -= 0x1800;
    self->position.x += self->velocity.x;
    self->position.y -= 0x8000;

    self->active = ACTIVE_BOUNDS;
}

#if RETRO_INCLUDE_EDITOR
void Tornado_EditorDraw(void)
{
    RSDK_THIS(Tornado);
    RSDK.SetSpriteAnimation(Tornado->aniFrames, 0, &self->animatorTornado, true, 0);
    RSDK.SetSpriteAnimation(Tornado->aniFrames, 1, &self->animatorPropeller, true, 0);
    RSDK.SetSpriteAnimation(Tornado->aniFrames, 2, &self->animatorFlame, true, 0);

    RSDK.DrawSprite(&self->animatorPilot, NULL, false);
    RSDK.DrawSprite(&self->animatorPropeller, NULL, false);
    RSDK.DrawSprite(&self->animatorTornado, NULL, false);
}

void Tornado_EditorLoad(void) { Tornado->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Tornado.bin", SCOPE_STAGE); }
#endif

void Tornado_Serialize(void) {}
