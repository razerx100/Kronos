#include "Vertex.hpp"

namespace Kronos {
	void TriangleVertices::ChangeColor(XMVECTORF32 color) {
		topPoint.ChangeColor(color);
		rightPoint.ChangeColor(color);
		leftPoint.ChangeColor(color);
	}
}