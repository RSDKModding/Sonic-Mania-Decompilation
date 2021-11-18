#include "SonicMania.h"

ObjectNewtron *Newtron;

void Newtron_Update(void)
{
    RSDK_THIS(Newtron);
    StateMachine_Run(self->state);
}

void Newtron_LateUpdate(void) {}

void Newtron_StaticUpdate(void) {}

void Newtron_Draw(void)
{
    RSDK_THIS(Newtron);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->state == Newtron_Unknown8)
        RSDK.DrawSprite(&self->animator2, NULL, false);
}

void Newtron_Create(void *data)
{
    RSDK_THIS(Newtron);
    if (!SceneInfo->inEditor) {
        self->drawFX |= FX_FLIP;
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->visible    = true;
        if (data == intToVoid(1)) {
            self->type          = 1;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
        }
        else if (data) {
            self->inkEffect = INK_ADD;
            self->alpha     = 0xC0;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 6, &self->animator1, true, 0);
            self->state         = Newtron_State_Projectile;
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x200000;
            self->updateRange.y = 0x200000;
            self->drawOrder     = Zone->drawOrderHigh;
            return;
        }
        self->inkEffect     = INK_ALPHA;
        self->alpha         = 0x00;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (self->type == 1) {
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 2, &self->animator1, true, 0);
            self->drawOrder = Zone->drawOrderLow;
        }
        else {
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &self->animator1, true, 0);
            self->drawOrder = Zone->drawOrderHigh;
        }
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 5, &self->animator2, true, 0);
        self->state = Newtron_Unknown4;
    }
}

void Newtron_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Newtron->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Newtron.bin", SCOPE_STAGE);
    Newtron->hitbox1.left   = -12;
    Newtron->hitbox1.top    = -14;
    Newtron->hitbox1.right  = 12;
    Newtron->hitbox1.bottom = 14;
    Newtron->hitbox2.left   = -16;
    Newtron->hitbox2.top    = -8;
    Newtron->hitbox2.right  = 16;
    Newtron->hitbox2.bottom = 8;
    Newtron->hitbox3.left   = -6;
    Newtron->hitbox3.top    = -6;
    Newtron->hitbox3.right  = 6;
    Newtron->hitbox3.bottom = 6;
    Newtron->hitbox4.left   = -128;
    Newtron->hitbox4.top    = -64;
    Newtron->hitbox4.right  = 128;
    Newtron->hitbox4.bottom = 64;

    DEBUGMODE_ADD_OBJ(Newtron);
}

void Newtron_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Newtron_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Newtron, NULL, self->position.x, self->position.y);
}

void Newtron_CheckHit(void)
{
    RSDK_THIS(Newtron);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Newtron->hitbox1))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Newtron_CheckOnScreen(void)
{
    RSDK_THIS(Newtron);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->timer      = 0;
        Newtron_Create(NULL);
    }
}

void Newtron_Unknown3(void)
{
    RSDK_THIS(Newtron);

    EntityPlayer *playerPtr = NULL;
    foreach_active(Player, player)
    {
        if (playerPtr) {
            if (abs(player->position.x - self->position.x) < abs(playerPtr->position.x - self->position.x))
                playerPtr = player;
        }
        else {
            playerPtr = player;
        }
    }

    if (playerPtr) {
        self->direction = playerPtr->position.x < self->position.x;
    }
}

void Newtron_Unknown4(void)
{
    RSDK_THIS(Newtron);
    self->active = ACTIVE_NORMAL;
    self->state  = Newtron_Unknown5;
    Newtron_Unknown5();
}

void Newtron_Unknown5(void)
{
    RSDK_THIS(Newtron);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Newtron->hitbox4)) {
            self->state = Newtron_Unknown6;
        }
    }
    Newtron_CheckOnScreen();
}

void Newtron_Unknown6(void)
{
    RSDK_THIS(Newtron);
    if (self->alpha >= 0xF8) {
        self->alpha = 0xFF;
        if (self->type == 1) {
            self->state = Newtron_Unknown7;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 3, &self->animator1, true, 0);
            RSDK.ProcessAnimation(&self->animator1);
            Newtron_CheckOnScreen();
            return;
        }
        self->state = Newtron_Unknown9;
    }
    else {
        self->alpha += 4;
        if (self->type == 1) {
            Newtron_Unknown3();
            RSDK.ProcessAnimation(&self->animator1);
            Newtron_CheckOnScreen();
            return;
        }
    }
    RSDK.ProcessAnimation(&self->animator1);
    Newtron_CheckOnScreen();
}

void Newtron_Unknown7(void)
{
    RSDK_THIS(Newtron);
    Newtron_Unknown3();
    if (self->animator1.frameID >= 2) {
        self->position.y += self->velocity.y;
        self->velocity.y += 0x3800;
        if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
            self->velocity.y = 0;
            if (self->direction == FLIP_NONE)
                self->velocity.x = 0x20000;
            else
                self->velocity.x = -0x20000;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 4, &self->animator1, true, 0);
            self->state = Newtron_Unknown8;
        }
    }
    RSDK.ProcessAnimation(&self->animator1);
    Newtron_CheckHit();
    Newtron_CheckOnScreen();
}

void Newtron_Unknown8(void)
{
    RSDK_THIS(Newtron);
    self->position.x += self->velocity.x;
    RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    Newtron_CheckHit();
    Newtron_CheckOnScreen();
}

void Newtron_Unknown9(void)
{
    RSDK_THIS(Newtron);
    switch (++self->timer) {
        case 30:
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 1, &self->animator1, true, 0);
            if (self->direction)
                RSDK.CreateEntity(Newtron->objectID, (void *)2, self->position.x - 0x140000, self->position.y - 0x80000)->velocity.x = -0x20000;
            else
                RSDK.CreateEntity(Newtron->objectID, (void *)2, self->position.x + 0x140000, self->position.y - 0x80000)->velocity.x = 0x20000;
            break;
        case 45: RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &self->animator1, true, 0); break;
        case 90: self->state = Newtron_Unknown10; break;
    }
    RSDK.ProcessAnimation(&self->animator1);
    Newtron_CheckHit();
    Newtron_CheckOnScreen();
}

void Newtron_Unknown10(void)
{
    RSDK_THIS(Newtron);
    RSDK.ProcessAnimation(&self->animator1);
    Newtron_CheckOnScreen();
    if (self->alpha <= 0)
        RSDK.ResetEntityPtr(self, TYPE_BLANK, NULL);
    self->alpha -= 4;
}

void Newtron_State_Projectile(void)
{
    RSDK_THIS(Newtron);
    self->position.x += self->velocity.x;
    if (!RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ResetEntityPtr(self, TYPE_BLANK, NULL);
    }
    else {
        RSDK.ProcessAnimation(&self->animator1);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Newtron->hitbox3)) {
                Player_CheckProjectileHit(player, self);
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Newtron_EditorDraw(void)
{
    RSDK_THIS(Newtron);
    self->drawFX = FX_FLIP;
    if (self->type == 1)
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 2, &self->animator1, true, 0);
    else 
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Newtron->aniFrames, 5, &self->animator2, true, 0);
    
    Newtron_Draw();
}

void Newtron_EditorLoad(void) { Newtron->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Newtron.bin", SCOPE_STAGE); }
#endif

void Newtron_Serialize(void)
{
    RSDK_EDITABLE_VAR(Newtron, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Newtron, VAR_UINT8, direction);
}
