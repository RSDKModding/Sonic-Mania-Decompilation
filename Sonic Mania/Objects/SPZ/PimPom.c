#include "SonicMania.h"

ObjectPimPom *PimPom;

void PimPom_Update(void)
{
    RSDK_THIS(PimPom);

    StateMachine_Run(self->state);
    StateMachine_Run(self->stateMove);

    if (self->timer > 0)
        self->timer--;
}

void PimPom_LateUpdate(void) {}

void PimPom_StaticUpdate(void) {}

void PimPom_Draw(void)
{
    RSDK_THIS(PimPom);

    Vector2 drawPos = self->drawPos;
    drawPos.x += self->offset.x;
    drawPos.y -= self->offset.y;
    for (int i = self->length; i >= 0; --i) {
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
        self->drawOrder     = Zone->drawOrderLow + 1;
        self->angle2        = 0x20 * self->angle;
        self->angle         = 0x20 * self->angle;
        self->negAngle      = 0x100 - self->angle;
        self->rotation      = 2 * self->negAngle;

        int offset = 8;
        if (self->type)
            offset = 24;

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
                self->state         = PimPom_State0_Unknown;
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
                    self->state         = PimPom_State1_Unknown;
                }
                else {
                    int len               = self->length * (self->gap + 24);
                    self->hitbox.bottom = 8;
                    self->state         = PimPom_State1_Unknown;
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
                if (self->direction == FLIP_X)
                    self->angle = 0x20;
                else
                    self->angle = 0xE0;
                self->negAngle = 0x100 - self->angle;
                self->state    = PimPom_State1_Unknown;
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
                    int len               = self->length * (self->gap + 24);
                    self->hitbox.bottom = 8;
                    self->hitbox.left   = -24 - len;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = len + 24;
                }
                self->state = PimPom_State3_Unknown;
                break;
            default: break;
        }

        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;

        int len = 0;

        switch (self->moveType) {
            case PIMPOM_MOVE_NONE:
            default:
                self->stateMove     = PimPom_StateMove_None;
                self->updateRange.x = (self->length * (self->gap + 24) + 88) << 16;
                self->updateRange.y = (self->length * (self->gap + 24) + 88) << 16;
                break;
            case PIMPOM_MOVE_NORMAL:
                len                   = (self->length * (self->gap + 24) + 88) << 6;
                self->updateRange.x = (abs(self->amplitude.x) + len) << 10;
                self->updateRange.y = (abs(self->amplitude.y) + len) << 10;
                self->stateMove     = PimPom_StateMove_Normal;
                break;
            case PIMPOM_MOVE_CIRCLE:
                len                   = (self->length * (self->gap + 24)) << 6;
                self->updateRange.x = (abs(self->amplitude.x) + len + 0x1600) << 10;
                self->updateRange.y = (abs(self->amplitude.y) + len + 0x1600) << 10;
                self->stateMove     = PimPom_StateMove_Circle;
                break;
            case PIMPOM_MOVE_3:
                len                   = (self->length * (self->gap + 24)) << 7;
                self->updateRange.x = (abs(self->amplitude.x) + len + 0x2C00) << 9;
                self->updateRange.y = (abs(self->amplitude.y) + len + 0x2C00) << 9;
                self->stateMove     = PimPom_StateMove3_Unknown;
                break;
            case PIMPOM_MOVE_4:
                self->active        = ACTIVE_NEVER;
                self->stateMove     = PimPom_StateMove_ToTarget;
                self->updateRange.x = (self->length * (self->gap + 24) + 88) << 16;
                self->updateRange.y = (self->length * (self->gap + 24) + 88) << 16;
                break;
        }
    }
}

void PimPom_StageLoad(void)
{
    PimPom->aniFrames  = RSDK.LoadSpriteAnimation("SPZ2/PimPom.bin", SCOPE_STAGE);
    PimPom->sfxPimPom  = RSDK.GetSFX("Stage/PimPom.wav");
    PimPom->sfxBumper2 = RSDK.GetSFX("Stage/Bumper2.wav");
}

