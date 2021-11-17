#include "SonicMania.h"

ObjectPimPom *PimPom;

void PimPom_Update(void)
{
    RSDK_THIS(PimPom);

    StateMachine_Run(entity->state);
    StateMachine_Run(entity->stateMove);

    if (entity->timer > 0)
        entity->timer--;
}

void PimPom_LateUpdate(void) {}

void PimPom_StaticUpdate(void) {}

void PimPom_Draw(void)
{
    RSDK_THIS(PimPom);

    Vector2 drawPos = entity->drawPos;
    drawPos.x += entity->offset.x;
    drawPos.y -= entity->offset.y;
    for (int i = entity->length; i >= 0; --i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x -= entity->moveAmount.x;
        drawPos.y += entity->moveAmount.y;
    }
}

void PimPom_Create(void *data)
{
    RSDK_THIS(PimPom);

    if (!SceneInfo->inEditor) {
        entity->active  = ACTIVE_BOUNDS;
        entity->visible = true;

        entity->updateRange.x = (40 - (entity->length * (entity->gap + 24))) << 16;
        entity->updateRange.y = (40 - (entity->length * (entity->gap + 24))) << 16;
        entity->drawOrder     = Zone->drawOrderLow + 1;
        entity->angle2        = 0x20 * entity->angle;
        entity->angle         = 0x20 * entity->angle;
        entity->negAngle      = 0x100 - entity->angle;
        entity->rotation      = 2 * entity->negAngle;

        int offset = 8;
        if (entity->type)
            offset = 24;

        entity->offset.x = ((entity->length * (offset + entity->gap)) << 8) * RSDK.Cos256(entity->angle);
        entity->offset.y = ((entity->length * (offset + entity->gap)) << 8) * RSDK.Sin256(entity->angle);

        entity->moveAmount.x = ((offset + entity->gap) * RSDK.Cos256(entity->angle)) << 9;
        entity->moveAmount.y = ((offset + entity->gap) * RSDK.Sin256(entity->angle)) << 9;
        entity->drawPos      = entity->position;

        switch (entity->type) {
            case PIMPOM_SINGLE:
                RSDK.SetSpriteAnimation(PimPom->aniFrames, 0, &entity->animator, true, entity->color);
                entity->hitbox.left   = -8;
                entity->hitbox.top    = -8;
                entity->hitbox.right  = 8;
                entity->hitbox.bottom = 8;
                entity->state         = PimPom_State0_Unknown;
                break;
            case PIMPOM_HORIZONTAL:
                switch (entity->angle) {
                    case 0x00: RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &entity->animator, true, 0); break;
                    case 0x20:
                        entity->drawFX    = FX_FLIP;
                        entity->direction = FLIP_X;
                        RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &entity->animator, true, 0);
                        break;
                    case 0x40: RSDK.SetSpriteAnimation(PimPom->aniFrames, 3, &entity->animator, true, 0); break;
                    case 0x60: RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &entity->animator, true, 0); break;
                    default: break;
                }

                if (entity->gap >= 16) {
                    entity->hitbox.left   = -24;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 24;
                    entity->hitbox.bottom = 8;
                    entity->state         = PimPom_State1_Unknown;
                }
                else {
                    int len               = entity->length * (entity->gap + 24);
                    entity->hitbox.bottom = 8;
                    entity->state         = PimPom_State1_Unknown;
                    entity->hitbox.left   = -24 - len;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = len + 24;
                }
                break;
            case PIMPOM_DIAGONAL:
                RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &entity->animator, true, 0);
                entity->drawFX = FX_ROTATE | FX_FLIP;

                entity->hitbox.left   = -24;
                entity->hitbox.top    = -6;
                entity->hitbox.right  = 24;
                entity->hitbox.bottom = 6;
                if (entity->direction == FLIP_X)
                    entity->angle = 0x20;
                else
                    entity->angle = 0xE0;
                entity->negAngle = 0x100 - entity->angle;
                entity->state    = PimPom_State1_Unknown;
                break;
            case PIMPOM_VERTICAL:
                RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &entity->animator, true, 0);
                entity->drawFX = FX_ROTATE;
                if (entity->gap >= 16) {
                    entity->hitbox.left   = -24;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 24;
                    entity->hitbox.bottom = 8;
                }
                else {
                    int len               = entity->length * (entity->gap + 24);
                    entity->hitbox.bottom = 8;
                    entity->hitbox.left   = -24 - len;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = len + 24;
                }
                entity->state = PimPom_State3_Unknown;
                break;
            default: break;
        }

        entity->amplitude.x >>= 10;
        entity->amplitude.y >>= 10;

        int len = 0;

        switch (entity->moveType) {
            case PIMPOM_MOVE_NONE:
            default:
                entity->stateMove     = PimPom_StateMove_None;
                entity->updateRange.x = (entity->length * (entity->gap + 24) + 88) << 16;
                entity->updateRange.y = (entity->length * (entity->gap + 24) + 88) << 16;
                break;
            case PIMPOM_MOVE_NORMAL:
                len                   = (entity->length * (entity->gap + 24) + 88) << 6;
                entity->updateRange.x = (abs(entity->amplitude.x) + len) << 10;
                entity->updateRange.y = (abs(entity->amplitude.y) + len) << 10;
                entity->stateMove     = PimPom_StateMove_Normal;
                break;
            case PIMPOM_MOVE_CIRCLE:
                len                   = (entity->length * (entity->gap + 24)) << 6;
                entity->updateRange.x = (abs(entity->amplitude.x) + len + 0x1600) << 10;
                entity->updateRange.y = (abs(entity->amplitude.y) + len + 0x1600) << 10;
                entity->stateMove     = PimPom_StateMove_Circle;
                break;
            case PIMPOM_MOVE_3:
                len                   = (entity->length * (entity->gap + 24)) << 7;
                entity->updateRange.x = (abs(entity->amplitude.x) + len + 0x2C00) << 9;
                entity->updateRange.y = (abs(entity->amplitude.y) + len + 0x2C00) << 9;
                entity->stateMove     = PimPom_StateMove3_Unknown;
                break;
            case PIMPOM_MOVE_4:
                entity->active        = ACTIVE_NEVER;
                entity->stateMove     = PimPom_StateMove_ToTarget;
                entity->updateRange.x = (entity->length * (entity->gap + 24) + 88) << 16;
                entity->updateRange.y = (entity->length * (entity->gap + 24) + 88) << 16;
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

    int storeX       = entity->position.x;
    int storeY       = entity->position.y;
    entity->position = entity->drawPos;
    entity->position.x += entity->offset.x;
    entity->position.y -= entity->offset.y;

    for (int l = 0; l <= entity->length; ++l) {

        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, entity, &entity->hitbox)) {
                int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);

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
                if (entity->timer < 1) {
                    RSDK.PlaySfx(PimPom->sfxPimPom, false, 255);
                    entity->timer = 8;
                }
            }
        }

        entity->position.x -= entity->moveAmount.x;
        entity->position.y += entity->moveAmount.y;
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void PimPom_State1_Unknown(void)
{
    RSDK_THIS(PimPom);

    int startX       = entity->position.x;
    int startY       = entity->position.y;
    entity->position = entity->drawPos;

    int len = 0;
    if (entity->gap >= 16) {
        entity->position.x += entity->offset.x;
        entity->position.y -= entity->offset.y;
        len = entity->length;
    }

    for (int l = 0; l <= len; ++l) {

        foreach_active(Player, player)
        {
            int playerX    = player->position.x;
            int playerY    = player->position.y;
            int playerVelX = player->velocity.x;
            int playerVelY = player->velocity.y;

            int distX = (player->position.x - entity->position.x) >> 8;
            int distY = (player->position.y - entity->position.y) >> 8;

            player->position.x = entity->position.x + distY * RSDK.Sin256(entity->negAngle) + distX * RSDK.Cos256(entity->negAngle);
            player->position.y = entity->position.y - distX * RSDK.Sin256(entity->negAngle) + distY * RSDK.Cos256(entity->negAngle);

            int velX = player->velocity.x >> 8;
            int velY = player->velocity.y >> 8;

            player->velocity.x = velY * RSDK.Sin256(entity->negAngle) + velX * RSDK.Cos256(entity->negAngle);
            player->velocity.y = velY * RSDK.Cos256(entity->negAngle) - velX * RSDK.Sin256(entity->negAngle);

            int storedVelX = player->velocity.x;
            int storedVelY = player->velocity.y;

            Hitbox hitbox;
            Hitbox *playerHitbox = Player_GetHitbox(player);
            if ((((entity->angle & 0xFF) - 32) & 0x7F) < 0x40) {
                hitbox.top    = playerHitbox->left;
                hitbox.bottom = playerHitbox->right;
                hitbox.left   = playerHitbox->top;
                hitbox.right  = playerHitbox->bottom;
            }
            else {
                hitbox = *playerHitbox;
            }

            int side = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, player, &hitbox, true);
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

                distX = (player->position.x - entity->position.x) >> 8;
                distY = (player->position.y - entity->position.y) >> 8;

                player->position.x = distY * RSDK.Sin256(entity->angle) + distX * RSDK.Cos256(entity->angle) + entity->position.x;
                player->position.y = entity->position.y - distX * RSDK.Sin256(entity->angle) + distY * RSDK.Cos256(entity->angle);

                velX = player->velocity.x >> 8;
                velY = player->velocity.y >> 8;

                player->velocity.x = velY * RSDK.Sin256(entity->angle) + velX * RSDK.Cos256(entity->angle);
                player->velocity.y = velY * RSDK.Cos256(entity->angle) - velX * RSDK.Sin256(entity->angle);

                if (entity->timer < 1) {
                    RSDK.PlaySfx(PimPom->sfxBumper2, false, 255);
                    entity->timer = 8;
                }
            }
        }

        entity->position.x -= entity->moveAmount.x;
        entity->position.y += entity->moveAmount.y;
    }

    entity->position.x = startX;
    entity->position.y = startY;
}

