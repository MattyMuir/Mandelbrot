#include "Main.h"
#include "Canvas.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Window Application", wxPoint(30, 30), wxSize(600, 600))
	, canvas(new Canvas(this)) {}

Main::~Main()
{

}