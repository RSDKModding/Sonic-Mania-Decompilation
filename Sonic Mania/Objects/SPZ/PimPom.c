#include "SonicMania.h"

ObjectPimPom *PimPom;

void PimPom_Update(void)
{

}

void PimPom_LateUpdate(void)
{

}

void PimPom_StaticUpdate(void)
{

}

void PimPom_Draw(void)
{
    RSDK_THIS(PimPom);

    Vector2 drawPos = entity->drawPos;
    drawPos.x += entity->offset.x;
    drawPos.y -= entity->offset.y;
    for (int i = entity->length; i >= 0; --i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x -= entity->moveAmount.x;
        drawPos.y += entity->moveAmount.y;
    }
}

void PimPom_Create(void* data)
{

}

void PimPom_StageLoad(void)
{
    PimPom->aniFrames  = RSDK.LoadSpriteAnimation("SPZ2/PimPom.bin", SCOPE_STAGE);
    PimPom->sfxPimPom  = RSDK.GetSFX("Stage/PimPom.wav");
    PimPom->sfxBumper2 = RSDK.GetSFX("Stage/Bumper2.wav");
}

#if RETRO_INCLUDE_EDITOR
void PimPom_EditorDraw(void)
{
    RSDK_THIS(PimPom);
    PimPom_Draw();
}

void PimPom_EditorLoad(void) { PimPom->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/PimPom.bin", SCOPE_STAGE); }
#endif

void PimPom_Serialize(void)
{

}

