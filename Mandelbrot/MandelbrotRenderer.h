#pragma once
#include <span>
#include <bit>

#include <BS_thread_pool.hpp>

#include "Colour.h"
#include "Bounds.h"

#include "ValuePack.h"

struct Image
{
	std::span<Colour> img;
	uint32_t w, h;
};

class MandelbrotRenderer
{
public:
	MandelbrotRenderer(uint32_t numThreads);

	void Render(Image img, Bounds bounds, uint32_t maxIter);

protected:
	BS::thread_pool pool;

	static void ProcessStridedPixels(Image img, Bounds bounds, uint32_t maxIter, uint32_t stride, uint32_t offset);
	static std::pair<double, double> IndexToCoord(int32_t index, uint32_t imgW, uint32_t imgH, Bounds bounds);
};