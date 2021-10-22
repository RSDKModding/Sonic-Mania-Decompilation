#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_HUD *PBL_HUD;

void PBL_HUD_Update(void)
{
    RSDK_THIS(PBL_HUD);
    StateMachine_Run(entity->state);
}

void PBL_HUD_LateUpdate(void) {}

void PBL_HUD_StaticUpdate(void) {}

void PBL_HUD_Draw(void)
{
    RSDK_THIS(PBL_HUD);
    entity->position.x = RSDK_screens->centerX << 16;
    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, true);
        RSDK.DrawSprite(&entity->animator2, NULL, true);
    }
}

void PBL_HUD_Create(void *data)
{
    RSDK_THIS(PBL_HUD);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible    = true;
        entity->drawOrder  = 12;
        entity->active     = ACTIVE_NORMAL;
        entity->position.y = 0;
        entity->scale.x    = 0x200;
        entity->scale.y    = 0x200;
        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 0, &entity->animator2, true, 1);
        RSDK.SetSpriteAnimation(PBL_HUD->aniFrames, 1, &entity->animator3, true, 0);
        RSDK.SetText(&entity->text, "", 64);

        if (globals->gameMode == MODE_ENCORE)
            PBL_HUD_DisplayMessage(entity, "BONUS STAGE! RESCUE YOUR BUDDIES!", 1);
        else
            PBL_HUD_DisplayMessage(entity, "BONUS STAGE!", 1);
    }
}

void PBL_HUD_StageLoad(void) { PBL_HUD->aniFrames = RSDK.LoadSpriteAnimation("Pinball/Backglass.bin", SCOPE_STAGE); }

void PBL_HUD_DisplayMessage(EntityPBL_HUD *entity, const char *message, int32 type)
{
    if (entity->state != PBL_HUD_State_Message7 && entity->state != PBL_HUD_Unknown12 && entity->state != PBL_HUD_Unknown13) {
        RSDK.PrependText(&entity->text, message);
        RSDK.SetSpriteString(PBL_HUD->aniFrames, 1, &entity->text);
        entity->stateDraw = PBL_HUD_StateDraw_Unknown2;
        switch (type) {
            case 0:
                entity->offset.x = 0x410000;
                entity->state    = PBL_HUD_State_Message0;
                entity->timer    = 12 * entity->text.textLength + 128;
                break;
            case 1:
                entity->offset.x = 0x410000;
                entity->state    = PBL_HUD_State_Message1;
                entity->timer    = 6 * entity->text.textLength + 64;
                break;
            case 2:
                entity->state    = PBL_HUD_State_Message2;
                entity->timer    = 12 * entity->text.textLength + 128;
                entity->offset.x = -0x410000 - 0xC0000 * entity->text.textLength;
                break;
            case 3:
                entity->state    = PBL_HUD_State_Message3;
                entity->timer    = 6 * entity->text.textLength + 64;
                entity->offset.x = -0x410000 - 0xC0000 * entity->text.textLength;
                break;
            case 4:
                entity->offset.y   = 0x120000;
                entity->timer      = -9;
                entity->velocity.y = -0x20000;
                entity->state      = PBL_HUD_State_Message4;
                entity->offset.x   = -0x60000 * entity->text.textLength;
                break;
            case 5:
                entity->offset.y   = -0x120000;
                entity->timer      = -9;
                entity->velocity.y = 0x20000;
                entity->state      = PBL_HUD_State_Message4;
                entity->offset.x   = -0x60000 * entity->text.textLength;
                break;
            case 6:
                entity->state    = PBL_HUD_State_Message6;
                entity->timer    = 60;
                entity->offset.x = -0x60000 * entity->text.textLength;
                break;
            case 7:
                entity->state    = PBL_HUD_State_Message7;
                entity->timer    = 60;
                entity->offset.x = -0x60000 * entity->text.textLength;
                break;
            default: break;
        }
    }
}

void PBL_HUD_State_Message0(void)
{
    RSDK_THIS(PBL_HUD);

    if (entity->timer <= 0) {
        entity->state     = StateMachine_None;
        entity->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        entity->timer--;
        if (!(entity->timer & 1))
            entity->offset.x -= 0x20000;
    }
}

void PBL_HUD_State_Message1(void)
{
    RSDK_THIS(PBL_HUD);

    if (entity->timer <= 0) {
        entity->state     = StateMachine_None;
        entity->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        entity->offset.x -= 0x20000;
        entity->timer--;
    }
}
void PBL_HUD_State_Message2(void)
{
    RSDK_THIS(PBL_HUD);

    if (entity->timer <= 0) {
        entity->state     = StateMachine_None;
        entity->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        entity->timer--;
        if (!(entity->timer & 1))
            entity->offset.x += 0x20000;
    }
}

