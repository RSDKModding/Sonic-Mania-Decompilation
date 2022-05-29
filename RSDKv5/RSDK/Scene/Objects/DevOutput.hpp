#ifndef OBJ_DEVOUTPUT_H
#define OBJ_DEVOUTPUT_H

#if RETRO_REV02
namespace RSDK
{

enum DevOutputStates {
    DEVOUTPUT_DELAY,
    DEVOUTPUT_ENTERPOPUP,
    DEVOUTPUT_SHOWPOPUP,
    DEVOUTPUT_EXITPOPUP,
};

// Object Class
struct ObjectDevOutput : Object {
    // Nothin'
};

// Entity Class
struct EntityDevOutput : Entity {
    int32 state;
    int32 timer;
    int32 ySize;
    char message[1012];
};

// Object Entity
extern ObjectDevOutput *DevOutput;

// Standard Entity Events
void DevOutput_Update();
void DevOutput_LateUpdate();
void DevOutput_StaticUpdate();
void DevOutput_Draw();
void DevOutput_Create(void *data);
void DevOutput_StageLoad();
void DevOutput_EditorDraw();
void DevOutput_EditorLoad();
void DevOutput_Serialize();

// Extra Entity Functions
int32 DevOutput_GetStringYSize(char *string);

} // namespace RSDK
#endif

#endif //! OBJ_DEVOUTPUT_H
