#include "../SonicMania.h"

ObjectEggman *Eggman;

void Eggman_Update()
{
    RSDK_THIS(Eggman);
    CallFunction(entity->state);
}

void Eggman_LateUpdate() {}

void Eggman_StaticUpdate() {}

void Eggman_Draw()
{
    RSDK_THIS(Eggman);
    if (entity->parent) {
        entity->position.x = entity->parent->position.x + entity->offset.x;
        entity->position.y = entity->parent->position.y + entity->offset.y;
    }
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void Eggman_Create(void *data)
{
    RSDK_THIS(Eggman);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            RSDK.SetSpriteAnimation(Eggman->spriteIndex, 0, &entity->data, true, 0);
            entity->active        = ACTIVE_NORMAL;
            entity->visible       = true;
            entity->drawFX        = FX_FLIP;
            entity->drawOrder     = Zone->drawOrderLow;
            entity->parent        = (Entity *)data;
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x800000;
            entity->hitbox.left   = -0x0A;
            entity->hitbox.top    = -0x17;
            entity->hitbox.right  = 0x0A;
            entity->hitbox.bottom = 0x1B;
        }
        else {
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
        }
    }
}

void Eggman_StageLoad()
{
    if (RSDK.CheckStageFolder("GHZCutscene")) {
        Eggman->spriteIndex = RSDK.LoadSpriteAnimation("Eggman/EggmanGHZCutt.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Eggman->spriteIndex = RSDK.LoadSpriteAnimation("Eggman/EggmanFBZ.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ3")) {
        Eggman->spriteIndex = RSDK.LoadSpriteAnimation("Eggman/EggmanTMZ.bin", SCOPE_STAGE);
    }
    else {
        Eggman->spriteIndex = RSDK.LoadSpriteAnimation("Eggman/EggmanAll.bin", SCOPE_STAGE);
    }
}

void Eggman_Unknown1()
{
    RSDK_THIS(Eggman);
    RSDK.ProcessAnimation(&entity->data);
}

void Eggman_Unknown2()
{
    RSDK_THIS(Eggman);

    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID >= entity->data.frameCount) {
        RSDK.SetSpriteAnimation(Eggman->spriteIndex, entity->animID, &entity->data, true, 0);
        entity->state = Eggman_Unknown1;
    }
}

void Eggman_Unknown3()
{
    RSDK_THIS(Eggman);

    if (entity->data.frameID < entity->data.frameCount - 1)
        RSDK.ProcessAnimation(&entity->data);
}

void Eggman_Unknown4()
{
    RSDK_THIS(Eggman);

    RSDK.ProcessAnimation(&entity->data);
    if (entity->onGround) {
        RSDK.SetSpriteAnimation(Eggman->spriteIndex, entity->animID, &entity->data, true, 0);
        entity->state = Eggman_Unknown1;
    }
    else {
        entity->velocity.y += 0x3800;
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
    }
}

void Eggman_Unknown5()
{
    RSDK_THIS(Eggman);
    RSDK.ProcessAnimation(&entity->data);
    if (entity->timer <= 0) {
        RSDK.SetSpriteAnimation(Eggman->spriteIndex, entity->animID, &entity->data, true, 0);
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->state      = entity->stateStore;
    }
    else {
        entity->position.x += entity->velocity.x;
        entity->velocity.y += 0x3800;
        entity->position.y += entity->velocity.y;
        entity->timer--;
    }
}

void Eggman_Unknown6()
{
    RSDK_THIS(Eggman);
    RSDK.ProcessAnimation(&entity->data);
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, entity->collisionPlane, 0, 0x100000, true)) {
        entity->onGround = true;
        entity->state    = Eggman_Unknown1;
    }
}

void Eggman_Unknown7()
{
    RSDK_THIS(Eggman);
    RSDK.ProcessAnimation(&entity->data);
    entity->position.x += entity->velocity.x;
    entity->velocity.x += 0x200;

    if (!RSDK.CheckOnScreen(entity, 0)) {
        entity->active = 0;
        entity->state  = Eggman_Unknown1;
    }
}

void Eggman_EditorDraw() {}

void Eggman_EditorLoad() {}

void Eggman_Serialize() {}
