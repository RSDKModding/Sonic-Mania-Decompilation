#include "SonicMania.h"

ObjectReagent *Reagent;

void Reagent_Update(void)
{
    RSDK_THIS(Reagent);
    StateMachine_Run(self->state);
}

void Reagent_LateUpdate(void) {}

void Reagent_StaticUpdate(void) {}

void Reagent_Draw(void)
{
    RSDK_THIS(Reagent);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Reagent_Create(void *data)
{
    RSDK_THIS(Reagent);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x2000000;
        self->inkEffect     = INK_ALPHA;
        self->alpha         = 0xC0;
#if RETRO_USE_PLUS
        self->velocity.x = RSDK.RandSeeded(-0xC000, 0xC000, &Zone->randSeed);
#else
        self->velocity.x = RSDK.Rand(-0xC000, 0xC000);
#endif
        self->type          = voidToInt(data);
        self->state         = Reagent_Unknown1;

        switch (self->type) {
            case 0:
                self->r = 0;
                self->g = 8;
                self->b = 192;
                break;
            case 1:
                self->r = 24;
                self->g = 144;
                self->b = 0;
                break;
            case 2:
                self->r = 0;
                self->g = 128;
                self->b = 176;
                break;
        }
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(Reagent->aniFrames, self->type + 1, &self->animator, true, RSDK.RandSeeded(0, 2, &Zone->randSeed));
#else
        RSDK.SetSpriteAnimation(Reagent->aniFrames, self->type + 1, &self->animator, true, RSDK.Rand(0, 2));
#endif
    }
}

void Reagent_StageLoad(void)
{
    Reagent->aniFrames     = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE);
    Reagent->hitbox.left   = -2;
    Reagent->hitbox.top    = -2;
    Reagent->hitbox.right  = 2;
    Reagent->hitbox.bottom = 2;
    Reagent->sfxLand       = RSDK.GetSfx("Puyo/Land.wav");
}

void Reagent_Unknown1(void)
{
    RSDK_THIS(Reagent);
    self->position.x += self->velocity.x;
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    foreach_all(ChemicalPool, chemPool)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &Reagent->hitbox, chemPool, &chemPool->hitbox)) {
            self->velocity.y >>= 2;
            self->startX   = self->position.x;
            self->state    = Reagent_Unknown2;
            chemPool->active = ACTIVE_NORMAL;
            ChemicalPool_ChangeState(chemPool, self->type, self->r, self->g, self->b);
            RSDK.PlaySfx(Reagent->sfxLand, 0, 255);
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Reagent_Unknown2(void)
{
    RSDK_THIS(Reagent);
    self->velocity.y -= 0x1800;
    self->alpha -= 4;
    ++self->timer;
    self->position.y += self->velocity.y;
    self->position.x = (RSDK.Sin256(self->timer) << 10) + self->startX;
    if (self->timer == 64)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void Reagent_EditorDraw(void)
{
    RSDK_THIS(Reagent);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x2000000;
    self->inkEffect     = INK_ALPHA;
    self->alpha         = 0xC0;

    switch (self->type) {
        case 0:
            self->r = 0;
            self->g = 8;
            self->b = 192;
            break;
        case 1:
            self->r = 24;
            self->g = 144;
            self->b = 0;
            break;
        case 2:
            self->r = 0;
            self->g = 128;
            self->b = 176;
            break;
    }
    RSDK.SetSpriteAnimation(Reagent->aniFrames, self->type + 1, &self->animator, true, 0);

    Reagent_Draw();
}

void Reagent_EditorLoad(void) { Reagent->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Syringe.bin", SCOPE_STAGE); }
#endif

void Reagent_Serialize(void) {}
