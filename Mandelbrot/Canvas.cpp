#include "Canvas.h"
#include "Main.h"

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
	EVT_SIZE(Canvas::Resized)
	EVT_KEY_DOWN(Canvas::OnKeyPressed)
	EVT_MOTION(Canvas::OnMouseMove)
	EVT_MOUSEWHEEL(Canvas::OnScroll)
wxEND_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent) : wxPanel(parent, wxID_ANY)
, renderer(std::thread::hardware_concurrency() - 1), bounds(-2.0, -2.0, 2.0, 2.0)
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
	renderer.Render(Image{ data, w, h }, bounds, maxIter);
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

void Canvas::OnKeyPressed(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode())
	{
	case 'W':
		maxIter *= 2;
		break;
	case 'S':
		maxIter /= 2;
		break;
	}

	Refresh();
}

void Canvas::OnMouseMove(wxMouseEvent& evt)
{
	int32_t mx = evt.GetX();
	int32_t my = evt.GetY();
	if (evt.LeftIsDown())
	{
		if (mousePos.valid)
			OnMouseDrag(mx - mousePos.x, my - mousePos.y);
	}
	mousePos = { mx, my, evt.LeftIsDown() };
}

void Canvas::OnMouseDrag(int32_t dx, int32_t dy)
{
	if (dx == 0 && dy == 0) return;

	bounds.xmin -= (double)dx / w * bounds.w();
	bounds.xmax -= (double)dx / w * bounds.w();

	bounds.ymin += (double)dy / h * bounds.h();
	bounds.ymax += (double)dy / h * bounds.h();

	Refresh();
}

void Canvas::OnScroll(wxMouseEvent& evt)
{
	double mx = evt.GetX();
	double my = evt.GetY();

	double factor = 1.5;
	if (evt.GetWheelRotation() < 0) factor = 1.0 / factor;

	double oldWidth = bounds.w();
	bounds.xmin += mx / w * oldWidth * (1 - 1.0 / factor);
	bounds.xmax = bounds.xmin + oldWidth / factor;

	double oldHeight = bounds.h();
	bounds.ymin += (h - 1 - my) / h * oldHeight * (1 - 1.0 / factor);
	bounds.ymax = bounds.ymin + oldHeight / factor;

	Refresh();
}