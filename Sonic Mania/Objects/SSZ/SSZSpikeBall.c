#include "SonicMania.h"

ObjectSSZSpikeBall *SSZSpikeBall;

void SSZSpikeBall_Update(void)
{
    RSDK_THIS(SSZSpikeBall);
    StateMachine_Run(self->state);

    foreach_active(Player, player)
    {
        int type = self->type;
        if (type < 4)
            Player_CheckCollisionBox(player, self, &SSZSpikeBall->hitboxBase[type]);

        if (Player_CheckCollisionTouch(player, &self->spikeBallPos, &SSZSpikeBall->hitboxBall)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
        }
    }
}

void SSZSpikeBall_LateUpdate(void) {}

void SSZSpikeBall_StaticUpdate(void) {}

void SSZSpikeBall_Draw(void)
{
    RSDK_THIS(SSZSpikeBall);
    RSDK.DrawSprite(&self->animator2, &self->spikeBallPos, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void SSZSpikeBall_Create(void *data)
{
    RSDK_THIS(SSZSpikeBall);
    if (!self->speed) {
        self->speed = 12;
        self->dist  = 64;
    }
    if (!self->interval)
        self->interval = 120;

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 1, &self->animator2, true, 0);

        self->active         = ACTIVE_BOUNDS;
        self->updateRange.x  = 0x600000;
        self->updateRange.y  = 0x600000;
        self->visible        = true;
        self->drawOrder      = Zone->drawOrderLow;
        self->speed <<= 15;
        self->dist           = (self->dist << 16) / self->speed;
        self->spikeBallPos.x = self->position.x;
        self->spikeBallPos.y = self->position.y;

        switch (self->type) {
            case 0:
            case 2:
                self->spikeBallPos.x = -0x200000;
                self->spikeBallPos.y = -0x200000;
                self->direction      = FLIP_NONE;
                break;
            case 1:
            case 3:
                self->spikeBallPos.x = -0x200000;
                self->spikeBallPos.y = -0x200000;
                self->direction      = FLIP_X;
                break;
            case 4: self->velocity.y = -self->speed; break;
            case 5: self->velocity.y = self->speed; break;
            case 6: self->velocity.x = -self->speed; break;
            case 7: self->velocity.x = self->speed; break;
            default: break;
        }

        self->state             = SSZSpikeBall_State_Setup;
        self->animator1.frameID = minVal(self->type, 4);
    }
}

void SSZSpikeBall_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeBall.bin", SCOPE_STAGE);
    else
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SpikeBall.bin", SCOPE_STAGE);

    SSZSpikeBall->hitboxBall.left  = -12;
    SSZSpikeBall->hitboxBall.top  = -12;
    SSZSpikeBall->hitboxBall.right = 12;
    SSZSpikeBall->hitboxBall.bottom = 12;

    SSZSpikeBall->hitboxBase[0].left  = -16;
    SSZSpikeBall->hitboxBase[0].top  = -12;
    SSZSpikeBall->hitboxBase[0].right = 16;
    SSZSpikeBall->hitboxBase[0].bottom = 0;

    SSZSpikeBall->hitboxBase[1].left   = -16;
    SSZSpikeBall->hitboxBase[1].top    = 0;
    SSZSpikeBall->hitboxBase[1].right  = 16;
    SSZSpikeBall->hitboxBase[1].bottom = 12;

    SSZSpikeBall->hitboxBase[2].left   = -12;
    SSZSpikeBall->hitboxBase[2].top    = -16;
    SSZSpikeBall->hitboxBase[2].right  = 0;
    SSZSpikeBall->hitboxBase[2].bottom = 16;

    SSZSpikeBall->hitboxBase[3].left   = -12;
    SSZSpikeBall->hitboxBase[3].top    = -16;
    SSZSpikeBall->hitboxBase[3].right  = 0;
    SSZSpikeBall->hitboxBase[3].bottom = 16;

    SSZSpikeBall->sfxPon = RSDK.GetSfx("Stage/Pon.wav");
}

void SSZSpikeBall_State_Setup(void)
{
    RSDK_THIS(SSZSpikeBall);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        self->spikeBallPos.x = self->position.x;
        self->spikeBallPos.y = self->position.y;
        self->active         = ACTIVE_NORMAL;
        self->timer          = 0;
        switch (self->type) {
            case 0:
                self->state = SSZSpikeBall_Unknown3;
                self->spikeBallPos.y += 0x40000;
                self->velocity.y = -self->speed;
                break;
            case 1:
                self->spikeBallPos.y -= 0x40000;
                self->state      = SSZSpikeBall_Unknown3;
                self->velocity.y = self->speed;
                break;
            case 2:
                self->state = SSZSpikeBall_Unknown2;
                self->spikeBallPos.x += 0x30000;
                self->velocity.x = -self->speed;
                break;
            case 3:
                self->spikeBallPos.x -= 0x30000;
                self->state      = SSZSpikeBall_Unknown2;
                self->velocity.x = self->speed;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                self->timer = self->dist;
                self->state = SSZSpikeBall_Unknown6;
                break;
            default: break;
        }
    }
}

