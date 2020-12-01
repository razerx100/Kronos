#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__
#include <string>
namespace Kronos {
	class Renderer {
	private:
		std::string m_title;
	protected:
		std::wstring m_assetsPath;
		unsigned int m_width, m_height;
		float m_aspectRatio;

		inline std::wstring GetAssetFullPath(wchar_t* assetName) {
			return m_assetsPath + assetName;
		}
	public:
		Renderer(unsigned int width, unsigned int height, std::string name)
			: m_width(width), m_height(height), m_title(name) {
			m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		}
		virtual ~Renderer() {}

		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnDestroy() = 0;

		virtual void GetAssetsPath() = 0;

		inline unsigned int GetWidth() const { return m_width; }
		inline unsigned int GetHeight() const { return m_height; }
		inline const char* GetTitle() const { return m_title.c_str(); }
	};
}
#endif