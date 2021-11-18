#include "SonicMania.h"

ObjectCanista *Canista;

void Canista_Update(void)
{
    RSDK_THIS(Canista);
    StateMachine_Run(self->state);
}

void Canista_LateUpdate(void) {}

void Canista_StaticUpdate(void) {}

void Canista_Draw(void)
{
    RSDK_THIS(Canista);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void Canista_Create(void *data)
{
    RSDK_THIS(Canista);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow + 1;
    if (data) {
        self->drawFX |= FX_FLIP;
        self->inkEffect     = INK_ALPHA;
        self->alpha         = 0x100;
        self->timer3        = 90;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->active        = ACTIVE_NORMAL;
        self->drawOrder     = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(Canista->aniFrames, 3, &self->animator1, true, 0);
        self->state = Canista_State2_Unknown1;
    }
    else {
        self->drawFX |= FX_ROTATE | FX_FLIP;
        self->shootOffset <<= 16;
        self->shootSize <<= 16;
        self->startPos.x     = self->position.x;
        self->startPos.y     = self->position.y;
        self->startDir       = self->direction;
        self->hitbox.left    = -self->detectSize.x >> 17;
        self->hitbox.top     = -self->detectSize.x >> 17;
        self->active         = ACTIVE_BOUNDS;
        self->updateRange.x  = 0x800000;
        self->updateRange.y  = 0x800000;
        self->detectedPlayer = 0;
        self->hitbox.right   = self->detectSize.x >> 17;
        self->hitbox.bottom  = self->detectSize.x >> 17;
        RSDK.SetSpriteAnimation(Canista->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Canista->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(Canista->aniFrames, 2, &self->animator3, true, 0);
        self->state = Canista_State_Setup;
    }
}

void Canista_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Canista.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ2"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Canista.bin", SCOPE_STAGE);

    Canista->hitbox1.left   = -14;
    Canista->hitbox1.top    = -14;
    Canista->hitbox1.right  = 14;
    Canista->hitbox1.bottom = 14;
    Canista->hitbox2.left   = -128;
    Canista->hitbox2.top    = -128;
    Canista->hitbox2.right  = 128;
    Canista->hitbox2.bottom = 128;
    Canista->hitbox3.left   = -132;
    Canista->hitbox3.top    = 48;
    Canista->hitbox3.right  = 132;
    Canista->hitbox3.bottom = 49;
    Canista->hitbox4.left   = -26;
    Canista->hitbox4.top    = -5;
    Canista->hitbox4.right  = -16;
    Canista->hitbox4.bottom = 5;

    DEBUGMODE_ADD_OBJ(Canista);
    Canista->sfxPon = RSDK.GetSFX("Stage/Pon.wav");
}

void Canista_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityCanista *canista = CREATE_ENTITY(Canista, NULL, self->position.x, self->position.y);
    canista->direction     = self->direction;
    canista->startDir      = self->direction;
}

void Canista_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Canista->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Canista_CheckPlayerCollisions(void)
{
    RSDK_THIS(Canista);

    int storeX = self->position.x;
    int storeY = self->position.y;

    foreach_active(Player, player)
    {
        if (!self->timer2) {
            if (self->triggerMode) {
                self->position.x = self->startPos.x;
                self->position.y = self->startPos.y;
                self->position.x += self->detectOffset.x;
                self->position.y += self->detectOffset.y;
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    EntityPlayer *playerPtr = (EntityPlayer *)self->detectedPlayer;
                    if (playerPtr) {
                        if (abs(self->position.x - player->position.x) < abs(self->position.x - playerPtr->position.x))
                            self->detectedPlayer = (Entity *)player;
                    }
                    else {
                        self->detectedPlayer = (Entity *)player;
                    }
                }
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &Canista->hitbox3)) {
                    EntityPlayer *playerPtr = (EntityPlayer *)self->detectedPlayer;
                    if (playerPtr) {
                        if (abs(self->position.x - player->position.x) < abs(self->position.x - playerPtr->position.x))
                            self->detectedPlayer = (Entity *)player;
                    }
                    else {
                        self->detectedPlayer = (Entity *)player;
                    }
                }
            }
        }

        self->position.x = storeX;
        self->position.y = storeY;
        if (Player_CheckBadnikTouch(player, self, &Canista->hitbox1))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Canista_CheckOnScreen(void)
{
    RSDK_THIS(Canista);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x  = self->startPos.x;
        self->position.y  = self->startPos.y;
        self->direction   = self->startDir;
        self->shootOffset = (self->shootOffset >> 16);
        Canista_Create(NULL);
    }
}

