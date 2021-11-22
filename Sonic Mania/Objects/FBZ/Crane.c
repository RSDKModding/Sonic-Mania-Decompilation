#include "SonicMania.h"

ObjectCrane *Crane;

void Crane_Update(void)
{
    RSDK_THIS(Crane);
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
}

void Crane_LateUpdate(void) {}

void Crane_StaticUpdate(void)
{
    foreach_active(Crane, crane) { RSDK.AddDrawListRef(Zone->playerDrawLow + 1, RSDK.GetEntityID(crane)); }
}

void Crane_Draw(void)
{
    RSDK_THIS(Crane);
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        SpriteFrame *frame = RSDK.GetFrame(Crane->aniFrames, 4, 0);
        int32 height         = (self->position.y - self->startPos.y) >> 16;
        frame->sprY        = 230 - height;
        frame->height      = height;
        frame->pivotY      = -16 - height;
        RSDK.DrawSprite(&self->animator3, NULL, false);
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void Crane_Create(void *data)
{
    RSDK_THIS(Crane);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_FLIP;
    RSDK.SetSpriteAnimation(Crane->aniFrames, 2, &self->animator1, true, 3);
    RSDK.SetSpriteAnimation(Crane->aniFrames, 3, &self->animator2, true, 3);
    RSDK.SetSpriteAnimation(Crane->aniFrames, 4, &self->animator3, true, 0);
    self->state = Crane_State_Unknown1;
}

void Crane_StageLoad(void)
{
    Crane->aniFrames      = RSDK.LoadSpriteAnimation("FBZ/Crane.bin", SCOPE_STAGE);
    Crane->hitbox1.left   = -8;
    Crane->hitbox1.top    = -8;
    Crane->hitbox1.right  = 8;
    Crane->hitbox1.bottom = 8;
    Crane->hitbox3.left   = -8;
    Crane->hitbox3.top    = 0;
    Crane->hitbox3.right  = 8;
    Crane->hitbox3.bottom = 128;
    Crane->hitbox2.left   = -8;
    Crane->hitbox2.top    = 0;
    Crane->hitbox2.right  = 8;
    Crane->hitbox2.bottom = 16;
    Crane->active         = ACTIVE_ALWAYS;
    Crane->sfxGrab        = RSDK.GetSfx("Global/Grab.wav");
    DEBUGMODE_ADD_OBJ(Crane);
}

void Crane_DebugSpawn(void)
{
    RSDK_THIS(Crane);
    EntityCrane *crane = CREATE_ENTITY(Crane, NULL, self->position.x, self->position.y);
    crane->direction   = self->direction;
    crane->startDir    = self->direction;
}

void Crane_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Crane->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Crane_HandlePlayerInteractions(void)
{
    RSDK_THIS(Crane);

    foreach_active(Player, player)
    {
        if (!self->grabbedPlayer) {
            if ((self->playerType || !player->sidekick) && (self->playerType != 1 || player->sidekick)) {
                if (Player_CheckCollisionTouch(player, self, &Crane->hitbox2)) {
                    RSDK.PlaySfx(Crane->sfxGrab, false, 255);
                    self->state       = Crane_State_Unknown5;
                    self->isPermanent = true;
                    RSDK.SetSpriteAnimation(Crane->aniFrames, 0, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(Crane->aniFrames, 1, &self->animator2, true, 0);
                    self->grabbedPlayer   = (Entity *)player;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->state           = Player_State_None;
                    player->nextGroundState = 0;
                    player->nextAirState    = 0;
                    player->onGround        = false;
                    player->direction       = self->direction;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, true, 0);
                    player->animator.animationSpeed = 0;
                }
            }
        }
    }
}

void Crane_CheckOnScreen(void)
{
    RSDK_THIS(Crane);
    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        int32 x              = self->position.x;
        int32 y              = self->position.y;
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        if (RSDK.CheckOnScreen(self, &self->updateRange)) {
            self->position.x = x;
            self->position.y = y;
        }
        else {
            self->grabbedPlayer = NULL;
            self->timer2        = 0;
            self->timer         = 0;
            self->direction     = self->startDir;
            self->visible       = false;
            self->state         = Crane_State_CheckOnScreen;
        }
    }
}

void Crane_State_CheckOnScreen(void)
{
    RSDK_THIS(Crane);
    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->state = Crane_State_Unknown1;
        Crane_Create(NULL);
    }
}

void Crane_State_Unknown1(void)
{
    RSDK_THIS(Crane);

    self->active = ACTIVE_NORMAL;
    if (!self->startType) {
        self->state = Crane_State_Unknown2;
        Crane_State_Unknown2();
    }
    else {
        self->position.y += 0x400000;
        self->timer = -1;
        self->state = Crane_State_Unknown4;
        Crane_HandlePlayerInteractions();
        Crane_CheckOnScreen();
    }
}

