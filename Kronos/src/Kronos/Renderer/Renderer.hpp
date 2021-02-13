#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__
#include <string>
#include "Kronos/Library/Vertex.hpp"

namespace Kronos {
	class Renderer {
	protected:
		std::string m_title;
		unsigned int m_width, m_height;
		TriangleVerticesT m_triangleVertices;

	public:
		Renderer(unsigned int width, unsigned int height, std::string name)
			: m_width(width), m_height(height), m_title(name) { }
		virtual ~Renderer() {}

		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnDestroy() = 0;

		inline unsigned int GetWidth() const { return m_width; }
		inline unsigned int GetHeight() const { return m_height; }
		inline const char* GetTitle() const { return m_title.c_str(); }

		static Renderer* Create(unsigned int width, unsigned int height);
	};
}
#endif