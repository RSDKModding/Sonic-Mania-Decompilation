#include "SonicMania.h"

ObjectPress *Press;

void Press_Update(void)
{
    RSDK_THIS(Press);
    StateMachine_Run(self->state);
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
    if (self->rotation <= 0x100)
        Press_DrawHandle();
    // threads
    Vector2 drawPos  = self->drawPos;
    SpriteFrame *spr = RSDK.GetFrame(Press->animID, 1u, self->threadAnimator.frameID);
    spr->height      = 56;
    spr->sprY        = (self->threadSprY >> 8) + 182;
    for (int32 i = 0; i < self->threads; ++i) {
        RSDK.DrawSprite(&self->threadAnimator, &drawPos, false);
        drawPos.y += 0x380000;
    }
    spr->height = self->height;
    RSDK.DrawSprite(&self->threadAnimator, &drawPos, false);

    // crusher platforms
    drawPos = self->position;
    drawPos.y += self->offTop;
    self->crusherAnimator.frameID = 3;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);
    self->crusherAnimator.frameID = 4;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);
    drawPos.y -= self->offTop;
    drawPos.y += self->offBottom;
    self->crusherAnimator.frameID = 3;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);
    self->crusherAnimator.frameID = 5;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);

    // bumper
    drawPos.y = self->drawPos.y - 0x80000;
    RSDK.DrawSprite(&self->bumperAnimator, &drawPos, false);
    drawPos.y += (self->size + 16) << 16;
    RSDK.DrawSprite(&self->bumperAnimator, &drawPos, false);
    if (self->rotation > 0x100)
        Press_DrawHandle();
}

void Press_Create(void *data)
{
    RSDK_THIS(Press);
    self->active    = ACTIVE_BOUNDS;
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    if (SceneInfo->inEditor != true) {
        self->size *= 8;
        int32 size = self->size;
        self->speed <<= 15;
        self->updateRange.x = 0x1000000;
        // i don't think i can make this any more readable :(
        self->threads = ((size / 7) + ((size - (size / 7)) / 2)) / 32;
        size <<= 15;
        self->scale.y       = 0x200;
        self->height        = size - 0x38 * self->threads;
        self->updateRange.y = 0x380000 * self->threads + 0x1000000;
        self->drawPos.x     = self->position.x;
        self->drawPos.y     = self->position.y;
        self->drawPos.y -= size;
        self->offTop    = (self->offTop << 16) - size;
        self->offBottom = (self->offBottom << 16) - size + 0xFFFF;
        RSDK.SetSpriteAnimation(Press->animID, 0, &self->crusherAnimator, true, 0);
        RSDK.SetSpriteAnimation(Press->animID, 1, &self->threadAnimator, true, 0);
        RSDK.SetSpriteAnimation(Press->animID, 2, &self->bumperAnimator, true, 0);
        self->state = Press_Crush;
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
    Player->canSuperCB = Press_SuperCheckCB;
#endif
    Press->impactSFX     = RSDK.GetSfx("Stage/Impact2.wav");
    Press->pressSFX      = RSDK.GetSfx("PSZ/Press.wav");
}

#if RETRO_USE_PLUS
bool32 Press_SuperCheckCB(bool32 isHUD) { return Press->canSuper; }
#endif

void Press_DrawHandle(void)
{
    RSDK_THIS(Press);
    Vector2 drawPos = self->drawPos;
    int32 rot         = self->rotation;
    int32 rotCos      = RSDK.Cos512(rot);
    if (rotCos >= 0)
        rotCos = RSDK.Cos512(rot);
    else
        rotCos = -RSDK.Cos512(rot);
    self->scale.x = rotCos + 1;
    int32 rotSin      = RSDK.Sin512(rot);
    if (rotSin >= 0)
        rotSin = RSDK.Sin512(rot);
    else
        rotSin = -RSDK.Sin512(rot);
    ++rotSin;
    drawPos.y -= 0x80000;
    drawPos.x += 0x2500 * RSDK.Cos512(self->rotation);
    self->crusherAnimator.frameID = 0;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);
    rot = self->rotation;
    self->drawFX |= FX_SCALE;
    drawPos.x = 0x1B80 * RSDK.Cos512(rot) + self->drawPos.x;
    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x += (rotSin << 9);
            self->crusherAnimator.frameID = 2;
            RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);
            drawPos.x += -0xC00 * self->scale.x - (rotSin << 9);
            break;
        case 1:
        case 3:
            drawPos.x -= (rotSin << 9);
            self->crusherAnimator.frameID = 2;
            RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);
            drawPos.x += (rotSin + 2 * (3 * self->scale.x - 32)) << 9;
            break;
        default: break;
    }
    self->scale.x                 = rotSin;
    self->crusherAnimator.frameID = 1;
    RSDK.DrawSprite(&self->crusherAnimator, &drawPos, false);
    self->drawFX &= ~FX_SCALE;
}

