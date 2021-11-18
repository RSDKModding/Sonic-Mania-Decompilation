#include "SonicMania.h"

ObjectClapperboard *Clapperboard;

void Clapperboard_Update(void)
{
    RSDK_THIS(Clapperboard);

    StateMachine_Run(self->state);
    self->rotation = self->angle >> 1;
    self->field_68 = 0;
    StateMachine_Run(self->stateCollide);
}

void Clapperboard_LateUpdate(void) {}

void Clapperboard_StaticUpdate(void) {}

void Clapperboard_Draw(void)
{
    RSDK_THIS(Clapperboard);
    Vector2 drawPos;

    self->drawFX = FX_ROTATE | FX_FLIP;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->direction == FLIP_X)
        drawPos.x -= 0x670000;
    else
        drawPos.x += 0x10000;
    self->animator4.frameID = self->takeID / 100;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    drawPos.x += 0xD0000;
    self->animator4.frameID = (self->takeID / 10) % 10;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    drawPos.x += 0xD0000;
    self->animator4.frameID = self->takeID % 10;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    drawPos.x += 0x120000;
    self->animator4.frameID = SceneInfo->minutes % 10;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    drawPos.x += 0x120000;
    self->animator4.frameID = SceneInfo->seconds / 10;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    drawPos.x += 0xD0000;
    self->animator4.frameID = SceneInfo->seconds % 10;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    drawPos.x += 0x110000;
    self->animator4.frameID = SceneInfo->milliseconds / 10;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);
}

void Clapperboard_Create(void *data)
{
    RSDK_THIS(Clapperboard);

    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &self->animator2, true, 1);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &self->animator3, true, 2);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 1, &self->animator4, true, 0);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (self->direction == FLIP_NONE) {
            self->dword74      = -0x40000;
            self->dword7C      = 0x6C0000;
            self->angle        = 960;
            self->stateCollide = Clapperboard_StateCollide_FlipX;
        }
        else {
            ++self->animator3.frameID;
            self->dword74      = -0x6C0000;
            self->dword7C      = 0x40000;
            self->angle        = 64;
            self->stateCollide = Clapperboard_StateCollide_NoFlip;
        }
        self->dword78   = -0x40000;
        self->dword80   = -0x40000;
        self->state     = Clapperboard_State_Unknown1;
        self->visible   = true;
        self->drawOrder = Zone->drawOrderLow;
    }
}

void Clapperboard_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Clapperboard.bin", SCOPE_STAGE);
    else
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Clapperboard.bin", SCOPE_STAGE);
    Clapperboard->sfxWhack = RSDK.GetSFX("Stage/Whack.wav");
}

void Clapperboard_StateCollide_NoFlip(void)
{
    RSDK_THIS(Clapperboard);

    uint8 negAngle = -(-self->angle >> 2);

    int val1 = (self->dword78 >> 8) * RSDK.Cos256(negAngle) - (self->dword74 >> 8) * RSDK.Sin256(negAngle);
    int val2 = (self->dword80 >> 8) * RSDK.Cos256(negAngle) - (self->dword7C >> 8) * RSDK.Sin256(negAngle);

    self->hitbox.left  = ((self->dword78 >> 8) * RSDK.Sin256(negAngle) + (self->dword74 >> 8) * RSDK.Cos256(negAngle)) >> 16;
    self->hitbox.right = ((self->dword80 >> 8) * RSDK.Sin256(negAngle) + (self->dword7C >> 8) * RSDK.Cos256(negAngle)) >> 16;
    int sizeX            = self->hitbox.right - self->hitbox.left;
    self->direction ^= FLIP_X;
    bool32 clapFlag = false;

    foreach_active(Player, player)
    {
        int dist = (player->position.x - self->position.x) >> 16;
        if (dist >= self->hitbox.left) {
            if (dist > self->hitbox.right)
                dist = self->hitbox.right;
        }
        else {
            dist = self->hitbox.left;
        }

        int angVal            = self->hitbox.right - dist;
        int top               = (val2 >> 16) + ((val2 >> 16) - (val1 >> 16)) * angVal / sizeX - (self->field_64 & 0xFFFF);
        self->hitbox.top    = top;
        self->hitbox.bottom = top + 24;
        if (Player_CheckCollisionPlatform(player, self, &self->hitbox)) {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif

            player->position.y += (self->field_64 + 2) << 16;
            if (angVal > (sizeX >> 2)) {
                self->field_68 |= RSDK.GetEntityID(player) + 1;
                if (self->state == Clapperboard_State_Unknown3 || (abs(player->groundVel) > 0x80000 && angVal > sizeX - (sizeX >> 2))) {
                    player->onGround   = false;
                    player->state      = Player_State_Air;
                    player->velocity.y = -0x40000 - 0x60000 * angVal / sizeX;
                    if (player->playerAnimator.animationID == ANI_WALK
                        || (player->playerAnimator.animationID > ANI_AIRWALK && player->playerAnimator.animationID <= ANI_DASH))
                        player->storedAnim = player->playerAnimator.animationID;
                    else
                        player->storedAnim = ANI_WALK;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGDIAGONAL, &player->playerAnimator, true, 0);
                    RSDK.PlaySfx(Clapperboard->sfxWhack, false, 255);
                    clapFlag = true;
                }
                else {
                    int val = 16;
                    if (abs(player->groundVel) < 0xA0000)
                        val = 16 * angVal / sizeX;
                    if (val > self->field_64)
                        self->field_64 = val;
                }
            }
        }
    }

    self->direction ^= FLIP_X;
    if (clapFlag)
        ++self->takeID;
}

