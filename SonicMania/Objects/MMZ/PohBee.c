// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PohBee Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPohBee *PohBee;

void PohBee_Update(void)
{
    RSDK_THIS(PohBee);

    RSDK.ProcessAnimation(&self->bodyAnimator);
    RSDK.ProcessAnimation(&self->wingsAnimator);

    StateMachine_Run(self->state);

    PohBee_SetupHitboxes();
    PohBee_CheckPlayerCollisions();

    if (self->drawGroup != 1 && self->state != PohBee_State_Init)
        PohBee_CheckOffScreen();
}

void PohBee_LateUpdate(void) {}

void PohBee_StaticUpdate(void) {}

void PohBee_Draw(void) { PohBee_DrawSprites(); }

void PohBee_Create(void *data)
{
    RSDK_THIS(PohBee);

    self->visible = true;

    if (self->planeFilter > 0 && ((uint8)(self->planeFilter - 1) & 2))
        self->drawGroup = Zone->objectDrawGroup[1];
    else
        self->drawGroup = Zone->objectDrawGroup[0];

    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = PohBee_State_Init;
}

void PohBee_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        PohBee->aniFrames = RSDK.LoadSpriteAnimation("MMZ/PohBee.bin", SCOPE_STAGE);

    PohBee->hitbox.left   = -16;
    PohBee->hitbox.top    = -16;
    PohBee->hitbox.right  = 16;
    PohBee->hitbox.bottom = 16;

    DEBUGMODE_ADD_OBJ(PohBee);
}

void PohBee_DebugSpawn(void)
{
    RSDK_THIS(PohBee);

    EntityPohBee *pohBee = CREATE_ENTITY(PohBee, NULL, self->position.x, self->position.y);
    pohBee->direction    = self->direction;
    pohBee->startDir     = self->direction;
}

void PohBee_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void PohBee_CheckOffScreen(void)
{
    RSDK_THIS(PohBee);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->direction = self->startDir;
        self->position  = self->startPos;
        PohBee_Create(NULL);
    }
}

void PohBee_CheckPlayerCollisions(void)
{
    RSDK_THIS(PohBee);

    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
            if (Player_CheckBadnikTouch(player, self, &PohBee->hitbox)) {
                if (self->drawGroup == 1) {
                    if (Player_CheckBadnikBreak(player, self, false)) {
                        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = 1;
                        destroyEntity(self);
                    }
                }
                else {
                    Player_CheckBadnikBreak(player, self, true);
                }
            }

            for (int32 i = 0; i < self->spikeCount + 1; ++i) {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxes[i])) {
#if MANIA_USE_PLUS
                    int32 storeX   = self->position.x;
                    int32 storeY   = self->position.y;
                    self->position = PohBee_GetSpikePos(i, (self->drawGroup != 1) + 6);
                    if (!Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer)) {
#endif
                        Player_Hurt(player, self);
#if MANIA_USE_PLUS
                    }
                    self->position.x = storeX;
                    self->position.y = storeY;
#endif
                }
            }
        }
    }
}

void PohBee_DrawSprites(void)
{
    RSDK_THIS(PohBee);

    self->drawFX = FX_FLIP;

    int32 offsetY = 16;
    int32 shift   = 7;
    if (self->drawGroup == 1) {
        offsetY = 15;
        shift   = 6;
        self->drawFX |= FX_SCALE;
    }

    if (self->bodyAnimator.animationID != 1)
        RSDK.DrawSprite(&self->wingsAnimator, NULL, false);

    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    Vector2 chainDrawPos;
    chainDrawPos.x = self->position.x;
    chainDrawPos.y = self->position.y + (16 << offsetY);
    self->drawFX |= FX_ROTATE;

    int32 id = 4;
    for (int32 i = 0; i < 10; ++i) {
        Vector2 drawPos = chainDrawPos;
        drawPos.x += (id * RSDK.Cos512(self->chainAngle[0])) << shift;
        drawPos.y += (id * RSDK.Sin512(self->chainAngle[0])) << shift;
        RSDK.DrawSprite(&self->chainAnimator, &drawPos, false);
        id += 8;
    }

    Vector2 drawPos = PohBee_GetSpikePos(0, shift);
    RSDK.DrawSprite(&self->spikeAnimator, &drawPos, false);

    if (self->spikeCount == 1) {
        id = 4;
        for (int32 i = 0; i < 4; ++i) {
            drawPos = chainDrawPos;
            drawPos.x += (id * RSDK.Cos512(self->chainAngle[1])) << shift;
            drawPos.y += (id * RSDK.Sin512(self->chainAngle[1])) << shift;
            RSDK.DrawSprite(&self->chainAnimator, &drawPos, false);
            id += 8;
        }

        drawPos = PohBee_GetSpikePos(1, shift);
        RSDK.DrawSprite(&self->spikeAnimator, &drawPos, false);
    }
}

