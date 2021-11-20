#include "SonicMania.h"

ObjectNewspaper *Newspaper;

void Newspaper_Update(void)
{
    RSDK_THIS(Newspaper);
#if RETRO_USE_PLUS
    StateMachine_Run(self->state);
#else
    Platform_Update();
#endif
}

void Newspaper_LateUpdate(void) {}

void Newspaper_StaticUpdate(void) {}

void Newspaper_Draw(void)
{
    RSDK_THIS(Newspaper);
#if RETRO_USE_PLUS
    RSDK.DrawSprite(&self->animator, NULL, false);
#else
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
#endif
}

void Newspaper_Create(void *data)
{
    RSDK_THIS(Newspaper);
#if RETRO_USE_PLUS
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Newspaper->aniFrames, 1, &self->animator, true, self->type);
        switch (self->type) {
            case 0:
            case 1:
                self->state     = Newspaper_HandleInteractions;
                self->drawOrder = Zone->playerDrawLow;
                break;
            case 2:
            case 3: self->drawOrder = Zone->drawOrderLow; break;
            default: break;
        }

        self->hitbox.left   = -16;
        self->hitbox.top    = -16;
        self->hitbox.right  = 16;
        self->hitbox.bottom = 16;
    }
#else
    self->collision = PLATFORM_C_SOLID_ALL;
    self->type      = PLATFORM_CONTROLLED;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Newspaper->aniFrames, 0, &self->animator, true, 0);
#endif
}

void Newspaper_StageLoad(void)
{
#if RETRO_USE_PLUS
    Newspaper->aniFrames     = RSDK.LoadSpriteAnimation("PSZ1/Newspaper.bin", SCOPE_STAGE);
    Newspaper->sfxPaperStack = RSDK.GetSfx("PSZ/PaperStack.wav");
#else
    if (RSDK.CheckStageFolder("PSZ1"))
        Newspaper->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Newspaper.bin", SCOPE_STAGE);
#endif
}

#if RETRO_USE_PLUS
void Newspaper_HandleInteractions(void)
{
    RSDK_THIS(Newspaper);

    if (self->timer <= 0) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, NULL, self->position.x + RSDK.Rand(-0x80000, 0x80000), self->position.y + RSDK.Rand(-0x80000, 0x80000));
                debris->state      = Debris_State_Fall;
                debris->gravity    = 0x400;
                debris->velocity.x = RSDK.Rand(0, 0x20000);
                debris->timer      = 240;
                self->timer      = 2;
                if (debris->position.x < self->position.x)
                    debris->velocity.x = -debris->velocity.x;
                debris->drawOrder = Zone->drawOrderLow;
                RSDK.SetSpriteAnimation(Newspaper->aniFrames, RSDK.Rand(0, 2) + 2, &debris->animator, true, RSDK.Rand(0, 6));

                if (Newspaper->sfxPaperStack) {
                    RSDK.PlaySfx(Newspaper->sfxPaperStack, false, 255);
                    Newspaper->sfxPaperStack = 0;
                }
            }
        }
    }
    else {
        self->timer--;
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void Newspaper_EditorDraw(void) {}

void Newspaper_EditorLoad(void) {}
#endif

void Newspaper_Serialize(void)
{
#if RETRO_USE_PLUS
    RSDK_EDITABLE_VAR(Newspaper, VAR_UINT8, type);
#else
    RSDK_EDITABLE_VAR(Newspaper, VAR_ENUM, node);
    RSDK_EDITABLE_VAR(Newspaper, VAR_ENUM, childCount);
#endif
}
