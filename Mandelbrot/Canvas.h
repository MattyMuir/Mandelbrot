#pragma once
#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>

#include <vector>
#include <thread>

#include "Colour.h"
#include "MandelbrotRenderer.h"
#include "Timer.h"

struct MousePosition
{
	int32_t x, y;
	bool valid = false;
};

class Canvas : public wxPanel
{
public:
	Canvas(wxWindow* parent);

protected:
	uint32_t w = 0, h = 0;
	MousePosition mousePos;
	Bounds bounds;

	MandelbrotRenderer renderer;
	uint32_t maxIter = 1024;

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void Resized(wxSizeEvent& evt);

	void OnKeyPressed(wxKeyEvent& evt);

	void OnMouseMove(wxMouseEvent& evt);
	void OnMouseDrag(int32_t dx, int32_t dy);
	void OnScroll(wxMouseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};