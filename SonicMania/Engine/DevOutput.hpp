#ifndef OBJ_DEVOUTPUT_H
#define OBJ_DEVOUTPUT_H

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
extern ObjectDevOutput DevOutput;

// Standard Entity Events
void DevOutput_Update();
void DevOutput_Draw();
void DevOutput_Create(void* data);

// Extra Entity Functions
int DevOutput_GetStringYOffset(char *string);

#endif //!OBJ_DEVOUTPUT_H