void Canista_State_Setup(void)
{
    RSDK_THIS(Canista);

    self->active = ACTIVE_NORMAL;
    int offset     = -0x110000;
    if (!(self->direction & FLIP_X))
        offset = 0x100000;
    RSDK.ObjectTileGrip(self, Zone->fgLayers, (2 * ((self->direction & FLIP_X) != 0) + 1), 0, offset, -0x180000, 8);
    self->field_8D      = 0;
    self->timer         = 0;
    self->timer3        = 0;
    self->updateRange.y = 0x2000000;
    self->state         = Canista_State_Unknown1;
    Canista_State_Unknown1();
}

void Canista_State_Unknown1(void)
{
    RSDK_THIS(Canista);

    if (self->timer2)
        self->timer2--;

    EntityPlayer *player = (EntityPlayer *)self->detectedPlayer;
    if (player) {
        int storeX = self->position.x;
        int storeY = self->position.y;
        if (self->triggerMode) {
            self->position.x = self->startPos.x;
            self->position.y = self->startPos.y;
            self->position.x += self->detectOffset.x;
            self->position.y += self->detectOffset.y;

            if (!Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                self->detectedPlayer = NULL;
            }
        }
        else {
            if (!Player_CheckCollisionTouch(player, self, &Canista->hitbox2)) {
                self->detectedPlayer = NULL;
            }
        }

        if (self->detectedPlayer) {
            if (self->triggerMode) {
                int playerY = player->position.y & 0xFFFF0000;
                int shootY  = (storeY + self->shootOffset - self->shootSize) & 0xFFFF0000;

                if (playerY < shootY || playerY > shootY) {
                    self->animator1.animationSpeed = 128;
                    self->velocity.y = ((self->shootOffset + storeY) & 0xFFFF0000) < (player->position.y & 0xFFFF0000) ? 0x10000 : -0x10000;
                }
                else {
                    self->state          = Canista_State_Unknown2;
                    self->timer3         = 60;
                    self->detectedPlayer = 0;
                    self->timer2         = 180;
                }
            }
            else {
                bool32 collided = Player_CheckCollisionTouch(player, self, &Canista->hitbox3);
                if (!collided) {
                    self->animator1.animationSpeed = 128;
                    if (player->position.y - 0x400000 > storeY)
                        self->velocity.y = 0x10000;
                    else
                        self->velocity.y = -0x1000;
                }
                else {
                    self->state          = Canista_State_Unknown2;
                    self->timer3         = 60;
                    self->detectedPlayer = 0;
                    self->timer2         = 180;
                }
            }
        }
        self->position.x = storeX;
        self->position.y = storeY;
    }
    else {
        self->animator1.animationSpeed = 85;
        if (self->field_8D)
            self->velocity.y = -0x8000;
        else
            self->velocity.y = 0x8000;
    }

    self->position.y += self->velocity.y;
    int rotate = self->velocity.y >> 12;
    if (self->direction)
        self->rotation += rotate;
    else
        self->rotation -= rotate;

    int offset = -0x110000;
    if (!(self->direction & FLIP_X))
        offset = 0x100000;
    if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, (2 * ((self->direction & 1) != 0) + 1), 0, offset,
                             ((self->velocity.y >> 31) & 0xFFD40000) + 0x140000, 0)) {
        self->state  = Canista_State_Unknown2;
        self->timer3 = 30;
        if (self->detectedPlayer) {
            self->detectedPlayer = 0;
            self->timer2         = 180;
        }
    }
    RSDK.ProcessAnimation(&self->animator1);

    if (self->timer) {
        self->timer--;
        if (self->timer == 1) {
            self->animator3.frameID = 3;
        }
    }
    else {
        self->animator3.animationTimer = (self->animator3.animationTimer + (abs(self->velocity.y) >> 15)) % 7;
        self->animator3.frameID        = (self->animator3.animationTimer / 3) & 1;
    }
    Canista_CheckPlayerCollisions();
    Canista_CheckOnScreen();
}

void Canista_State_Unknown2(void)
{
    RSDK_THIS(Canista);
    if (!--self->timer3) {
        if (abs(self->velocity.y) == 0x8000)
            self->field_8D ^= 1;
        self->state = Canista_State_Unknown1;
        Canista_State_Unknown1();
    }
    else {
        if (self->timer3 == 30) {
            int offsetX = -0x180000;
            if (!(self->direction & 1))
                offsetX = 0x180000;

            EntityCanista *shot = CREATE_ENTITY(Canista, intToVoid(true), offsetX + self->position.x, self->position.y);
            shot->direction     = self->direction;
            if (shot->direction == FLIP_X)
                shot->velocity.x = 0x40000;
            else
                shot->velocity.x = -0x40000;

            shot            = CREATE_ENTITY(Canista, intToVoid(true), offsetX + self->position.x, self->position.y);
            shot->direction = self->direction;
            if (shot->direction == FLIP_X)
                shot->velocity.x = 0x4C000;
            else
                shot->velocity.x = -0x4C000;

            shot            = CREATE_ENTITY(Canista, intToVoid(true), offsetX + self->position.x, self->position.y);
            shot->direction = self->direction;
            if (shot->direction == FLIP_X)
                shot->velocity.x = 0x58000;
            else
                shot->velocity.x = -0x58000;

            shot            = CREATE_ENTITY(Canista, intToVoid(true), offsetX + self->position.x, self->position.y);
            shot->direction = self->direction;
            if (shot->direction == FLIP_X)
                shot->velocity.x = 0x64000;
            else
                shot->velocity.x = -0x64000;

            self->animator3.frameID = 2;
            self->timer             = 4;
            RSDK.PlaySfx(Canista->sfxPon, false, 255);
        }
        Canista_CheckPlayerCollisions();
        Canista_CheckOnScreen();
    }

    if (self->timer) {
        self->timer--;
        if (self->timer == 1) {
            self->animator3.frameID = 3;
        }
    }
    else {
        self->animator3.frameID = (self->animator3.animationTimer / 3) & 1;
    }
    Canista_CheckOnScreen();
}

