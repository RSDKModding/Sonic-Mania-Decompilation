#include "SonicMania.h"

ObjectSentryBug *SentryBug;

void SentryBug_Update(void)
{
    RSDK_THIS(SentryBug);

    RSDK.ProcessAnimation(&self->animator2);
    StateMachine_Run(self->state);
    StateMachine_Run(self->stateLate);
}

void SentryBug_LateUpdate(void) {}

void SentryBug_StaticUpdate(void) {}

void SentryBug_Draw(void)
{
    RSDK_THIS(SentryBug);

    if (self->drawFlag) {
        RSDK.DrawSprite(&self->animator2, NULL, false);
        if (self->drawNet) {
            RSDK.MatrixScaleXYZ(&self->matrix1, self->netScale >> 4, self->netScale >> 4, self->netScale >> 4);
            RSDK.MatrixTranslateXYZ(&self->matrix1, self->netX, self->netY, 0, false);
            RSDK.MatrixRotateXYZ(&self->matrix2, self->netRotation, self->netRotation, 0);
            RSDK.MatrixMultiply(&self->matrix2, &self->matrix2, &self->matrix1);
            RSDK.Prepare3DScene(SentryBug->sceneIndex);
            RSDK.AddModelTo3DScene(SentryBug->meshFrames, SentryBug->sceneIndex, S3D_FLATCLR_WIREFRAME, &self->matrix2, 0, self->netColour);

            self->inkEffect = INK_ALPHA;
            RSDK.Draw3DScene(SentryBug->sceneIndex);
            self->inkEffect = INK_NONE;
        }

        for (int32 angle = 0, i = 0; angle < 0xFC; angle += 0x2A, ++i) {
            self->animator1.frameID = ((angle + self->netRotation + 8) >> 5) & 7;
            if (self->netColour == 0xF0F000)
                self->animator1.frameID += 8;
            RSDK.DrawSprite(&self->animator1, &self->positions[i], false);
        }
    }
    else {
        RSDK.DrawSprite(&self->animator1, self->positions, false);
        RSDK.DrawSprite(&self->animator1, &self->positions[1], false);
        RSDK.DrawSprite(&self->animator1, &self->positions[2], false);
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator1, &self->positions[3], false);
        RSDK.DrawSprite(&self->animator1, &self->positions[4], false);
        RSDK.DrawSprite(&self->animator1, &self->positions[5], false);
    }
}

void SentryBug_Create(void *data)
{
    RSDK_THIS(SentryBug);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->amplitude.x >>= 10;
        self->amplitude.y >>= 10;
        self->field_78  = self->position;
        self->field_80  = self->position;
        self->netColour = 0xF02000;
        self->alpha     = 256;
        if (!self->speed)
            self->speed = 4;

        if (self->amplitude.x >= 0)
            RSDK.SetSpriteAnimation(SentryBug->aniFrames, 1, &self->animator2, true, 6);
        else
            RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &self->animator2, true, 6);
        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 2, &self->animator1, true, 0);
        self->state     = SentryBug_State_Unknown1;
        self->stateLate = SentryBug_StateLate_Unknown1;
    }
}

void SentryBug_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        SentryBug->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/SentryBug.bin", SCOPE_STAGE);
    SentryBug->meshFrames    = RSDK.LoadMesh("TMZ/OrbNet.bin", SCOPE_STAGE);
    SentryBug->sceneIndex    = RSDK.Create3DScene("View:SentryBug", 512, SCOPE_STAGE);
    SentryBug->hitbox.left   = -12;
    SentryBug->hitbox.top    = -12;
    SentryBug->hitbox.right  = 12;
    SentryBug->hitbox.bottom = 12;
    SentryBug->sfxPon        = RSDK.GetSfx("Stage/Pon.wav");
    SentryBug->sfxSwarm      = RSDK.GetSfx("TMZ1/CyberSwarm.wav");

    DEBUGMODE_ADD_OBJ(SentryBug);
}

void SentryBug_DebugSpawn(void)
{
    RSDK_THIS(SentryBug);

    EntitySentryBug *sentryBug = CREATE_ENTITY(SentryBug, NULL, self->position.x, self->position.y);
    sentryBug->amplitude.x     = 0x1200;
    sentryBug->amplitude.y     = 0x400;
    sentryBug->timer           = 60;
}

void SentryBug_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void SentryBug_CheckPlayerCollisions(void)
{
    RSDK_THIS(SentryBug);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &SentryBug->hitbox)) {
            if (Player_CheckBadnikBreak(self, player, true))
                RSDK.StopSfx(SentryBug->sfxSwarm);
        }
    }
}

void SentryBug_StateLate_Unknown1(void)
{
    RSDK_THIS(SentryBug);

    int32 *posPtrs = NULL;
    if (self->animator2.animationID == 1)
        posPtrs = &SentryBug->value1[12 * self->animator2.frameID];
    else
        posPtrs = &SentryBug->value1[-12 * self->animator2.frameID + 72];

    int32 pos = 0;
    for (int32 i = 0; i < 6; ++i) {
        self->positions[i].x = self->position.x + posPtrs[pos + 0];
        self->positions[i].y = self->position.y + posPtrs[pos + 1];

        pos += 2;
    }
}

