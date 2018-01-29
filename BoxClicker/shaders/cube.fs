#version 430 core

in vec3 surfaceNormal;
in vec3 lightSourceVector;
in vec3 cameraVector;
flat in int fs_InstanceID;

out vec4 gl_FragColor;

uniform vec3 lightColor;

void main(void) 
{
	float shineDamper = 2.0f;
	float reflectivity = 1.0f;

	vec3 nSurfaceNormal = normalize(surfaceNormal);
	vec3 nLightSourceVector = normalize(lightSourceVector);
	float prod = dot(nSurfaceNormal,nLightSourceVector);
	float brightness = max(prod,0.0);
	vec3 diff = brightness * lightColor;

	vec3 nCameraVector = normalize(cameraVector);
	vec3 reflectedLightDir = reflect(-nLightSourceVector,nSurfaceNormal);
	float spec =max(0, dot(nCameraVector,reflectedLightDir));
	float dampFactor = pow(spec,shineDamper);
	vec3 finalSpecular = dampFactor * reflectivity * lightColor;
	

	if (fs_InstanceID !=0)
		gl_FragColor =vec4(diff,1.0) + vec4(finalSpecular,1.0f);
	else
		gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}


