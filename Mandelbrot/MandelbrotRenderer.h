#pragma once
#include <span>

#include <BS_thread_pool.hpp>

#include "Colour.h"
#include "Bounds.h"

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
};