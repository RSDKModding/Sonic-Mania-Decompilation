#include "SonicMania.h"

ObjectPress *Press;

void Press_Update(void)
{
    RSDK_THIS(Press);
    StateMachine_Run(entity->state);
}

void Press_LateUpdate(void) {}

void Press_StaticUpdate(void)
{
#if RETRO_USE_PLUS
    Press->canSuper = true;
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    
    if (player->objectID == Player->objectID) {
        foreach_active(Press, press)
        {
            Hitbox checkbox;
            checkbox.top    = -4 * press->size;
            checkbox.bottom = 4 * press->size;
            checkbox.left   = -112;
            checkbox.right  = 112;
            if (Player_CheckCollisionTouch(player, press, &checkbox)) {
                Press->canSuper = false;
                foreach_break;
            }
        }
    }
    else
        Press->canSuper = false;
#endif
}

void Press_Draw(void)
{
    RSDK_THIS(Press);
    if (entity->rotation <= 0x100)
        Press_DrawHandle();
    // threads
    Vector2 drawPos  = entity->drawPos;
    SpriteFrame *spr = RSDK.GetFrame(Press->animID, 1u, entity->threadAnimator.frameID);
    spr->height      = 56;
    spr->sprY        = (entity->threadSprY >> 8) + 182;
    for (int i = 0; i < entity->threads; ++i) {
        RSDK.DrawSprite(&entity->threadAnimator, &drawPos, false);
        drawPos.y += 0x380000;
    }
    spr->height = entity->height;
    RSDK.DrawSprite(&entity->threadAnimator, &drawPos, false);

    // crusher platforms
    drawPos = entity->position;
    drawPos.y += entity->offTop;
    entity->crusherAnimator.frameID = 3;
    RSDK.DrawSprite(&entity->crusherAnimator, &drawPos, false);
    entity->crusherAnimator.frameID = 4;
    RSDK.DrawSprite(&entity->crusherAnimator, &drawPos, false);
    drawPos.y -= entity->offTop;
    drawPos.y += entity->offBottom;
    entity->crusherAnimator.frameID = 3;
    RSDK.DrawSprite(&entity->crusherAnimator, &drawPos, false);
    entity->crusherAnimator.frameID = 5;
    RSDK.DrawSprite(&entity->crusherAnimator, &drawPos, false);

    // bumper
    drawPos.y = entity->drawPos.y - 0x80000;
    RSDK.DrawSprite(&entity->bumperAnimator, &drawPos, false);
    drawPos.y += (entity->size + 16) << 16;
    RSDK.DrawSprite(&entity->bumperAnimator, &drawPos, false);
    if (entity->rotation > 0x100)
        Press_DrawHandle();
}

void Press_Create(void *data)
{
    RSDK_THIS(Press);
    entity->active    = ACTIVE_BOUNDS;
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    if (RSDK_sceneInfo->inEditor != true) {
        entity->size *= 8;
        int size = entity->size;
        entity->speed <<= 15;
        entity->updateRange.x = 0x1000000;
        // i don't think i can make this any more readable :(
        entity->threads = ((size / 7) + ((size - (size / 7)) / 2)) / 32;
        size <<= 15;
        entity->scale.y       = 0x200;
        entity->height        = size - 0x38 * entity->threads;
        entity->updateRange.y = 0x380000 * entity->threads + 0x1000000;
        entity->drawPos.x     = entity->position.x;
        entity->drawPos.y     = entity->position.y;
        entity->drawPos.y -= size;
        entity->offTop    = (entity->offTop << 16) - size;
        entity->offBottom = (entity->offBottom << 16) - size + 0xFFFF;
        RSDK.SetSpriteAnimation(Press->animID, 0, &entity->crusherAnimator, true, 0);
        RSDK.SetSpriteAnimation(Press->animID, 1, &entity->threadAnimator, true, 0);
        RSDK.SetSpriteAnimation(Press->animID, 2, &entity->bumperAnimator, true, 0);
        entity->state = Press_Crush;
    }
}

void Press_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Press->animID = RSDK.LoadSpriteAnimation("PSZ1/Press.bin", SCOPE_STAGE);
    Press->hitbox.left   = -112;
    Press->hitbox.top    = -16;
    Press->hitbox.right  = 112;
    Press->hitbox.bottom = 16;
#if RETRO_USE_PLUS
    Player->canSuper     = Press_SuperCheck;
