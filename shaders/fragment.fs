

#version 460 core
out vec4 FragColor;
in vec4 vertexColors;
void main()
{
	FragColor = vertexColors;
}
