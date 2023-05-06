// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Kabasira Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectKabasira *Kabasira;

void Kabasira_Update(void)
{
    RSDK_THIS(Kabasira);

    StateMachine_Run(self->state);
}

void Kabasira_LateUpdate(void) {}

void Kabasira_StaticUpdate(void) {}

void Kabasira_Draw(void)
{
    RSDK_THIS(Kabasira);

    if (self->state == Kabasira_State_LaunchedAttack) {
        self->drawFX = FX_SCALE | FX_FLIP;
        RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
        RSDK.DrawSprite(&self->wingsAnimator, NULL, false);

        // Probably shouldn't be in Draw?
        RSDK.ProcessAnimation(&self->wingsAnimator);
    }
    else {
        int32 speed = (2 * (self->direction == FLIP_NONE) - 1);

        int32 angle = self->angle + 72 * speed;
        if (angle < 0)
            angle = ((-1 - angle) & -0x200) + angle + 0x200;
        angle &= 0x1FF;

        int32 alpha    = 0x100 / KABASIRA_BODY_COUNT; // default is 0x40
        int32 angleVel = 24 * speed;

        for (int32 i = 0; i < KABASIRA_BODY_COUNT - 1; ++i) {
            Kabasira_DrawSegment(angle, alpha);

            angle -= angleVel;
            if (angle < 0)
                angle = ((-1 - angle) & -0x200) + angle + 0x200;
            angle &= 0x1FF;

            alpha += 0x100 / KABASIRA_BODY_COUNT; // default is 0x40
        }

        Kabasira_DrawSegment(angle, alpha);
        self->angle = angle;
    }
}

void Kabasira_Create(void *data)
{
    RSDK_THIS(Kabasira);

    self->inkEffect = INK_ALPHA;
    self->drawFX    = FX_FLIP | FX_ROTATE | FX_SCALE;
    self->scale.x   = 0x200;
    self->scale.y   = 0x200;

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->startPos      = self->position;
        self->startDir      = self->direction;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(Kabasira->aniFrames, 0, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(Kabasira->aniFrames, 1, &self->wingsAnimator, true, 0);
        self->wingsAnimator.frameCount = 3;

        if (data) {
            self->alpha = 0x80;
            self->state = Kabasira_State_LaunchedAttack;
        }
        else {
            self->wingsAnimator.frameCount = 3;
            self->wingsAnimator.loopIndex  = 0;

            self->state = Kabasira_State_Init;
        }
    }
}

void Kabasira_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        Kabasira->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Kabasira.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        Kabasira->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Kabasira.bin", SCOPE_STAGE);

    Kabasira->hitboxBadnik.left   = -6;
    Kabasira->hitboxBadnik.top    = -6;
    Kabasira->hitboxBadnik.right  = 6;
    Kabasira->hitboxBadnik.bottom = 6;

    Kabasira->onScreenRange.x = 0x60000;
    Kabasira->onScreenRange.y = 0x60000;

    Kabasira->sfxPon        = RSDK.GetSfx("Stage/Pon.wav");
    Kabasira->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");

    DEBUGMODE_ADD_OBJ(Kabasira);
}

void Kabasira_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Kabasira, NULL, self->position.x, self->position.y);
}

void Kabasira_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Kabasira->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

bool32 Kabasira_HandleAnimations(uint8 angle)
{
    RSDK_THIS(Kabasira);

    int32 rotation  = 2 * angle;
    int32 prevFrame = self->wingsAnimator.frameID % 3;
    int32 frame     = 0;

    switch (rotation >> 7) {
        case 0:
        case 2: frame = (rotation >> 5) & 3; break;

        case 1:
        case 3: frame = 3 - ((rotation >> 5) & 3); break;
    }

    int32 frameCount               = 3 * frame;
    self->wingsAnimator.loopIndex  = frameCount;
    self->wingsAnimator.frameID    = prevFrame + frameCount;
    self->wingsAnimator.frameCount = frameCount + 3;

    return rotation > 0x80 && rotation < 0x180;
}

