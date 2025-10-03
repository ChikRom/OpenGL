

#version 460 core
out vec4 FragColor;
in vec4 vertexColors;
in vec2 TexCoord;
uniform sampler2D texObject;
void main()
{
	FragColor = texture(texObject,TexCoord) * vertexColors;
}
