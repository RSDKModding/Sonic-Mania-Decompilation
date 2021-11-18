#include "SonicMania.h"

ObjectStegway *Stegway = NULL;

void Stegway_Update(void)
{
    RSDK_THIS(Stegway);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    if (self->dustFlag)
        RSDK.ProcessAnimation(&self->animator3);

    StateMachine_Run(self->state);

    Stegway_HandlePlayerInteractions();
    if (self->state != Stegway_State_Setup)
        Stegway_CheckOnScreen();
}

void Stegway_LateUpdate(void) {}

void Stegway_StaticUpdate(void) {}

void Stegway_Draw(void)
{
    RSDK_THIS(Stegway);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    if (self->dustFlag)
        RSDK.DrawSprite(&self->animator3, NULL, false);
}

void Stegway_Create(void *data)
{
    RSDK_THIS(Stegway);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Stegway_State_Setup;
}

void Stegway_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HPZ"))
        Stegway->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Stegway.bin", SCOPE_STAGE);
    Stegway->hitboxBadnik.left   = -20;
    Stegway->hitboxBadnik.top    = -14;
    Stegway->hitboxBadnik.right  = 20;
    Stegway->hitboxBadnik.bottom = 14;
    Stegway->hitbox2.left        = -96;
    Stegway->hitbox2.top         = -32;
    Stegway->hitbox2.right       = 0;
    Stegway->hitbox2.bottom      = 14;
    Stegway->sfxRev              = RSDK.GetSFX("Stage/Rev.wav");
    Stegway->sfxRelease          = RSDK.GetSFX("Global/Release.wav");
    DEBUGMODE_ADD_OBJ(Stegway);
}

void Stegway_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityStegway *stegway = CREATE_ENTITY(Stegway, NULL, self->position.x, self->position.y);
    stegway->direction     = self->direction;
    stegway->startDir      = self->direction;
}

void Stegway_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Stegway_CheckOnScreen(void)
{
    RSDK_THIS(Stegway);
    if (!RSDK.CheckOnScreen(self, 0) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->direction = self->startDir;
        self->position  = self->startPos;
        Stegway_Create(NULL);
    }
}

void Stegway_HandlePlayerInteractions(void)
{
    RSDK_THIS(Stegway);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Stegway->hitboxBadnik))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Stegway_SetupAnims(char type, bool32 force)
{
    RSDK_THIS(Stegway);
    switch (type) {
        case 0:
            if (force || self->animator1.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &self->animator1, true, 0);
            self->animator1.animationSpeed = 1;
            self->animator2.animationSpeed = 6;
            break;
        case 1:
            if (force || self->animator1.animationID != 1)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 1, &self->animator1, true, 0);
            self->animator2.animationSpeed = 16;
            break;
        case 2:
            if (force || self->animator1.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &self->animator1, true, 0);
            if (self->animator1.frameID == 2)
                self->animator1.frameID = 1;
            self->animator1.animationSpeed = 0;
            self->animator2.animationSpeed = 24;
            break;
        case 3:
            if (force || self->animator1.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &self->animator1, true, 0);
            if (self->animator1.frameID == 2)
                self->animator1.frameID = 1;
            self->animator1.animationSpeed = 0;
            self->animator2.animationSpeed = 0;
            break;
        default: break;
    }
}

void Stegway_State_Setup(void)
{
    RSDK_THIS(Stegway);

    self->active = ACTIVE_NORMAL;
    if (self->direction == FLIP_NONE)
        self->velocity.x = -0x4000;
    else
        self->velocity.x = 0x4000;
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 2, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 3, &self->animator3, true, 0);
    Stegway_SetupAnims(0, true);
    self->state = Stegway_Unknown6;
    Stegway_Unknown6();
}

void Stegway_Unknown6(void)
{
    RSDK_THIS(Stegway);

    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Stegway->hitbox2)) {
            self->state = Stegway_Unknown8;
            Stegway_SetupAnims(3, false);
        }
    }

    bool32 collided = false;
    if (self->direction)
        collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0xC0000, 0x100000, 8);
    else
        collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, -0xC0000, 0x100000, 8);
    if (!collided) {
        self->state = Stegway_Unknown7;
        Stegway_SetupAnims(3, false);
        self->dustFlag = false;
        self->flag     = false;
    }
    Stegway_CheckOnScreen();
}

void Stegway_Unknown7(void)
{
    RSDK_THIS(Stegway);

    if (self->timer >= 29) {
        self->timer = 0;
        self->state = Stegway_Unknown6;
        Stegway_SetupAnims(0, false);

        self->direction ^= FLIP_X;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
    }
    else {
        self->timer++;
    }
}

void Stegway_Unknown8(void)
{
    RSDK_THIS(Stegway);
    if (++self->timer == 8) {
        self->timer = 0;
        self->state = Stegway_Unknown9;
        Stegway_SetupAnims(2, false);
        RSDK.PlaySfx(Stegway->sfxRev, false, 255);
    }
}

void Stegway_Unknown9(void)
{
    RSDK_THIS(Stegway);
    if (++self->timer == 32) {
        self->timer = 0;
        self->state = Stegway_Unknown10;
        EntityDust *dust =
            CREATE_ENTITY(Dust, self, self->position.x - 0xA0000 * (2 * (self->direction != FLIP_NONE) - 1), self->position.y + 0x100000);
        RSDK.SetSpriteAnimation(Dust->aniFrames, 2, &dust->animator, true, 0);
        dust->state     = Dust_State_DropDash;
        dust->direction = 1 - self->direction;
        dust->drawOrder = self->drawOrder;
        Stegway_SetupAnims(1, false);
        self->dustFlag = true;
        self->velocity.x *= 12;
        RSDK.PlaySfx(Stegway->sfxRelease, false, 255);
    }
}

void Stegway_Unknown10(void)
{
    RSDK_THIS(Stegway);

    self->position.x += self->velocity.x;
    int32 val = 2 * (self->direction != FLIP_NONE) - 1;

    if (!self->flag) {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;
        if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, val << 22, 0x100000, 8))
            self->flag = true;
        self->position.x = storeX;
        self->position.y = storeY;
    }

    bool32 collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0xC0000 * val, 0x100000, 8);

    if (self->flag) {
        if (self->velocity.x * val >= 0x4000) {
            self->velocity.x -= 0xA00 * val;
            if (val * self->velocity.x < 0x4000) {
                self->velocity.x = val << 14;
                self->state      = Stegway_Unknown6;
                Stegway_SetupAnims(0, false);
                self->animator1.frameID = 0;
                self->dustFlag          = false;
                self->flag              = false;
            }
        }
    }

    if (!collided) {
        self->state = Stegway_Unknown7;
        Stegway_SetupAnims(3, false);
        self->dustFlag = false;
        self->flag     = false;
    }
}

#if RETRO_INCLUDE_EDITOR
void Stegway_EditorDraw(void)
{
    RSDK_THIS(Stegway);
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 2, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 3, &self->animator3, true, 0);
    Stegway_SetupAnims(0, true);

    Stegway_Draw();
}

void Stegway_EditorLoad(void)
{
    Stegway->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Stegway.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Stegway, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Stegway_Serialize(void) { RSDK_EDITABLE_VAR(Stegway, VAR_UINT8, direction); }
