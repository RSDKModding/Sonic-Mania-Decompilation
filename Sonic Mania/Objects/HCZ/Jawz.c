#include "SonicMania.h"

ObjectJawz *Jawz;

void Jawz_Update(void)
{
    RSDK_THIS(Jawz);
    StateMachine_Run(self->state);
}

void Jawz_LateUpdate(void) {}

void Jawz_StaticUpdate(void) {}

void Jawz_Draw(void)
{
    RSDK_THIS(Jawz);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Jawz_Create(void *data)
{
    RSDK_THIS(Jawz);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->velocity.x    = 0;
    RSDK.SetSpriteAnimation(Jawz->aniFrames, 0, &self->animator, true, 0);
    self->state = Jawz_CheckPlayerTrigger;
}

void Jawz_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        Jawz->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jawz.bin", SCOPE_STAGE);
    Jawz->hitbox.left   = -20;
    Jawz->hitbox.top    = -5;
    Jawz->hitbox.right  = 20;
    Jawz->hitbox.bottom = 5;
    DEBUGMODE_ADD_OBJ(Jawz);
}

void Jawz_DebugSpawn(void)
{
    RSDK_THIS(Jawz);
    CREATE_ENTITY(Jawz, NULL, self->position.x, self->position.y);
}

void Jawz_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Jawz->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Jawz_CheckPlayerInteractions(void)
{
    RSDK_THIS(Jawz);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Jawz->hitbox) && !Player_CheckBadnikBreak(self, player, true)) {
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(self);
        }
    }
}

void Jawz_CheckPlayerTrigger(void)
{
    RSDK_THIS(Jawz);

    EntityPlayer *player = NULL;
    if (Player->playerCount <= 1 || RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->sidekick)
        player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    else
        player = Player_GetNearestPlayer();

    if (player) {
        if (abs(player->position.x - self->position.x) < 0x1800000) {

            int32 playerID = RSDK.GetEntityID(player);
            switch (self->triggerDir) {
                case 0:
                    if (player->position.x > self->position.x) {
                        self->direction  = FLIP_X;
                        self->velocity.x = 0x20000;
                        self->position.x = (ScreenInfo[playerID].position.x - 64) << 16;
                    }
                    else if (player->position.x <= self->position.x) {
                        self->direction  = FLIP_NONE;
                        self->velocity.x = -0x20000;
                        self->position.x = (ScreenInfo[playerID].position.x + ScreenInfo[playerID].width + 64) << 16;
                    }
                    break;
                case 1:
                    if (player->position.x < self->position.x) {
                        self->direction  = FLIP_NONE;
                        self->velocity.x = -0x20000;
                        self->position.x = (ScreenInfo[playerID].position.x + ScreenInfo[playerID].width + 64) << 16;
                    }
                    break;
                case 2:
                    if (player->position.x > self->position.x) {
                        self->direction  = FLIP_X;
                        self->velocity.x = 0x20000;
                        self->position.x = (ScreenInfo[playerID].position.x - 64) << 16;
                    }
                    break;
            }
        }
    }

    if (self->velocity.x) {
        self->active  = ACTIVE_NORMAL;
        self->visible = 1;
        self->state   = Jawz_State_Main;
        Jawz_State_Main();
    }
    else {
        self->active  = ACTIVE_BOUNDS;
        self->visible = 0;
    }
}

void Jawz_State_Main(void)
{
    RSDK_THIS(Jawz);
    self->position.x += self->velocity.x;
    RSDK.ProcessAnimation(&self->animator);
    Jawz_CheckPlayerInteractions();

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        Jawz_Create(NULL);
    }
}

#if RETRO_INCLUDE_EDITOR
void Jawz_EditorDraw(void) { Jawz_Draw(); }

void Jawz_EditorLoad(void) { Jawz->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Jawz.bin", SCOPE_STAGE); }
#endif

void Jawz_Serialize(void) { RSDK_EDITABLE_VAR(Jawz, VAR_UINT8, triggerDir); }
