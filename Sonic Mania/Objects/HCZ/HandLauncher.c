#include "SonicMania.h"

ObjectHandLauncher *HandLauncher;

void HandLauncher_Update(void)
{
    RSDK_THIS(HandLauncher);

    if (self->animator2.animationID == 2 && self->animator2.frameID == self->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 3, &self->animator2, true, 0);
    }
    else if (self->animator2.animationID == 4 && self->animator2.frameID == self->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 1, &self->animator2, true, 0);
    }
    StateMachine_Run(self->state);
    if (self->field_80 > 0)
        self->field_80--;
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
}

void HandLauncher_LateUpdate(void) {}

void HandLauncher_StaticUpdate(void)
{
    foreach_active(HandLauncher, launcher) { RSDK.AddDrawListRef(Zone->playerDrawHigh, RSDK.GetEntityID(launcher)); }
}

void HandLauncher_Draw(void)
{
    RSDK_THIS(HandLauncher);
    if (SceneInfo->currentDrawGroup == Zone->playerDrawHigh) {
        if (self->state == HandLauncher_Unknown8)
            RSDK.DrawSprite(&self->animator2, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
        if (self->state != HandLauncher_Unknown8)
            RSDK.DrawSprite(&self->animator2, NULL, false);
    }
}

void HandLauncher_Create(void *data)
{
    RSDK_THIS(HandLauncher);

    self->active    = ACTIVE_BOUNDS;
    self->drawOrder = Zone->drawOrderLow;
    self->visible   = true;
    self->drawFX    = FX_FLIP;
    if (!self->speed)
        self->speed = 16;
    self->startPos      = self->position;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = HandLauncher_Unknown5;
}

void HandLauncher_StageLoad(void)
{
    HandLauncher->active         = ACTIVE_ALWAYS;
    HandLauncher->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/HandLauncher.bin", SCOPE_STAGE);
    HandLauncher->sfxDunkey      = RSDK.GetSfx("HCZ/Dunkey.wav");
    HandLauncher->hitbox1.left   = -24;
    HandLauncher->hitbox1.top    = -240;
    HandLauncher->hitbox1.right  = 24;
    HandLauncher->hitbox1.bottom = 0;
    HandLauncher->hitbox2.left   = -4;
    HandLauncher->hitbox2.top    = -54;
    HandLauncher->hitbox2.right  = 4;
    HandLauncher->hitbox2.bottom = 0;
    HandLauncher->hitbox3.left   = -32;
    HandLauncher->hitbox3.top    = 0;
    HandLauncher->hitbox3.right  = 32;
    HandLauncher->hitbox3.bottom = 24;
}

void HandLauncher_CheckPlayerCollisions(void)
{
    RSDK_THIS(HandLauncher);
    if (self->field_80 <= 0) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);
            if ((self->state == HandLauncher_Unknown8 || !player->sidekick) && !((1 << playerID) & self->activePlayers)) {
                if (player->onGround) {
                    if (Player_CheckCollisionTouch(player, self, &HandLauncher->hitbox2)) {
                        self->activePlayers |= 1 << playerID;
                        player->groundVel = 0;
                        if (self->state != HandLauncher_Unknown8) {
                            if (HandLauncher->dunkeyMode)
                                RSDK.PlaySfx(HandLauncher->sfxDunkey, 0, 255);
                            else
                                RSDK.PlaySfx(Player->sfxRoll, 0, 255);
                        }
                        if (!HandLauncher->dunkeyMode)
                            RSDK.PlaySfx(Player->sfxGrab, 0, 255);
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 0);
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->rotation        = 0;
                        player->state           = Player_State_None;
                    }
                }
            }
        }
    }
}

bool32 HandLauncher_Unknown1(void)
{
    RSDK_THIS(HandLauncher);
    if (self->activePlayers)
        return true;

    int32 storeX       = self->position.x;
    int32 storeY       = self->position.y;
    self->position = self->playerPos;
    bool32 flag      = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &HandLauncher->hitbox1)) {
            flag = true;
            foreach_break;
        }
    }
    self->position.x = storeX;
    self->position.y = storeY;
    return flag;
}

