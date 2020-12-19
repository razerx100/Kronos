#ifndef __GRAPHIC_CONTEXT_HPP__
#define __GRAPHIC_CONTEXT_HPP__

namespace Kronos {
	class Dx12Context;
	class GraphicsContext {
	private:
		static GraphicsContext* s_Context;
	protected:
		virtual void swapBuffers() = 0;
	public:
		virtual ~GraphicsContext() {}
		static void CreateContext(unsigned int width, unsigned int height);
		inline static void CleanUpContext() { delete s_Context; }
		static inline GraphicsContext* GetGraphicsContext() { return s_Context; }
		static Dx12Context* GetDirectXContext();
		static void SwapBuffers();
	};
}

#endif