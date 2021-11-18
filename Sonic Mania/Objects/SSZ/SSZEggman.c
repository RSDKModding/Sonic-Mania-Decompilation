#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectSSZEggman *SSZEggman;

void SSZEggman_Update(void)
{
    RSDK_THIS(SSZEggman);
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator1);
}

void SSZEggman_LateUpdate(void) {}

void SSZEggman_StaticUpdate(void) {}

void SSZEggman_Draw(void)
{
    RSDK_THIS(SSZEggman);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->offset.x;
    drawPos.y += self->offset.y;

    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator4, NULL, false);
}

void SSZEggman_Create(void *data)
{
    RSDK_THIS(SSZEggman);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->drawFX        = FX_FLIP;
        self->alpha         = 64;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 2, &self->animator1, true, 0);
        if (data) {
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 6, &self->animator4, true, 0);
        }
        else {
            self->offset.y = -0x100000;
            self->state    = SSZEggman_Unknown1;
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 4, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 5, &self->animator3, true, 0);
        }
    }
}

void SSZEggman_StageLoad(void) { SSZEggman->aniFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanSSZ.bin", SCOPE_STAGE); }

void SSZEggman_Unknown1(void)
{
    RSDK_THIS(SSZEggman);

    foreach_all(PhantomRuby, ruby) { self->ruby = ruby; }
    self->rubyPos.x = self->position.x;
    self->rubyPos.y = self->position.y;
    self->state     = SSZEggman_Unknown2;
}

void SSZEggman_Unknown2(void)
{
    RSDK_THIS(SSZEggman);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->rubyPos.y) & 0xFFFF0000;

    if (self->ruby) {
        self->ruby->position.x = self->position.x - 0xC0000;
        self->ruby->position.y = self->position.y - 0x100000;
    }

    foreach_active(MetalSonic, metal)
    {
        if (metal->state == MetalSonic_State_ObtainRuby) {
            self->speed = -0x28000;
            RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 3, &self->animator1, true, 0);
            self->state = SSZEggman_Unknown3;
            if (self->ruby) {
                self->ruby->state      = PhantomRuby_Unknown6;
                self->ruby->velocity.x = -0x40000;
                self->ruby->velocity.y = -0x40000;
            }
        }
    }
}

void SSZEggman_Unknown3(void)
{
    RSDK_THIS(SSZEggman);

    self->speed += 0x3800;
    self->offset.y += self->speed;

    if (self->offset.y >= 0) {
        self->offset.y = 0;
        self->speed    = 0;
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 0, &self->animator1, true, 0);
        self->state = SSZEggman_Unknown4;
    }

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->rubyPos.y) & 0xFFFF0000;
}

void SSZEggman_Unknown4(void)
{
    RSDK_THIS(SSZEggman);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->rubyPos.y) & 0xFFFF0000;

    if (++self->timer == 120)
        RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 1, &self->animator1, true, 0);
    if (self->timer == 180) {
        self->timer     = 0;
        self->direction = FLIP_X;
        self->active    = ACTIVE_NORMAL;
        self->state     = SSZEggman_Unknown5;
    }

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->rubyPos.y) & 0xFFFF0000;
}

void SSZEggman_Unknown5(void)
{
    RSDK_THIS(SSZEggman);

    self->velocity.x += 0x1000;
    self->position.x += self->velocity.x;
    self->rubyPos.y -= 0x8000;
    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->rubyPos.y) & 0xFFFF0000;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void SSZEggman_EditorDraw(void)
{
    RSDK_THIS(SSZEggman);
    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 2, &self->animator1, true, 0);
    self->offset.y = -0x100000;
    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 4, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(SSZEggman->aniFrames, 5, &self->animator3, true, 0);

    SSZEggman_Draw();
}

void SSZEggman_EditorLoad(void) { SSZEggman->aniFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanSSZ.bin", SCOPE_STAGE); }
#endif

void SSZEggman_Serialize(void) {}
#endif
