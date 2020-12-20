#ifndef __VERTEX_HPP__
#define __VERTEX_HPP__
#include "DirectXMath.h"

namespace Kronos {
	using namespace DirectX;
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
}
#endif