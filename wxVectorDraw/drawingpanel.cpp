#include "drawingpanel.h"
wxDEFINE_EVENT(wxEVT_FINISH_DRAW_OBJECT, wxCommandEvent);
IMPLEMENT_DYNAMIC_CLASS(DrawingCanvas, wxWindow)
wxBEGIN_EVENT_TABLE(DrawingCanvas, wxWindow)
EVT_PAINT(DrawingCanvas::OnPaint)
EVT_LEFT_UP(DrawingCanvas::OnLButtonUp)
EVT_LEFT_DOWN(DrawingCanvas::OnLButtonDown)
EVT_MOTION(DrawingCanvas::OnMouseMove)
wxEND_EVENT_TABLE();
float relativePoints[][2] = { { 0.f,0.f },{ 0.5f,0.f },{ 1.f,0.f },{ 0.f,0.5f },{ 1.f,0.5f },{ 0.f,1.f },{ 0.5f,1.f },{ 1.f,1.f } };
wxSize trackerSize(8, 8);
wxBrush trackerBrush(wxColor(0xFF, 0xFF, 0xFF));
wxPen trackerLine(wxColor(0, 0, 0), 1);

Tracker CheckTrackerSelect(DrawingObject * obj,const  wxPoint & position)
{
	wxPoint pos = obj->GetPosition();
	wxSize size = obj->GetSize();
	wxPoint mousePosition = position;
	for (int i = 0; i < 8; i++)
	{
		auto & point = relativePoints[i];
		wxPoint pt1(point[0] * size.x + pos.x - trackerSize.x / 2, point[1] * size.y + pos.y - trackerSize.y / 2);
		wxPoint pt2(point[0] * size.x + pos.x + trackerSize.x / 2, point[1] * size.y + pos.y + trackerSize.y / 2);
		if (pt1.x > mousePosition.x || pt1.y > mousePosition.y)
		{
			continue;
		}
		if (pt2.x < mousePosition.x || pt2.y < mousePosition.y)
		{
			continue;
		}
		return (Tracker)i;
	}
	return Tracker::None;
}

void DrawingCanvas::Init()
{
	SetBackgroundColour(wxColor(255, 255, 255));
	m_selectedObject = nullptr;
	m_trackerSelect = Tracker::None;
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
	m_selectedObject = nullptr;
	m_drawType = DrawingObjectType::NONE;
}
void DrawingCanvas::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);
	for (DrawingObject * it : m_drawingObjects)
	{
		it->DoDraw(dc);
	}
	if (m_selectedObject != nullptr && m_drawType == DrawingObjectType::NONE)
	{
		
		wxPoint pos = m_selectedObject->GetPosition();
		wxSize size = m_selectedObject->GetSize();

		dc.SetPen(trackerLine);
		dc.SetBrush(trackerBrush);
		for (auto& point : relativePoints)
		{
			wxPoint pt(point[0] * size.x + pos.x - trackerSize.x/2, point[1] * size.y + pos.y - trackerSize.y / 2);
			dc.DrawRectangle(pt, trackerSize);
		}
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
		m_trackerSelect = CheckTrackerSelect(m_selectedObject, event.GetPosition());
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
			m_selectedObject = nullptr;
		}
		
		Refresh();
	}
	m_trackerSelect = Tracker::None;
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
				//트래커를 누른 것이 아닐 때는 이동을 한다. 
				wxPoint delta = event.GetPosition() - m_preMousePosition;
				m_preMousePosition = event.GetPosition();
				wxPoint pos = m_selectedObject->GetPosition();
				wxSize size = m_selectedObject->GetSize();
				wxPoint mousePosition = event.GetPosition();
				switch (m_trackerSelect)
				{
				case Tracker::NorthWest:
					pos += delta;
					delta.x *= -1;
					delta.y *= -1;
				case Tracker::SouthEast:
					size.x += delta.x;
					size.y += delta.y;
					break;
				case Tracker::North:
					pos.y += delta.y;
					delta.y *= -1;
				case Tracker::South:
					size.y += delta.y;
					break;
				case Tracker::West:
					pos.x += delta.x;
					delta.x *= -1;
				case Tracker::East:
					size.x += delta.x;
					break;
				case Tracker::NorthEast:
					pos.y += delta.y;
					size.x += delta.x;
					size.y -= delta.y;
					break;
				case Tracker::SouthWest:
					pos.x  += delta.x;
					size.y += delta.y;
					size.x -= delta.x;
					break;
				default:
					pos += delta;
					break;
				}
				m_selectedObject->SetPosition(pos);
				m_selectedObject->SetSize(size);
			}
			Refresh();
		}
	}
	else if (m_selectedObject != nullptr && m_drawType == DrawingObjectType::NONE)
	{
		switch (CheckTrackerSelect(m_selectedObject, event.GetPosition()))
		{
		case Tracker::NorthWest:
		case Tracker::SouthEast:
			this->SetCursor(wxStockCursor::wxCURSOR_SIZENWSE);
			break;
		case Tracker::North:
		case Tracker::South:
			this->SetCursor(wxStockCursor::wxCURSOR_SIZENS);
			break;
		case Tracker::West:
		case Tracker::East:
			this->SetCursor(wxStockCursor::wxCURSOR_SIZEWE);
			break;
		case Tracker::NorthEast:
		case Tracker::SouthWest:
			this->SetCursor(wxStockCursor::wxCURSOR_SIZENESW);
			break;
		default:
			this->SetCursor(*wxSTANDARD_CURSOR);
		}
	}
}

