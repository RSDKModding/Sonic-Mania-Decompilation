#include "../SonicMania.h"

ObjectCPZ1Intro *CPZ1Intro;

void CPZ1Intro_Update(void)
{

}

void CPZ1Intro_LateUpdate(void)
{

}

void CPZ1Intro_StaticUpdate(void)
{

}

void CPZ1Intro_Draw(void)
{

}

void CPZ1Intro_Create(void* data)
{

}

void CPZ1Intro_StageLoad(void)
{

}

void CPZ1Intro_Particle_CB(Entity *debris)
{
    debris->velocity.y += RSDK.Rand(-0x28000, -0x20000);
    //RSDK.SetSpriteAnimation(CPZ1Intro->particleSpriteIndex, 1, &debris->data, true, 0);
}

void CPZ1Intro_EditorDraw(void)
{

}

void CPZ1Intro_EditorLoad(void)
{

}

void CPZ1Intro_Serialize(void)
{

}

