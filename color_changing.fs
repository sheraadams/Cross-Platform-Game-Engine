#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec3 spriteColor;

void main()
{
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	color = vec4(spriteColor, 1.0) * texture(ourTexture, texCoord);
}