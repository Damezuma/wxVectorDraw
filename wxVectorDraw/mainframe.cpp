#include "mainframe.h"
#include <wx/artprov.h>
wxBEGIN_EVENT_TABLE(DrawMainFrame, wxFrame)
EVT_TOOL(5004, DrawMainFrame::OnSelectToolSelect)
EVT_TOOL(5005, DrawMainFrame::OnSelectLine)
EVT_TOOL(5006, DrawMainFrame::OnSelectSquare)
EVT_TOOL(5007, DrawMainFrame::OnSelectEllipse)
EVT_COMMAND(wxID_ANY, wxEVT_FINISH_DRAW_OBJECT,DrawMainFrame::OnFinishDrawObject)
wxEND_EVENT_TABLE();

DrawMainFrame::DrawMainFrame() : wxFrame(nullptr,wxID_ANY,wxT("wxVectorDraw"))
{
	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	ui_canvas = new DrawingCanvas(this, wxID_ANY, wxPoint(0,0), wxSize(200,200));
	ui_toolbar = new wxToolBar(this, wxID_ANY);
	ui_toolbar->AddTool(5001, wxT("새 파일"), wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR),wxT("새 파일"));
	ui_toolbar->AddTool(5002, wxT("불러오기"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR),wxT("불러오기"));
	ui_toolbar->AddTool(5003, wxT("저장"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR),wxT("저장"));
	ui_toolbar->AddSeparator();
	wxBitmap bmp;
	ui_toolbar->AddRadioTool(5004, wxT("선택"), wxArtProvider::GetBitmap(wxART_EDIT, wxART_TOOLBAR), wxArtProvider::GetBitmap(wxART_EDIT, wxART_TOOLBAR),wxT("선택"));

	bmp = wxBitmap(wxT("./tb_line.png"), wxBitmapType::wxBITMAP_TYPE_PNG);
	ui_toolbar->AddRadioTool(5005, wxT("선"), bmp, bmp, wxT("선"));

	bmp = wxBitmap(wxT("./tb_square.png"), wxBitmapType::wxBITMAP_TYPE_PNG);
	ui_toolbar->AddRadioTool(5006, wxT("사각형"),bmp, bmp, wxT("사각형"));

	bmp = wxBitmap(wxT("./tb_ellipse.png"), wxBitmapType::wxBITMAP_TYPE_PNG);
	ui_toolbar->AddRadioTool(5007, wxT("타원"), bmp, bmp, wxT("타원"));
	
	sizer->Add(ui_toolbar, 0, wxEXPAND);
	sizer->Add(ui_canvas, 1, wxEXPAND);
	ui_toolbar->Realize();
	this->SetSizer(sizer);
	this->Layout();
}

void DrawMainFrame::OnSelectToolSelect(wxCommandEvent & event)
{
	ui_canvas->SetDrawToolSelect();
}

void DrawMainFrame::OnSelectSquare(wxCommandEvent & event)
{
	ui_canvas->SetDrawToolRectangle();
}
void DrawMainFrame::OnSelectLine(wxCommandEvent & event)
{
	ui_canvas->SetDrawToolLine();
}
void DrawMainFrame::OnSelectEllipse(wxCommandEvent & event)
{
	ui_canvas->SetDrawToolEllipse();
}

void DrawMainFrame::OnFinishDrawObject(wxCommandEvent & event)
{
	ui_toolbar->ToggleTool(5004, true);
	ui_canvas->SetDrawToolSelect();
}