void Press_Move(void)
{
    RSDK_THIS(Press);
    self->onRoof  = 0;
    self->onFloor = 0;
    int32 playerBit   = 0;
    foreach_active(Player, player)
    {
        self->position.y += self->offBottom;
        if (Player_CheckCollisionBox(player, self, &Press->hitbox) == 1) // collision top
        {
            if (self->state == Press_Crush && !player->sidekick) {
                if (abs(self->position.x - player->position.x) <= 0x600000) {
                    RSDK.PlaySfx(Press->pressSFX, 0, 255);
                    self->state  = Press_HandleMovement;
                    self->active = ACTIVE_NORMAL;
                }
            }
            self->onFloor |= 1 << playerBit;
        }
        self->position.y += self->offTop - self->offBottom;
        int32 collide = Player_CheckCollisionBox(player, self, &Press->hitbox);
        if (collide == 4) { // collision bottom (crush detection)
            player->collisionFlagV |= 2;
        }
        else if (collide == 1) // if top
            self->onRoof |= 1 << playerBit;
        ++playerBit;
        self->position.y -= self->offTop;
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
    RSDK.ProcessAnimation(&self->threadAnimator);
    RSDK.ProcessAnimation(&self->bumperAnimator);
    Press_Move();
    uint32 speed        = self->speed;
    int32 oldBottom    = self->offBottom;
    int32 newBottom    = oldBottom - speed;
    self->offBottom = oldBottom - speed;
    uint32 rot          = self->rotation;
    self->threadSprY += (speed >> 11);
    self->threadSprY &= 0x7FF;
    int32 oldTop       = self->offTop;
    int32 newTop       = speed + self->offTop + self->topOffset;
    self->rotation  = (rot - (speed >> 15)) & 0x1FF;
    self->offTop    = newTop;
    self->topOffset = 0;
    if (newTop + 0x100000 >= newBottom - 0x100000) {
        int32 diff = newTop - newBottom + 0x200000;
        if (diff > 0) {
            diff >>= 1;
            self->offTop    = newTop - diff;
            self->offBottom = newBottom + diff;
        }
        RSDK.PlaySfx(Press->impactSFX, 0, 255);
        self->active = ACTIVE_BOUNDS;
        Camera_ShakeScreen(0, 0, 5);
        self->state = Press_FinalCrush;
    }
    uint32 waitTime = 0;
    bool32 top = false, bottom = false;
    int32 floorOffset   = (oldBottom & 0xFFFF0000) - (self->offBottom & 0xFFFF0000);
    int32 actualPos    = self->position.y;
    self->topOffset = self->offTop;
    foreach_active(Crate, crate)
    {
        self->position.y += self->offBottom;
        if (RSDK.CheckObjectCollisionBox(self, &Press->hitbox, crate, &crate->hitbox, false) == 1) {
            bottom = true;
            Crate_MoveY(crate, -floorOffset);
        }
        self->position.y += self->offTop - self->offBottom;
        if (RSDK.CheckObjectCollisionBox(crate, &crate->hitbox, self, &Press->hitbox, false) == 1) {
            top       = true;
            int32 frame = crate->frameID;
            if (!frame) // white (segregation)
                waitTime += 60;
            if (frame == 1) // that's why yo crate raggedy
                waitTime += 30;
            if (frame == 2) // colored (racism)
                waitTime += 90;
            if (frame == 3) // dark blue only (she got the momma jeans
                waitTime += 60;
        }
        self->offTop = self->position.y - actualPos;
        self->position.y = actualPos;
    }
    self->topOffset -= self->offTop;
    if (bottom && top) {
        RSDK.PlaySfx(Press->impactSFX, 0, 255);
        Camera_ShakeScreen(0, 0, 3);
        self->state      = Press_HandleCrates;
        self->timerStart = waitTime;
        self->timer      = waitTime;
    }
    int32 playerBit  = 1;
    int32 roofOffset = (oldTop & 0xFFFF0000) - (self->offTop & 0xFFFF0000);
    foreach_active(Player, player)
    {
        if (playerBit & self->onFloor)
            player->position.y -= floorOffset;
        if (playerBit & self->onRoof)
            player->position.y -= roofOffset;
        playerBit <<= 1;
    }
}

void Press_HandleCrates(void)
{
    RSDK_THIS(Press);
    Press_Move();
    if (self->timer <= 0) {
        foreach_active(Crate, crate)
        {
            self->position.y += self->offTop + 0x80000;
            if (RSDK.CheckObjectCollisionTouchBox(self, &Press->hitbox, crate, &crate->hitbox))
                Crate_Break(crate);
            self->position.y += -0x80000 - self->offTop;
        }
        self->state = Press_HandleMovement;
    }
    else {
        int32 percentDone = ((self->timerStart - self->timer) << 16) / self->timerStart;
        int32 crateOff = 0;
        foreach_active(Crate, crate)
        {
            self->position.y += self->offTop + 0x80000;
            if (RSDK.CheckObjectCollisionTouchBox(self, &Press->hitbox, crate, &crate->hitbox)
                && percentDone > 0x8000) {
                int32 percent = percentDone >> 9;
                int32 percent2 = percent * percent;
                // i plugged it in desmos to see if i could shorten it
                // i couldn't
                // https://varyex.dev/rmgfile/171715b2.png
                // -rmg
                int32 angle = crateOff + ((percent * self->timerStart * (percent2 >> 8)) >> 8);
                crateOff += 0x100;
                crate->drawPos.x = (RSDK.Sin512(angle & 0x1FF) << 7) + crate->centerPos.x;
            }
            self->position.y += -0x80000 - self->offTop;
        }
        --self->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void Press_EditorDraw(void) {}

void Press_EditorLoad(void) {}
#endif

void Press_Serialize(void)
{
    RSDK_EDITABLE_VAR(Press, VAR_UINT16, size);
    RSDK_EDITABLE_VAR(Press, VAR_UINT32, speed);
    RSDK_EDITABLE_VAR(Press, VAR_ENUM, offTop);
    RSDK_EDITABLE_VAR(Press, VAR_ENUM, offBottom);
}
