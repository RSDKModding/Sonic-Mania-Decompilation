// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Pinata Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPinata *Pinata;

void Pinata_Update(void)
{
    RSDK_THIS(Pinata);

    StateMachine_Run(self->state);
}

void Pinata_LateUpdate(void) {}

void Pinata_StaticUpdate(void) {}

void Pinata_Draw(void)
{
    RSDK_THIS(Pinata);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Pinata_Create(void *data)
{
    RSDK_THIS(Pinata);

    self->drawGroup     = self->priority != PINATA_PRIO_HIGH ? Zone->objectDrawGroup[0] : Zone->objectDrawGroup[1];
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->state         = Pinata_State_CheckPlayerCollisions;

    RSDK.SetSpriteAnimation(Pinata->aniFrames, 3, &self->animator, true, 0);
}

void Pinata_StageLoad(void)
{
    Pinata->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Pinata.bin", SCOPE_STAGE);

    Pinata->hitboxPinata.left   = -12;
    Pinata->hitboxPinata.top    = -16;
    Pinata->hitboxPinata.right  = 12;
    Pinata->hitboxPinata.bottom = 16;

    Pinata->sfxPinata = RSDK.GetSfx("MSZ/Pinata.wav");

    DEBUGMODE_ADD_OBJ(Pinata);
}

void Pinata_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Pinata->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Pinata_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Pinata, NULL, self->position.x, self->position.y);
}

void Pinata_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(Pinata);

    RSDK.ProcessAnimation(&self->animator);

    foreach_active(Player, player)
    {
        if (player->animator.animationID != ANI_HURT && Player_CheckBadnikTouch(player, self, &Pinata->hitboxPinata)) {
            RSDK.PlaySfx(Pinata->sfxPinata, false, 0xFF);

#if MANIA_USE_PLUS
            if (player->state != Player_State_MightyHammerDrop) {
#endif
                if (player->state == Player_State_FlyCarried)
                    RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

                int32 anim = player->animator.animationID;
                if (anim != ANI_FLY && anim != ANI_FLY_LIFT_TIRED) {
                    if (player->state != Player_State_TailsFlight) {
                        if (player->state != Player_State_DropDash)
                            player->state = Player_State_Air;

                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->animator.animationID = ANI_WALK;
                    }
                }

                if (player->animator.animationID != ANI_FLY)
                    player->applyJumpCap = false;

                if (player->velocity.y > -0x80000)
                    player->velocity.y = -0x80000;

                player->onGround       = false;
                player->tileCollisions = TILECOLLISION_DOWN;
#if MANIA_USE_PLUS
            }
#endif
            CREATE_ENTITY(ScoreBonus, NULL, self->position.x, self->position.y)->animator.frameID = 16;
            Player_GiveScore(player, 10);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), self->position.x, self->position.y - 0x100000)->drawGroup =
                Zone->objectDrawGroup[1];

            for (int32 i = 0; i < 6; ++i) {
                int32 x              = self->position.x + RSDK.Rand(-0x80000, 0x80000);
                int32 y              = self->position.y + RSDK.Rand(-0x80000, 0x80000);
                EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x, y);

                debris->state           = Debris_State_Fall;
                debris->gravityStrength = 0x4000;
                debris->velocity.x      = RSDK.Rand(0, 0x20000);
                if (debris->position.x < self->position.x)
                    debris->velocity.x = -debris->velocity.x;

                debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                debris->drawFX     = FX_FLIP;
                debris->direction  = i & 3;
                debris->drawGroup  = Zone->objectDrawGroup[1];
                RSDK.SetSpriteAnimation(Pinata->aniFrames, 0, &debris->animator, true, RSDK.Rand(0, 4));
            }

            self->state   = Pinata_State_Destroyed;
            self->visible = false;
            self->active  = ACTIVE_NORMAL;
        }
    }
}

void Pinata_State_Destroyed(void)
{
    RSDK_THIS(Pinata);

    Vector2 range = { TO_FIXED(256), TO_FIXED(256) };
    if (!RSDK.CheckOnScreen(self, &range)) {
        self->state   = Pinata_State_CheckPlayerCollisions;
        self->visible = true;
        self->active  = ACTIVE_BOUNDS;
    }
}

#if GAME_INCLUDE_EDITOR
void Pinata_EditorDraw(void) { Pinata_Draw(); }

void Pinata_EditorLoad(void)
{
    Pinata->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Pinata.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Pinata, priority);
    RSDK_ENUM_VAR("High", PINATA_PRIO_HIGH);
    RSDK_ENUM_VAR("Low", PINATA_PRIO_LOW);
}
#endif

void Pinata_Serialize(void)
{
    RSDK_EDITABLE_VAR(Pinata, VAR_BOOL, respawn);
    RSDK_EDITABLE_VAR(Pinata, VAR_ENUM, priority);
}
