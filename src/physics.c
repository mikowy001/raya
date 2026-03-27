#include "./universe.h"
#include <raylib.h>
#include <raymath.h>

void atomPhysics() {
	if(atomCount == 0) return;

	float dt = GetFrameTime();
	
    rlEnableShader(shaderCompute.id);
	
	rlUpdateShaderBuffer(ssbo, atomsList, sizeof(atom) * atomCount, 0);

    SetShaderValue(shaderCompute, dtLoc, &dt, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shaderCompute, countLoc, &atomCount, SHADER_UNIFORM_INT);
    
	//TraceLog(LOG_INFO, "dtLock:%d      countLoc:%d", GetShaderLocation(shaderCompute, &dtLoc), GetShaderLocation(shaderCompute, &countLoc));

    rlBindShaderBuffer(ssbo, 0);
    rlComputeShaderDispatch((atomCount / 63) + 1, 1, 1);
    rlReadShaderBuffer(ssbo, atomsList, sizeof(atom) * atomCount, 0);

    rlDisableShader();

    rlReadShaderBuffer(ssbo, atomsList, sizeof(atom) * atomCount, 0);

	//TraceLog(LOG_INFO, "shader id: %d", shaderCompute.id);
}

void updateSSBO() {
    // Jeśli bufor już istnieje, usuwamy go, by stworzyć nowy o nowym rozmiarze
    if (ssbo > 0) rlUnloadShaderBuffer(ssbo);
    
    if (atomCount > 0) {
        ssbo = rlLoadShaderBuffer(sizeof(atom) * atomCount, atomsList, RL_DYNAMIC_COPY);
    }
}

void initComputeShader(){
	char *code = LoadFileText("src/compute.glsl");
	if (code != NULL) {
    // Kompilujemy bezpośrednio jako shader obliczeniowy
    //unsigned int shaderId = rlCompileShader(code, RL_COMPUTE_SHADER);
    //unsigned int programId = rlLoadComputeShaderProgram(shaderId);
    
	int shaderData = rlCompileShader(code, RL_COMPUTE_SHADER);
    int shaderCompute = rlLoadComputeShaderProgram(shaderData);
	
    //shaderCompute.id = programId;
    
    UnloadFileText(code);
	
	if (shaderCompute == 0) {
        TraceLog(LOG_ERROR, "Shader error !!!    NOT TTT.");
    }

}
}
