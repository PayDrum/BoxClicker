#version 430 core

flat in int fs_InstanceID;

out vec4 gl_FragColor;

void main(void) 
{
	gl_FragColor =vec4(0.0f,fs_InstanceID/255.0f,0.0f,1.0);
}


