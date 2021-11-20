#include "SonicMania.h"

ObjectPohBee *PohBee;

void PohBee_Update(void)
{
    RSDK_THIS(PohBee);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    StateMachine_Run(self->state);

    PohBee_SetupHitboxes();
    PohBee_CheckPlayerCollisions();
    if (self->drawOrder != 1 && self->state != PohBee_State_Setup)
        PohBee_CheckOnScreen();
}

void PohBee_LateUpdate(void) {}

void PohBee_StaticUpdate(void) {}

void PohBee_Draw(void) { PohBee_DrawSprites(); }

void PohBee_Create(void *data)
{
    RSDK_THIS(PohBee);

    self->visible = true;
    if (self->planeFilter > 0 && ((uint8)(self->planeFilter - 1) & 2))
        self->drawOrder = Zone->drawOrderHigh;
    else
        self->drawOrder = Zone->drawOrderLow;

    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = PohBee_State_Setup;
}

void PohBee_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
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

void PohBee_CheckOnScreen(void)
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
                if (self->drawOrder == 1) {
                    if (Player_CheckBadnikBreak(self, player, false)) {
                        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = 1;
                        destroyEntity(self);
                    }
                }
                else {
                    Player_CheckBadnikBreak(self, player, true);
                }
            }

            for (int32 i = 0; i < self->spikeCount + 1; ++i) {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxes[i])) {
#if RETRO_USE_PLUS
                    int32 storeX       = self->position.x;
                    int32 storeY       = self->position.y;
                    self->position = PohBee_GetSpikePos(i, (self->drawOrder != 1) + 6);
                    if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer)) {
#endif
                        Player_CheckHit(player, self);
#if RETRO_USE_PLUS
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
    Vector2 drawPos;
    Vector2 drawPos2;

    self->drawFX = FX_FLIP;

    int32 offsetY   = 16;
    int32 shift = 7;
    if (self->drawOrder == 1) {
        offsetY   = 15;
        shift = 6;
        self->drawFX |= FX_SCALE;
    }

    if (self->animator1.animationID != 1)
        RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);

    drawPos2.x = self->position.x;
    drawPos2.y = self->position.y + (16 << offsetY);
    self->drawFX |= FX_ROTATE;

    int32 id = 4;
    for (int32 i = 0; i < 10; ++i) {
        drawPos = drawPos2;
        drawPos.x += (id * RSDK.Cos512(self->field_70)) << shift;
        drawPos.y += (id * RSDK.Sin512(self->field_70)) << shift;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);
        id += 8;
    }

    drawPos = PohBee_GetSpikePos(0, shift);
    RSDK.DrawSprite(&self->animator4, &drawPos, false);
    if (self->spikeCount == 1) {
        id = 4;
        for (int32 i = 0; i < 4; ++i) {
            drawPos = drawPos2;
            drawPos.x += (id * RSDK.Cos512(self->field_74)) << shift;
            drawPos.y += (id * RSDK.Sin512(self->field_74)) << shift;
            RSDK.DrawSprite(&self->animator3, &drawPos, false);
            id += 8;
        }
        drawPos = PohBee_GetSpikePos(1, shift);
        RSDK.DrawSprite(&self->animator4, &drawPos, false);
    }
}

Vector2 PohBee_GetSpikePos(uint8 spikeID, uint8 shift)
{
    RSDK_THIS(PohBee);

    Vector2 pos = self->position;

    int32 y = self->position.y;
    if (self->drawOrder == 1)
        y += 0x100000;
    else
        y += 0x80000;


    switch (spikeID) {
        case 0:
            pos.x += ((84 * RSDK.Cos512(self->field_70)) << shift);
            pos.y = y + ((84 * RSDK.Sin512(self->field_70)) << shift);
            break;
        case 1:
            pos.x += ((36 * RSDK.Cos512(self->field_74)) << shift);
            pos.y = y + ((4 * (9 * RSDK.Sin512(self->field_74))) << shift);
            break;
    }

    return pos;
}

void PohBee_SetupHitboxes(void)
{
    RSDK_THIS(PohBee);

    for (int32 i = 0; i < 2; ++i) {
        Vector2 pos = PohBee_GetSpikePos(i, 7);
        int32 x       = (pos.x - self->position.x) >> 16;
        int32 y       = (pos.y - self->position.y) >> 16;

        Hitbox hitbox;
        hitbox.left   = x - 12;
        hitbox.right  = x + 12;
        hitbox.bottom = y + 12;
        if (self->direction == FLIP_X) {
            hitbox.left  = 12 - x;
            hitbox.right = -hitbox.right;
        }
        hitbox.top          = y - 12;
        self->hitboxes[i] = hitbox;
    }
}

void PohBee_State_Setup(void)
{
    RSDK_THIS(PohBee);

    if (self->drawOrder != 1)
        self->active = ACTIVE_NORMAL;
    self->position  = self->startPos;
    self->direction = self->startDir;
    self->field_70  = 128;
    self->field_74  = 172;
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 2, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 3, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 4, &self->animator4, true, 0);
    self->state = PohBee_State_Move;
    PohBee_State_Move();
}

void PohBee_State_Move(void)
{
    RSDK_THIS(PohBee);

    int32 distX = abs(self->position.x - self->startPos.x);

    int32 move = (2 * (self->direction != FLIP_NONE) - 1) << 15;
    if (self->animator1.animationID == 1) {
        switch (self->animator1.frameID) {
            case 1: move >>= 1; break;
            case 2: move = 0; break;
            case 3: move = (-0x8000 * (2 * (self->direction != FLIP_NONE) - 1)) >> 1; break;
            case 4: move = -0x8000 * (2 * (self->direction != FLIP_NONE) - 1); break;
            default: break;
        }
    }
    self->position.x += move;

    if (distX < self->amplitude.x) {
        self->field_7C = 0;
    }
    else if (!self->animator1.animationID && !self->field_7C) {
        RSDK.SetSpriteAnimation(PohBee->aniFrames, 1, &self->animator1, true, 0);
        self->field_7C = 1;
    }
    if (self->animator1.animationID == 1 && self->animator1.frameID == self->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &self->animator1, true, 0);
        self->direction = self->direction == FLIP_NONE;
    }

    self->angle      = (self->angle + 8) & 0x1FF;
    self->position.y = self->startPos.y + (RSDK.Sin512(self->angle) << 7) * (self->amplitude.y >> 16);

    self->field_68 = (self->field_68 + 9) & 0x7FF;
    self->field_70 = (112 * ((RSDK.Sin512(self->field_68 >> 2) >> 2) + 136)) >> 7;

    self->field_6C = (self->field_6C + 10) & 0x7FF;
    self->field_74 = (112 * ((RSDK.Sin512(self->field_6C >> 2) >> 2) + 144)) >> 7;
}

void PohBee_EditorDraw(void)
{
    RSDK_THIS(PohBee);

    self->field_70  = 128;
    self->field_74  = 172;
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 2, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 3, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(PohBee->aniFrames, 4, &self->animator4, true, 0);

    PohBee_DrawSprites();
}

void PohBee_EditorLoad(void)
{
    PohBee->aniFrames = RSDK.LoadSpriteAnimation("MMZ/PohBee.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PohBee, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}

void PohBee_Serialize(void)
{
    RSDK_EDITABLE_VAR(PohBee, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PohBee, VAR_UINT8, spikeCount);
    RSDK_EDITABLE_VAR(PohBee, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(PohBee, VAR_ENUM, planeFilter);
}
