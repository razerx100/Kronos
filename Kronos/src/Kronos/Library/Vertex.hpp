#ifndef __VERTEX_HPP__
#define __VERTEX_HPP__
#include "DirectXMath.h"

namespace Kronos {
	using namespace DirectX;
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	struct TriangleVertices {
		Vertex point1;
		Vertex point2;
		Vertex point3;
	};
}
#endif