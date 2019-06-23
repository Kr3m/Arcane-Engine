#include "pch.h"
#include "Texture.h"

namespace arcane {

	Texture::Texture(TextureSettings &settings) : m_TextureId(0), m_TextureTarget(0), m_Width(0), m_Height(0), m_TextureSettings(settings) {}

	Texture::~Texture() {
		glDeleteTextures(1, &m_TextureId);
	}

	void Texture::generate2DTexture(unsigned int width, unsigned int height, GLenum dataFormat, const void *data) {
		m_TextureTarget = GL_TEXTURE_2D;
		m_Width = width;
		m_Height = height;

		// If GL_NONE is specified, set the texture format to the data format
		if (m_TextureSettings.TextureFormat == GL_NONE) {
			m_TextureSettings.TextureFormat = dataFormat;
		}
		// Check if the texture is SRGB, if so change the texture format
		if (m_TextureSettings.IsSRGB) {
			switch (dataFormat) {
			case GL_RGB: m_TextureSettings.TextureFormat = GL_SRGB; break;
			case GL_RGBA: m_TextureSettings.TextureFormat = GL_SRGB_ALPHA; break;
			}
		}

		glGenTextures(1, &m_TextureId);
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, m_TextureSettings.TextureFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		// Texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TextureSettings.TextureWrapSMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TextureSettings.TextureWrapTMode);

		// Texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureSettings.TextureMinificationFilterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TextureSettings.TextureMagnificationFilterMode);

		// Mipmapping
		if (m_TextureSettings.HasMips) {
			glGenerateMipmap(GL_TEXTURE_2D); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, m_TextureSettings.MipBias);
		}

		// Anisotropic filtering (TODO: Move the anistropyAmount calculation to Defs.h to avoid querying the OpenGL driver everytime)
		float maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		float anistropyAmount = glm::min(maxAnisotropy, m_TextureSettings.TextureAnisotropyLevel);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistropyAmount);

		unbind();
	}

	void Texture::bind(int unit) {
		if (unit >= 0)
			glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(m_TextureTarget, m_TextureId);
	}

	void Texture::unbind() {
		glBindTexture(m_TextureTarget, 0);
	}



	void Texture::setTextureWrapS(GLenum textureWrapMode, bool shouldBind) {
		if (m_TextureSettings.TextureWrapSMode == textureWrapMode)
			return;

		m_TextureSettings.TextureWrapSMode = textureWrapMode;
		if (shouldBind)
			bind();
		if (m_TextureTarget)
			glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_S, m_TextureSettings.TextureWrapSMode);
	}

	void Texture::setTextureWrapT(GLenum textureWrapMode, bool shouldBind) {
		if (m_TextureSettings.TextureWrapTMode == textureWrapMode)
			return;

		m_TextureSettings.TextureWrapTMode = textureWrapMode;
		if (shouldBind)
			bind();
		if (m_TextureTarget)
			glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_T, m_TextureSettings.TextureWrapTMode);
	}

	void Texture::setTextureMinFilter(GLenum textureFilterMode, bool shouldBind) {
		if (m_TextureSettings.TextureMinificationFilterMode == textureFilterMode)
			return;

		m_TextureSettings.TextureMinificationFilterMode = textureFilterMode;
		if (shouldBind)
			bind();
		if (m_TextureTarget)
			glTexParameteri(m_TextureTarget, GL_TEXTURE_MIN_FILTER, m_TextureSettings.TextureMinificationFilterMode);
	}

	void Texture::setTextureMagFilter(GLenum textureFilterMode, bool shouldBind) {
		// If mag filter mode exceeds GL_Linear (bilinear) report an error because it is useless to perform more expensive filtering with magnification
		if (textureFilterMode > GL_LINEAR)
			Logger::getInstance().warning("logged_files/textures.txt", "Texture Filter Tuning", "Texture's magnification filter exceeded bilinear filtering which won't result in any visual improvements and will just cost more");

		if (m_TextureSettings.TextureMagnificationFilterMode == textureFilterMode)
			return;

		m_TextureSettings.TextureMagnificationFilterMode = textureFilterMode;
		if (shouldBind)
			bind();
		if (m_TextureTarget)
			glTexParameteri(m_TextureTarget, GL_TEXTURE_MAG_FILTER, m_TextureSettings.TextureMagnificationFilterMode);
	}

	void Texture::setAnisotropicFilteringMode(float textureAnisotropyLevel, bool shouldBind) {
		if (m_TextureSettings.TextureAnisotropyLevel == textureAnisotropyLevel)
			return;

		m_TextureSettings.TextureAnisotropyLevel = textureAnisotropyLevel;
		if (shouldBind)
			bind();
		if (m_TextureTarget) {
			float maxAnisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			float anistropyAmount = glm::min(maxAnisotropy, m_TextureSettings.TextureAnisotropyLevel);
			glTexParameterf(m_TextureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistropyAmount);
		}
	}


	void Texture::setMipMode(bool shouldGenMips, int mipBias) {
		m_TextureSettings.HasMips = shouldGenMips;
		m_TextureSettings.MipBias = mipBias;
	}

}
