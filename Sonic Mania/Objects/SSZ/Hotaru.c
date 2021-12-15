// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Hotaru Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHotaru *Hotaru;

void Hotaru_Update(void)
{
    RSDK_THIS(Hotaru);
    StateMachine_Run(self->state);
}

void Hotaru_LateUpdate(void) {}

void Hotaru_StaticUpdate(void) {}

void Hotaru_Draw(void)
{
    RSDK_THIS(Hotaru);
    Vector2 drawPos;

    self->direction = self->field_88;
    if (self->field_A2 == 2) {
        self->inkEffect = INK_ADD;
        self->alpha     = 0x80;
        drawPos.x         = self->position.x + self->offset.x;
        drawPos.y         = self->offset.y + 0xC0000 + self->position.y;
        drawPos.x += self->offset2.x;
        drawPos.y += self->offset2.y;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        drawPos.x += self->offset2.x;
        drawPos.y += self->offset2.y;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        drawPos.x += self->offset2.x;
        drawPos.y += self->offset2.y;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->alpha     = 0x100;
        self->inkEffect = INK_NONE;
    }

    if (!(self->flags & 1)) {
        drawPos.x = self->position.x + self->dist2.x;
        drawPos.y = self->position.y + self->dist2.y;
        if (self->alpha > 0 && (((Zone->timer & 1) << (self->state == Hotaru_State_Unknown7)) & 0x7FFFFFFF)) {
            self->inkEffect = INK_ADD;
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            self->inkEffect = INK_NONE;
        }
        RSDK.DrawSprite(&self->animator1, &drawPos, false);
    }

    if (!(self->flags & 2)) {
        self->direction ^= FLIP_X;
        drawPos.x = self->position.x + self->offset.x;
        drawPos.y = self->position.y + self->offset.y;
        if (self->alpha > 0 && (((Zone->timer & 1) << (self->state == Hotaru_State_Unknown7)) & 0x7FFFFFFF)) {
            self->inkEffect = INK_ADD;
            RSDK.DrawSprite(&self->animator2, &drawPos, false);

            self->inkEffect = INK_NONE;
        }
        RSDK.DrawSprite(&self->animator1, &drawPos, false);
    }
}

void Hotaru_Create(void *data)
{
    RSDK_THIS(Hotaru);
    if (!SceneInfo->inEditor) {
        self->distX1 <<= 16;
        self->distX2 <<= 16;
        self->speed <<= 15;
        self->offset.x  = -self->dist.x;
        self->offset.y  = -self->dist.y;
        self->dist2.x   = self->dist.x;
        self->dist2.y   = self->dist.y;
        self->visible   = true;
        self->drawOrder = Zone->drawOrderLow;
        self->active    = ACTIVE_BOUNDS;
        if (self->distX1 <= self->distX2)
            self->updateRange.x = self->distX2 + 0x400000;
        else
            self->updateRange.x = self->distX1 + 0x400000;
        self->updateRange.y = self->dist.y + 0x400000;

        self->drawFX     = FX_ROTATE | FX_FLIP;
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->startDir   = self->direction;
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, self->quality, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, 2, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, 3, &self->animator3, true, 0);
        self->state = Hotaru_State_Setup;
    }
}

void Hotaru_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Hotaru.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Hotaru.bin", SCOPE_STAGE);

    Hotaru->hitbox1.top    = -6;
    Hotaru->hitbox1.left   = -6;
    Hotaru->hitbox1.right  = 6;
    Hotaru->hitbox1.bottom = 6;
    Hotaru->hitbox2.top    = -32;
    Hotaru->hitbox2.left   = -32;
    Hotaru->hitbox2.right  = 32;
    Hotaru->hitbox2.bottom = 32;
    Hotaru->hitbox3.top    = -8;
    Hotaru->hitbox3.left   = -8;
    Hotaru->hitbox3.right  = 8;
    Hotaru->hitbox3.bottom = 8;

    DEBUGMODE_ADD_OBJ(Hotaru);
}

void Hotaru_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Hotaru, NULL, self->position.x, self->position.y);
}

