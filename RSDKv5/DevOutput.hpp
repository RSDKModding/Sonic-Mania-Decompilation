#ifndef OBJ_DEVOUTPUT_H
#define OBJ_DEVOUTPUT_H

#if RETRO_REV02
// Object Class
struct ObjectDevOutput : Object {

};

// Entity Class
struct EntityDevOutput : Entity {
    int type;
    int id;
    int yOffset;
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
int DevOutput_GetStringYOffset(char *string);
#endif

#endif //!OBJ_DEVOUTPUT_H
