#pragma once
#include <wx/wx.h>
#include "drawingpanel.h"
class DrawMainFrame : public wxFrame
{
public:
	DrawMainFrame();
private:
	//private event handlers
	void OnSelectToolSelect(wxCommandEvent & event);
	void OnSelectSquare(wxCommandEvent & event);
	void OnSelectLine(wxCommandEvent & event);
	void OnSelectEllipse(wxCommandEvent & event);
	void OnFinishDrawObject(wxCommandEvent & event);
private:
	DrawingCanvas * ui_canvas;
	wxToolBar * ui_toolbar;
	
	DECLARE_EVENT_TABLE()
};