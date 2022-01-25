// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSPanel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMSPanel *MSPanel;

void MSPanel_Update(void)
{
    RSDK_THIS(MSPanel);
    StateMachine_Run(self->state);
}

void MSPanel_LateUpdate(void) {}

void MSPanel_StaticUpdate(void) {}

void MSPanel_Draw(void)
{
    RSDK_THIS(MSPanel);

    if (MetalSonic->field_8 & 1) {
        RSDK.CopyPalette(2, 32, 0, 32, 16);
        RSDK.CopyPalette(2, 160, 0, 160, 16);
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator1, NULL, false);
        RSDK.CopyPalette(1, 32, 0, 32, 16);
        RSDK.CopyPalette(1, 160, 0, 160, 16);
    }
    else {
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void MSPanel_Create(void *data)
{
    RSDK_THIS(MSPanel);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(MSPanel->aniFrames, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(MSPanel->aniFrames, 0, &self->animator2, true, 0);
        self->state     = MSPanel_Unknown1;
        self->drawOrder = Zone->drawOrderLow;
    }
}

void MSPanel_StageLoad(void) { MSPanel->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSPanel.bin", SCOPE_STAGE); }

void MSPanel_Unknown1(void)
{
    RSDK_THIS(MSPanel);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > self->position.x && player1->position.y < self->position.y + 0xE80000) {

        foreach_active(MetalSonic, metal)
        {
            if (abs(self->position.x - metal->position.x) < 0x400000 && metal->position.y - self->position.y < 0x1000000) {
                metal->panel     = (Entity *)self;
                metal->state     = MetalSonic_State_EnterPanel;
                metal->angle     = 0;
                metal->targetPos = self->position;
                metal->targetPos.y += 0x240000;
                metal->timer = 0;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &metal->animator, false, 0);
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &metal->animator2, false, 0);
                self->state = StateMachine_None;
            }
        }
    }
}

void MSPanel_Unknown2(void)
{
    RSDK_THIS(MSPanel);
    RSDK.ProcessAnimation(&self->animator1);
}

void MSPanel_Unknown3(void)
{
    RSDK_THIS(MSPanel);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);
        if (!(Zone->timer & 4)) {
            int x = self->position.x + RSDK.Rand(-0x300000, 0x300000);
            int y = self->position.y + RSDK.Rand(-0x100000, 0x400000);

            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }
}

void MSPanel_Unknown4(void)
{
    RSDK_THIS(MSPanel);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);
        if (!(Zone->timer & 4)) {
            int x                      = self->position.x + RSDK.Rand(-0x600000, 0x600000);
            int y                      = self->position.y + RSDK.Rand(-0x200000, 0x600000);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    if (!(Zone->timer & 0xF)) {
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySfx(MetalSonic->sfxRumble, false, 255);
    }
}

#if RETRO_INCLUDE_EDITOR
void MSPanel_EditorDraw(void)
{
    RSDK_THIS(MSPanel);

    RSDK.SetSpriteAnimation(MSPanel->aniFrames, 1, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(MSPanel->aniFrames, 0, &self->animator2, false, 0);

    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void MSPanel_EditorLoad(void) { MSPanel->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSPanel.bin", SCOPE_STAGE); }
#endif

void MSPanel_Serialize(void) {}
