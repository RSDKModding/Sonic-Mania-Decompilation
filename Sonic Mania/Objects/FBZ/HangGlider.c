#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectHangGlider *HangGlider;

void HangGlider_Update(void)
{
    RSDK_THIS(HangGlider);
    StateMachine_Run(self->state);
}

void HangGlider_LateUpdate(void) {}

void HangGlider_StaticUpdate(void) {}

void HangGlider_Draw(void)
{
    RSDK_THIS(HangGlider);
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        RSDK.DrawSprite(&self->animator1, NULL, false);
        RSDK.DrawSprite(&self->animator2, NULL, false);
        if (self->playerPtr)
            RSDK.DrawSprite(&self->animator3, NULL, false);
        RSDK.AddDrawListRef(Zone->drawOrderHigh, SceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&self->animator4, NULL, false);
    }
}

void HangGlider_Create(void *data)
{
    RSDK_THIS(HangGlider);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->field_60      = 4096;
        self->state         = HangGlider_Unknown1;
        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 1, &self->animator4, true, 1);
    }
}

void HangGlider_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        HangGlider->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangGlider.bin", SCOPE_STAGE);
    else
        HangGlider->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/HangGlider.bin", SCOPE_STAGE);

    HangGlider->hitbox.left   = 12;
    HangGlider->hitbox.top    = 8;
    HangGlider->hitbox.right  = 24;
    HangGlider->hitbox.bottom = 128;
}

void HangGlider_Unknown1(void)
{
    RSDK_THIS(HangGlider);
    foreach_active(Player, player)
    {
        if (abs(player->position.x - self->position.x) < 0x40000) {
            if (abs(player->position.y - self->position.y) < 0x140000) {
                self->playerPtr  = (Entity *)player;
                player->active     = ACTIVE_NEVER;
                player->visible    = 0;
                self->velocity.x = (192 * player->velocity.x) >> 8;
                RSDK.SetSpriteAnimation(player->aniFrames, 12, &self->animator3, true, 0);
                self->animator3.rotationFlag = 1;
                self->rotation               = 128;
                self->drawFX                 = FX_ROTATE;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                self->state = HangGlider_Unknown2;
            }
        }
    }
}
void HangGlider_Unknown2(void)
{
    RSDK_THIS(HangGlider);

    self->velocity.y += self->field_60;
    if (self->velocity.y > 0x10000)
        self->velocity.y = 0x10000;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    Entity *player = self->playerPtr;
    if (player) {
        player->position.x = self->position.x;
        player->position.y = self->position.y;
    }
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator3);
}

#if RETRO_INCLUDE_EDITOR
void HangGlider_EditorDraw(void) {}

void HangGlider_EditorLoad(void) {}
#endif

void HangGlider_Serialize(void) {}
#endif
