#ifndef __VERTEX_HPP__
#define __VERTEX_HPP__
#include "DirectXMath.h"
#include "DirectXColors.h"

namespace Kronos {
	using namespace DirectX;
	struct Vertex {
		XMFLOAT3 position;
		XMVECTORF32 color;

		inline void ChangeColor(XMVECTORF32 color) { this->color = color; }
	};
	struct TriangleVertices {
		Vertex topPoint;
		Vertex rightPoint;
		Vertex leftPoint;

		void ChangeColor(XMVECTORF32 color);
	};
}
#endif