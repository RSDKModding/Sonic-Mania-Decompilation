// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PimPom Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPimPom *PimPom;

void PimPom_Update(void)
{
    RSDK_THIS(PimPom);

    StateMachine_Run(self->state);
    StateMachine_Run(self->stateMove);

    if (self->sfxTimer > 0)
        self->sfxTimer--;
}

void PimPom_LateUpdate(void) {}

void PimPom_StaticUpdate(void) {}

void PimPom_Draw(void)
{
    RSDK_THIS(PimPom);

    Vector2 drawPos = self->drawPos;
    drawPos.x       = self->drawPos.x + self->offset.x;
    drawPos.y       = self->drawPos.y - self->offset.y;

    for (int32 i = self->length; i >= 0; --i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x -= self->moveAmount.x;
        drawPos.y += self->moveAmount.y;
    }
}

void PimPom_Create(void *data)
{
    RSDK_THIS(PimPom);

    if (!SceneInfo->inEditor) {
        self->active  = ACTIVE_BOUNDS;
        self->visible = true;

        self->updateRange.x = (40 - (self->length * (self->gap + 24))) << 16;
        self->updateRange.y = (40 - (self->length * (self->gap + 24))) << 16;
        self->drawGroup     = Zone->objectDrawGroup[0] + 1;
        self->newAngle      = 0x20 * self->angle;
        self->angle         = 0x20 * self->angle;
        self->negAngle      = 0x100 - self->angle;
        self->rotation      = 2 * self->negAngle;

        int32 offset = self->type ? 24 : 8;

        self->offset.x = ((self->length * (offset + self->gap)) << 8) * RSDK.Cos256(self->angle);
        self->offset.y = ((self->length * (offset + self->gap)) << 8) * RSDK.Sin256(self->angle);

        self->moveAmount.x = ((offset + self->gap) * RSDK.Cos256(self->angle)) << 9;
        self->moveAmount.y = ((offset + self->gap) * RSDK.Sin256(self->angle)) << 9;
        self->drawPos      = self->position;

        switch (self->type) {
            case PIMPOM_SINGLE:
                RSDK.SetSpriteAnimation(PimPom->aniFrames, 0, &self->animator, true, self->color);
                self->hitbox.left   = -8;
                self->hitbox.top    = -8;
                self->hitbox.right  = 8;
                self->hitbox.bottom = 8;
                self->state         = PimPom_State_Single;
                break;

            case PIMPOM_HORIZONTAL:
                switch (self->angle) {
                    case 0x00: RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &self->animator, true, 0); break;

                    case 0x20:
                        self->drawFX    = FX_FLIP;
                        self->direction = FLIP_X;
                        RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &self->animator, true, 0);
                        break;

                    case 0x40: RSDK.SetSpriteAnimation(PimPom->aniFrames, 3, &self->animator, true, 0); break;
                    case 0x60: RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &self->animator, true, 0); break;
                    default: break;
                }

                if (self->gap >= 16) {
                    self->hitbox.left   = -24;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 24;
                    self->hitbox.bottom = 8;
                    self->state         = PimPom_State_Horizontal;
                }
                else {
                    int32 len           = self->length * (self->gap + 24);
                    self->hitbox.bottom = 8;
                    self->state         = PimPom_State_Horizontal;
                    self->hitbox.left   = -24 - len;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = len + 24;
                }
                break;

            case PIMPOM_DIAGONAL:
                RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &self->animator, true, 0);
                self->drawFX = FX_ROTATE | FX_FLIP;

                self->hitbox.left   = -24;
                self->hitbox.top    = -6;
                self->hitbox.right  = 24;
                self->hitbox.bottom = 6;
                self->angle         = self->direction == FLIP_X ? 0x20 : 0xE0;
                self->negAngle      = 0x100 - self->angle;
                self->state         = PimPom_State_Horizontal;
                break;

            case PIMPOM_VERTICAL:
                RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &self->animator, true, 0);
                self->drawFX = FX_ROTATE;

                if (self->gap >= 16) {
                    self->hitbox.left   = -24;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 24;
                    self->hitbox.bottom = 8;
                }
                else {
                    int32 len           = self->length * (self->gap + 24);
                    self->hitbox.bottom = 8;
                    self->hitbox.left   = -24 - len;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = len + 24;
                }

                self->state = PimPom_State_Vertical;
                break;

            default: break;
        }

        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;

        int32 len = 0;

        switch (self->moveType) {
            case PIMPOM_MOVE_FIXED:
            default:
                self->stateMove     = PimPom_Move_Fixed;
                self->updateRange.x = (self->length * (self->gap + 24) + 88) << 16;
                self->updateRange.y = (self->length * (self->gap + 24) + 88) << 16;
                break;

            case PIMPOM_MOVE_NORMAL:
                len                 = (self->length * (self->gap + 24) + 88) << 6;
                self->updateRange.x = (abs(self->amplitude.x) + len) << 10;
                self->updateRange.y = (abs(self->amplitude.y) + len) << 10;
                self->stateMove     = PimPom_Move_Normal;
                break;

            case PIMPOM_MOVE_CIRCLE:
                len                 = (self->length * (self->gap + 24)) << 6;
                self->updateRange.x = (abs(self->amplitude.x) + len + 0x1600) << 10;
                self->updateRange.y = (abs(self->amplitude.y) + len + 0x1600) << 10;
                self->stateMove     = PimPom_Move_Circle;
                break;

            case PIMPOM_MOVE_TRACK:
                len                 = (self->length * (self->gap + 24)) << 7;
                self->updateRange.x = (abs(self->amplitude.x) + len + 0x2C00) << 9;
                self->updateRange.y = (abs(self->amplitude.y) + len + 0x2C00) << 9;
                self->stateMove     = PimPom_Move_Track;
                break;

            case PIMPOM_MOVE_PATH:
                self->active        = ACTIVE_NEVER;
                self->stateMove     = PimPom_Move_Path;
                self->updateRange.x = (self->length * (self->gap + 24) + 88) << 16;
                self->updateRange.y = (self->length * (self->gap + 24) + 88) << 16;
                break;
        }
    }
}

