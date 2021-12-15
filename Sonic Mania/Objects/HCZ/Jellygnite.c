// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Jellygnite Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectJellygnite *Jellygnite;

void Jellygnite_Update(void)
{
    RSDK_THIS(Jellygnite);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator2);

    StateMachine_Run(self->state);

    if (self->state != Jellygnite_State_Setup && self->objectID == Jellygnite->objectID) {
        if (self->field_66 > 0)
            self->field_66--;
        Jellygnite_CheckPlayerCollisions();

        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            self->position       = self->startPos;
            self->direction      = self->startDir;
            self->visible        = false;
            self->timer          = 0;
            self->angle          = 0;
            self->field_68       = 0;
            self->field_6C       = 0;
            self->grabbedPlayer  = 0;
            self->shakeTimer     = 0;
            self->shakeCount     = 0;
            self->lastShakeFlags = 0;
            Jellygnite_Create(NULL);
        }
    }
}

void Jellygnite_LateUpdate(void) {}

void Jellygnite_StaticUpdate(void)
{
    foreach_active(Jellygnite, jellygnite) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(jellygnite)); }
}

void Jellygnite_Draw(void)
{
    RSDK_THIS(Jellygnite);
    Jellygnite_DrawPart1();
    Jellygnite_DrawPart2();
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Jellygnite_Create(void *data)
{
    RSDK_THIS(Jellygnite);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Jellygnite_State_Setup;
}

void Jellygnite_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HPZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Jellygnite.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("HCZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jellygnite.bin", SCOPE_STAGE);

    Jellygnite->hitbox.left   = -14;
    Jellygnite->hitbox.top    = -14;
    Jellygnite->hitbox.right  = 14;
    Jellygnite->hitbox.bottom = 14;
    Jellygnite->sfxGrab       = RSDK.GetSfx("Global/Grab.wav");
    Jellygnite->sfxElectrify  = RSDK.GetSfx("Stage/Electrify2.wav");
    DEBUGMODE_ADD_OBJ(Jellygnite);
}

void Jellygnite_DebugSpawn(void)
{
    RSDK_THIS(Jellygnite);
    EntityJellygnite *jellygnite = CREATE_ENTITY(Jellygnite, NULL, self->position.x, self->position.y);
    jellygnite->direction        = self->direction;
    jellygnite->startDir         = self->direction;
}

void Jellygnite_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Jellygnite_SetupAnimations(uint8 id)
{
    RSDK_THIS(Jellygnite);

    switch (id) {
        case 0:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 3, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 5, &self->animator3, true, 0);
            break;
        case 1:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 1, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 3, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 5, &self->animator3, true, 0);
            break;
        case 2:
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 2, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 4, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(Jellygnite->aniFrames, 6, &self->animator3, true, 0);
            break;
    }

    self->id = id;
}

void Jellygnite_CheckPlayerCollisions(void)
{
    RSDK_THIS(Jellygnite);

    foreach_active(Player, player)
    {
        if (player != (EntityPlayer *)self->grabbedPlayer) {
            if (Player_CheckCollisionTouch(player, self, &Jellygnite->hitbox) && self->state == Jellygnite_State_Unknown1
                && player->position.y >= self->position.y) {
                self->state = Jellygnite_State_Unknown2;
                Jellygnite_SetupAnimations(1);
                self->grabbedPlayer = (Entity *)player;
                if (Water) {
                    EntityWater *bubble = Water_GetPlayerBubble(player);
                    if (bubble) {
                        bubble->timer = 0;
                        Water_HCZBubbleBurst(bubble, false);
                    }
                }
                self->isPermanent = true;
                RSDK.PlaySfx(Jellygnite->sfxGrab, false, 255);
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->state           = Player_State_None;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->onGround        = false;
                player->direction       = self->direction;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, true, 0);
                player->animator.speed = 0;
                player->direction                     = self->direction ^ 1;
            }

            if (player != (EntityPlayer *)self->grabbedPlayer && player->position.y < self->position.y
                && Player_CheckBadnikTouch(player, self, &Jellygnite->hitbox) && Player_CheckBadnikBreak(self, player, false)) {
                EntityPlayer *playerPtr = (EntityPlayer *)self->grabbedPlayer;
                if (playerPtr)
                    playerPtr->state = Player_State_Air;
                destroyEntity(self);
            }
        }
    }
}

void Jellygnite_HandlePlayerStruggle(void)
{
    RSDK_THIS(Jellygnite);
    EntityPlayer *player = (EntityPlayer *)self->grabbedPlayer;

    if (player) {
        if (self->lastShakeFlags) {
            if (!--self->shakeTimer) {
                self->shakeCount     = 0;
                self->lastShakeFlags = 0;
            }
            uint8 flags = 0;
            if (player->left)
                flags = 1;
            if (player->right)
                flags |= 2;
            if (flags && flags != 3 && flags != self->lastShakeFlags) {
                self->lastShakeFlags = flags;
                if (++self->shakeCount >= 4) {
                    player->state         = Player_State_Air;
                    self->field_66      = 16;
                    self->grabbedPlayer = NULL;
                }
                else {
                    self->shakeTimer = 64;
                }
            }
        }
        else {
            if (player->left) {
                self->lastShakeFlags = 1;
                self->shakeTimer     = 32;
            }
            if (player->right) {
                self->lastShakeFlags |= 2;
                self->shakeTimer = 32;
            }
        }
        player->position.x = self->position.x;
        player->position.y = self->position.y + 0xC0000;
    }
}

