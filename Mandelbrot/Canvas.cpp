#include "Canvas.h"
#include "Main.h"

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
	EVT_SIZE(Canvas::Resized)
wxEND_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent) : wxPanel(parent, wxID_ANY)
	, renderer(std::thread::hardware_concurrency() - 1)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void Canvas::OnDraw(wxDC& dc)
{
	dc.Clear();

	//Drawing here
	dc.DrawLine(0, 0, w, h);

	std::vector<Colour> data(w * h);
	TIMER(t);
	renderer.Render(Image{ data, w, h }, { -2.0, -2.0, 2.0, 2.0 }, 100);
	STOP_LOG(t);

	wxImage img{ (int)w, (int)h, (uint8_t*)data.data(), true };
	wxBitmap bmp{ img };

	dc.DrawBitmap(bmp, 0, 0);
}

void Canvas::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc{ this };
	int newW, newH;
	GetSize(&newW, &newH);
	w = newW;
	h = newH;

	PrepareDC(dc);
	OnDraw(dc);
	evt.Skip();
}

void Canvas::Resized(wxSizeEvent& evt)
{
	Refresh();
	evt.Skip();
}