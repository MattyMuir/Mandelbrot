#pragma once
#include "wx/wx.h"
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>

#include <vector>
#include <thread>

#include "Colour.h"
#include "MandelbrotRenderer.h"
#include "Timer.h"

class Canvas : public wxPanel
{
public:
	Canvas(wxWindow* parent);

protected:
	uint32_t w = 0, h = 0;

	MandelbrotRenderer renderer;

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void Resized(wxSizeEvent& evt);

	wxDECLARE_EVENT_TABLE();
};