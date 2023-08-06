// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZ1Intro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPSZ1Intro *PSZ1Intro;

void PSZ1Intro_Update(void)
{
    RSDK_THIS(PSZ1Intro);

    CutsceneSeq_StartSequence(self, PSZ1Intro_Cutscene_SetupGliders, PSZ1Intro_Cutscene_GlideAndJump, PSZ1Intro_Cutscene_HandleLanding,
                              StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif

    self->active = ACTIVE_NEVER;
}

void PSZ1Intro_LateUpdate(void) {}

void PSZ1Intro_StaticUpdate(void) {}

void PSZ1Intro_Draw(void) {}

void PSZ1Intro_Create(void *data)
{
    RSDK_THIS(PSZ1Intro);

    if (!SceneInfo->inEditor) {
        self->active      = ACTIVE_NORMAL;
        self->isPermanent = true;

        if (!isMainGameMode() || !globals->enableIntro || CutsceneRules_CheckStageReload()) {
            destroyEntity(self);

            foreach_all(HangGlider, glider) { destroyEntity(glider); }
        }
    }
}

void PSZ1Intro_StageLoad(void) {}

void PSZ1Intro_HandleGliderJump(EntityHangGlider *glider)
{
    EntityPlayer *player = glider->attachedPlayer;

    if (player) {
        player->state      = Player_State_Air;
        player->onGround   = false;
        player->velocity.x = glider->velocity.x >> 2;
        player->velocity.y = -0x20000;
        player->active     = ACTIVE_NORMAL;
        player->stateInput = StateMachine_None;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
        glider->attachedPlayer = NULL;
    }
}

bool32 PSZ1Intro_Cutscene_SetupGliders(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ1Intro);

    CutsceneSeq_LockAllPlayerControl();

    int32 id = 0;
    foreach_all(HangGlider, glider)
    {
        if (id < Player->playerCount) {
            EntityPlayer *player   = RSDK_GET_ENTITY(id, Player);
            player->position.x     = glider->position.x;
            player->position.y     = glider->position.y;
            player->active         = ACTIVE_NEVER;
            glider->active         = ACTIVE_NORMAL;
            glider->attachedPlayer = player;
            glider->velocity.x     = 0x40000;
            glider->velocity.y     = -0x20000;

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &glider->playerAnimator, true, 0);
            glider->playerAnimator.rotationStyle = ROTSTYLE_FULL;
            glider->rotation                     = 0x80;
            glider->drawFX                       = FX_ROTATE;
            glider->state                        = HangGlider_State_Glide;

            EntityCamera *camera = player->camera;
            if (camera) {
                camera->position.x     = glider->position.x;
                camera->position.y     = glider->position.y;
                camera->disableYOffset = true;
                camera->offset.y       = 0x200000;
            }
            else {
                glider->velocity.x      = 0x38000;
                glider->gravityStrength = 0xE00;
            }

            self->gliders[id] = glider;
        }
        else {
            destroyEntity(glider);
        }

        ++id;
    }

    BGSwitch->screenID = 0;

    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = 0;               // Background 1
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT; // Background 2
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT; // Background 3

    return true;
}

bool32 PSZ1Intro_Cutscene_GlideAndJump(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ1Intro);

    if (++self->timer == 90) {
        if (self->gliders[0])
            PSZ1Intro_HandleGliderJump(self->gliders[0]);
    }

    if (self->timer == 120) {
        self->timer = 0;

        if (self->gliders[1])
            PSZ1Intro_HandleGliderJump(self->gliders[1]);

        return true;
    }

    return false;
}

bool32 PSZ1Intro_Cutscene_HandleLanding(EntityCutsceneSeq *host)
{
    foreach_active(HangGlider, glider)
    {
        if (!glider->onScreen)
            destroyEntity(glider);
    }

    foreach_active(Player, player)
    {
        if (player->onGround) {
            if (player->sidekick) {
                if (player->stateInput == StateMachine_None) {
                    player->stateInput = Player_Input_P2_AI;

                    for (int32 i = 0; i < 16; ++i) Player->leaderPositionBuffer[i] = player->position;
                }
            }
            else {
                player->stateInput = Player_Input_P1;
            }

            player->collisionPlane = 0;
            player->groundVel      = 0;
            player->direction      = FLIP_NONE;
        }
        else if (player->stateInput == StateMachine_None) {
            return false;
        }
    }

    return true;
}

#if GAME_INCLUDE_EDITOR
void PSZ1Intro_EditorDraw(void) {}

void PSZ1Intro_EditorLoad(void) {}
#endif

void PSZ1Intro_Serialize(void) {}
#endif