Vector2 PohBee_GetSpikePos(uint8 spikeID, uint8 shift)
{
    RSDK_THIS(PohBee);

    Vector2 spikePos = self->position;
    spikePos.y       = self->position.y + (self->drawGroup == 1 ? 0x100000 : 0x80000);

    switch (spikeID) {
        default: break;

        case 0:
            spikePos.x += (84 * RSDK.Cos512(self->chainAngle[0])) << shift;
            spikePos.y += (84 * RSDK.Sin512(self->chainAngle[0])) << shift;
            break;

        case 1:
            spikePos.x += (36 * RSDK.Cos512(self->chainAngle[1])) << shift;
            spikePos.y += (36 * RSDK.Sin512(self->chainAngle[1])) << shift;
            break;
    }

    return spikePos;
}

void PohBee_SetupHitboxes(void)
{
    RSDK_THIS(PohBee);

    for (int32 i = 0; i < 2; ++i) {
        Vector2 pos = PohBee_GetSpikePos(i, 7);
        int32 x     = (pos.x - self->position.x) >> 16;
        int32 y     = (pos.y - self->position.y) >> 16;

        Hitbox hitbox;
        hitbox.left   = x - 12;
        hitbox.top    = y - 12;
        hitbox.right  = x + 12;
        hitbox.bottom = y + 12;
        if (self->direction == FLIP_X) {
            hitbox.left  = 12 - x;
            hitbox.right = -hitbox.right;
        }

        self->hitboxes[i] = hitbox;
    }
}

void PohBee_State_Init(void)
{
    RSDK_THIS(PohBee);

    if (self->drawGroup != 1)
        self->active = ACTIVE_NORMAL;

    self->position      = self->startPos;
    self->direction     = self->startDir;
    self->chainAngle[0] = 0x80;
    self->chainAngle[1] = 0xAC;

    RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 2, &self->wingsAnimator, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 3, &self->chainAnimator, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 4, &self->spikeAnimator, true, 0);

    self->state = PohBee_State_Move;

    PohBee_State_Move();
}

void PohBee_State_Move(void)
{
    RSDK_THIS(PohBee);

    int32 distX = abs(self->position.x - self->startPos.x);

    int32 move = (2 * (self->direction != FLIP_NONE) - 1) << 15;
    if (self->bodyAnimator.animationID == 1) {
        switch (self->bodyAnimator.frameID) {
            default:
            case 0: break;

            case 1: move >>= 1; break;
            case 2: move = 0; break;
            case 3: move = (-0x8000 * (2 * (self->direction != FLIP_NONE) - 1)) >> 1; break;
            case 4: move = -0x8000 * (2 * (self->direction != FLIP_NONE) - 1); break;
        }
    }

    self->position.x += move;

    if (distX < self->amplitude.x) {
        self->isTurning = false;
    }
    else if (!self->bodyAnimator.animationID && !self->isTurning) {
        RSDK.SetSpriteAnimation(PohBee->aniFrames, 1, &self->bodyAnimator, true, 0);
        self->isTurning = true;
    }
    if (self->bodyAnimator.animationID == 1 && self->bodyAnimator.frameID == self->bodyAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &self->bodyAnimator, true, 0);
        self->direction = self->direction == FLIP_NONE;
    }

    self->angle      = (self->angle + 8) & 0x1FF;
    self->position.y = self->startPos.y + (RSDK.Sin512(self->angle) << 7) * (self->amplitude.y >> 16);

    self->chainPos[0]   = (self->chainPos[0] + 9) & 0x7FF;
    self->chainAngle[0] = (0x70 * ((RSDK.Sin512(self->chainPos[0] >> 2) >> 2) + 0x88)) >> 7;

    self->chainPos[1]   = (self->chainPos[1] + 10) & 0x7FF;
    self->chainAngle[1] = (0x70 * ((RSDK.Sin512(self->chainPos[1] >> 2) >> 2) + 0x90)) >> 7;
}

#if GAME_INCLUDE_EDITOR
void PohBee_EditorDraw(void)
{
    RSDK_THIS(PohBee);

    self->chainAngle[0] = 0x80;
    self->chainAngle[1] = 0xAC;
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 2, &self->wingsAnimator, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 3, &self->chainAnimator, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 4, &self->spikeAnimator, true, 0);

    PohBee_DrawSprites();
}

void PohBee_EditorLoad(void)
{
    PohBee->aniFrames = RSDK.LoadSpriteAnimation("MMZ/PohBee.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PohBee, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);

    RSDK_ACTIVE_VAR(PohBee, spikeCount);
    RSDK_ENUM_VAR("1 Spike", 0);
    RSDK_ENUM_VAR("2 Spikes", 1);

    RSDK_ACTIVE_VAR(PohBee, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);
}
#endif

void PohBee_Serialize(void)
{
    RSDK_EDITABLE_VAR(PohBee, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PohBee, VAR_UINT8, spikeCount);
    RSDK_EDITABLE_VAR(PohBee, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(PohBee, VAR_ENUM, planeFilter);
}
