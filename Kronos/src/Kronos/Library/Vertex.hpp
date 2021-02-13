#ifndef __VERTEX_HPP__
#define __VERTEX_HPP__
#include "DirectXMath.h"

namespace Kronos {
	using namespace DirectX;
	struct VertexC {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	struct VertexT {
		XMFLOAT3 position;
		XMFLOAT2 uv;
	};
	struct TriangleVerticesC {
		VertexC point1;
		VertexC point2;
		VertexC point3;
	};
	struct TriangleVerticesT {
		VertexT point1;
		VertexT point2;
		VertexT point3;
	};
}
#endif