bool32 Jellygnite_CheckInWater(void *p)
{
    RSDK_THIS(Jellygnite);
    EntityPlayer *player = (EntityPlayer *)p;

    if (player->position.y > Water->waterLevel)
        return true;

    foreach_active(Water, water)
    {
        if (water->type == WATER_RECT) {
            if (Player_CheckCollisionTouch(player, self, &water->hitbox)
                && RSDK.CheckObjectCollisionTouchBox(self, &Jellygnite->hitbox, water, &water->hitbox)) {
                return true;
            }
        }
    }
    return false;
}

void Jellygnite_DrawPart1(void)
{
    RSDK_THIS(Jellygnite);
    Vector2 drawPos;

    int angle = self->angle & 0x1FF;
    int y     = self->position.y + 0x70000;

    for (int i = 0; i < 4; ++i) {
        drawPos.x = self->position.x + (RSDK.Cos512(angle) << 9);
        drawPos.y = y + (RSDK.Sin512(angle) << 8);
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        angle = (angle + 32) & 0x1FF;
        y += 0x60000;
    }
}

void Jellygnite_DrawPart2(void)
{
    RSDK_THIS(Jellygnite);
    Vector2 drawPos;

    int x   = 0;
    int y   = 0;
    int ang = self->field_68;
    for (int i = 0; i < 4; ++i) {
        int angle = (ang >> 7) & 0x1FF;
        x += 0x312 * RSDK.Sin512(angle);
        y += 0x312 * RSDK.Cos512(angle);

        drawPos.x = x + self->position.x - 0xD0000;
        drawPos.y = y + self->position.y + 0x10000;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        drawPos.x = -x;
        drawPos.x = self->position.x + 0xD0000 - x;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        ang <<= 1;
    }
}

void Jellygnite_State_Setup(void)
{
    RSDK_THIS(Jellygnite);
    if (self->position.y >= Water->waterLevel) {
        self->active         = ACTIVE_NORMAL;
        self->timer          = 0;
        self->angle          = 0;
        self->field_68       = 0;
        self->field_6C       = 0;
        self->grabbedPlayer  = 0;
        self->shakeTimer     = 0;
        self->shakeCount     = 0;
        self->lastShakeFlags = 0;
        Jellygnite_SetupAnimations(0);
        self->state = Jellygnite_State_Unknown1;
        Jellygnite_State_Unknown1();
    }
    else {
        destroyEntity(self);
    }
}

void Jellygnite_State_Unknown1(void)
{
    RSDK_THIS(Jellygnite);

    self->angle      = (self->angle + 4) & 0x1FF;
    self->field_68   = RSDK.Sin512(self->angle) << 1;
    self->field_6C   = (self->field_6C + 1) & 0x1FF;
    self->position.x = (RSDK.Sin512(self->field_6C) << 11) + self->startPos.x;

    EntityPlayer *playerPtr = NULL;
    int distance            = 0x7FFFFFFF;
    foreach_active(Player, player)
    {
        if (abs(self->position.x - player->position.x) < distance) {
            playerPtr = player;
            distance  = abs(self->position.x - player->position.x);
        }
    }

    if (playerPtr) {
        if (Jellygnite_CheckInWater(playerPtr)) {
            if (self->position.y <= playerPtr->position.y - 0x200000) {
                self->velocity.y += 0x800;
                if (self->velocity.y >= 0xC000)
                    self->velocity.y = 0xC000;
            }
            else {
                self->velocity.y -= 0x800;
                if (self->velocity.y <= -0xC000)
                    self->velocity.y = -0xC000;
            }
        }
        else {
            self->velocity.y >>= 1;
        }
    }

    self->position.y += self->velocity.y;
    if (self->position.y - 0x100000 < Water->waterLevel && self->velocity.y < 0) {
        self->position.y = self->position.y - self->velocity.y;
        self->velocity.y = -self->velocity.y;
    }
}

void Jellygnite_State_Unknown2(void)
{
    RSDK_THIS(Jellygnite);
    if (self->field_68 >= 0x600) {
        self->state = Jellygnite_State_Unknown3;
        Jellygnite_SetupAnimations(2);
        RSDK.PlaySfx(Jellygnite->sfxElectrify, false, 255);
    }
    else {
        self->field_68 += 128;
    }

    EntityPlayer *player = (EntityPlayer *)self->grabbedPlayer;
    player->position.x   = self->position.x;
    player->position.y   = self->position.y + 0xC0000;
}

void Jellygnite_State_Unknown3(void)
{
    RSDK_THIS(Jellygnite);
    Jellygnite_HandlePlayerStruggle();

    if (++self->timer == 60) {
        EntityPlayer *player = (EntityPlayer *)self->grabbedPlayer;
        if (player && player->state == Player_State_None) {
            Player_CheckHit(player, self);
            if (player->state != Player_State_Hit && Player_CheckValidState(player))
                player->state = Player_State_Air;
            self->grabbedPlayer = NULL;
        }
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Jellygnite_EditorDraw(void)
{
    Jellygnite_SetupAnimations(0);

    Jellygnite_Draw();
}

void Jellygnite_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("HPZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Jellygnite.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("HCZ"))
        Jellygnite->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jellygnite.bin", SCOPE_STAGE);
}
#endif

void Jellygnite_Serialize(void) { RSDK_EDITABLE_VAR(Jellygnite, VAR_UINT8, direction); }