void Clapperboard_StateCollide_FlipX(void)
{
    RSDK_THIS(Clapperboard);

    int negAngle = -(-self->angle >> 2);

    int val1 = (self->dword78 >> 8) * RSDK.Cos256(negAngle) - (self->dword74 >> 8) * RSDK.Sin256(negAngle);
    int val2 = (val1 >> 16) - ((((self->dword80 >> 8) * RSDK.Cos256(negAngle)) - (self->dword7C >> 8) * RSDK.Sin256(negAngle)) >> 16);

    self->hitbox.left  = (((self->dword78 >> 8) * RSDK.Sin256(negAngle)) + (self->dword74 >> 8) * RSDK.Cos256(negAngle)) >> 16;
    self->hitbox.right = (((self->dword80 >> 8) * RSDK.Sin256(negAngle)) + (self->dword7C >> 8) * RSDK.Cos256(negAngle)) >> 16;
    int Angle            = self->hitbox.right - self->hitbox.left;
    bool32 clapFlag      = false;

    foreach_active(Player, player)
    {
        int dist = (player->position.x - self->position.x) >> 16;
        if (dist >= self->hitbox.left) {
            if (dist > self->hitbox.right)
                dist = self->hitbox.right;
        }
        else {
            dist = self->hitbox.left;
        }

        int angVal            = dist - self->hitbox.left;
        int top               = (val1 >> 16) + val2 * angVal / Angle - (self->field_64 & 0xFFFF);
        self->hitbox.top    = top;
        self->hitbox.bottom = top + 24;

        if (Player_CheckCollisionPlatform(player, self, &self->hitbox)) {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif

            player->position.y += (self->field_64 + 2) << 16;
            if (angVal > Angle >> 2) {
                self->field_68 |= RSDK.GetEntityID(player) + 1;
                if (self->state == Clapperboard_State_Unknown5 || abs(player->groundVel) > 0x80000 && angVal > Angle - (Angle >> 2)) {
                    player->onGround   = false;
                    player->state      = Player_State_Air;
                    player->velocity.y = -0x40000 - 0x60000 * angVal / Angle;
                    if (player->playerAnimator.animationID == ANI_WALK
                        || (player->playerAnimator.animationID > ANI_AIRWALK && player->playerAnimator.animationID <= ANI_DASH))
                        player->storedAnim = player->playerAnimator.animationID;
                    else
                        player->storedAnim = ANI_WALK;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGDIAGONAL, &player->playerAnimator, true, 0);
                    RSDK.PlaySfx(Clapperboard->sfxWhack, false, 255);
                    clapFlag = true;
                    clapFlag = true;
                }
                else {
                    int val = 16;
                    if (abs(player->groundVel) < 0xA0000)
                        val = 16 * angVal / Angle;
                    if (val > self->field_64)
                        self->field_64 = val;
                }
            }
        }
    }

    if (clapFlag)
        ++self->takeID;
}

void Clapperboard_State_Unknown1(void)
{
    RSDK_THIS(Clapperboard);

    if (self->field_68 > 0) {
        if (self->direction == FLIP_NONE)
            self->state = Clapperboard_State_Unknown4;
        else
            self->state = Clapperboard_State_Unknown2;
    }
}

void Clapperboard_State_Unknown2(void)
{
    RSDK_THIS(Clapperboard);

    self->angle -= self->field_64;
    if (self->angle <= 0) {
        self->angle = 0;
        self->state = Clapperboard_State_Unknown3;
    }
}

void Clapperboard_State_Unknown3(void)
{
    RSDK_THIS(Clapperboard);

    self->angle += self->field_64;
    if (self->angle >= 64) {
        self->angle    = 64;
        self->field_64 = 0;
        self->state    = Clapperboard_State_Unknown1;
    }
}

void Clapperboard_State_Unknown4(void)
{
    RSDK_THIS(Clapperboard);

    self->angle += self->field_64;
    if (self->angle >= 1024) {
        self->angle = 1024;
        self->state = Clapperboard_State_Unknown5;
    }
}

void Clapperboard_State_Unknown5(void)
{
    RSDK_THIS(Clapperboard);

    self->angle -= self->field_64;
    if (self->angle <= 0x3C0) {
        self->angle    = 0x3C0;
        self->field_64 = 0;
        self->state    = Clapperboard_State_Unknown1;
    }
}

#if RETRO_INCLUDE_EDITOR
void Clapperboard_EditorDraw(void)
{
    RSDK_THIS(Clapperboard);
    if (self->direction == FLIP_NONE) 
        self->animator3.frameID = 2;
    else 
        self->animator3.frameID = 3;

    Clapperboard_Draw();
}

void Clapperboard_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Clapperboard.bin", SCOPE_STAGE);
    else
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Clapperboard.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Clapperboard, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Clapperboard_Serialize(void) { RSDK_EDITABLE_VAR(Clapperboard, VAR_UINT8, direction); }