void HandLauncher_State_Release(void)
{
    RSDK_THIS(HandLauncher);
    int32 vel = self->speed * (2 * (self->direction == FLIP_NONE) - 1);
    foreach_active(Player, player)
    {
        int32 pID = RSDK.GetEntityID(player);
        if (((1 << pID) & self->activePlayers)) {
            self->activePlayers &= ~(1 << pID);
            player->onGround  = true;
            player->groundVel = vel << 16;
            player->state     = Player_State_Ground;
        }
    }
}

void HandLauncher_Unknown3(void)
{
    RSDK_THIS(HandLauncher);
    foreach_active(Player, player)
    {
        if (((1 << RSDK.GetEntityID(player)) & self->activePlayers)) {
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->direction  = self->direction != FLIP_NONE;
            player->position.x = self->position.x;
            player->position.y = self->position.y;
            player->position.y -= 0x140000;
            if (self->playerPos.y - 0x140000 < player->position.y)
                player->position.y -= 0x140000;
        }
    }
}

void HandLauncher_Unknown5(void)
{
    RSDK_THIS(HandLauncher);
    self->field_74 = self->startPos;
    self->field_74.y += 0x4E0000;
    self->playerPos = self->field_74;
    self->playerPos.y -= 0x360000;
    self->position.x = self->field_74.x;
    self->position.y = self->field_74.y;
    RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 0, &self->animator1, true, 0);
    self->animator1.speed = 0;
    RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 1, &self->animator2, true, 0);
    self->state = HandLauncher_Unknown6;
}

void HandLauncher_Unknown6(void)
{
    RSDK_THIS(HandLauncher);
    self->position = self->field_74;
    self->position.y -= ((self->field_74.y - self->playerPos.y) >> 3) * minVal(self->field_7C, 8);
    if (self->field_7C > 0)
        self->field_7C--;
    if (self->field_7C < 0)
        self->field_7C = 0;

    if (HandLauncher_Unknown1())
        self->state = HandLauncher_Unknown7;
}

void HandLauncher_Unknown7(void)
{
    RSDK_THIS(HandLauncher);
    self->position = self->field_74;
    self->position.y -= ((self->field_74.y - self->playerPos.y) >> 3) * minVal(self->field_7C, 8);
    if (self->field_7C < 8)
        self->field_7C++;
    if (self->field_7C > 8)
        self->field_7C = 8;

    HandLauncher_CheckPlayerCollisions();
    if (self->activePlayers) {
        RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 2, &self->animator2, true, 0);
        self->field_7C                 = 0;
        self->animator1.speed = 1;
        self->state                    = HandLauncher_Unknown8;
    }

    HandLauncher_Unknown3();
    if (!HandLauncher_Unknown1())
        self->state = HandLauncher_Unknown6;
}

void HandLauncher_Unknown8(void)
{
    RSDK_THIS(HandLauncher);

    if (self->field_7C < 3) {
        self->position = self->playerPos;
        int32 dist         = (self->playerPos.y - self->startPos.y) / 3;
        if (self->field_7C <= 3) {
            self->position.y -= dist * self->field_7C;
            self->field_7C++;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
        else {
            self->position.y -= dist * 3;
            self->field_7C++;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
    }
    else {
        if (self->field_7C < 63) {
            self->position = self->startPos;
            self->field_7C++;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
        else if (self->field_7C < 66) {
            self->position = self->playerPos;
            int32 dist          = (self->playerPos.y - self->startPos.y) / 3;
            self->position.y -= dist * minVal(66 - self->field_7C, 3);
            self->field_7C++;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
        else {
            HandLauncher_State_Release();
            if (!HandLauncher->dunkeyMode) {
                RSDK.PlaySfx(Player->sfxRelease, 0, 255);
            }
            RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 4, &self->animator2, true, 0);
            self->position                 = self->playerPos;
            self->field_80                 = 30;
            self->field_7C                 = 0;
            self->animator1.speed = 0;
            self->state                    = HandLauncher_Unknown6;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void HandLauncher_EditorDraw(void)
{
    RSDK_THIS(HandLauncher);
    RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 1, &self->animator2, true, 0);

    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void HandLauncher_EditorLoad(void) { HandLauncher->aniFrames = RSDK.LoadSpriteAnimation("HCZ/HandLauncher.bin", SCOPE_STAGE); }
#endif

void HandLauncher_Serialize(void)
{
    RSDK_EDITABLE_VAR(HandLauncher, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(HandLauncher, VAR_UINT32, speed);
}