#endif
    Press->impactSFX     = RSDK.GetSFX("Stage/Impact2.wav");
    Press->pressSFX      = RSDK.GetSFX("PSZ/Press.wav");
}

#if RETRO_USE_PLUS
bool32 Press_SuperCheck(bool32 hud) { return Press->canSuper; }
#endif

void Press_DrawHandle(void)
{
    RSDK_THIS(Press);
    Vector2 drawPos = entity->drawPos;
    int rot         = entity->rotation;
    int rotCos      = RSDK.Cos512(rot);
    if (rotCos >= 0)
        rotCos = RSDK.Cos512(rot);
    else
        rotCos = -RSDK.Cos512(rot);
    entity->scale.x = rotCos + 1;
    int rotSin      = RSDK.Sin512(rot);
    if (rotSin >= 0)
        rotSin = RSDK.Sin512(rot);
    else
        rotSin = -RSDK.Sin512(rot);
    ++rotSin;
    drawPos.y -= 0x80000;
    drawPos.x += 0x2500 * RSDK.Cos512(entity->rotation);
    entity->crusherAnimator.frameID = 0;
    RSDK.DrawSprite(&entity->crusherAnimator, &drawPos, false);
    rot = entity->rotation;
    entity->drawFX |= FX_SCALE;
    drawPos.x = 0x1B80 * RSDK.Cos512(rot) + entity->drawPos.x;
    switch (entity->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x += (rotSin << 9);
            entity->crusherAnimator.frameID = 2;
            RSDK.DrawSprite(&entity->crusherAnimator, &drawPos, false);
            drawPos.x += -0xC00 * entity->scale.x - (rotSin << 9);
            break;
        case 1:
        case 3:
            drawPos.x -= (rotSin << 9);
            entity->crusherAnimator.frameID = 2;
            RSDK.DrawSprite(&entity->crusherAnimator, &drawPos, false);
            drawPos.x += (rotSin + 2 * (3 * entity->scale.x - 32)) << 9;
            break;
        default: break;
    }
    entity->scale.x                 = rotSin;
    entity->crusherAnimator.frameID = 1;
    RSDK.DrawSprite(&entity->crusherAnimator, &drawPos, false);
    entity->drawFX &= ~FX_SCALE;
}

void Press_Move(void)
{
    RSDK_THIS(Press);
    entity->onRoof  = 0;
    entity->onFloor = 0;
    int playerBit   = 0;
    foreach_active(Player, player)
    {
        entity->position.y += entity->offBottom;
        if (Player_CheckCollisionBox(player, entity, &Press->hitbox) == 1) // collision top
        {
            if (entity->state == Press_Crush && !player->sidekick) {
                if (abs(entity->position.x - player->position.x) <= 0x600000) {
                    RSDK.PlaySfx(Press->pressSFX, 0, 255);
                    entity->state  = Press_HandleMovement;
                    entity->active = ACTIVE_NORMAL;
                }
            }
            entity->onFloor |= 1 << playerBit;
        }
        entity->position.y += entity->offTop - entity->offBottom;
        int collide = Player_CheckCollisionBox(player, entity, &Press->hitbox);
        if (collide == 4) { // collision bottom (crush detection)
            player->collisionFlagV |= 2;
        }
        else if (collide == 1) // if top
            entity->onRoof |= 1 << playerBit;
        ++playerBit;
        entity->position.y -= entity->offTop;
    }
}