void Canista_CheckPlayerCollisions2(void)
{
    RSDK_THIS(Canista);
    if (self->alpha >= 0x80) {

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Canista->hitbox4)) {
                int anim = player->playerAnimator.animationID;
#if RETRO_USE_PLUS
                //checkMightyShellHit
                if (player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)) {
                    int angle           = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
                    self->velocity.x  = -0x400 * RSDK.Cos256(angle);
                    self->velocity.y  = -0x400 * RSDK.Sin256(angle);
                    self->timer3      = 0;
                    self->interaction = false;
                }
                else
#endif
                    Player_CheckHit(player, self);
            }
        }
    }

    RSDK.ProcessAnimation(&self->animator1);
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Canista_State2_Unknown1(void)
{
    RSDK_THIS(Canista);
    self->position.x += self->velocity.x;
    if (self->direction == FLIP_X) {
        self->velocity.x -= 0x2000;
        if (self->velocity.x - 0x2000 < 0) {
            self->startPos.x = self->position.x;
            self->startPos.y = self->position.y;
            self->velocity.x = 0;
            self->rotation   = 6;
            self->angle      = 2;
            RSDK.SetSpriteAnimation(Canista->aniFrames, 4, &self->animator1, true, 0);
            self->state = Canista_State2_Unknown2;
        }
    }
    else {
        self->velocity.x += 0x2000;
        if (self->velocity.x > 0) {
            self->startPos.x = self->position.x;
            self->startPos.y = self->position.y;
            self->velocity.x = 0;
            self->rotation   = 6;
            self->angle      = 2;
            RSDK.SetSpriteAnimation(Canista->aniFrames, 4, &self->animator1, true, 0);
            self->state = Canista_State2_Unknown2;
        }
    }
    Canista_CheckPlayerCollisions2();
}

void Canista_State2_Unknown2(void)
{
    RSDK_THIS(Canista);
    self->angle += self->rotation;
    if (!(Zone->timer & 0x1F) && RSDK.Rand(0, 2) == 1)
        self->rotation = -self->rotation;

    self->startPos.x += self->velocity.x;
    self->startPos.y += self->velocity.y;
    self->velocity.y += 0x800;
    if (self->velocity.y > 0x10000) {
        self->velocity.y -= 0x8000;
        if (self->velocity.y < 0x10000)
            self->velocity.y = 0x10000;
    }

    if (self->velocity.x) {
        if (self->velocity.x <= 0) {
            self->velocity.x += 0x2000;
            if (self->velocity.x > 0)
                self->velocity.x = 0;
        }
        else {
            self->velocity.x -= 0x2000;
            if (self->velocity.x < 0)
                self->velocity.x = 0;
        }
    }

    self->position.x = (RSDK.Cos256(self->angle) << 9) + self->startPos.x;
    self->position.y = (RSDK.Sin256(self->angle) << 9) + self->startPos.y;

    if (self->timer3) {
        self->timer3--;
    }
    else {
        self->alpha -= 4;
        if (self->alpha <= 0)
            destroyEntity(self);
    }
    Canista_CheckPlayerCollisions2();
}

#if RETRO_INCLUDE_EDITOR
void Canista_EditorDraw(void) { Canista_Draw(); }

void Canista_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Canista.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ2"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Canista.bin", SCOPE_STAGE);
}
#endif

void Canista_Serialize(void)
{
    RSDK_EDITABLE_VAR(Canista, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Canista, VAR_UINT8, triggerMode);
    RSDK_EDITABLE_VAR(Canista, VAR_VECTOR2, detectOffset);
    RSDK_EDITABLE_VAR(Canista, VAR_VECTOR2, detectSize);
    RSDK_EDITABLE_VAR(Canista, VAR_ENUM, shootOffset);
    RSDK_EDITABLE_VAR(Canista, VAR_ENUM, shootSize);
}
