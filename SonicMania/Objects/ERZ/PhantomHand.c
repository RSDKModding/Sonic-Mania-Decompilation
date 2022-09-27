// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomHand Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPhantomHand *PhantomHand;

void PhantomHand_Update(void)
{
    RSDK_THIS(PhantomHand);

    StateMachine_Run(self->state);
}

void PhantomHand_LateUpdate(void) {}

void PhantomHand_StaticUpdate(void) {}

void PhantomHand_Draw(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.CopyPalette(1, 128, 0, 128, 128);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->handAnimator, NULL, false);

    if (self->state != PhantomHand_State_Disappear) {
        self->inkEffect = INK_ALPHA;
        self->alpha     = 0xD0;
        RSDK.DrawSprite(&self->crystalAnimator, NULL, false);

        self->inkEffect = INK_ADD;
        self->alpha     = 0x100;
        RSDK.DrawSprite(&self->shineAnimator, NULL, false);
    }
}

void PhantomHand_Create(void *data)
{
    RSDK_THIS(PhantomHand);

    if (!SceneInfo->inEditor) {
        self->visible            = true;
        self->drawGroup          = Zone->objectDrawGroup[0];
        self->parent             = (Entity *)data;
        self->targetPos          = self->position;
        self->active             = ACTIVE_NORMAL;
        self->inkEffect          = INK_ADD;
        self->drawFX             = FX_FLIP | FX_SCALE;
        self->alpha              = 0x100;
        self->updateRange.x      = 0x800000;
        self->updateRange.y      = 0x800000;
        self->timerDecreaseSpeed = 64;
        self->state              = PhantomHand_State_Summon;
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 0, &self->handAnimator, true, 0);
    }
}

void PhantomHand_StageLoad(void)
{
    PhantomHand->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomHand.bin", SCOPE_STAGE);

    PhantomHand->hitbox.left   = -20;
    PhantomHand->hitbox.top    = -20;
    PhantomHand->hitbox.right  = 20;
    PhantomHand->hitbox.bottom = 20;

    int32 pos = 3;
    for (int32 i = 0; i < PhantomHand->debrisInfo[0]; ++i) {
        PhantomHand->debrisInfo[pos + 0] = RSDK.Rand(-0x20000, 0x20000);
        PhantomHand->debrisInfo[pos + 1] = RSDK.Rand(-0x10000, -0x40000);

        pos += 4;
    }

    PhantomHand->grabbedPlayers[0] = false;
    PhantomHand->grabbedPlayers[1] = false;
    PhantomHand->grabbedPlayers[2] = false;
    PhantomHand->grabbedPlayers[3] = false;
}

void PhantomHand_CheckPlayerGrab(int32 playerX, int32 playerY)
{
    RSDK_THIS(PhantomHand);

    if (self->position.x >= playerX) {
        if (self->position.x > playerX) {
            int32 dist = MIN((self->position.x - playerX) >> 5, 0x10000);
            self->position.x -= dist;
        }
    }
    else {
        int32 dist = MIN((playerX - self->position.x) >> 5, 0x10000);
        self->position.x += dist;
    }

    if (self->targetPos.y < playerY)
        self->targetPos.y += 0x10000;

    if (playerY <= self->targetPos.y)
        self->position.y += (playerY - self->position.y) >> 3;
    else
        self->position.y = self->targetPos.y;

    foreach_active(Player, player)
    {
        if (player->state != Player_State_Static) {
            int32 rx = (player->position.x - self->position.x) >> 16;
            int32 ry = (player->position.y - self->position.y) >> 16;
            if (rx * rx + ry * ry < 0x100) {
                if (!PhantomHand->grabbedPlayers[player->playerID]) {
                    PhantomHand->grabbedPlayers[player->playerID] = true;
                    self->state                                   = PhantomHand_State_GrabbedPlayer;
                }
            }
        }
    }
}

void PhantomHand_State_Summon(void)
{
    RSDK_THIS(PhantomHand);

    if (!self->timer)
        RSDK.PlaySfx(PhantomEgg->sfxSummon, false, 255);

    self->position.x += self->velocity.x;
    if (self->timer > 0)
        self->timerDecreaseSpeed -= 4;

    self->timer += self->timerDecreaseSpeed;

    if (self->timer < 0x200 && self->timerDecreaseSpeed < 0) {
        self->timer = 0x200;
        self->state = PhantomHand_State_Appear;
    }

    self->scaleAngle += 8;
    self->scale.x = self->timer + ((self->timer * RSDK.Sin256(self->scaleAngle)) >> 11);
    self->scale.y = self->timer + ((self->timer * RSDK.Cos256(self->scaleAngle)) >> 11);
}

