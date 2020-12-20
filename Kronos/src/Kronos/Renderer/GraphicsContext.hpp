#ifndef __GRAPHIC_CONTEXT_HPP__
#define __GRAPHIC_CONTEXT_HPP__
#include "Shader.hpp"

namespace Kronos {
	class Dx12Context;

	class GraphicsContext {
	private:
		static GraphicsContext* s_Context;
		static Shader* s_shaderAsset;
	protected:
		virtual void swapBuffers() = 0;
	public:
		virtual ~GraphicsContext() {}

		static void CreateContext(unsigned int width, unsigned int height);

		inline static void CleanUpContext() {
			delete s_shaderAsset;
			delete s_Context;
		}
		static void SwapBuffers();

		static inline GraphicsContext* GetGraphicsContext() { return s_Context; }
		static inline Shader* GetShaderAsset() { return s_shaderAsset; }
		static Dx12Context* GetDirectXContext();
	};
}

#endif