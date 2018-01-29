#version 430 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in mat4 in_transformMat;

uniform mat4 projectionMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform vec3 lightSourcePos;

out vec3 surfaceNormal;
out vec3 lightSourceVector;
out vec3 cameraVector;
flat out int fs_InstanceID;

void main(void)
{
	mat4 newModelMat = modelMat * in_transformMat ;
	gl_Position = projectionMat * viewMat * newModelMat * vec4(in_Position, 1.0);
    surfaceNormal = (newModelMat * vec4(in_Normal, 0.0)).xyz;
	lightSourceVector = lightSourcePos -(newModelMat * vec4(in_Position, 1.0)).xyz;
	cameraVector = (inverse(viewMat) * vec4(0.0f,0.0f,0.0f,1.0f)).xyz - (newModelMat *vec4(in_Position, 1.0)).xyz;
	fs_InstanceID = gl_InstanceID;
}
