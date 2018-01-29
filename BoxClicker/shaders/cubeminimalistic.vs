#version 430 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in mat4 in_transformMat;

uniform mat4 mvpMat;


flat out int fs_InstanceID;

void main(void)
{
	gl_Position = mvpMat * in_transformMat *  vec4(in_Position, 1.0);
    
	fs_InstanceID = gl_InstanceID;
}
