#include "SonicMania.h"

ObjectLaunchSpring *LaunchSpring;

void LaunchSpring_Update(void)
{
    RSDK_THIS(LaunchSpring);
    StateMachine_Run(self->state);
}

void LaunchSpring_LateUpdate(void) {}

void LaunchSpring_StaticUpdate(void) {}

void LaunchSpring_Draw(void)
{
    RSDK_THIS(LaunchSpring);

    RSDK.GetFrame(LaunchSpring->aniFrames, 2, 0)->pivotY = -8 - self->field_B1;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    self->animator1.frameID = 0;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    SpriteFrame *frame = RSDK.GetFrame(LaunchSpring->aniFrames, 0, 0);
    RSDK.DrawSprite(&self->animator1, NULL, false);

    frame->pivotY     = self->timer - 55;
    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    frame->pivotY     = -55;
    self->direction = FLIP_NONE;
}

void LaunchSpring_Create(void *data)
{
    RSDK_THIS(LaunchSpring);
    if (!self->rotSpeed)
        self->rotSpeed = 4;

    self->drawFX   = FX_ROTATE | FX_FLIP;
    self->rotation = self->angle << 6;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2");
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->animator3, true, 0);
        self->drawOrder = Zone->drawOrderHigh;

        switch (self->type) {
            case 0:
                self->timer = 47;
                self->state = LaunchSpring_Unknown4;
                break;
            case 1:
                self->state             = LaunchSpring_Unknown9;
                self->animator3.frameID = 1;
                break;
            case 2:
                self->field_B4          = self->rotation - 64;
                self->field_B6          = self->rotation + 64;
                self->state             = LaunchSpring_Unknown13;
                self->animator3.frameID = 1;
                break;
            case 3:
                self->field_B4          = self->rotation - 128;
                self->field_B6          = self->rotation + 128;
                self->state             = LaunchSpring_Unknown13;
                self->animator3.frameID = 1;
                break;
            case 4:
                self->field_B4          = self->rotation - 192;
                self->field_B6          = self->rotation + 192;
                self->state             = LaunchSpring_Unknown13;
                self->animator3.frameID = 1;
                break;
            case 5:
                self->field_B4          = self->rotation - 256;
                self->field_B6          = self->rotation + 256;
                self->state             = LaunchSpring_Unknown13;
                self->animator3.frameID = 1;
                break;
            default: break;
        }

        LaunchSpring->sfxGrab = RSDK.GetSfx("Global/Grab.wav");
        if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
            LaunchSpring->flag            = false;
            LaunchSpring->sfxClack        = RSDK.GetSfx("Stage/Clack.wav");
            LaunchSpring->sfxSpeedBooster = RSDK.GetSfx("Stage/SpeedBooster.wav");
        }
        else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2")) {
            LaunchSpring->flag            = true;
            LaunchSpring->sfxSpeedBooster = RSDK.GetSfx("Stage/CannonFire.wav");
        }
    }
}

void LaunchSpring_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/LaunchSpring.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/LaunchSpring.bin", SCOPE_STAGE);

    LaunchSpring->hitbox.top    = -24;
    LaunchSpring->hitbox.left   = -24;
    LaunchSpring->hitbox.right  = 24;
    LaunchSpring->hitbox.bottom = 24;
}

bool32 LaunchSpring_Unknown1(void *p)
{
    if (!Firework)
        return false;

    EntityPlayer *player = (EntityPlayer *)p;
    int32 playerID = RSDK.GetEntityID(player);
    bool32 flag  = false;
    foreach_active(Firework, firework)
    {
        if ((1 << playerID) & firework->activePlayers) {
            flag = true;
            foreach_break;
        }
    }
    return flag;
}

void LaunchSpring_CheckPlayerCollisions(void *nextState)
{
    RSDK_THIS(LaunchSpring);

    foreach_active(Player, player)
    {
        if (!player->sidekick && !LaunchSpring_Unknown1(player)) {
            if (Player_CheckCollisionTouch(player, self, &LaunchSpring->hitbox)) {
                RSDK.PlaySfx(LaunchSpring->sfxGrab, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->position.x += RSDK.Cos512(self->rotation - 128) << 12;
                player->position.y += RSDK.Sin512(self->rotation - 128) << 12;
                player->velocity.x        = 0;
                player->velocity.y        = 0;
                player->groundVel         = 0;
                player->onGround          = false;
                player->state             = Player_State_None;
                player->nextGroundState   = StateMachine_None;
                player->nextAirState      = StateMachine_None;
                self->animator3.frameID = 1;
                self->playerPtr         = (Entity *)player;
                self->state             = nextState;
                foreach_break;
            }
        }
    }
}

void LaunchSpring_Unknown3(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        RSDK.PlaySfx(LaunchSpring->sfxSpeedBooster, false, 255);
        if (self->strict) {
            self->rotation   = (self->rotation + 64) & 0x1C0;
            player->position.x = self->position.x;
            player->position.y = self->position.y;
            player->position.x += RSDK.Cos512(self->rotation - 128) << 12;
            player->position.y += RSDK.Sin512(self->rotation - 128) << 12;
        }
        player->jumpAbility      = 0;
        player->jumpAbilityTimer = 0;
        player->velocity.x       = RSDK.Cos512(self->rotation - 128) << 11;
        player->velocity.y       = RSDK.Sin512(self->rotation - 128) << 11;
        player->state            = Player_State_Air;
    }
}

void LaunchSpring_Unknown4(void)
{
    RSDK_THIS(LaunchSpring);

    self->rotation = (self->rotation - self->rotSpeed) & 0x1FF;
    LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown5);
}

