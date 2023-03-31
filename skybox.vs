#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	//TexCoords = aPos;
	TexCoords = vec3(-aPos.x, aPos.yz); // Flip X so that words make sense again
	vec4 pos = projection * view * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}