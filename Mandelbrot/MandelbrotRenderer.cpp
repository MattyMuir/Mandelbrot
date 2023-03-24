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
	uint32_t numPix = img.w * img.h;
	for (uint32_t pixIndex = offset; pixIndex < numPix; pixIndex += stride)
	{
		// Determine pixel coordinates
		uint32_t pixX = pixIndex % img.w;
		uint32_t pixY = pixIndex / img.w;

		// Convert to world coordinates
		double cr = ((double)pixX / img.w) * bounds.w() + bounds.xmin;
		double ci = ((double)(img.h - 1 - pixY) / img.h) * bounds.h() + bounds.ymin;

		double zr = 0.0, zi = 0.0;
		uint32_t i;
		for (i = 0; i < maxIter; i++)
		{
			double tempZr = zr;
			zr = zr * zr - zi * zi + cr;
			zi = zi * tempZr * 2 + ci;

			if (sqrt(zr * zr + zi * zi) > 2) break;
		}

		uint8_t value = 255 * (maxIter - i) / maxIter;
		img.img[pixIndex] = { value, value, value };
	}
}