void SSZSpikeBall_Unknown2(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (++self->timer >= 0x20) {
        if (self->timer == 48) {
            RSDK.PlaySfx(SSZSpikeBall->sfxPon, false, 255);
            self->state = SSZSpikeBall_Unknown4;
        }
    }
    else if (self->direction)
        self->spikeBallPos.x += 0x8000;
    else
        self->spikeBallPos.x -= 0x8000;
}

void SSZSpikeBall_Unknown3(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (++self->timer >= 0x20) {
        if (self->timer == 48) {
            RSDK.PlaySfx(SSZSpikeBall->sfxPon, false, 255);
            self->state = SSZSpikeBall_Unknown5;
        }
    }
    else if (self->direction)
        self->spikeBallPos.y += 0x8000;
    else
        self->spikeBallPos.y -= 0x8000;
}

void SSZSpikeBall_Unknown4(void)
{
    RSDK_THIS(SSZSpikeBall);

    self->spikeBallPos.x += self->velocity.x;
    if (self->direction) {
        self->velocity.x -= 0x3800;
        if (self->spikeBallPos.x < self->position.x) {
            self->state          = SSZSpikeBall_State_Setup;
            self->active         = ACTIVE_BOUNDS;
            self->spikeBallPos.x = -0x200000;
        }
    }
    else {
        self->velocity.x += 0x3800;
        if (self->spikeBallPos.x > self->position.x) {
            self->state          = SSZSpikeBall_State_Setup;
            self->active         = ACTIVE_BOUNDS;
            self->spikeBallPos.x = -0x200000;
        }
    }
}

void SSZSpikeBall_Unknown5(void)
{
    RSDK_THIS(SSZSpikeBall);

    self->spikeBallPos.y += self->velocity.y;
    if (self->direction) {
        self->velocity.y -= 0x3800;
        if (self->spikeBallPos.y < self->position.y) {
            self->state          = SSZSpikeBall_State_Setup;
            self->active         = ACTIVE_BOUNDS;
            self->spikeBallPos.y = -0x200000;
        }
    }
    else {
        self->velocity.y += 0x3800;
        if (self->spikeBallPos.y > self->position.y) {
            self->state          = SSZSpikeBall_State_Setup;
            self->active         = ACTIVE_BOUNDS;
            self->spikeBallPos.y = -0x200000;
        }
    }
}

void SSZSpikeBall_Unknown6(void)
{
    RSDK_THIS(SSZSpikeBall);

    self->spikeBallPos.x += self->velocity.x;
    self->spikeBallPos.y += self->velocity.y;
    if (!--self->timer) {
        self->timer = (self->interval >> 1) - self->dist;
        self->state = SSZSpikeBall_Unknown7;
    }
}

void SSZSpikeBall_Unknown7(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (!--self->timer) {
        self->timer = self->dist;
        self->state = SSZSpikeBall_Unknown8;
    }
}

void SSZSpikeBall_Unknown8(void)
{
    RSDK_THIS(SSZSpikeBall);

    self->spikeBallPos.x -= self->velocity.x;
    self->spikeBallPos.y -= self->velocity.y;
    if (!--self->timer) {
        self->active = ACTIVE_BOUNDS;
        self->state  = SSZSpikeBall_State_Setup;
    }
}

#if RETRO_INCLUDE_EDITOR
void SSZSpikeBall_EditorDraw(void)
{
    RSDK_THIS(SSZSpikeBall);
    RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 1, &self->animator2, true, 0);

    self->spikeBallPos.x = self->position.x;
    self->spikeBallPos.y = self->position.y;

    switch (self->type) {
        case 0:
        case 2:
            self->spikeBallPos.x = -0x200000;
            self->spikeBallPos.y = -0x200000;
            self->direction      = FLIP_NONE;
            break;
        case 1:
        case 3:
            self->spikeBallPos.x = -0x200000;
            self->spikeBallPos.y = -0x200000;
            self->direction      = FLIP_X;
            break;
        default: break;
    }

    self->animator1.frameID = minVal(self->type, 4);

    SSZSpikeBall_Draw();
}

void SSZSpikeBall_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeBall.bin", SCOPE_STAGE);
    else
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SpikeBall.bin", SCOPE_STAGE);
}
#endif

void SSZSpikeBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_ENUM, dist);
}