void PimPom_StageLoad(void)
{
    PimPom->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/PimPom.bin", SCOPE_STAGE);

    PimPom->sfxPimPom  = RSDK.GetSfx("Stage/PimPom.wav");
    PimPom->sfxBumper2 = RSDK.GetSfx("Stage/Bumper2.wav");
}

void PimPom_State_Single(void)
{
    RSDK_THIS(PimPom);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->drawPos.x + self->offset.x;
    self->position.y = self->drawPos.y - self->offset.y;

    for (int32 l = 0; l <= self->length; ++l) {

        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &self->hitbox)) {
                int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

                int32 vel = MAX(abs(player->velocity.y + player->velocity.x) >> 8, 0x600);

                int32 power = RSDK.Rand(0x400, vel);
                angle += RSDK.Rand(-6, 6);

                if (player->animator.animationID != ANI_FLY) {
                    player->velocity.x = power * RSDK.Cos256(angle);
                    player->groundVel  = power * RSDK.Cos256(angle);
                }
                player->velocity.y   = power * RSDK.Sin256(angle);
                player->onGround     = false;
                player->applyJumpCap = false;

#if MANIA_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->state = Player_State_Air;
                }
#endif
                if (self->sfxTimer < 1) {
                    RSDK.PlaySfx(PimPom->sfxPimPom, false, 255);
                    self->sfxTimer = 8;
                }
            }
        }

        self->position.x -= self->moveAmount.x;
        self->position.y += self->moveAmount.y;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void PimPom_State_Horizontal(void)
{
    RSDK_THIS(PimPom);

    int32 startX = self->position.x;
    int32 startY = self->position.y;

    int32 len      = 0;
    self->position = self->drawPos;
    if (self->gap >= 16) {
        self->position.x += self->offset.x;
        self->position.y -= self->offset.y;
        len = self->length;
    }

    for (int32 l = 0; l <= len; ++l) {
        foreach_active(Player, player)
        {
            int32 playerX    = player->position.x;
            int32 playerY    = player->position.y;
            int32 playerVelX = player->velocity.x;
            int32 playerVelY = player->velocity.y;

            Vector2 originVel = { 0, 0 };

            Zone_RotateOnPivot(&player->position, &self->position, self->negAngle);
            Zone_RotateOnPivot(&player->velocity, &originVel, self->negAngle);

            int32 storedVelX = player->velocity.x;
            int32 storedVelY = player->velocity.y;

            Hitbox hitbox;
            Hitbox *playerHitbox = Player_GetHitbox(player);
            if ((((self->angle & 0xFF) - 32) & 0x7F) < 0x40) {
                hitbox.top    = playerHitbox->left;
                hitbox.bottom = playerHitbox->right;
                hitbox.left   = playerHitbox->top;
                hitbox.right  = playerHitbox->bottom;
            }
            else {
                hitbox = *playerHitbox;
            }

            uint8 side = RSDK.CheckObjectCollisionBox(self, &self->hitbox, player, &hitbox, true);
            switch (side) {
                case C_NONE:
                    player->position.x = playerX;
                    player->position.y = playerY;
                    player->velocity.x = playerVelX;
                    player->velocity.y = playerVelY;
                    break;

                case C_TOP: player->velocity.y = CLAMP(-(storedVelY + (storedVelY >> 4)), -0xE0000, -0x40000); break;
                case C_LEFT: player->velocity.x = CLAMP(-storedVelX, -0x80000, -0x20000); break;
                case C_RIGHT: player->velocity.x = CLAMP(-storedVelX, 0x20000, 0x80000); break;
                case C_BOTTOM: player->velocity.y = CLAMP(-(storedVelY + (storedVelY >> 4)), 0x40000, 0xE0000); break;

                default: break;
            }

            if (side != C_NONE) {
#if MANIA_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->state = Player_State_Air;
                }
#endif
                player->onGround     = false;
                player->applyJumpCap = false;

                Vector2 originVel = { 0, 0 };

                Zone_RotateOnPivot(&player->position, &self->position, self->angle);
                Zone_RotateOnPivot(&player->velocity, &originVel, self->angle);

                if (self->sfxTimer <= 0) {
                    RSDK.PlaySfx(PimPom->sfxBumper2, false, 255);
                    self->sfxTimer = 8;
                }
            }
        }

        self->position.x -= self->moveAmount.x;
        self->position.y += self->moveAmount.y;
    }

    self->position.x = startX;
    self->position.y = startY;
}