void PimPom_State0_Unknown(void)
{
    RSDK_THIS(PimPom);

    int storeX       = self->position.x;
    int storeY       = self->position.y;
    self->position = self->drawPos;
    self->position.x += self->offset.x;
    self->position.y -= self->offset.y;

    for (int l = 0; l <= self->length; ++l) {

        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &self->hitbox)) {
                int angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

                int vel = abs(player->velocity.y + player->velocity.x) >> 8;
                if (vel < 0x600)
                    vel = 0x600;

                int power = RSDK.Rand(0x400, vel);
                angle += RSDK.Rand(-6, 6);

                if (player->playerAnimator.animationID != ANI_FLY) {
                    player->velocity.x = power * RSDK.Cos256(angle);
                    player->groundVel  = power * RSDK.Cos256(angle);
                }
                player->velocity.y  = power * RSDK.Sin256(angle);
                player->onGround    = false;
                player->jumpAbility = 0;
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->state = Player_State_Air;
                }
#endif
                if (self->timer < 1) {
                    RSDK.PlaySfx(PimPom->sfxPimPom, false, 255);
                    self->timer = 8;
                }
            }
        }

        self->position.x -= self->moveAmount.x;
        self->position.y += self->moveAmount.y;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void PimPom_State1_Unknown(void)
{
    RSDK_THIS(PimPom);

    int startX       = self->position.x;
    int startY       = self->position.y;
    self->position = self->drawPos;

    int len = 0;
    if (self->gap >= 16) {
        self->position.x += self->offset.x;
        self->position.y -= self->offset.y;
        len = self->length;
    }

    for (int l = 0; l <= len; ++l) {

        foreach_active(Player, player)
        {
            int playerX    = player->position.x;
            int playerY    = player->position.y;
            int playerVelX = player->velocity.x;
            int playerVelY = player->velocity.y;

            int distX = (player->position.x - self->position.x) >> 8;
            int distY = (player->position.y - self->position.y) >> 8;

            player->position.x = self->position.x + distY * RSDK.Sin256(self->negAngle) + distX * RSDK.Cos256(self->negAngle);
            player->position.y = self->position.y - distX * RSDK.Sin256(self->negAngle) + distY * RSDK.Cos256(self->negAngle);

            int velX = player->velocity.x >> 8;
            int velY = player->velocity.y >> 8;

            player->velocity.x = velY * RSDK.Sin256(self->negAngle) + velX * RSDK.Cos256(self->negAngle);
            player->velocity.y = velY * RSDK.Cos256(self->negAngle) - velX * RSDK.Sin256(self->negAngle);

            int storedVelX = player->velocity.x;
            int storedVelY = player->velocity.y;

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

            int side = RSDK.CheckObjectCollisionBox(self, &self->hitbox, player, &hitbox, true);
            switch (side) {
                case C_NONE:
                    player->position.x = playerX;
                    player->position.y = playerY;
                    player->velocity.x = playerVelX;
                    player->velocity.y = playerVelY;
                    break;
                case C_TOP: player->velocity.y = clampVal(-(storedVelY + (storedVelY >> 4)), -0xE0000, -0x40000);
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                        player->state = Player_State_Air;
                    }
#endif
                    break;
                case C_LEFT: player->velocity.x = clampVal(-storedVelX, -0x80000, -0x20000);
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                        player->state = Player_State_Air;
                    }
#endif
                    break;
                case C_RIGHT: player->velocity.x = clampVal(-storedVelX, 0x20000, 0x80000);
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                        player->state = Player_State_Air;
                    }
#endif
                    break;
                case C_BOTTOM: player->velocity.y = clampVal(-(storedVelY + (storedVelY >> 4)), 0x40000, 0xE0000);
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                        player->state = Player_State_Air;
                    }
