#ifndef SHADER_H
#define SHADER_H

#define SHADER_MAX (0x20)

struct ShaderEntry {
    byte scope;
    uint hash[4];
    char name[0x20];
};

extern int shaderCount;
extern ShaderEntry shaderList[SHADER_MAX];

bool InitShaders();
bool ReloadShaders();

void LoadShader(const char *fileName, byte scope);

#endif
