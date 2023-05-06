// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AIZTornado Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAIZTornado *AIZTornado;

void AIZTornado_Update(void)
{
    RSDK_THIS(AIZTornado);
    self->prevPos.x = self->position.x;
    self->prevPos.y = self->position.y;
    StateMachine_Run(self->state);

    self->animatorTornado.frameID = self->turnAngle >> 4;
    RSDK.ProcessAnimation(&self->animatorPropeller);
    RSDK.ProcessAnimation(&self->animatorPilot);
    RSDK.ProcessAnimation(&self->animatorFlame);
}

void AIZTornado_LateUpdate(void) {}

void AIZTornado_StaticUpdate(void) {}

void AIZTornado_Draw(void)
{
    RSDK_THIS(AIZTornado);
    RSDK.DrawSprite(&self->animatorPilot, NULL, false);
    RSDK.DrawSprite(&self->animatorPropeller, NULL, false);
    RSDK.DrawSprite(&self->animatorTornado, NULL, false);
    if (self->showFlame) {
        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.y += AIZTornado->flameOffsets[self->animatorTornado.frameID];
        RSDK.DrawSprite(&self->animatorFlame, &drawPos, false);
    }
}

void AIZTornado_Create(void *data)
{
    RSDK_THIS(AIZTornado);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->drawFX        = FX_FLIP;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->movePos.x     = self->position.x;
        self->movePos.y     = self->position.y;
        self->turnAngle     = 48;
        self->isStood       = true;
        self->offsetX       = 0x80000;
        self->showFlame     = true;

        if (RSDK.CheckSceneFolder("AIZ") || RSDK.CheckSceneFolder("Credits"))
            self->active = ACTIVE_NORMAL;

        if (!StarPost->postIDs[0]) {
            self->active = ACTIVE_NORMAL;
            self->state  = AIZTornado_State_Move;
        }

        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 0, &self->animatorTornado, true, 0);
        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 1, &self->animatorPropeller, true, 0);
        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 2, &self->animatorFlame, true, 0);
        if (GET_CHARACTER_ID(1) == ID_SONIC) // if sonic is the leader
            RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 6, &self->animatorPilot, true, 0);
        else
            RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 3, &self->animatorPilot, true, 0);
    }
}

void AIZTornado_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("AIZ") || RSDK.CheckSceneFolder("Credits"))
        AIZTornado->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZTornado.bin", SCOPE_STAGE);
}

void AIZTornado_HandleMovement(void)
{
    RSDK_THIS(AIZTornado);
    self->movePos.x += AIZTornadoPath->moveVel.x;
    self->movePos.y += AIZTornadoPath->moveVel.y;
    self->newPos.x = self->movePos.x;
    self->newPos.y = self->movePos.y;

    if (!self->disableInteractions) {
        int32 x = self->movePos.x;
        int32 y = self->movePos.y;
        if (AIZTornadoPath->moveVel.y) {
            self->turnAngle = 0;
        }
        else {
            y               = 0xA00 * RSDK.Sin256(2 * Zone->timer) + self->movePos.y;
            self->turnAngle = (RSDK.Sin256(2 * Zone->timer) >> 3) + 32;
        }
        self->newPos.x += (x - self->movePos.x) >> 1;
        self->newPos.y += (y - self->movePos.y) >> 1;
    }
    self->position.x = self->newPos.x;
    self->position.y = self->newPos.y;
}

void AIZTornado_HandlePlayerCollisions(void)
{
    RSDK_THIS(AIZTornado);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Hitbox *hitbox       = RSDK.GetHitbox(&self->animatorTornado, 0);
    if (self->turnAngle >= 32)
        player->drawGroup = self->drawGroup + 1;
    else
        player->drawGroup = self->drawGroup;
    int32 x = self->position.x;
    int32 y = self->position.y;
    self->prevPos.x &= 0xFFFF0000;
    self->prevPos.y &= 0xFFFF0000;
    self->isStood        = false;
    self->moveVelocity.x = (x & 0xFFFF0000) - self->prevPos.x;
    self->moveVelocity.y = (y & 0xFFFF0000) - self->prevPos.y;
    self->position.x     = self->prevPos.x;
    self->position.y     = self->prevPos.y;

    if (Player_CheckCollisionPlatform(player, self, hitbox)) {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_RIDE, &player->animator, false, 0);
        player->state = Player_State_Static;
        player->position.x += self->moveVelocity.x;
        player->position.y += self->moveVelocity.y;
        player->flailing = false;
        self->isStood    = true;
        if (player->velocity.y > 0x10000) {
            self->collideTimer = 0;
            self->gravityForce = 0x20000;
        }
    }
    self->position.x = x;
    self->position.y = y;
}

void AIZTornado_State_Move(void)
{
    RSDK_THIS(AIZTornado);
    self->prevPos.x = self->position.x;
    self->prevPos.y = self->position.y;
    AIZTornado_HandleMovement();
    if (!self->disableInteractions)
        AIZTornado_HandlePlayerCollisions();
}

#if GAME_INCLUDE_EDITOR
void AIZTornado_EditorDraw(void)
{
    RSDK_THIS(AIZTornado);
    RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 0, &self->animatorTornado, true, 0);
    RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 1, &self->animatorPropeller, true, 0);
    RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 2, &self->animatorFlame, true, 0);
    RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 3, &self->animatorPilot, true, 0);

    RSDK.DrawSprite(&self->animatorPilot, NULL, false);
    RSDK.DrawSprite(&self->animatorPropeller, NULL, false);
    RSDK.DrawSprite(&self->animatorTornado, NULL, false);
}

void AIZTornado_EditorLoad(void) { AIZTornado->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZTornado.bin", SCOPE_STAGE); }
#endif

void AIZTornado_Serialize(void) {}
