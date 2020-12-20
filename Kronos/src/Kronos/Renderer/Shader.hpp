#ifndef __SHADER_HPP__
#define __SHADER_HPP__
#include<string>

namespace Kronos {
	class Shader {
	protected:
		std::wstring m_assetsPath;

		inline std::wstring GetAssetFullPath(wchar_t* assetName) {
			return m_assetsPath + assetName;
		}

		virtual void GetAssetsPath() = 0;

	public:
		virtual void Initialize() = 0;
	};
}
#endif