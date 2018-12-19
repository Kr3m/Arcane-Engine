#pragma once

#include <platform/OpenGL/Framebuffers/Framebuffer.h>

namespace arcane
{

	enum RenderPassType
	{
		ShadowmapPassType,
		LightingPassType,
		PostProcessPassType
	};

	struct ShadowmapPassOutput
	{
		glm::mat4 directionalLightViewProjMatrix;
		unsigned int shadowmapTexture;
	};

	struct LightingPassOutput
	{
		Framebuffer *outputFramebuffer;
	};

}