void PhantomHand_State_Appear(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->handAnimator);

    self->scaleAngle += 16;
    self->position.x += self->velocity.x;

    self->scale.x = ((self->timer * RSDK.Sin256(self->scaleAngle)) >> 11) + 0x200;
    self->scale.y = ((self->timer * RSDK.Cos256(self->scaleAngle)) >> 11) + 0x200;

    if (self->timer <= 0) {
        self->drawFX = FX_FLIP;

        if (Player->playerCount < 2)
            self->state = PhantomHand_State_TryGrabPlayer;
        else
            self->state = PhantomHand_State_TryGrabPlayers;
    }
    else {
        self->timer -= 32;
    }
}

void PhantomHand_State_TryGrabPlayer(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->handAnimator);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->interaction == true && player1->state == Player_State_Static) {
        self->state = PhantomHand_State_Disappear;
    }
    else {
        if ((self->direction && player1->position.x <= PhantomEgg->boundsM) || (self->direction && player1->position.x >= PhantomEgg->boundsM))
            PhantomHand_CheckPlayerGrab(self->parent->position.x, player1->position.y);
        else
            PhantomHand_CheckPlayerGrab(player1->position.x, player1->position.y);
    }
}

void PhantomHand_State_TryGrabPlayers(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->handAnimator);

    bool32 playersActive = true;
    for (int32 i = SLOT_PLAYER1; i < SLOT_PLAYER1 + 2; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        if (player->classID == Player->classID)
            playersActive = playersActive && player->interaction && player->state == Player_State_Static;
    }

    if (playersActive) {
        self->state = PhantomHand_State_Disappear;
    }
    else {
        int32 dist = 0x7FFFFFFF;

        EntityPlayer *playerPtr = NULL;
        if (self->direction == FLIP_NONE) {
            foreach_active(Player, player)
            {
                if (player->state != Player_State_Static && self->position.x - player->position.x < dist) {
                    dist      = self->position.x - self->position.x;
                    playerPtr = player;
                }
            }

            if (playerPtr)
                PhantomHand_CheckPlayerGrab(playerPtr->position.x, playerPtr->position.y);
        }
        else {
            foreach_active(Player, player)
            {
                if (player->state != Player_State_Static && player->position.x - self->position.x < dist) {
                    dist      = player->position.x - self->position.x;
                    playerPtr = player;
                }
            }
        }

        if (playerPtr)
            PhantomHand_CheckPlayerGrab(playerPtr->position.x, playerPtr->position.y);
    }
}

void PhantomHand_State_GrabbedPlayer(void)
{
    RSDK_THIS(PhantomHand);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &PhantomHand->hitbox)) {
            player->state           = Player_State_Static;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->position.x      = self->position.x;
            player->position.y      = self->position.y;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            player->onGround        = false;
            player->groundVel       = 0;

            if (!player->playerID)
                self->doScanlineCB = true;

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        }
    }

    if (++self->grabTimer == 8) {
        self->grabTimer = 0;
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 1, &self->handAnimator, true, 0);
        self->state = PhantomHand_State_Clasp;
    }
}

void PhantomHand_State_Clasp(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->handAnimator);

    if (self->handAnimator.frameID == 5)
        self->drawGroup = Zone->objectDrawGroup[1];

    if (self->handAnimator.frameID == 10) {
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 2, &self->crystalAnimator, true, 0);
        self->state = PhantomHand_State_Crystalize;
    }
}

void PhantomHand_State_Crystalize(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->handAnimator);
    RSDK.ProcessAnimation(&self->crystalAnimator);

    if (self->crystalAnimator.frameID == self->crystalAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(-1, 0, &self->handAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 3, &self->shineAnimator, true, 0);
        self->state = PhantomHand_State_Shine;
    }
}

void PhantomHand_State_Shine(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&self->shineAnimator);

    if (self->shineAnimator.frameID == self->shineAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(-1, 0, &self->shineAnimator, true, 0);

        if (self->doScanlineCB)
            PhantomEgg_SetupWarpFX();

        self->state = StateMachine_None;
    }
}

void PhantomHand_State_BreakApart(void)
{
    RSDK_THIS(PhantomHand);

    Debris_CreateFromEntries(PhantomHand->aniFrames, PhantomHand->debrisInfo, 4);
    RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &PhantomHand->hitbox)) {
            player->state                                 = Player_State_Air;
            player->velocity.y                            = -0x40000;
            player->onGround                              = false;
            player->groundVel                             = 0;
            PhantomHand->grabbedPlayers[player->playerID] = false;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        }
    }

    destroyEntity(self);
}

void PhantomHand_State_Disappear(void)
{
    RSDK_THIS(PhantomHand);

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 8;
}

#if RETRO_INCLUDE_EDITOR
void PhantomHand_EditorDraw(void)
{
    RSDK_THIS(PhantomHand);
    RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 0, &self->handAnimator, true, 0);

    RSDK.DrawSprite(&self->handAnimator, NULL, false);
}

void PhantomHand_EditorLoad(void) { PhantomHand->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomHand.bin", SCOPE_STAGE); }
#endif

void PhantomHand_Serialize(void) {}
