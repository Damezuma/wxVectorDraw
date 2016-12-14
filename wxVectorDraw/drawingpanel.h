#pragma once
#include <wx/wx.h>
#include <vector>
#include "drawingobject.h"
wxDECLARE_EVENT(wxEVT_FINISH_DRAW_OBJECT, wxCommandEvent);
class DrawingCanvas :public wxWindow
{
public:
	DrawingCanvas();
	DrawingCanvas(wxWindow * parent, wxWindowID winId,const wxPoint & pos, const wxSize& size);
	~DrawingCanvas();
	bool Create(wxWindow * parent, wxWindowID winId, const wxPoint & pos, const wxSize& size);
	void SetDrawToolLine();
	void SetDrawToolRectangle();
	void SetDrawToolEllipse();
	void SetDrawToolSelect();
protected:
	void OnPaint(wxPaintEvent & event);
	void OnLButtonDown(wxMouseEvent & event);
	void OnLButtonUp(wxMouseEvent & event);
	void OnMouseMove(wxMouseEvent & event);
	std::vector<DrawingObject *> m_drawingObjects;
	DrawingObject * m_selectedObject;
	DrawingObjectType m_drawType;
	wxPoint m_preMousePosition;
private:
	void Init();
	DECLARE_DYNAMIC_CLASS(DrawingCanvas)
	DECLARE_EVENT_TABLE()
};