void PimPom_State3_Unknown(void)
{
    RSDK_THIS(PimPom);

    int spinAngle = Zone->timer * entity->spinSpeed;
    if (entity->direction)
        spinAngle = -spinAngle;

    entity->angle    = (entity->angle2 & 0xFF) + (spinAngle >> 1);
    entity->negAngle = 0x100 - entity->angle;
    entity->rotation = 2 * entity->negAngle;

    entity->offset.x = ((entity->length * (entity->gap + 24)) << 8) * RSDK.Cos256(entity->angle);
    entity->offset.y = ((entity->length * (entity->gap + 24)) << 8) * RSDK.Sin256(entity->angle);

    entity->moveAmount.x = ((entity->gap + 24) * RSDK.Cos256(entity->angle)) << 9;
    entity->moveAmount.y = ((entity->gap + 24) * RSDK.Sin256(entity->angle)) << 9;

    PimPom_State1_Unknown();
}

void PimPom_StateMove_None(void)
{
    RSDK_THIS(PimPom);

    entity->drawPos.x = entity->position.x;
    entity->drawPos.y = entity->position.y;
}

void PimPom_StateMove_Normal(void)
{
    RSDK_THIS(PimPom);

    int drawX         = entity->drawPos.x;
    int drawY         = entity->drawPos.y;
    entity->drawPos.x = entity->position.x + entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer);
    entity->drawPos.y = entity->position.y + entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer);
    int moveX         = entity->drawPos.x - drawX;
    int moveY         = entity->drawPos.y - drawY;

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < entity->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_StateMove_Circle(void)
{
    RSDK_THIS(PimPom);

    int drawX         = entity->drawPos.x;
    int drawY         = entity->drawPos.y;
    entity->drawPos.x = entity->position.x + entity->amplitude.x * RSDK.Cos1024(entity->speed * Zone->timer + 4 * entity->angleM);
    entity->drawPos.y = entity->position.y + entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer + 4 * entity->angleM);
    int moveX         = entity->drawPos.x - drawX;
    int moveY         = entity->drawPos.y - drawY;

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < entity->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_StateMove_ToTarget(void)
{
    RSDK_THIS(PimPom);

    int drawX = entity->drawPos.x;
    int drawY = entity->drawPos.y;
    entity->drawPos.x += entity->velocity.x;
    entity->drawPos.y += entity->velocity.y;

    Entity *target = RSDK_GET_ENTITY(entity->speed, );

    if (entity->velocity.x <= 0) {
        if (entity->drawPos.x < target->position.x)
            entity->drawPos.x = target->position.x;
    }
    else if (entity->drawPos.x > target->position.x)
        entity->drawPos.x = target->position.x;

    if (entity->velocity.y <= 0) {
        if (entity->drawPos.y < target->position.y)
            entity->drawPos.y = target->position.y;
    }
    else if (entity->drawPos.y > target->position.y)
        entity->drawPos.y = target->position.y;

    int moveX = entity->drawPos.x - drawX;
    int moveY = entity->drawPos.y - drawY;

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < entity->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

void PimPom_StateMove3_Unknown(void)
{
    RSDK_THIS(PimPom);

    int drawX = entity->drawPos.x;
    int drawY = entity->drawPos.y;

    if (((Zone->timer >> 9) & 1) == entity->direction) {
        entity->drawPos.x = entity->position.x + ((((Zone->timer & 0xFFFF) << 7) * entity->amplitude.x) >> 6) - (entity->amplitude.x << 15);
        entity->drawPos.y = entity->position.y + ((((Zone->timer & 0xFFFF) << 7) * entity->amplitude.y) >> 6) - (entity->amplitude.y << 15);
    }
    else {
        entity->drawPos.x = entity->position.x + (entity->amplitude.x << 15) - ((((Zone->timer & 0xFFFF) << 7) * entity->amplitude.x) >> 6);
        entity->drawPos.y = entity->position.y + (entity->amplitude.y << 15) - ((((Zone->timer & 0xFFFF) << 7) * entity->amplitude.y) >> 6);
    }

    int moveX = entity->drawPos.x - drawX;
    int moveY = entity->drawPos.y - drawY;

    int slot = SceneInfo->entitySlot + 1;
    for (int i = 0; i < entity->numChildren; ++i) {
        Entity *child = RSDK_GET_ENTITY(slot + i, );
        child->position.x += moveX;
        child->position.y += moveY;
    }
}

#if RETRO_INCLUDE_EDITOR
void PimPom_EditorDraw(void)
{
    RSDK_THIS(PimPom);

    entity->drawFX    = FX_NONE;
    entity->direction = FLIP_NONE;
    entity->active    = ACTIVE_BOUNDS;
    entity->visible   = true;

    entity->updateRange.x = (40 - (entity->length * (entity->gap + 24))) << 16;
    entity->updateRange.y = (40 - (entity->length * (entity->gap + 24))) << 16;
    entity->drawOrder     = Zone->drawOrderLow + 1;
    entity->angle2        = 0x20 * entity->angle;
    entity->angle         = 0x20 * entity->angle;
    entity->negAngle      = 0x100 - entity->angle;
    entity->rotation      = 2 * entity->negAngle;

    int offset = 8;
    if (entity->type)
        offset = 24;

    entity->offset.x = ((entity->length * (offset + entity->gap)) << 8) * RSDK.Cos256(entity->angle);
    entity->offset.y = ((entity->length * (offset + entity->gap)) << 8) * RSDK.Sin256(entity->angle);

    entity->moveAmount.x = ((offset + entity->gap) * RSDK.Cos256(entity->angle)) << 9;
    entity->moveAmount.y = ((offset + entity->gap) * RSDK.Sin256(entity->angle)) << 9;
    entity->drawPos      = entity->position;

    switch (entity->type) {
        case PIMPOM_SINGLE:
            RSDK.SetSpriteAnimation(PimPom->aniFrames, 0, &entity->animator, true, entity->color);
            entity->hitbox.left   = -8;
            entity->hitbox.top    = -8;
            entity->hitbox.right  = 8;
            entity->hitbox.bottom = 8;
            entity->state         = PimPom_State0_Unknown;
            break;
        case PIMPOM_HORIZONTAL:
            switch (entity->angle) {
                case 0x00: RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &entity->animator, true, 0); break;
                case 0x20:
                    entity->drawFX    = FX_FLIP;
                    entity->direction = FLIP_X;
                    RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &entity->animator, true, 0);
                    break;
                case 0x40: RSDK.SetSpriteAnimation(PimPom->aniFrames, 3, &entity->animator, true, 0); break;
                case 0x60: RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &entity->animator, true, 0); break;
                default: break;
            }
            break;
        case PIMPOM_DIAGONAL:
            RSDK.SetSpriteAnimation(PimPom->aniFrames, 2, &entity->animator, true, 0);
            entity->drawFX = FX_ROTATE | FX_FLIP;
            if (entity->direction == FLIP_X)
                entity->angle = 0x20;
            else
                entity->angle = 0xE0;
            entity->negAngle = 0x100 - entity->angle;
            break;
        case PIMPOM_VERTICAL:
            RSDK.SetSpriteAnimation(PimPom->aniFrames, 1, &entity->animator, true, 0);
            entity->drawFX = FX_ROTATE;
            break;
        default: break;
    }

    int ax = entity->amplitude.x >> 10;
    int ay = entity->amplitude.y >> 10;

    int len = 0;

    switch (entity->moveType) {
        case PIMPOM_MOVE_NONE:
        default:
            entity->updateRange.x = (entity->length * (entity->gap + 24) + 88) << 16;
            entity->updateRange.y = (entity->length * (entity->gap + 24) + 88) << 16;
            break;
        case PIMPOM_MOVE_NORMAL:
            len                   = (entity->length * (entity->gap + 24) + 88) << 6;
            entity->updateRange.x = (abs(ax) + len) << 10;
            entity->updateRange.y = (abs(ay) + len) << 10;
            break;
        case PIMPOM_MOVE_CIRCLE:
            len                   = (entity->length * (entity->gap + 24)) << 6;
            entity->updateRange.x = (abs(ax) + len + 0x1600) << 10;
            entity->updateRange.y = (abs(ay) + len + 0x1600) << 10;
            break;
        case PIMPOM_MOVE_3:
            len                   = (entity->length * (entity->gap + 24)) << 7;
            entity->updateRange.x = (abs(ax) + len + 0x2C00) << 9;
            entity->updateRange.y = (abs(ay) + len + 0x2C00) << 9;
            break;
        case PIMPOM_MOVE_4:
            entity->updateRange.x = (entity->length * (entity->gap + 24) + 88) << 16;
            entity->updateRange.y = (entity->length * (entity->gap + 24) + 88) << 16;
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
