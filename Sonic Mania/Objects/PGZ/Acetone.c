#include "SonicMania.h"

ObjectAcetone *Acetone;

void Acetone_Update(void)
{
    RSDK_THIS(Acetone);
    StateMachine_Run(entity->state);
}

void Acetone_LateUpdate(void) {}

void Acetone_StaticUpdate(void) {}

void Acetone_Draw(void)
{
    RSDK_THIS(Acetone);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->flag)
        Acetone_Unknown1();
}

void Acetone_Create(void *data)
{
    RSDK_THIS(Acetone);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->drawOrder     = Zone->playerDrawLow + 1;
    entity->updateRange.x = 0x100000;
    entity->updateRange.y = 0x100000;
    entity->hitbox.left   = -14;
    entity->hitbox.right  = 15;
    entity->field_A4[0]   = 2;
    entity->field_A4[1]   = 2;
    entity->field_A4[2]   = 3;
    entity->field_A4[3]   = 3;
    Acetone_Unknown3();
    Acetone_Unknown4();
    RSDK.SetSpriteAnimation(Acetone->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Acetone->aniFrames, 1, &entity->animator2, true, 0);
    entity->state = Acetone_Unknown5;
}

void Acetone_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Acetone->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Acetone.bin", SCOPE_STAGE);
    Acetone->sfxFrostThrower = RSDK.GetSFX("PSZ/FrostThrower.wav");
    Acetone->sfxFreeze       = RSDK.GetSFX("PSZ/Freeze.wav");
}

void Acetone_Unknown1(void)
{
    RSDK_THIS(Acetone);

    int pos = 0;
    for (int i = 0; i < 4; ++i) {
        int count = minVal(entity->field_B4[i], 3);

        RSDK.SetSpriteAnimation(Acetone->aniFrames, 1, &entity->animator2, true, i);
        for (int p = 0; p < count; ++p) {
            Vector2 drawPos;
            drawPos.x = entity->position.x + entity->field_C4[pos + p].x;
            drawPos.y = entity->position.y + entity->field_C4[pos + p].y;
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        }
        pos += 3;
    }
}

void Acetone_Unknown2(void)
{
    RSDK_THIS(Acetone);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && !Ice->playerTimers[RSDK.GetEntityID(player)]) {
            //Unused pre-plus object, was never updated for plus
            switch (player->characterID) {
                case ID_SONIC: RSDK.CopyPalette(6, 2, 0, 2, 6); break;
                case ID_TAILS: RSDK.CopyPalette(6, 70, 0, 70, 6); break;
                case ID_KNUCKLES: RSDK.CopyPalette(6, 80, 0, 80, 6); break;
            }
            Ink->playerTypes[RSDK.GetEntityID(player)] = 0;
        }
    }
}

void Acetone_Unknown3(void)
{
    RSDK_THIS(Acetone);

    for (int i = 0; i < 4; ++i) {
        entity->field_B4[i] = entity->field_A4[i];
        if (entity->field_A4[i] > 1) {
            if (RSDK.Rand(0, 10) <= 6)
                entity->field_B4[i] = RSDK.Rand(1, entity->field_A4[i]);
        }
    }
}

void Acetone_Unknown4(void)
{
    RSDK_THIS(Acetone);
    int pos = 0;

    int yMin[] = { 2, 20, 45, 55 };
    int yMax[] = { 22, 45, 62, 78 };
    int xMin[] = { -3, -5, -7, -9 };
    int xMax[] = { 3, 5, 7, 9 };
    for (int i = 0; i < 4; ++i) {
        if (entity->field_B4[i]) {
            for (int p = 0; p < entity->field_B4[i]; ++p) {
                entity->field_C4[pos + p].x = RSDK.Rand(xMin[i], xMax[i]) << 16;
                entity->field_C4[pos + p].y = RSDK.Rand(yMin[i], yMax[i]) << 16;
            }
        }
        pos += 3;
    }
}

void Acetone_Unknown5(void)
{
    RSDK_THIS(Acetone);
    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        entity->active = ACTIVE_NORMAL;
        entity->timer  = 0;
        entity->flag   = true;
        entity->state  = Acetone_Unknown6;
        RSDK.PlaySfx(Acetone->sfxFrostThrower, false, 255);
    }
}

void Acetone_Unknown6(void)
{
    RSDK_THIS(Acetone);
    entity->hitbox.top = 0;
    if (entity->timer >= 20)
        entity->hitbox.bottom = 80;
    else
        entity->hitbox.bottom = 4 * entity->timer;

    Acetone_Unknown2();

    entity->field_A4[0] = 0;
    if (entity->timer > 3)
        entity->field_A4[0] = 2;
    else
        entity->field_A4[0] = 1;

    entity->field_A4[1] = 0;
    if (entity->timer <= 8 && entity->timer > 5)
        entity->field_A4[1] = 1;
    else if (entity->timer > 8)
        entity->field_A4[1] = 2;

    entity->field_A4[2] = 0;
    if (entity->timer <= 16) {
        if (entity->timer <= 14) {
            if (entity->timer > 12)
                entity->field_A4[2] = 1;
        }
        else {
            entity->field_A4[2] = 2;
        }
    }
    else {
        entity->field_A4[2] = 3;
    }

    entity->field_A4[3] = 0;
    if (entity->timer <= 19) {
        if (entity->timer <= 17) {
            if (entity->timer > 15)
                entity->field_A4[3] = 1;
        }
        else {
            entity->field_A4[3] = 2;
        }
    }
    else {
        entity->field_A4[3] = 3;
    }

    Acetone_Unknown3();
    Acetone_Unknown4();

    ++entity->timer;
    if (entity->timer >= entity->duration) {
        entity->state = Acetone_Unknown7;
        entity->timer = 0;
    }
}

void Acetone_Unknown7(void)
{
    RSDK_THIS(Acetone);
    entity->hitbox.top    = 4 * entity->timer;
    entity->hitbox.bottom = 80;
    Acetone_Unknown2();

    entity->field_A4[0] = 2;
    if (entity->timer <= 8) {
        if (entity->timer > 5)
            entity->field_A4[0] = 1;
    }
    else {
        entity->field_A4[0] = 0;
    }

    entity->field_A4[1] = 2;
    if (entity->timer <= 8) {
        if (entity->timer > 5)
            entity->field_A4[1] = 1;
    }
    else {
        entity->field_A4[1] = 0;
    }

    entity->field_A4[2] = 3;
    if (entity->timer <= 19) {
        if (entity->timer <= 17) {
            if (entity->timer > 15)
                entity->field_A4[2] = 2;
        }
        else {
            entity->field_A4[2] = 1;
        }
    }
    else {
        entity->field_A4[2] = 0;
    }

    entity->field_A4[3] = 3;
    if (entity->timer <= 16) {
        if (entity->timer <= 14) {
            if (entity->timer > 12)
                entity->field_A4[3] = 2;
        }
        else {
            entity->field_A4[3] = 1;
        }
    }
    else {
        entity->field_A4[3] = 0;
    }
    Acetone_Unknown3();
    Acetone_Unknown4();

    if (entity->timer >= 20) {
        entity->active = ACTIVE_BOUNDS;
        entity->flag   = false;
        entity->state  = Acetone_Unknown5;
        entity->timer  = 0;
    }
    else {
        entity->timer++;
    }
}

void Acetone_EditorDraw(void) {}

void Acetone_EditorLoad(void) {}

void Acetone_Serialize(void)
{
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, duration);
}