void SentryBug_SetupPositions2(void)
{
    RSDK_THIS(SentryBug);

    int32 *posPtrs = NULL;
    if (self->animator2.animationID == 1)
        posPtrs = SentryBug->value2;
    else
        posPtrs = SentryBug->value3;

    int32 pos = 0;
    for (int32 i = 0; i < 6; ++i) {
        self->positions2[i].x = posPtrs[pos + 0];
        self->positions2[i].y = posPtrs[pos + 1];

        pos += 2;
    }
}

void SentryBug_StateLate_Unknown2(void)
{
    RSDK_THIS(SentryBug);

    for (int32 i = 0; i < 6; ++i) {
        self->positions2[i].y += 0x2800;
        self->positions[i].x += self->positions2[i].x;
        self->positions[i].y += self->positions2[i].y;
    }
}

void SentryBug_StateLate_Unknown3(void)
{
    RSDK_THIS(SentryBug);

    int32 angle = self->netRotation;
    for (int32 i = 0; i < 6; ++i) {
        int32 x = self->netScale * RSDK.Cos256(angle) + self->netX;
        int32 y = self->netScale * RSDK.Sin256(angle) + self->netY;

        self->positions[i + 6].x = (x - self->positions[i].x) >> 3;
        self->positions[i + 6].y = (y - self->positions[i].y) >> 3;
        self->positions[i].x += self->positions[i + 6].x;
        self->positions[i].y += self->positions[i + 6].y;
        angle += 42;
    }
    self->netRotation += 4;
}

void SentryBug_StateLate_Unknown4(void)
{
    RSDK_THIS(SentryBug);

    int32 angle = self->netRotation;
    for (int32 i = 0; i < 6; ++i) {
        self->positions[i].x = self->netScale * RSDK.Cos256(angle) + self->netX;
        self->positions[i].y = self->netScale * RSDK.Sin256(angle) + self->netY;
        angle += 42;
    }

    self->netRotation += 4;
}

void SentryBug_StateLate_Unknown5(void)
{
    RSDK_THIS(SentryBug);

    int32 *posPtr = NULL;
    if (self->animator2.animationID == 1)
        posPtr = &SentryBug->value1[12 * self->animator2.frameID];
    else
        posPtr = &SentryBug->value1[-12 * self->animator2.frameID + 72];

    for (int32 i = 0; i < 6; ++i) {
        self->positions2[i].x = clampVal((self->position.x + posPtr[0] - self->positions[i].x) >> 3, -0xC0000, 0xC0000);
        self->positions2[i].y = clampVal((self->position.y + posPtr[1] - self->positions[i].y) >> 3, -0xC0000, 0xC0000);

        self->positions[i].x += self->positions2[i].x;
        self->positions[i].y += self->positions2[i].y;
    }
}

void SentryBug_State_Unknown1(void)
{
    RSDK_THIS(SentryBug);

    int32 x = self->position.x;

    self->angle      = (self->angle + self->speed) & 0x3FF;
    self->position.x = self->amplitude.x * RSDK.Sin1024(self->angle) + self->field_78.x;
    self->position.y = self->amplitude.y * RSDK.Sin1024(2 * self->angle) + self->field_78.y;
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, (x - self->position.x) < 0, &self->animator2, false, 0);

    if (self->timer > 0)
        self->timer--;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        if (!self->timer) {
            int32 rx = (self->position.x - player->position.x) >> 16;
            int32 ry = (self->position.y - player->position.y) >> 16;
            if (rx * rx + ry * ry < 0x2400) {
                self->field_78.x = self->position.x;
                self->field_78.y = self->position.y;
                self->angle      = 0;
                self->playerPtr  = player;
                self->netScale   = 0x6800;
                self->drawOrder  = Zone->drawOrderHigh;
                self->active     = ACTIVE_NORMAL;
                RSDK.PlaySfx(SentryBug->sfxPon, false, 255);
                SentryBug_SetupPositions2();
                self->stateLate = SentryBug_StateLate_Unknown2;
                RSDK.PlaySfx(SentryBug->sfxSwarm, false, 255);
                self->state = SentryBug_State_Unknown2;
            }
        }
    }
    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_Unknown2(void)
{
    RSDK_THIS(SentryBug);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    self->angle      = (self->angle + 4) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_78.y) & 0xFFFF0000;

    self->netX = player->position.x;
    self->netY = player->position.y;
    if (++self->timer == 120) {
        self->timer     = 0;
        self->netScale  = 0x6000;
        self->stateLate = SentryBug_StateLate_Unknown4;
        self->drawNet   = true;
        self->state     = SentryBug_State_Unknown3;
    }
    if (self->timer == 30) {
        self->drawFlag  = true;
        self->stateLate = SentryBug_StateLate_Unknown3;
    }
    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_Unknown3(void)
{
    RSDK_THIS(SentryBug);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    self->angle      = (self->angle + 4) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_78.y) & 0xFFFF0000;

    self->netX += (player->position.x - self->netX) >> 3;
    self->netY += ((player->position.y - self->netY) >> 3);
    SentryBug_CheckPlayerCollisions();

    self->alpha = (RSDK.Sin256(4 * Zone->timer) >> 2) + 160;
    if (++self->timer == 60) {
        self->timer = 0;
        self->state = SentryBug_State_Unknown4;
    }
}

