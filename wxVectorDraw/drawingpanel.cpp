#include "drawingpanel.h"
wxDEFINE_EVENT(wxEVT_FINISH_DRAW_OBJECT, wxCommandEvent);
IMPLEMENT_DYNAMIC_CLASS(DrawingCanvas, wxWindow)
wxBEGIN_EVENT_TABLE(DrawingCanvas, wxWindow)
EVT_PAINT(DrawingCanvas::OnPaint)
EVT_LEFT_UP(DrawingCanvas::OnLButtonUp)
EVT_LEFT_DOWN(DrawingCanvas::OnLButtonDown)
EVT_MOTION(DrawingCanvas::OnMouseMove)
wxEND_EVENT_TABLE();
void DrawingCanvas::Init()
{
	SetBackgroundColour(wxColor(255, 255, 255));
	m_selectedObject = nullptr;
}

DrawingCanvas::DrawingCanvas()
{
	Init();
}

DrawingCanvas::DrawingCanvas(wxWindow * parent, wxWindowID winId, const wxPoint & pos,const wxSize & size) 
	: wxWindow(parent,winId,pos,size)
{
	m_drawType = DrawingObjectType::NONE;
	Init();
}
DrawingCanvas::~DrawingCanvas()
{
	for (DrawingObject * it : m_drawingObjects)
	{
		delete it;
	}
}
bool DrawingCanvas::Create(wxWindow * parent, wxWindowID winId, const wxPoint & pos, const wxSize & size)
{
	wxWindow::Create(parent, winId, pos, size);
	Init();
	return true;
}
void DrawingCanvas::SetDrawToolLine()
{
	m_drawType = DrawingObjectType::LINE;
}
void DrawingCanvas::SetDrawToolRectangle()
{
	m_drawType = DrawingObjectType::RECTANGLE;
}
void DrawingCanvas::SetDrawToolEllipse()
{
	m_drawType = DrawingObjectType::ELLIPSE;
}
void DrawingCanvas::SetDrawToolSelect()
{
	m_drawType = DrawingObjectType::NONE;
}
void DrawingCanvas::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);
	for (DrawingObject * it : m_drawingObjects)
	{
		it->DoDraw(dc);
	}
}

void DrawingCanvas::OnLButtonDown(wxMouseEvent & event)
{
	wxPoint mousePosition = event.GetPosition();
	switch (m_drawType)
	{
	case DrawingObjectType::RECTANGLE:
		m_selectedObject = 
			new RectangleObject(mousePosition,
				Line(wxColor(0, 0, 0),3),
				Shape(wxColor(0xFF, 0xFF, 0xFF)));
		break;
	case DrawingObjectType::ELLIPSE:
		m_selectedObject = 
			new EllipseObject(mousePosition,
				Line(wxColor(0, 0, 0), 3),
				Shape(wxColor(0xFF, 0xFF, 0xFF)));
		break;
	case DrawingObjectType::LINE:
		m_selectedObject =
			new LineObject(mousePosition,
				Line(wxColor(0, 0, 0), 3));
		break;
	case DrawingObjectType::NONE:
		//객체를 선택하는 과정
		DrawingObject * obj = nullptr;
		int size = m_drawingObjects.size() - 1;
		for (int i = 0 ; i < m_drawingObjects.size() ; i++)
		{
			obj = m_drawingObjects[size - i];
			wxPoint objPos1 = obj->GetPosition();
			wxSize objSize = obj->GetSize();
			wxPoint objPos2 = objPos1 + objSize;
			if (objSize.x < 0 || objSize.y < 0)
			{
				objPos1 = objPos2;
				objPos2 = obj->GetPosition();
			}
			if (objPos1.x > mousePosition.x || objPos1.y > mousePosition.y)
			{
				continue;
			}
			if (objPos2.x < mousePosition.x || objPos2.y < mousePosition.y)
			{
				continue;
			}
			m_selectedObject = obj;
			break;
		}
		if (m_selectedObject != nullptr)
		{
			m_preMousePosition = mousePosition;
		}
		//NONE, 선택상태에서는 push_back를 해서는 안된다. 고로 하기 전에 return한다.
		return;
	}
	m_drawingObjects.push_back(m_selectedObject);
}

void DrawingCanvas::OnLButtonUp(wxMouseEvent & event)
{
	if (m_selectedObject != nullptr)
	{
		if (m_drawType != DrawingObjectType::NONE)
		{
			this->QueueEvent(new wxCommandEvent(wxEVT_FINISH_DRAW_OBJECT));
		}
		m_selectedObject = nullptr;
		Refresh();
	}
}

void DrawingCanvas::OnMouseMove(wxMouseEvent & event)
{
	if (event.LeftIsDown())
	{
		if (m_selectedObject != nullptr)
		{
			auto s = event.GetPosition() - m_selectedObject->GetPosition();
			if (m_drawType != DrawingObjectType::NONE)
			{
				//객체를 그릴 떄는 크기를 지정한다.
				m_selectedObject->SetSize(wxSize(s.x, s.y));
			}
			else
			{
				//객체를 움직일 때는 
				wxPoint delta = event.GetPosition() - m_preMousePosition;
				m_selectedObject->SetPosition(m_selectedObject->GetPosition() + delta);
				m_preMousePosition = event.GetPosition();
			}
			Refresh();
		}
	}
}