void Crane_State_Unknown2(void)
{
    RSDK_THIS(Crane);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Crane->hitbox3)) {
            self->timer = 32;
            self->state = Crane_State_Unknown3;
        }
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown3(void)
{
    RSDK_THIS(Crane);
    Crane_HandlePlayerInteractions();
    Crane_CheckOnScreen();
    --self->timer;
    if (self->state == Crane_State_Unknown3) {
        if (self->timer >= 0)
            self->position.y += 0x20000;
        else
            self->state = Crane_State_Unknown4;
    }
}

void Crane_State_Unknown4(void)
{
    Crane_HandlePlayerInteractions();
    Crane_CheckOnScreen();
}

void Crane_State_Unknown5(void)
{
    RSDK_THIS(Crane);
    if (++self->timer < 32) {
        self->position.y -= 0x20000;
        EntityPlayer *player = (EntityPlayer *)self->grabbedPlayer;
        if (player) {
            if (Player_CheckValidState(player)) {
                player->animator.animationSpeed = 0;
                player->position.x                    = self->position.x;
                player->position.y                    = self->position.y;
                if (self->direction) {
                    player->position.x += 0x20000;
                    player->position.y += 0xE0000;
                }
                else {
                    player->position.x -= 0x20000;
                    player->position.y += 0xE0000;
                }
            }
            else {
                self->grabbedPlayer = NULL;
            }
        }
    }
    else {
        self->timer2     = 1;
        self->timer      = 16;
        self->velocity.x = 0;
        self->state      = Crane_State_Unknown6;
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown6(void)
{
    RSDK_THIS(Crane);

    self->velocity.x += 0x1000;
    if (self->direction == FLIP_NONE) {
        self->position.x += self->velocity.x;
        if (self->position.x >= self->startPos.x + (self->distance << 15))
            self->state = Crane_State_Unknown7;
    }
    else {
        self->position.x -= self->velocity.x;
        if (self->position.x <= self->startPos.x - (self->distance << 15))
            self->state = Crane_State_Unknown7;
    }

    EntityPlayer *player = (EntityPlayer *)self->grabbedPlayer;
    if (player) {
        if (Player_CheckValidState(player)) {
            player->animator.animationSpeed = 0;
            player->position.x                    = self->position.x;
            player->position.y                    = self->position.y;
            if (self->direction) {
                player->position.x += 0x20000;
                player->position.y += 0xE0000;
            }
            else {
                player->position.x -= 0x20000;
                player->position.y += 0xE0000;
            }
        }
        else {
            self->grabbedPlayer = NULL;
        }
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown7(void)
{
    RSDK_THIS(Crane);

    EntityPlayer *player = (EntityPlayer *)self->grabbedPlayer;
    self->velocity.x -= 0x1000;
    if (self->direction == FLIP_NONE)
        self->position.x += self->velocity.x;
    else
        self->position.x -= self->velocity.x;
    if (self->velocity.x <= 0) {
        if (player) {
            if (Player_CheckValidState(player))
                player->state = Player_State_Air;
            else
                self->grabbedPlayer = 0;
        }
        RSDK.SetSpriteAnimation(Crane->aniFrames, 2, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Crane->aniFrames, 3, &self->animator2, true, 0);
        self->timer2      = 30;
        self->state       = Crane_State_Unknown8;
        self->isPermanent = false;
    }

    if (player) {
        if (Player_CheckValidState(player)) {
            player->animator.animationSpeed = 0;
            player->position.x                    = self->position.x;
            player->position.y                    = self->position.y;
            if (self->direction) {
                player->position.x += 0x20000;
                player->position.y += 0xE0000;
            }
            else {
                player->position.x -= 0x20000;
                player->position.y += 0xE0000;
            }
        }
        else {
            self->grabbedPlayer = NULL;
        }
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown8(void)
{
    RSDK_THIS(Crane);
    if (--self->timer2 <= 0)
        self->state = Crane_State_Unknown9;
    Crane_HandlePlayerInteractions();
    Crane_CheckOnScreen();
}

void Crane_State_Unknown9(void)
{
    RSDK_THIS(Crane);

    self->velocity.x -= 0x1000;
    if (self->direction) {
        self->position.x -= self->velocity.x;
        if (self->position.x >= self->startPos.x - (self->distance << 15))
            self->state = Crane_State_Unknown10;
    }
    else {
        self->position.x += self->velocity.x;
        if (self->position.x <= self->startPos.x + (self->distance << 15))
            self->state = Crane_State_Unknown10;
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown10(void)
{
    RSDK_THIS(Crane);
    self->velocity.x += 0x1000;

    if (self->direction)
        self->position.x -= self->velocity.x;
    else
        self->position.x += self->velocity.x;
    if (self->velocity.x >= 0) {
        if (self->startType) {
            self->timer         = 32;
            self->state         = Crane_State_Unknown3;
            self->grabbedPlayer = NULL;
        }
        else {
            self->state         = Crane_State_Unknown2;
            self->grabbedPlayer = NULL;
        }
    }
    Crane_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Crane_EditorDraw(void) {}

void Crane_EditorLoad(void) {}
#endif

void Crane_Serialize(void)
{
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT16, distance);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, startType);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, playerType);
}
