

#version 460 core
out vec4 FragColor;
in vec4 vertexColors;
in vec2 TexCoord;
uniform sampler2D texObject1;
uniform sampler2D texObject2;

void main()
{
	FragColor = mix(texture(texObject1,TexCoord),texture(texObject2,TexCoord),0.29);
}
