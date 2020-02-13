#include "pch.h"
#include "ShaderLoader.h"

namespace arcane {
	namespace editor {
		// Static declarations
		std::unordered_map<std::size_t, Shader*> editor::ShaderLoader::s_ShaderCache;
		std::hash<std::string> editor::ShaderLoader::s_Hasher;

		Shader* editor::ShaderLoader::loadShader(const std::string& path) {
			std::size_t hash = s_Hasher(path);

			// Check the cache
			auto iter = s_ShaderCache.find(hash);
			if (iter != s_ShaderCache.end()) {
				return iter->second;
			}

			// Load the shader
			Shader* shader = new Shader(path);

			s_ShaderCache.insert(std::pair<std::size_t, Shader*>(hash, shader));
			return s_ShaderCache[hash];
		}
	}
}