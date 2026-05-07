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
    const float k = 100000; 

	float forceMag;
    for (int i = 0; i < particleCount; i++) {
        if (id == i) continue;

        vec2 dir = atomsList[id].pos - atomsList[i].pos;
		if(dir.x > 10 || dir.y > 10) {

        	float distSq = dot(dir, dir) + 0.1;
        	forceMag = k * (atomsList[id].charge * atomsList[i].charge) / distSq;
		} else {
			forceMag = 0.0;
		}

        dV += normalize(dir) * forceMag;
    }

    atomsList[id].vel += dV;
    atomsList[id].pos += atomsList[id].vel * dt;
}
