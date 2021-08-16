#include "ProperTest.h"

ObjectSpikes* Spikes;

void Spikes_StateDraw_Stage(void)
{
    Vector2 drawPos;

    RSDK_THIS(Spikes);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    int cnt   = entity->count >> 1;
    switch (entity->type) {
        case 1:
        case 4:
            drawPos.x = (0x100000 - (entity->count << 19)) + entity->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->vData, &drawPos, 0);
                drawPos.x += 0x200000;
            }
            if (entity->count & 1) {
                drawPos.x -= 0x100000;
                RSDK.DrawSprite(&Spikes->vData, &drawPos, 0);
            }
            break;
        case 2:
        case 3:
            drawPos.y = (0x100000 - (entity->count << 19)) + entity->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->hData, &drawPos, 0);
                drawPos.y += 0x200000;
            }
            if (entity->count & 1) {
                drawPos.y -= 0x100000;
                RSDK.DrawSprite(&Spikes->hData, &drawPos, 0);
            }
            break;
        default: break;
    }
}

void Spikes_Update(void) { 
    RSDK_THIS(Spikes); 
    entity->position.x += RSDK.Sin256(entity->dword6C++) << 9;
    Mod.Super(entity->objectID, SUPER_UPDATE, NULL);
}

void Spikes_StageLoad(void) {
    Mod.Super(Spikes->objectID, SUPER_STAGELOAD, NULL);
    Spikes->spriteIndex = RSDK.LoadSpriteAnimation("PSZ2/Spikes.bin", SCOPE_STAGE);
    Spikes->stateDraw   = Spikes_StateDraw_Stage;
    RSDK.SetSpriteAnimation(Spikes->spriteIndex, 0, &Spikes->vData, true, 0);
    RSDK.SetSpriteAnimation(Spikes->spriteIndex, 1, &Spikes->hData, true, 0);
}