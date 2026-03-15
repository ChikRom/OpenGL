#version 460 core


out vec4 FragColor;

uniform sampler2D diffuse_texture1;
in vec2 TexCoords;

void main()
{
	FragColor = texture(diffuse_texture1,TexCoords);
}