void Press_Crush(void)
{
    Press_Move();
    // fun press fact!
}
void Press_FinalCrush(void)
{
    Press_Move();
    // every second you don't move the press only gets closer
}
void Press_HandleMovement(void)
{
    RSDK_THIS(Press);
    RSDK.ProcessAnimation(&entity->threadAnimator);
    RSDK.ProcessAnimation(&entity->bumperAnimator);
    Press_Move();
    uint32 speed        = entity->speed;
    int oldBottom    = entity->offBottom;
    int newBottom    = oldBottom - speed;
    entity->offBottom = oldBottom - speed;
    uint32 rot          = entity->rotation;
    entity->threadSprY += (speed >> 11);
    entity->threadSprY &= 0x7FF;
    int oldTop       = entity->offTop;
    int newTop       = speed + entity->offTop + entity->topOffset;
    entity->rotation  = (rot - (speed >> 15)) & 0x1FF;
    entity->offTop    = newTop;
    entity->topOffset = 0;
    if (newTop + 0x100000 >= newBottom - 0x100000) {
        int diff = newTop - newBottom + 0x200000;
        if (diff > 0) {
            diff >>= 1;
            entity->offTop    = newTop - diff;
            entity->offBottom = newBottom + diff;
        }
        RSDK.PlaySfx(Press->impactSFX, 0, 255);
        entity->active = ACTIVE_BOUNDS;
        Camera_ShakeScreen(0, 0, 5);
        entity->state = Press_FinalCrush;
    }
    uint32 waitTime = 0;
    bool32 top = false, bottom = false;
    int floorOffset   = (oldBottom & 0xFFFF0000) - (entity->offBottom & 0xFFFF0000);
    int actualPos    = entity->position.y;
    entity->topOffset = entity->offTop;
    foreach_active(Crate, crate)
    {
        entity->position.y += entity->offBottom;
        if (RSDK.CheckObjectCollisionBox(entity, &Press->hitbox, crate, &crate->hitbox, false) == 1) {
            bottom = true;
            Crate_MoveY(crate, -floorOffset);
        }
        entity->position.y += entity->offTop - entity->offBottom;
        if (RSDK.CheckObjectCollisionBox(crate, &crate->hitbox, entity, &Press->hitbox, false) == 1) {
            top       = true;
            int frame = crate->frameID;
            if (!frame) // white (segregation)
                waitTime += 60;
            if (frame == 1) // that's why yo crate raggedy
                waitTime += 30;
            if (frame == 2) // colored (racism)
                waitTime += 90;
            if (frame == 3) // dark blue only (she got the momma jeans
                waitTime += 60;
        }
        entity->offTop = entity->position.y - actualPos;
        entity->position.y = actualPos;
    }
    entity->topOffset -= entity->offTop;
    if (bottom && top) {
        RSDK.PlaySfx(Press->impactSFX, 0, 255);
        Camera_ShakeScreen(0, 0, 3);
        entity->state      = Press_HandleCrates;
        entity->timerStart = waitTime;
        entity->timer      = waitTime;
    }
    int playerBit  = 1;
    int roofOffset = (oldTop & 0xFFFF0000) - (entity->offTop & 0xFFFF0000);
    foreach_active(Player, player)
    {
        if (playerBit & entity->onFloor)
            player->position.y -= floorOffset;
        if (playerBit & entity->onRoof)
            player->position.y -= roofOffset;
        playerBit <<= 1;
    }
}

void Press_HandleCrates(void)
{
    RSDK_THIS(Press);
    Press_Move();
    if (entity->timer <= 0) {
        foreach_active(Crate, crate)
        {
            entity->position.y += entity->offTop + 0x80000;
            if (RSDK.CheckObjectCollisionTouchBox(entity, &Press->hitbox, crate, &crate->hitbox))
                Crate_Break(crate);
            entity->position.y += -0x80000 - entity->offTop;
        }
        entity->state = Press_HandleMovement;
    }
    else {
        int percentDone = ((entity->timerStart - entity->timer) << 16) / entity->timerStart;
        int crateOff = 0;
        foreach_active(Crate, crate)
        {
            entity->position.y += entity->offTop + 0x80000;
            if (RSDK.CheckObjectCollisionTouchBox(entity, &Press->hitbox, crate, &crate->hitbox)
                && percentDone > 0x8000) {
                int percent = percentDone >> 9;
                int percent2 = percent * percent;
                // i plugged it in desmos to see if i could shorten it
                // i couldn't
                // https://varyex.dev/rmgfile/171715b2.png
                // -rmg
                int angle = crateOff + ((percent * entity->timerStart * (percent2 >> 8)) >> 8);
                crateOff += 0x100;
                crate->drawPos.x = (RSDK.Sin512(angle & 0x1FF) << 7) + crate->centerPos.x;
            }
            entity->position.y += -0x80000 - entity->offTop;
        }
        --entity->timer;
    }
}

void Press_EditorDraw(void) {}

void Press_EditorLoad(void) {}

void Press_Serialize(void)
{
    RSDK_EDITABLE_VAR(Press, VAR_UINT16, size);
    RSDK_EDITABLE_VAR(Press, VAR_UINT32, speed);
    RSDK_EDITABLE_VAR(Press, VAR_ENUM, offTop);
    RSDK_EDITABLE_VAR(Press, VAR_ENUM, offBottom);
}