void Hotaru_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Hotaru_CheckPlayerCollisions(void)
{
    RSDK_THIS(Hotaru);

    int storeX = self->position.x;
    int storeY = self->position.y;

    foreach_active(Player, player)
    {
        if (!(self->flags & 1)) {
            self->position.x += self->dist2.x;
            self->position.y += self->dist2.y;
            if (Player_CheckBadnikTouch(player, self, &Hotaru->hitbox1) && Player_CheckBadnikBreak(self, player, false)) {
                self->flags |= 1;
                self->field_A2 = 0;
            }
            self->position.x = storeX;
            self->position.y = storeY;
        }
        if (!(self->flags & 2)) {
            self->position.x += self->offset.x;
            self->position.y += self->offset.y;
            if (Player_CheckBadnikTouch(player, self, &Hotaru->hitbox1) && Player_CheckBadnikBreak(&self, player, false)) {
                self->flags |= 2;
                self->field_A2 = 0;
            }
            self->position.x = storeX;
            self->position.y = storeY;
        }
    }
}

void Hotaru_CheckOnScreen(void)
{
    RSDK_THIS(Hotaru);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->speed >>= 15;
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->distX1     = (self->distX1 >> 16);
        self->distX2     = (self->distX2 >> 16);
        self->direction  = self->startDir;
        Hotaru_Create(NULL);
    }
}

void Hotaru_State_Setup(void)
{
    RSDK_THIS(Hotaru);

    self->active   = ACTIVE_NORMAL;
    self->field_88 = 0;
    self->alpha    = 0;
    self->field_A2 = 0;
    self->state    = Hotaru_State_Unknown1;
    Hotaru_State_Unknown1();
}

