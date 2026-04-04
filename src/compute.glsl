#version 430

struct atom {
	vec2 pos;
	vec2 vel;
	int charge;
    int rot;
	float size;
	int selected;
};


layout(std430, binding = 0) buffer ParticleBuffer {
    atom atomsList[];
};

uniform int particleCount;
uniform float dt;

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

void main() {
    uint id = gl_GlobalInvocationID.x;
    if (id >= particleCount) return;

    vec2 dV = vec2(0.0);
    const float k = 10000; 

    for (int i = 0; i < particleCount; i++) {
        if (i == id) continue;

        vec2 dir = atomsList[id].pos - atomsList[i].pos;
        float distSq = dot(dir, dir) + 0.1;
        float forceMag = k * (atomsList[id].charge * atomsList[i].charge) / distSq;
        
        dV += normalize(dir) * forceMag;
    }

    atomsList[id].vel += dV;
    atomsList[id].pos += atomsList[id].vel * dt;
}
