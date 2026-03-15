#ifndef SHADOW_MAP_FBO
#define SHADOW_MAP_FBO

#include <iostream>
#include <GLAD/glad/glad.h>
#include <GLFW/glfw3.h>
class ShadowMapFBO
{
public:
	ShadowMapFBO() : m_fbo(0), m_shadowMap(0) {};

	~ShadowMapFBO();

	bool Init(uint32_t WindowWidth, uint32_t WindowHeight);

	void BindForReading(GLenum TextureUnit);
	void BindForWriting();


private:
	uint32_t m_fbo;
	uint32_t m_shadowMap;
	uint32_t m_windowWidth;
	uint32_t m_windowHeight;
	
};

#endif 