void PimPom_State_Vertical(void)
{
    RSDK_THIS(PimPom);

    int32 spinAngle = Zone->timer * self->spinSpeed;
    if (self->direction)
        spinAngle = -spinAngle;

    self->angle    = (self->newAngle & 0xFF) + (spinAngle >> 1);
    self->negAngle = 0x100 - self->angle;
    self->rotation = 2 * self->negAngle;

    self->offset.x = ((self->length * (self->gap + 24)) << 8) * RSDK.Cos256(self->angle);
    self->offset.y = ((self->length * (self->gap + 24)) << 8) * RSDK.Sin256(self->angle);

    self->moveAmount.x = ((self->gap + 24) * RSDK.Cos256(self->angle)) << 9;
    self->moveAmount.y = ((self->gap + 24) * RSDK.Sin256(self->angle)) << 9;

    PimPom_State_Horizontal();
}

void PimPom_Move_Fixed(void)
{
    RSDK_THIS(PimPom);

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;
}

void PimPom_Move_Normal(void)
{
    RSDK_THIS(PimPom);

    int32 drawX     = self->drawPos.x;
    int32 drawY     = self->drawPos.y;
    self->drawPos.x = self->position.x + self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer);
    self->drawPos.y = self->position.y + self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer);
    int32 moveX     = self->drawPos.x - drawX;
    int32 moveY     = self->drawPos.y - drawY;

    int32 slot = SceneInfo->entitySlot + 1;
    for (int32 i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY_GEN(slot + i);
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_Move_Circle(void)
{
    RSDK_THIS(PimPom);

    int32 drawX     = self->drawPos.x;
    int32 drawY     = self->drawPos.y;
    self->drawPos.x = self->position.x + self->amplitude.x * RSDK.Cos1024(self->speed * Zone->timer + 4 * self->angleM);
    self->drawPos.y = self->position.y + self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer + 4 * self->angleM);
    int32 moveX     = self->drawPos.x - drawX;
    int32 moveY     = self->drawPos.y - drawY;

    int32 slot = SceneInfo->entitySlot + 1;
    for (int32 i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY_GEN(slot + i);
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_Move_Path(void)
{
    RSDK_THIS(PimPom);

    int32 drawX = self->drawPos.x;
    int32 drawY = self->drawPos.y;
    self->drawPos.x += self->velocity.x;
    self->drawPos.y += self->velocity.y;

    Entity *target = RSDK_GET_ENTITY_GEN(self->speed);

    if (self->velocity.x <= 0) {
        if (self->drawPos.x < target->position.x)
            self->drawPos.x = target->position.x;
    }
    else if (self->drawPos.x > target->position.x)
        self->drawPos.x = target->position.x;

    if (self->velocity.y <= 0) {
        if (self->drawPos.y < target->position.y)
            self->drawPos.y = target->position.y;
    }
    else if (self->drawPos.y > target->position.y)
        self->drawPos.y = target->position.y;

    int32 moveX = self->drawPos.x - drawX;
    int32 moveY = self->drawPos.y - drawY;

    int32 slot = SceneInfo->entitySlot + 1;
    for (int32 i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY_GEN(slot + i);
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_Move_Track(void)
{
    RSDK_THIS(PimPom);

    int32 drawX = self->drawPos.x;
    int32 drawY = self->drawPos.y;

    if (((Zone->timer >> 9) & 1) == self->direction) {
        self->drawPos.x = self->position.x + ((((Zone->timer & 0xFFFF) << 7) * self->amplitude.x) >> 6) - (self->amplitude.x << 15);
        self->drawPos.y = self->position.y + ((((Zone->timer & 0xFFFF) << 7) * self->amplitude.y) >> 6) - (self->amplitude.y << 15);
    }
    else {
        self->drawPos.x = self->position.x + (self->amplitude.x << 15) - ((((Zone->timer & 0xFFFF) << 7) * self->amplitude.x) >> 6);
        self->drawPos.y = self->position.y + (self->amplitude.y << 15) - ((((Zone->timer & 0xFFFF) << 7) * self->amplitude.y) >> 6);
    }

    int32 moveX = self->drawPos.x - drawX;
    int32 moveY = self->drawPos.y - drawY;

    int32 slot = SceneInfo->entitySlot + 1;
    for (int32 i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY_GEN(slot + i);
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

#if GAME_INCLUDE_EDITOR
void PimPom_EditorDraw(void)
{
    RSDK_THIS(PimPom);

    self->drawFX    = FX_NONE;
    self->direction = FLIP_NONE;
    self->active    = ACTIVE_BOUNDS;
    self->visible   = true;

    self->updateRange.x = (40 - (self->length * (self->gap + 24))) << 16;
    self->updateRange.y = (40 - (self->length * (self->gap + 24))) << 16;
    self->drawGroup     = Zone->objectDrawGroup[0] + 1;
    self->newAngle      = 0x20 * self->angle;
    self->angle         = 0x20 * self->angle;
    self->negAngle      = 0x100 - self->angle;
    self->rotation      = 2 * self->negAngle;

    int32 offset = self->type ? 24 : 8;

    self->offset.x = ((self->length * (offset + self->gap)) << 8) * RSDK.Cos256(self->angle);
    self->offset.y = ((self->length * (offset + self->gap)) << 8) * RSDK.Sin256(self->angle);

    self->moveAmount.x = ((offset + self->gap) * RSDK.Cos256(self->angle)) << 9;
    self->moveAmount.y = ((offset + self->gap) * RSDK.Sin256(self->angle)) << 9;
    self->drawPos      = self->position;

    switch (self->type) {
        case PIMPOM_SINGLE:
            RSDK.SetSpriteAnimation(PimPom->aniFrames, 0, &self->animator, true, self->color);
            self->hitbox.left   = -8;
            self->hitbox.top    = -8;
            self->hitbox.right  = 8;
            self->hitbox.bottom = 8;
            self->state         = PimPom_State_Single;
            break;

        case PIMPOM_HORIZONTAL:
            switch (self->angle) {
                case 0x00: RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &self->animator, true, 0); break;

                case 0x20:
                    self->drawFX    = FX_FLIP;
                    self->direction = FLIP_X;
                    RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &self->animator, true, 0);
                    break;

                case 0x40: RSDK.SetSpriteAnimation(PimPom->aniFrames, 3, &self->animator, true, 0); break;
                case 0x60: RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &self->animator, true, 0); break;

                default: break;
            }
            break;

        case PIMPOM_DIAGONAL:
            RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &self->animator, true, 0);
            self->drawFX   = FX_ROTATE | FX_FLIP;
            self->angle    = self->direction == FLIP_X ? 0x20 : 0xE0;
            self->negAngle = 0x100 - self->angle;
            break;

        case PIMPOM_VERTICAL:
            RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &self->animator, true, 0);
            self->drawFX = FX_ROTATE;
            break;

        default: break;
    }

    Vector2 amplitude;
    amplitude.x = self->amplitude.x >> 10;
    amplitude.y = self->amplitude.y >> 10;

    int32 len = 0;

    switch (self->moveType) {
        case PIMPOM_MOVE_FIXED:
        default:
            self->updateRange.x = (self->length * (self->gap + 24) + 88) << 16;
            self->updateRange.y = (self->length * (self->gap + 24) + 88) << 16;
            break;

        case PIMPOM_MOVE_NORMAL:
            len                 = (self->length * (self->gap + 24) + 88) << 6;
            self->updateRange.x = (abs(amplitude.x) + len) << 10;
            self->updateRange.y = (abs(amplitude.y) + len) << 10;
            break;

        case PIMPOM_MOVE_CIRCLE:
            len                 = (self->length * (self->gap + 24)) << 6;
            self->updateRange.x = (abs(amplitude.x) + len + 0x1600) << 10;
            self->updateRange.y = (abs(amplitude.y) + len + 0x1600) << 10;
            break;

        case PIMPOM_MOVE_TRACK:
            len                 = (self->length * (self->gap + 24)) << 7;
            self->updateRange.x = (abs(amplitude.x) + len + 0x2C00) << 9;
            self->updateRange.y = (abs(amplitude.y) + len + 0x2C00) << 9;
            break;

        case PIMPOM_MOVE_PATH:
            self->updateRange.x = (self->length * (self->gap + 24) + 88) << 16;
            self->updateRange.y = (self->length * (self->gap + 24) + 88) << 16;
            break;
    }

    PimPom_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        Vector2 pos;
        switch (self->moveType) {
            case PIMPOM_MOVE_NORMAL:
                // draw distance previews
                self->inkEffect = INK_BLEND;
                self->drawPos.x = amplitude.x * RSDK.Sin1024(0x100) + self->position.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(0x100) + self->position.y;
                pos             = self->drawPos;
                PimPom_Draw();

                self->drawPos.x = amplitude.x * RSDK.Sin1024(0x300) + self->position.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(0x300) + self->position.y;
                PimPom_Draw();

                RSDK.DrawLine(pos.x, pos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, 0, INK_NONE, false);
                break;

            case PIMPOM_MOVE_CIRCLE:
                // draw distance preview
                self->inkEffect = INK_BLEND;
                self->drawPos.x = amplitude.x * RSDK.Cos1024(4 * self->angle) + self->position.x;
                self->drawPos.y = amplitude.y * RSDK.Sin1024(4 * self->angle) + self->position.y;

                PimPom_Draw();
                break;

            case PIMPOM_MOVE_TRACK:
                // draw distance preview
                self->inkEffect = INK_BLEND;

                // draw distance previews
                self->inkEffect = INK_BLEND;
                self->drawPos.x = self->position.x + (0x100 * amplitude.x >> 6) - (amplitude.x << 15);
                self->drawPos.y = self->position.y + (0x100 * amplitude.y >> 6) - (amplitude.y << 15);
                pos             = self->drawPos;
                PimPom_Draw();

                self->drawPos.x = self->position.x + (amplitude.x << 15) - (0x000 * amplitude.x >> 6);
                self->drawPos.y = self->position.y + (amplitude.y << 15) - (0x000 * amplitude.y >> 6);
                PimPom_Draw();

                RSDK.DrawLine(pos.x, pos.y, self->drawPos.x, self->drawPos.y, 0x00FF00, 0x00, INK_NONE, false);
                break;

            case PIMPOM_MOVE_PATH: {
                Entity *target = RSDK_GET_ENTITY_GEN(self->speed);

                if (target)
                    DrawHelpers_DrawArrow(self->position.x, self->position.y, target->position.x, target->position.y, 0x00FF00, INK_NONE, 0xFF);
                break;
            }

            default:
            case PIMPOM_MOVE_FIXED:
                // nothin'
                break;
        }

        self->inkEffect = INK_NONE;

        int32 slot = SceneInfo->entitySlot + 1;
        for (int32 i = 0; i < self->numChildren; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(slot + i);

            if (child) {
                DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
            }
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void PimPom_EditorLoad(void)
{
    PimPom->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/PimPom.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PimPom, type);
    RSDK_ENUM_VAR("Single", PIMPOM_SINGLE);
    RSDK_ENUM_VAR("Horizontal", PIMPOM_HORIZONTAL);
    RSDK_ENUM_VAR("Diagonal", PIMPOM_DIAGONAL);
    RSDK_ENUM_VAR("Vertical", PIMPOM_VERTICAL);

    RSDK_ACTIVE_VAR(PimPom, moveType);
    RSDK_ENUM_VAR("Fixed", PIMPOM_MOVE_FIXED);
    RSDK_ENUM_VAR("Normal", PIMPOM_MOVE_NORMAL);
    RSDK_ENUM_VAR("Circle", PIMPOM_MOVE_CIRCLE);
    RSDK_ENUM_VAR("Track", PIMPOM_MOVE_TRACK);
    RSDK_ENUM_VAR("Path", PIMPOM_MOVE_PATH);

    RSDK_ACTIVE_VAR(PimPom, color);
    RSDK_ENUM_VAR("Cyan", PIMPOM_CLR_CYAN);
    RSDK_ENUM_VAR("Purple", PIMPOM_CLR_PURPLE);
    RSDK_ENUM_VAR("Green", PIMPOM_CLR_GREEN);

    RSDK_ACTIVE_VAR(PimPom, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void PimPom_Serialize(void)
{
    RSDK_EDITABLE_VAR(PimPom, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(PimPom, VAR_UINT8, color);
    RSDK_EDITABLE_VAR(PimPom, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PimPom, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(PimPom, VAR_UINT8, spinSpeed);
    RSDK_EDITABLE_VAR(PimPom, VAR_ENUM, moveType);
    RSDK_EDITABLE_VAR(PimPom, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(PimPom, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(PimPom, VAR_INT32, angleM);
    RSDK_EDITABLE_VAR(PimPom, VAR_UINT8, length);
    RSDK_EDITABLE_VAR(PimPom, VAR_UINT8, gap);
    RSDK_EDITABLE_VAR(PimPom, VAR_UINT8, numChildren);
}
