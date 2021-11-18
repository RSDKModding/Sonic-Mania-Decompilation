#include "SonicMania.h"

ObjectBSS_Collected *BSS_Collected;

void BSS_Collected_Update(void)
{
    RSDK_THIS(BSS_Collected);
    EntityBSS_Setup *setup = RSDK_GET_ENTITY(SLOT_BSS_SETUP, BSS_Setup);
    int32 fieldPos           = self->position.y + (BSS_PLAYFIELD_W * self->position.x);
    switch (self->type) {
        case 0:
            ++BSS_Setup->field_1C;
            BSS_Setup->field_1C &= 0xF;
            if (++self->timer >= 16 && setup->state == BSS_Setup_State_HandleStage) {
                BSS_Setup->playField[fieldPos] = BSS_NONE;
                RSDK.ResetEntityPtr(self, TYPE_BLANK, 0);
            }
            break;
        case 1:
            if (BSS_Setup->sphereCount <= 0) {
                if (BSS_Setup->playField[fieldPos] == BSS_BLUE_STOOD)
                    BSS_Setup->playField[fieldPos] = BSS_SPHERE_RED;
                destroyEntity(self);
            }
            else {
                if (setup->globeTimer < 32 || setup->globeTimer > 224)
                    self->type = 2;
            }
            break;
        case 2:
            if (setup->state == BSS_Setup_State_HandleStage) {
                if (setup->globeTimer > 32 && setup->globeTimer < 224) {
                    if (BSS_Setup->playField[fieldPos] == BSS_BLUE_STOOD)
                        BSS_Setup->playField[fieldPos] = BSS_SPHERE_RED;
                    destroyEntity(self);
                }
            }
            break;
        case 3:
            if (setup->globeTimer < 32 || setup->globeTimer > 224) {
                self->timer = 10;
                self->type  = 4;
            }
            break;
        case 4:
            if (setup->state == BSS_Setup_State_HandleStage && --self->timer <= 0) {
                if (BSS_Setup->playField[fieldPos] == BSS_SPHERE_GREEN_STOOD)
                    BSS_Setup->playField[fieldPos] = BSS_SPHERE_BLUE;
                destroyEntity(self);
            }
            break;
        case 5:
            if (setup->state == BSS_Setup_State_HandleStage) {
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
        self->type   = voidToInt(data);
    }
}

void BSS_Collected_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void BSS_Collected_EditorDraw(void) {}

void BSS_Collected_EditorLoad(void) {}
#endif

void BSS_Collected_Serialize(void) {}