void Hotaru_State_Unknown1(void)
{
    RSDK_THIS(Hotaru);

    if (self->field_88) {
        self->offset.x -= self->speed;
        self->dist2.x += self->speed;
        if (self->dist2.x >= self->distX2) {
            self->offset.x = -self->distX2;
            self->dist2.x  = self->distX2;
            self->field_88 = 0;
        }
    }
    else {
        self->offset.x += self->speed;
        self->dist2.x -= self->speed;
        if (self->offset.x >= self->distX1) {
            self->offset.x = self->distX1;
            self->dist2.x  = -self->distX1;
            self->field_88 = 1;
        }
    }
    Hotaru_CheckPlayerCollisions();
    int storeX = self->position.x;
    int storeY = self->position.y;

    foreach_active(Player, player)
    {
        bool32 flag = false;
        if (!(self->flags & 1)) {
            self->position.x += self->dist2.x;
            self->position.y += self->dist2.y;
            if (Player_CheckCollisionTouch(player, self, &Hotaru->hitbox2)) {
                flag = true;
            }
        }
        self->position.x = storeX;
        self->position.y = storeY;

        if (!(self->flags & 2) && !flag) {
            self->position.x += self->offset.x;
            self->position.y += self->offset.y;
            if (Player_CheckCollisionTouch(player, self, &Hotaru->hitbox2)) {
                flag = true;
            }
        }

        if (flag) {
            self->playerPtr = (Entity *)player;
            if (player->sidekick)
                self->screenID = 0;
            else
                self->screenID = player->camera->screenID;

            self->position.x = storeX;
            self->position.y = storeY;
            self->state      = Hotaru_State_Unknown2;
            foreach_break;
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void Hotaru_State_Unknown2(void)
{
    RSDK_THIS(Hotaru);
    self->state = Hotaru_State_Unknown3;
    Hotaru_State_Unknown3();
}

void Hotaru_State_Unknown3(void)
{
    RSDK_THIS(Hotaru);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;
    RSDKScreenInfo *screen   = &ScreenInfo[self->screenID];

    int screenX = (screen->position.x + screen->centerX) << 16;
    int screenY = (screen->position.y + screen->centerY) << 16;

    if (self->position.x <= screenX) {
        self->position.x += player->velocity.x + 0x20000;
        if (self->position.x > screenX)
            self->position.x = screenX;
    }
    else {
        self->position.x -= player->velocity.x - 0x20000;
        if (self->position.x < screenX)
            self->position.x = screenX;
    }

    if (self->position.y <= screenY) {
        self->position.y += player->velocity.y + 0x20000;
        if (self->position.y > screenY)
            self->position.y = screenY;
    }
    else {
        self->position.y -= player->velocity.y - 0x20000;
        if (self->position.y < screenY)
            self->position.y = screenY;
    }
    if (self->position.x == screenX && self->position.y == screenY) {
        self->timer = 100;
        self->state = Hotaru_State_Unknown4;
    }

    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOnScreen();
}

void Hotaru_State_Unknown4(void)
{
    RSDK_THIS(Hotaru);
    RSDKScreenInfo *screen = &ScreenInfo[self->screenID];

    self->position.x = (screen->position.x + screen->centerX) << 16;
    self->position.y = (screen->position.y + screen->centerY) << 16;
    if (++self->rotation == 128) {
        self->rotation          = 0;
        self->animator1.frameID = 1;
        self->state             = Hotaru_State_Unknown5;
    }
    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOnScreen();
}

void Hotaru_State_Unknown5(void)
{
    RSDK_THIS(Hotaru);

    if (self->alpha >= 128) {
        Hitbox hitbox;
        if (self->offset.x >= self->dist2.x) {
            hitbox.left  = self->dist2.x >> 16;
            hitbox.right = self->offset.x >> 16;
        }
        else {
            hitbox.left  = self->offset.x >> 16;
            hitbox.right = self->dist2.x >> 16;
        }

        if (self->offset.y >= self->dist2.y) {
            hitbox.top    = self->dist2.y >> 16;
            hitbox.bottom = self->offset.y >> 16;
        }
        else {
            hitbox.top    = self->offset.y >> 16;
            hitbox.bottom = self->dist2.y >> 16;
        }

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                self->field_A2 = 1;
            }
        }
    }

    self->alpha += 2;
    if (self->alpha == 256) {
        self->timer = 128;
        if (!self->flags && !self->quality) {
            ++self->field_A2;
            self->offset2.x = (self->dist2.x - self->offset.x) >> 2;
            self->offset2.y = (self->dist2.y - self->offset.y) >> 2;
        }
        self->state = Hotaru_State_Unknown6;
    }
    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOnScreen();
}

void Hotaru_State_Unknown6(void)
{
    RSDK_THIS(Hotaru);

    RSDK.ProcessAnimation(&self->animator3);
    if (--self->timer <= 0) {
        self->field_A2 = 0;
        self->alpha    = 192;
        self->state    = Hotaru_State_Unknown7;
    }
    Hotaru_CheckPlayerCollisions();

    if (self->field_A2 == 2) {
        int storeX = self->position.x;
        int storeY = self->position.y;

        foreach_active(Player, player)
        {
            self->position.x += self->offset.x;
            self->position.y += self->offset.y;
            for (int i = 0; i < 3; ++i) {
                self->position.x += self->offset2.x;
                self->position.y += self->offset2.y;
                if (Player_CheckCollisionTouch(player, self, &Hotaru->hitbox3)) {
                    Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
                }
            }
            self->position.x = storeX;
            self->position.y = storeY;
        }
    }
    Hotaru_CheckOnScreen();
}

void Hotaru_State_Unknown7(void)
{
    RSDK_THIS(Hotaru);
    self->position.y -= 0x20000;
    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Hotaru_EditorDraw(void)
{
    RSDK_THIS(Hotaru);

    self->drawOrder = Zone->drawOrderLow;
    self->active    = ACTIVE_BOUNDS;
    if (self->distX1 <= self->distX2)
        self->updateRange.x = (self->distX2 << 16) + 0x400000;
    else
        self->updateRange.x = (self->distX1 << 16) + 0x400000;
    self->updateRange.y = self->dist.y + 0x400000;

    self->drawFX     = FX_ROTATE | FX_FLIP;
    self->offset.x   = -self->dist.x;
    self->offset.y   = -self->dist.y;
    self->dist2.x    = self->dist.x;
    self->dist2.y    = self->dist.y;
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, self->quality, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 2, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 3, &self->animator3, true, 0);

    Hotaru_Draw();
}

void Hotaru_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Hotaru.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Hotaru.bin", SCOPE_STAGE);
}
#endif

void Hotaru_Serialize(void)
{
    RSDK_EDITABLE_VAR(Hotaru, VAR_UINT8, quality);
    RSDK_EDITABLE_VAR(Hotaru, VAR_VECTOR2, dist);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, distX1);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, distX2);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Hotaru, VAR_UINT8, preset);
}
