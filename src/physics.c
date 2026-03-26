#include "./universe.h"
#include <raymath.h>
void atomPhysics() {
    float dt = GetFrameTime();
    float minSafeDistance = 30.0f;
    float softening = 100.0f; 
    float maxForce = 5000.0f; 

    for (size_t i = 0; i < atomCount; i++) {
        Vector2 acceleration = { 0.0f, 0.0f };

        for (size_t j = 0; j < atomCount; j++) {
            if (i == j) continue;
            
            float distance = Vector2Distance(atomsList[i].pos, atomsList[j].pos);
            
            if (distance > 0 && distance < 200.0f) {
                float force = 0.0f;
                
                if (distance < minSafeDistance) {
                    force = (distance / minSafeDistance - 1.0f) * maxForce;
                } else {
                    float influence = 1.0f - fabsf(2.0f * distance - minSafeDistance - 200.0f) / (200.0f - minSafeDistance);
                    force = influence * softening;
                }
                
                Vector2 direction = Vector2Scale(delta, 1.0f / distance);
                acceleration = Vector2Add(acceleration, Vector2Scale(direction, force));
            }
        }

        atomsList[i].vel = Vector2Add(atomsList[i].vel, Vector2Scale(acceleration, dt));
        atomsList[i].vel = Vector2Scale(atomsList[i].vel, 0.95f);
        atomsList[i].pos = Vector2Add(atomsList[i].pos, Vector2Scale(atomsList[i].vel, dt * 100.0f));
    }
}
