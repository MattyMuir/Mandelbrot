#pragma once
struct Bounds
{
	double xmin, ymin, xmax, ymax;

	Bounds()
		: xmin(0.0), ymin(0.0), xmax(0.0), ymax(0.0) {}
	Bounds(double xmin_, double ymin_, double xmax_, double ymax_)
		: xmin(xmin_), ymin(ymin_), xmax(xmax_), ymax(ymax_) {}

	double w() const { return xmax - xmin; }
	double h() const { return ymax - ymin; }

	bool In(double x, double y) const
	{
		return x > xmin && x < xmax&& y > ymin && y < ymax;
	}
};