void Kabasira_DrawSegment(int32 angle, int32 alpha)
{
    RSDK_THIS(Kabasira);
    Vector2 drawPos;

    int32 rotation = 2 * angle;
    drawPos.x      = RSDK.Sin512(rotation) * 0xA00 + self->startPos.x;
    drawPos.y      = (RSDK.Cos512(angle) << 13) + self->startPos.y;

    int32 frame = 11 - rotation % 512 / 42 % 12;
    RSDK.SetSpriteAnimation(Kabasira->aniFrames, 0, &self->bodyAnimator, true, frame);

    if (!self->bodyAnimator.frameID || self->bodyAnimator.frameID > 6) {
        self->scale.x = 0x200;
        self->scale.y = 0x200;
    }
    else {
        self->scale.x = ((abs(2 * self->bodyAnimator.frameID - 7) << 5) / 5) + 480;
        self->scale.y = self->scale.x;
    }

    uint8 dir         = Kabasira_HandleAnimations(angle) ? FLIP_X : FLIP_NONE;
    int32 storedDir   = self->direction;
    int32 storedAlpha = self->alpha;

    if (self->angle < 0x100) {
        self->direction = FLIP_NONE;
        self->alpha     = alpha;
        RSDK.DrawSprite(&self->bodyAnimator, &drawPos, false);

        self->direction = dir;
        alpha >>= 1;
        self->alpha = alpha;
        RSDK.DrawSprite(&self->wingsAnimator, &drawPos, false);
    }
    else {
        self->direction = dir;
        self->alpha     = alpha >> 1;
        RSDK.DrawSprite(&self->wingsAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        self->alpha     = alpha;
        RSDK.DrawSprite(&self->bodyAnimator, &drawPos, false);
    }

    self->alpha     = storedAlpha;
    self->direction = storedDir;
}

void Kabasira_CheckPlayerCollisions(void)
{
    RSDK_THIS(Kabasira);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Kabasira->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Kabasira_CheckOffScreen(void)
{
    RSDK_THIS(Kabasira);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Kabasira_Create(NULL);
    }
}

void Kabasira_State_Init(void)
{
    RSDK_THIS(Kabasira);

    self->active = ACTIVE_NORMAL;
    self->angle  = 0;
    self->timer  = 0;

    self->state = Kabasira_State_Moving;
    Kabasira_State_Moving();
}

void Kabasira_State_Moving(void)
{
    RSDK_THIS(Kabasira);

    RSDK.ProcessAnimation(&self->wingsAnimator);

    self->angle += 2;
    if (self->timer > 0) {
        self->timer--;
    }
    else {
        self->timer = 240;
        if (RSDK.CheckOnScreen(self, &Kabasira->onScreenRange)) {
            EntityPlayer *player = Player_GetNearestPlayer();

            if (player) {
                RSDK.PlaySfx(Kabasira->sfxPon, false, 255);

                EntityKabasira *attack = CREATE_ENTITY(Kabasira, INT_TO_VOID(true), self->position.x, self->position.y);
                int32 angle            = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
                attack->velocity.x     = RSDK.Cos256(angle) << 9;
                attack->velocity.y     = RSDK.Sin256(angle) << 9;
                attack->direction      = player->position.x < self->position.x;
            }
        }
    }

    self->position.x = self->startPos.x + (0xA00 * RSDK.Sin512(2 * self->angle));
    self->position.y = self->startPos.y + (RSDK.Cos512(self->angle) << 13);

    Kabasira_CheckPlayerCollisions();
    Kabasira_CheckOffScreen();
}

void Kabasira_State_LaunchedAttack(void)
{
    RSDK_THIS(Kabasira);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Kabasira->hitboxBadnik)) {
            if (Player_CheckAttacking(player, self)) {
                CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ITEMBOX), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
                RSDK.PlaySfx(Kabasira->sfxExplosion2, false, 255);

                destroyEntity(self);
                foreach_break;
            }
            else {
                Player_Hurt(player, self);
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Kabasira_EditorDraw(void)
{
    RSDK_THIS(Kabasira);

    RSDK.SetSpriteAnimation(Kabasira->aniFrames, 0, &self->bodyAnimator, false, 0);
    RSDK.SetSpriteAnimation(Kabasira->aniFrames, 1, &self->wingsAnimator, false, 0);
    self->startPos = self->position;
    self->startDir = self->direction;

    Kabasira_Draw();
}

void Kabasira_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        Kabasira->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Kabasira.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        Kabasira->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Kabasira.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Kabasira, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Kabasira_Serialize(void) { RSDK_EDITABLE_VAR(Kabasira, VAR_UINT8, direction); }
