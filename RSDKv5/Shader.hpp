#ifndef SHADER_H
#define SHADER_H

#define SHADER_MAX (0x20)

enum ShaderIDs {
    SHADER_NONE,
    SHADER_CLEAN,
    SHADER_CRT_YEETRON,
    SHADER_CRT_YEE64,
    SHADER_YUV_420,
    SHADER_YUV_422,
    SHADER_YUV_444,
    SHADER_RGB_IMAGE,
};

struct ShaderEntry {
    void *vertexShaderObject;
    void *pixelShaderObject;
    byte linear;
    char name[0x20];
};

extern int shaderCount;
extern ShaderEntry shaderList[SHADER_MAX];

bool InitShaders();
bool ReloadShaders();

void LoadShader(const char *fileName, bool32 linear);

#endif
