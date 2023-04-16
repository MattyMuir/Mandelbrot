#include "MandelbrotRenderer.h"

MandelbrotRenderer::MandelbrotRenderer(uint32_t numThreads)
	: pool(numThreads) {}

void MandelbrotRenderer::Render(Image img, Bounds bounds, uint32_t maxIter)
{
	uint32_t numThreads = pool.get_thread_count();

	std::vector<std::future<void>> futs;

	for (uint32_t threadIndex = 0; threadIndex < numThreads; threadIndex++)
		futs.push_back(pool.submit(MandelbrotRenderer::ProcessStridedPixels, img, bounds, maxIter, numThreads, threadIndex));

	for (const std::future<void>& fut : futs)
		fut.wait();
}

void MandelbrotRenderer::ProcessStridedPixels(Image img, Bounds bounds, uint32_t maxIter, uint32_t stride, uint32_t offset)
{
	static constexpr int32_t BATCH = 16;
	uint32_t numPix = img.w * img.h;
	
	maxIter /= BATCH;

	ValuePack<int32_t, 4> pixIndicies { (int32_t)offset, (int32_t)offset + stride, (int32_t)offset + stride * 2, (int32_t)offset + stride * 3 };
	int32_t nextIndex = offset + stride * 4;

	// Convert initial pixel indicies to world coordinates
	ValuePack<double, 4> cr, ci;
	for (int i = 0; i < 4; i++)
		std::tie(cr[i], ci[i]) = IndexToCoord(pixIndicies[i], img.w, img.h, bounds);

	// Initialize z and iter values
	ValuePack<double, 4> zr{ cr }, zi{ ci };
	ValuePack<int32_t, 4> iter { 0 };
	for (;;)
	{
		// Iterate Mandelbrot function
		for (int i = 0; i < BATCH; i++)
		{
			auto tempZr = zr;
			zr = zr * zr - zi * zi + cr;
			zi = zi * tempZr * 2 + ci;
		}

		iter += 1;

		BoolPack tooBig = cmp<GREATER_NAN_TRUE>(sqrt(zr * zr + zi * zi), { 2.0 });
		BoolPack finished = (iter == maxIter);
		if (tooBig.None() && finished.None()) continue;

		for (int i = 0; i < 4; i++)
		{
			if (tooBig[i] || finished[i])
			{
				// Update image
				uint8_t value = (maxIter - iter[i]) * 256 / maxIter;
				img.img[pixIndicies[i]] = Colour{ value, value, value };
				
				// Update with new index and coordinates
				pixIndicies[i] = nextIndex;
				nextIndex += stride;
				if (nextIndex > numPix) return;

				std::tie(cr[i], ci[i]) = IndexToCoord(pixIndicies[i], img.w, img.h, bounds);

				// Reset z and iter values
				zr[i] = cr[i], zi[i] = ci[i], iter[i] = 0;
			}
		}
	}
}

std::pair<double, double> MandelbrotRenderer::IndexToCoord(int32_t index, uint32_t imgW, uint32_t imgH, Bounds bounds)
{
	int32_t pixX = index % imgW;
	int32_t pixY = index / imgW;

	double cr = (double)pixX / imgW * bounds.w() + bounds.xmin;
	double ci = (double)(imgH - 1 - pixY) / imgH * bounds.h() + bounds.ymin;

	return { cr, ci };
}