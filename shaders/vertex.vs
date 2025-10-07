
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColors;
layout (location = 2) in vec2 aTexCoord;
out vec4 vertexColors;
out vec2 TexCoord;
uniform mat4 transform;
void main()
{
	gl_Position = transform * vec4(aPos,1.0);
	vertexColors = vec4(aColors,1.0f);
	TexCoord = aTexCoord;
}