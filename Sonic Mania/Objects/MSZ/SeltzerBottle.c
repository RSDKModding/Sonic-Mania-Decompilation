#include "SonicMania.h"

ObjectSeltzerBottle *SeltzerBottle;

void SeltzerBottle_Update(void)
{
    RSDK_THIS(SeltzerBottle);
    RSDK.ProcessAnimation(&self->animator3);

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &SeltzerBottle->hitbox1);
        if (Player_CheckCollisionPlatform(player, self, &SeltzerBottle->hitbox2) && !self->animator2.frameID) {
            self->animator2.frameID = 1;
            self->active       = ACTIVE_NORMAL;
            self->state             = SeltzerBottle_Unknown1;
            RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 6, &self->animator4, false, 0);
            player->groundVel = clampVal(player->groundVel, -0xC0000, 0xC0000);
            for (int32 p = 0; p < Player->playerCount; ++p) {
                RSDK_GET_ENTITY(p, Player)->collisionLayers |= SeltzerBottle->layerBitFlags;
            }
            RSDK.PlaySfx(SeltzerBottle->sfxSpray, 0, 255);
        }
    }
    StateMachine_Run(self->state);
}

void SeltzerBottle_LateUpdate(void) {}

void SeltzerBottle_StaticUpdate(void) {}

void SeltzerBottle_Draw(void)
{
    RSDK_THIS(SeltzerBottle);
    Vector2 drawPos;

    RSDK.DrawRect(self->position.x - 0x2E0000, self->position.y - self->field_64 + 0x2C0000, 0x5C0000, self->field_64, 0x00F0F0, 64, INK_SUB,
                  false);

    drawPos = self->position;
    drawPos.y += 0x2C0000 - self->field_64;
    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    self->animator1.frameID = 0;
    self->inkEffect         = INK_SUB;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 1;
    self->inkEffect         = INK_ADD;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    self->inkEffect         = INK_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 3;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 4;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 5;
    self->drawFX            = FX_FLIP;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void SeltzerBottle_Create(void *data)
{
    RSDK_THIS(SeltzerBottle);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 2, &self->animator3, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderHigh;
        self->alpha         = 255;
        self->field_60      = self->sprayTime;
        self->field_64      = 0x400000;
    }
}

void SeltzerBottle_StageLoad(void)
{
    SeltzerBottle->aniFrames      = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE);
    SeltzerBottle->hitbox1.left   = -48;
    SeltzerBottle->hitbox1.top    = -80;
    SeltzerBottle->hitbox1.right  = 48;
    SeltzerBottle->hitbox1.bottom = 64;
    SeltzerBottle->hitbox2.left   = -16;
    SeltzerBottle->hitbox2.top    = -90;
    SeltzerBottle->hitbox2.right  = 16;
    SeltzerBottle->hitbox2.bottom = SeltzerBottle->hitbox2.top + 16;
    SeltzerBottle->layerID        = RSDK.GetSceneLayerID("Seltzer Path");
    if (SeltzerBottle->layerID)
        SeltzerBottle->layerBitFlags = 1 << SeltzerBottle->layerID;
    SeltzerBottle->sfxSpray = RSDK.GetSFX("MSZ/Spray.wav");
}

void SeltzerBottle_Unknown1(void)
{
    RSDK_THIS(SeltzerBottle);
    int32 timer = ((self->field_60 << 14) / self->sprayTime) << 8;
    if (timer < 0x40000)
        timer = 0x40000;
    self->field_64 = timer;

    EntitySeltzerWater *spray = CREATE_ENTITY(SeltzerWater, NULL, self->position.x, self->position.y - 0x4C0000);
    if (self->direction == FLIP_X)
        spray->position.x += 0x300000;
    else
        spray->position.x -= 0x300000;
    spray->field_64  = RSDK.Rand(0xA00, 0xC00);
    spray->field_5C  = RSDK.Rand(0, 0x100);
    spray->drawOrder = Zone->playerDrawLow;
    spray->nodeSlot  = SceneInfo->entitySlot + 1;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        if (player->onGround) {
            self->position.x = player->position.x;
            self->position.y = player->position.y;
            self->position.x += RSDK.Sin256(player->angle) << 13;
            self->position.y += RSDK.Cos256(player->angle) << 13;
            if (RSDK.ObjectTileCollision(self, SeltzerBottle->layerBitFlags, player->collisionMode, 0, 0, 0, false)) {
                if (self->direction == FLIP_X) {
                    if (player->groundVel < 0x50000)
                        player->groundVel = 0x50000;
                }
                else if (player->groundVel > -0x50000) {
                    player->groundVel = -0x50000;
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
    RSDK.ProcessAnimation(&self->animator4);

    if (--self->field_60 <= 0) {
        for (int32 p = 0; p < Player->playerCount; ++p) {
            RSDK_GET_ENTITY(p, Player)->collisionLayers &= ~SeltzerBottle->layerBitFlags;
        }
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, false, 0);
        self->state = SeltzerBottle_Unknown2;
        foreach_active(SeltzerWater, water)
        {
            water->gravityStrength = RSDK.Rand(0x3800, 0x4000);
            water->state           = SeltzerWater_Unknown2;
        }
    }
}

void SeltzerBottle_Unknown2(void)
{
    RSDK_THIS(SeltzerBottle);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->state             = StateMachine_None;
        self->field_60          = self->sprayTime;
        self->field_64          = 0x400000;
        self->animator2.frameID = 0;
        self->active            = ACTIVE_BOUNDS;
    }
}

void SeltzerBottle_EditorDraw(void)
{
    RSDK_THIS(SeltzerBottle);
    RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 2, &self->animator3, true, 0);
    
    SeltzerBottle_Draw();
}

void SeltzerBottle_EditorLoad(void) { SeltzerBottle->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE); }

void SeltzerBottle_Serialize(void)
{
    RSDK_EDITABLE_VAR(SeltzerBottle, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SeltzerBottle, VAR_ENUM, sprayTime);
}
