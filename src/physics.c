#include 

void atomPhysics(){
	
	if(atomCount > 0){
		for(size_t i = 0; i < atomCount; i++){
		
			for(size_t j = 0; i < atomCount; j++){
			
				if(i == j) break;
			
				int iCharge = atomsList[i].charge;
				int jCharge = atomsList[j].charge;
				Vector2 direction = Vector2Subtract(atomsList[i].pos, atomsList[j].pos);
				Vector2 dirNormalized = Vector2Normalize(direction);
				float force = ( newtonianConst * (int){iCharge * jCharge} ) / Vector2DotProduct(atomsList[i].pos, atomsList[j].pos);
				float distance = Vector2Distance(atomsList[i].pos, atomsList[j].pos);
				Vector2 vF;
				if(distance >= 10){
					vF = Vector2Scale(dirNormalized, force);
				} //else {
					//break;
				//}
				
				atomsList[i].vel = Vector2Add(atomsList[i].vel, vF);
				atomsList[i].pos = Vector2Add(atomsList[i].pos, Vector2Scale(atomsList[i].vel, GetFrameTime()));
				
			}
		}
	}
	if(IsKeyDown(KEY_N)){TraceLog(LOG_INFO, "x=%.1f, y=%.1f", atomsList[1].pos.x, atomsList[1].pos.y);}
}