void PBL_HUD_State_Message3(void)
{
    RSDK_THIS(PBL_HUD);

    if (entity->timer <= 0) {
        entity->state     = 0;
        entity->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        entity->offset.x += 0x20000;
        entity->timer--;
    }
}

void PBL_HUD_State_Message4(void)
{
    RSDK_THIS(PBL_HUD);

    if (entity->timer < 0 || entity->timer > 60)
        entity->offset.y += entity->velocity.y;

    entity->timer++;
    if (entity->timer > 90) {
        entity->timer     = 0;
        entity->offset.y  = 0;
        entity->state     = StateMachine_None;
        entity->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
}

void PBL_HUD_State_Message6(void)
{
    RSDK_THIS(PBL_HUD);

    if (entity->timer <= 0) {
        entity->state     = StateMachine_None;
        entity->stateDraw = PBL_HUD_StateDraw_Unknown3;
    }
    else {
        entity->timer--;
        if ((entity->timer & 7) < 4)
            entity->stateDraw = StateMachine_None;
        else
            entity->stateDraw = PBL_HUD_StateDraw_Unknown2;
    }
}

void PBL_HUD_State_Message7(void)
{
    RSDK_THIS(PBL_HUD);

    if (entity->timer <= 0) {
        entity->state     = PBL_HUD_Unknown12;
        entity->stateDraw = PBL_HUD_StateDraw_Unknown1;
        entity->drawFX    = FX_SCALE;

        foreach_active(PBL_Crane, crane) { crane->state = PBL_Crane_State_CreatePrizes; }
    }
    else {
        entity->timer--;
        if ((entity->timer & 7) < 4)
            entity->stateDraw = StateMachine_None;
        else
            entity->stateDraw = PBL_HUD_StateDraw_Unknown2;
    }
}

void PBL_HUD_StateDraw_Unknown1(void)
{
    RSDK_THIS(PBL_HUD);
    RSDK.DrawSprite(&entity->animator1, NULL, true);
}

void PBL_HUD_StateDraw_Unknown2(void)
{
    RSDK_THIS(PBL_HUD);
    RSDK.DrawSprite(&entity->animator1, 0, true);
    RSDK.DrawSprite(&entity->animator2, 0, true);

    Vector2 position;
    position.x = entity->position.x;
    position.y = entity->position.y;
    position.x += entity->offset.x;
    position.y += entity->offset.y;
    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, RSDK_screens->centerX - 55, 40, RSDK_screens->centerX + 56, 58);
    RSDK.DrawText(&entity->animator3, &position, &entity->text, 0, 0, 0, 1, 0, 0, true);
    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, 0, 0, RSDK_screens->width, RSDK_screens->height);
}

void PBL_HUD_StateDraw_Unknown3(void)
{
    RSDK_THIS(PBL_HUD);
    int32 score = PBL_Setup->score;
    int32 count = 0;
    int32 mult  = 1;

    while (score > 0) {
        ++count;
        score /= 10;
    }

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += 0x320000;
    RSDK.DrawSprite(&entity->animator1, NULL, true);
    RSDK.DrawSprite(&entity->animator2, NULL, true);

    int32 div = 10;
    while (count > 0) {
        entity->animator3.frameID = PBL_Setup->score / mult % div;
        RSDK.DrawSprite(&entity->animator3, &drawPos, true);
        drawPos.x -= 0xC0000;
        mult *= 10;
        --count;
    }
    entity->animator3.frameID = 0;
}

void PBL_HUD_Unknown12(void)
{
    RSDK_THIS(PBL_HUD);
    entity->scale.x += 4;
    entity->position.y -= 0x40000;
    if (entity->position.y < -0x480000) {
        entity->state   = StateMachine_None;
        entity->visible = false;
    }
}

void PBL_HUD_Unknown13(void)
{
    RSDK_THIS(PBL_HUD);
    entity->scale.x -= 4;
    entity->position.y += 0x40000;
    if (entity->position.y >= 0) {
        entity->position.y = 0;
        entity->scale.x    = 0x200;
        entity->scale.y    = 0x200;
        entity->drawFX     = FX_NONE;
        entity->state      = StateMachine_None;
        entity->stateDraw  = PBL_HUD_StateDraw_Unknown3;

        foreach_all(PBL_Player, player)
        {
            player->active     = ACTIVE_NORMAL;
            player->velocity.y = 0;
        }
    }
}

void PBL_HUD_EditorDraw(void) {}

void PBL_HUD_EditorLoad(void) {}

void PBL_HUD_Serialize(void) {}
#endif
