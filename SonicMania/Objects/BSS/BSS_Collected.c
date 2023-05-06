// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BSS_Collected Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBSS_Collected *BSS_Collected;

void BSS_Collected_Update(void)
{
    RSDK_THIS(BSS_Collected);

    EntityBSS_Setup *setup = RSDK_GET_ENTITY(SLOT_BSS_SETUP, BSS_Setup);
    int32 fieldPos         = self->position.y + (BSS_PLAYFIELD_W * self->position.x);

    switch (self->type) {
        case BSS_COLLECTED_RING:
            ++BSS_Setup->ringID;
            BSS_Setup->ringID &= 0xF;

            if (++self->timer >= 16 && setup->state == BSS_Setup_State_GlobeMoveZ) {
                BSS_Setup->playField[fieldPos] = BSS_NONE;
                destroyEntity(self);
            }
            break;

        case BSS_COLLECTED_BLUE:
            if (BSS_Setup->sphereCount <= 0) {
                if (BSS_Setup->playField[fieldPos] == BSS_BLUE_STOOD)
                    BSS_Setup->playField[fieldPos] = BSS_SPHERE_RED;

                destroyEntity(self);
            }
            else {
                if (setup->globeTimer < 32 || setup->globeTimer > 224)
                    self->type = BSS_COLLECTED_BLUE_STOOD;
            }
            break;

        case BSS_COLLECTED_BLUE_STOOD:
            if (setup->state == BSS_Setup_State_GlobeMoveZ) {
                if (setup->globeTimer > 32 && setup->globeTimer < 224) {
                    if (BSS_Setup->playField[fieldPos] == BSS_BLUE_STOOD)
                        BSS_Setup->playField[fieldPos] = BSS_SPHERE_RED;

                    destroyEntity(self);
                }
            }
            break;

        case BSS_COLLECTED_GREEN:
            if (setup->globeTimer < 32 || setup->globeTimer > 224) {
                self->timer = 10;
                self->type  = BSS_COLLECTED_GREEN_STOOD;
            }
            break;

        case BSS_COLLECTED_GREEN_STOOD:
            if (setup->state == BSS_Setup_State_GlobeMoveZ && --self->timer <= 0) {
                if (BSS_Setup->playField[fieldPos] == BSS_SPHERE_GREEN_STOOD)
                    BSS_Setup->playField[fieldPos] = BSS_SPHERE_BLUE;

                destroyEntity(self);
            }
            break;

        case BSS_COLLECTED_PINK:
            if (setup->state == BSS_Setup_State_GlobeMoveZ) {
                if (setup->playerPos.x != self->position.x || setup->playerPos.y != self->position.y) {
                    if (BSS_Setup->playField[fieldPos] == BSS_SPHERE_PINK_STOOD)
                        BSS_Setup->playField[fieldPos] = BSS_SPHERE_PINK;

                    destroyEntity(self);
                }
            }
            break;

        default: break;
    }
}

void BSS_Collected_LateUpdate(void) {}

void BSS_Collected_StaticUpdate(void) {}

void BSS_Collected_Draw(void) {}

void BSS_Collected_Create(void *data)
{
    RSDK_THIS(BSS_Collected);

    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_NORMAL;
        self->type   = VOID_TO_INT(data);
    }
}

void BSS_Collected_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void BSS_Collected_EditorDraw(void) {}

void BSS_Collected_EditorLoad(void) {}
#endif

void BSS_Collected_Serialize(void) {}