void LaunchSpring_Unknown5(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    player->position.x = self->position.x;
    player->position.y = self->position.y;
    player->position.x += RSDK.Cos512(self->rotation - 128) << 12;
    player->position.y += RSDK.Sin512(self->rotation - 128) << 12;
    if (!--self->timer) {
        RSDK.PlaySfx(LaunchSpring->sfxClack, false, 255);
        self->state = LaunchSpring_Unknown6;
    }
}

void LaunchSpring_Unknown6(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        self->rotation   = (self->rotation - self->rotSpeed) & 0x1FF;
        player->position.x = self->position.x;
        player->position.y = self->position.y;
        player->position.x += RSDK.Cos512(self->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(self->rotation - 128) << 12;
        if (player->jumpPress) {
            LaunchSpring_Unknown3();
            self->state = LaunchSpring_Unknown7;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_Unknown7;
    }
}

void LaunchSpring_Unknown7(void)
{
    RSDK_THIS(LaunchSpring);

    RSDK.ProcessAnimation(&self->animator3);
    if (self->animator3.frameID == self->animator3.frameCount - 1) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->animator3, true, 0);
        self->state = LaunchSpring_Unknown8;
    }
}

void LaunchSpring_Unknown8(void)
{
    RSDK_THIS(LaunchSpring);

    self->timer += 2;
    if (self->timer >= 47) {
        self->timer = 47;
        self->state = LaunchSpring_Unknown4;
    }
}

void LaunchSpring_Unknown9(void)
{
    RSDK_THIS(LaunchSpring);

    if (self->manual)
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown11);
    else
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown10);
}

void LaunchSpring_Unknown10(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        player->position.x = self->position.x;
        player->position.y = self->position.y;
        player->position.x += RSDK.Cos512(self->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(self->rotation - 128) << 12;
        if (++self->timer2 == 60) {
            self->timer2 = 0;
            LaunchSpring_Unknown3();
            self->state = LaunchSpring_Unknown12;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_Unknown12;
    }
}

void LaunchSpring_Unknown11(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        player->position.x = self->position.x;
        player->position.y = self->position.y;
        player->position.x += RSDK.Cos512(self->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(self->rotation - 128) << 12;
        if (player->jumpPress) {
            LaunchSpring_Unknown3();
            self->timer2 = 0;
            self->state  = LaunchSpring_Unknown12;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_Unknown12;
    }
}

void LaunchSpring_Unknown12(void)
{
    RSDK_THIS(LaunchSpring);

    if (LaunchSpring->flag) {
        if (self->timer2 >= 30) {
            self->state = LaunchSpring_Unknown9;
        }
        else {
            self->timer2++;
        }
    }
    else {
        RSDK.ProcessAnimation(&self->animator3);
        if (self->animator3.frameID == self->animator3.frameCount - 1) {
            RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->animator3, true, 0);
            self->animator3.frameID = 1;
            self->state             = LaunchSpring_Unknown9;
        }
    }
}

void LaunchSpring_Unknown13(void)
{
    RSDK_THIS(LaunchSpring);

    if (self->rotDir) {
        self->rotation -= self->rotSpeed;
        if (self->rotation <= self->field_B4) {
            self->rotDir   = 0;
            self->rotation = self->field_B4;
        }
    }
    else {
        self->rotation += self->rotSpeed;
        if (self->rotation >= self->field_B6) {
            self->rotDir   = 1;
            self->rotation = self->field_B6;
        }
    }

    if (self->manual)
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown15);
    else
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown14);
}

void LaunchSpring_Unknown14(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        if (self->strict)
            self->rotation = (self->rotation + 32) & 0xC0;
        player->position.x = self->position.x;
        player->position.y = self->position.y;
        player->position.x += RSDK.Cos512(self->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(self->rotation - 128) << 12;
        if (++self->timer2 == 60) {
            self->timer2 = 0;
            LaunchSpring_Unknown3();
            self->state = LaunchSpring_Unknown16;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_Unknown16;
    }
}

void LaunchSpring_Unknown15(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (player->state == Player_State_None) {
        if (self->rotDir) {
            self->rotation -= self->rotSpeed;
            if (self->rotation <= self->field_B4) {
                self->rotation = self->field_B4;
                self->rotDir   = 0;
            }
        }
        else {
            self->rotation += self->rotSpeed;
            if (self->rotation >= self->field_B6) {
                self->rotation = self->field_B6;
                self->rotDir   = 1;
            }
        }
        player->position.x = self->position.x;
        player->position.y = self->position.y;
        player->position.x += RSDK.Cos512(self->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(self->rotation - 128) << 12;
        if (player->jumpPress) {
            LaunchSpring_Unknown3();
            self->state = LaunchSpring_Unknown16;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_Unknown16;
    }
}

void LaunchSpring_Unknown16(void)
{
    RSDK_THIS(LaunchSpring);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->animator3.frameID == self->animator3.frameCount - 1) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->animator3, true, 0);
        self->animator3.frameID = 1;
        self->state             = LaunchSpring_Unknown13;
    }
}

#if RETRO_INCLUDE_EDITOR
void LaunchSpring_EditorDraw(void)
{
    RSDK_THIS(LaunchSpring);
    if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->animator3, true, 0);

        LaunchSpring_Draw();
    }
}

void LaunchSpring_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/LaunchSpring.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/LaunchSpring.bin", SCOPE_STAGE);
}
#endif

void LaunchSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, rotDir);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, rotSpeed);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_BOOL, manual);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_BOOL, strict);
}