#endif
                    break;
                default: break;
            }

            if (side) {
                player->onGround    = false;
                player->jumpAbility = 0;

                distX = (player->position.x - self->position.x) >> 8;
                distY = (player->position.y - self->position.y) >> 8;

                player->position.x = distY * RSDK.Sin256(self->angle) + distX * RSDK.Cos256(self->angle) + self->position.x;
                player->position.y = self->position.y - distX * RSDK.Sin256(self->angle) + distY * RSDK.Cos256(self->angle);

                velX = player->velocity.x >> 8;
                velY = player->velocity.y >> 8;

                player->velocity.x = velY * RSDK.Sin256(self->angle) + velX * RSDK.Cos256(self->angle);
                player->velocity.y = velY * RSDK.Cos256(self->angle) - velX * RSDK.Sin256(self->angle);

                if (self->timer < 1) {
                    RSDK.PlaySfx(PimPom->sfxBumper2, false, 255);
                    self->timer = 8;
                }
            }
        }

        self->position.x -= self->moveAmount.x;
        self->position.y += self->moveAmount.y;
    }

    self->position.x = startX;
    self->position.y = startY;
}

void PimPom_State3_Unknown(void)
{
    RSDK_THIS(PimPom);

    int spinAngle = Zone->timer * self->spinSpeed;
    if (self->direction)
        spinAngle = -spinAngle;

    self->angle    = (self->angle2 & 0xFF) + (spinAngle >> 1);
    self->negAngle = 0x100 - self->angle;
    self->rotation = 2 * self->negAngle;

    self->offset.x = ((self->length * (self->gap + 24)) << 8) * RSDK.Cos256(self->angle);
    self->offset.y = ((self->length * (self->gap + 24)) << 8) * RSDK.Sin256(self->angle);

    self->moveAmount.x = ((self->gap + 24) * RSDK.Cos256(self->angle)) << 9;
    self->moveAmount.y = ((self->gap + 24) * RSDK.Sin256(self->angle)) << 9;

    PimPom_State1_Unknown();
}

void PimPom_StateMove_None(void)
{
    RSDK_THIS(PimPom);

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;
}

