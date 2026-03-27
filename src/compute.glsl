#version 430

struct atom {
	vec2 pos;
	vec2 vel;
	int charge;
};


// Deklaracja bufora SSBO
layout(std430, binding = 0) buffer ParticleBuffer {
    atom atomsList[];
};

uniform int particleCout;
uniform float dT;

layout(local_size_x = 32, local_size_y = 1, local_size_z = 1) in;

void main() {
    uint id = gl_GlobalInvocationID.x;
    if (id >= particleCount) return;

    vec2 totalForce = vec2(0.0);
    const float k = 8.987e3; 

    for (int i = 0; i < particleCount; i++) {
        if (i == id) continue;

        vec2 dir = particles[i].pos - particles[id].pos;
        float distSq = dot(dir, dir) + 0.1;
        float forceMag = k * (particles[id].charge * particles[i].charge) / distSq;
        
        totalForce += normalize(dir) * forceMag;
    }

    particles[id].vel += totalForce;
    particles[id].pos += particles[id].vel * dt;
}