void SentryBug_State_Unknown4(void)
{
    RSDK_THIS(SentryBug);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    self->angle      = (self->angle + 4) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_78.y) & 0xFFFF0000;
    self->netX += (player->position.x - self->netX) >> 3;
    self->netY += ((player->position.y - self->netY) >> 3);

    int32 rx        = (self->netX - self->position.x) >> 16;
    int32 ry        = (self->netY - self->position.y) >> 16;
    self->alpha = (RSDK.Sin256(4 * Zone->timer) >> 2) + 160;

    if (rx * rx + ry * ry <= 0x40000) {
        if (self->netScale <= 0x1800) {
            int32 rx = (self->netX - player->position.x) >> 16;
            int32 ry = (self->netY - player->position.y) >> 16;
            if (rx * rx + ry * ry < 512 && Player_CheckValidState(player)) {
                self->drawNet     = true;
                self->alpha       = 256;
                self->state       = SentryBug_State_Unknown5;
                self->isPermanent = true;
            }
            else {
                self->animator1.frameID = 0;
                self->drawFlag          = false;
                self->stateLate         = SentryBug_StateLate_Unknown5;
                self->state             = SentryBug_State_Unknown6;
                RSDK.StopSfx(SentryBug->sfxSwarm);
            }
        }
        else {
            self->netScale -= 0x80;
        }
        SentryBug_CheckPlayerCollisions();
    }
    else {
        self->animator1.frameID = 0;
        self->drawFlag          = false;
        self->stateLate         = SentryBug_StateLate_Unknown5;
        self->state             = SentryBug_State_Unknown6;
        RSDK.StopSfx(SentryBug->sfxSwarm);
    }
}

void SentryBug_State_Unknown5(void)
{
    RSDK_THIS(SentryBug);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    self->angle      = (self->angle + 4) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_78.y) & 0xFFFF0000;
    self->netX       = player->position.x;
    self->netY       = player->position.y;
    player->groundVel >>= 2;
    player->velocity.x >>= 2;
    player->velocity.y >>= 2;
    if (Zone->timer & 2)
        self->netColour = 0xF0F000;
    else
        self->netColour = 0xF02000;
    if (++self->timer == 30) {
        self->timer = 0;
        Player_CheckHit(player, self);
        self->animator1.frameID = 0;
        self->netColour         = 0xF02000;
        self->drawFlag          = false;
        self->stateLate         = SentryBug_StateLate_Unknown5;
        self->state             = SentryBug_State_Unknown6;
        RSDK.StopSfx(SentryBug->sfxSwarm);
        self->isPermanent = false;
    }
    SentryBug_CheckPlayerCollisions();
}

void SentryBug_State_Unknown6(void)
{
    RSDK_THIS(SentryBug);

    if (self->alpha > 0)
        self->alpha -= 64;
    self->velocity.x = (self->field_80.x - self->position.x) >> 4;
    self->velocity.y = (self->field_80.y - self->position.y) >> 4;
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, self->velocity.x > 0, &self->animator2, false, 0);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (++self->timer == 60) {
        self->field_78.x = self->field_80.x;
        self->field_78.y = self->field_80.y;
        self->angle      = 0;
        self->timer      = 60;
        self->active     = ACTIVE_BOUNDS;
        self->drawOrder  = Zone->drawOrderLow;
        self->stateLate  = SentryBug_StateLate_Unknown1;
        self->state      = SentryBug_State_Unknown1;
    }
    SentryBug_CheckPlayerCollisions();
}

#if RETRO_INCLUDE_EDITOR
void SentryBug_EditorDraw(void)
{
    RSDK_THIS(SentryBug);

    self->field_78 = self->position;
    self->field_80 = self->position;

    if (self->amplitude.x >= 0)
        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 1, &self->animator2, true, 6);
    else
        RSDK.SetSpriteAnimation(SentryBug->aniFrames, 0, &self->animator2, true, 6);
    RSDK.SetSpriteAnimation(SentryBug->aniFrames, 2, &self->animator1, true, 0);

    SentryBug_Draw();
}

void SentryBug_EditorLoad(void) { SentryBug->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/SentryBug.bin", SCOPE_STAGE); }
#endif

void SentryBug_Serialize(void)
{
    RSDK_EDITABLE_VAR(SentryBug, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(SentryBug, VAR_VECTOR2, amplitude);
}