void PimPom_StateMove_Normal(void)
{
    RSDK_THIS(PimPom);

    int drawX         = self->drawPos.x;
    int drawY         = self->drawPos.y;
    self->drawPos.x = self->position.x + self->amplitude.x * RSDK.Sin1024(self->speed * Zone->timer);
    self->drawPos.y = self->position.y + self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer);
    int moveX         = self->drawPos.x - drawX;
    int moveY         = self->drawPos.y - drawY;

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_StateMove_Circle(void)
{
    RSDK_THIS(PimPom);

    int drawX         = self->drawPos.x;
    int drawY         = self->drawPos.y;
    self->drawPos.x = self->position.x + self->amplitude.x * RSDK.Cos1024(self->speed * Zone->timer + 4 * self->angleM);
    self->drawPos.y = self->position.y + self->amplitude.y * RSDK.Sin1024(self->speed * Zone->timer + 4 * self->angleM);
    int moveX         = self->drawPos.x - drawX;
    int moveY         = self->drawPos.y - drawY;

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_StateMove_ToTarget(void)
{
    RSDK_THIS(PimPom);

    int drawX = self->drawPos.x;
    int drawY = self->drawPos.y;
    self->drawPos.x += self->velocity.x;
    self->drawPos.y += self->velocity.y;

    Entity *target = RSDK_GET_ENTITY(self->speed, );

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

    int moveX = self->drawPos.x - drawX;
    int moveY = self->drawPos.y - drawY;

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_StateMove3_Unknown(void)
{
    RSDK_THIS(PimPom);

    int drawX = self->drawPos.x;
    int drawY = self->drawPos.y;

    if (((Zone->timer >> 9) & 1) == self->direction) {
        self->drawPos.x = self->position.x + ((((Zone->timer & 0xFFFF) << 7) * self->amplitude.x) >> 6) - (self->amplitude.x << 15);
        self->drawPos.y = self->position.y + ((((Zone->timer & 0xFFFF) << 7) * self->amplitude.y) >> 6) - (self->amplitude.y << 15);
    }
    else {
        self->drawPos.x = self->position.x + (self->amplitude.x << 15) - ((((Zone->timer & 0xFFFF) << 7) * self->amplitude.x) >> 6);
        self->drawPos.y = self->position.y + (self->amplitude.y << 15) - ((((Zone->timer & 0xFFFF) << 7) * self->amplitude.y) >> 6);
    }

    int moveX = self->drawPos.x - drawX;
    int moveY = self->drawPos.y - drawY;

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < self->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

#if RETRO_INCLUDE_EDITOR
void PimPom_EditorDraw(void)
{
    RSDK_THIS(PimPom);

    self->drawFX    = FX_NONE;
    self->direction = FLIP_NONE;
    self->active    = ACTIVE_BOUNDS;
    self->visible   = true;

    self->updateRange.x = (40 - (self->length * (self->gap + 24))) << 16;
    self->updateRange.y = (40 - (self->length * (self->gap + 24))) << 16;
    self->drawOrder     = Zone->drawOrderLow + 1;
    self->angle2        = 0x20 * self->angle;
    self->angle         = 0x20 * self->angle;
    self->negAngle      = 0x100 - self->angle;
    self->rotation      = 2 * self->negAngle;

    int offset = 8;
    if (self->type)
        offset = 24;

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
            self->state         = PimPom_State0_Unknown;
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
            self->drawFX = FX_ROTATE | FX_FLIP;
            if (self->direction == FLIP_X)
                self->angle = 0x20;
            else
                self->angle = 0xE0;
            self->negAngle = 0x100 - self->angle;
            break;
        case PIMPOM_VERTICAL:
            RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &self->animator, true, 0);
            self->drawFX = FX_ROTATE;
            break;
        default: break;
    }

    int ax = self->amplitude.x >> 10;
    int ay = self->amplitude.y >> 10;

    int len = 0;

    switch (self->moveType) {
        case PIMPOM_MOVE_NONE:
        default:
            self->updateRange.x = (self->length * (self->gap + 24) + 88) << 16;
            self->updateRange.y = (self->length * (self->gap + 24) + 88) << 16;
            break;
        case PIMPOM_MOVE_NORMAL:
            len                   = (self->length * (self->gap + 24) + 88) << 6;
            self->updateRange.x = (abs(ax) + len) << 10;
            self->updateRange.y = (abs(ay) + len) << 10;
            break;
        case PIMPOM_MOVE_CIRCLE:
            len                   = (self->length * (self->gap + 24)) << 6;
            self->updateRange.x = (abs(ax) + len + 0x1600) << 10;
            self->updateRange.y = (abs(ay) + len + 0x1600) << 10;
            break;
        case PIMPOM_MOVE_3:
            len                   = (self->length * (self->gap + 24)) << 7;
            self->updateRange.x = (abs(ax) + len + 0x2C00) << 9;
            self->updateRange.y = (abs(ay) + len + 0x2C00) << 9;
            break;
        case PIMPOM_MOVE_4:
            self->updateRange.x = (self->length * (self->gap + 24) + 88) << 16;
            self->updateRange.y = (self->length * (self->gap + 24) + 88) << 16;
            break;
    }

    PimPom_Draw();
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
    RSDK_ENUM_VAR("None", PIMPOM_MOVE_NONE);
    RSDK_ENUM_VAR("Normal", PIMPOM_MOVE_NORMAL);
    RSDK_ENUM_VAR("Circle", PIMPOM_MOVE_CIRCLE);
    RSDK_ENUM_VAR("Type 3", PIMPOM_MOVE_3);
    RSDK_ENUM_VAR("Type 4", PIMPOM_MOVE_4);
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
