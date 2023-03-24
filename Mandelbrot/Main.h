#pragma once
#include "wx/wx.h"

class Canvas;

class Main : public wxFrame
{
public:
	Main();
	~Main();

protected:
	Canvas* canvas;

	wxDECLARE_EVENT_TABLE();
};