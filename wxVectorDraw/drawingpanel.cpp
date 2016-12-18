#include "drawingpanel.h"
#include "propertydialog.h"
#include <wx/dcbuffer.h>
wxDEFINE_EVENT(wxEVT_FINISH_DRAW_OBJECT, wxCommandEvent);
IMPLEMENT_DYNAMIC_CLASS(DrawingCanvas, wxWindow)
wxBEGIN_EVENT_TABLE(DrawingCanvas, wxWindow)
EVT_PAINT(DrawingCanvas::OnPaint)
EVT_LEFT_UP(DrawingCanvas::OnLButtonUp)
EVT_LEFT_DOWN(DrawingCanvas::OnLButtonDown)
EVT_RIGHT_DOWN(DrawingCanvas::OnRButtonDown)
EVT_MOTION(DrawingCanvas::OnMouseMove)
EVT_MENU(6001,DrawingCanvas::OnClickProperty)
EVT_MENU(6002, DrawingCanvas::OnClickDelete)
EVT_MENU(6003, DrawingCanvas::OnClickMoveUpZindex)
EVT_MENU(6004, DrawingCanvas::OnClickMoveDownZindex)
wxEND_EVENT_TABLE();

void DrawingCanvas::Init()
{
	SetBackgroundColour(wxColor(255, 255, 255));
	m_selectedObject = nullptr;
	m_trackerDirection = Tracker::Direction::None;
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
	wxBufferedPaintDC dc(this);
	dc.Clear();
	for (DrawingObject * it : m_drawingObjects)
	{
		it->DoDraw(dc);
	}
	if (m_selectedObject != nullptr && m_drawType == DrawingObjectType::NONE)
	{
		for (auto & tracker : m_selectedObject->GetTrackers())
		{
			tracker.DoDraw(dc);
		}
	}
}
DrawingObject * DrawingCanvas::HitTest(const wxPoint & pos)
{
	DrawingObject * obj = nullptr;
	int size = m_drawingObjects.size() - 1;
	for (int i = 0; i < m_drawingObjects.size(); i++)
	{
		obj = m_drawingObjects[size - i];
		wxPoint objPos1 = obj->GetPosition();
		wxSize objSize = obj->GetSize();
		wxPoint objPos2 = objPos1 + objSize;
		if (objPos1.x > objPos2.x)
		{
			int t = objPos1.x;
			objPos1.x = objPos2.x;
			objPos2.x = t;
		}
		if (objPos1.y > objPos2.y)
		{
			int t = objPos1.y;
			objPos1.y = objPos2.y;
			objPos2.y = t;
		}
		if ((pos.x < objPos1.x || pos.y < objPos1.y) ||
			(pos.x > objPos2.x || pos.y > objPos2.y))
		{
			continue;
		}
		return obj;
	}
	return nullptr;
}

void DrawingCanvas::OnLButtonDown(wxMouseEvent & event)
{
	wxPoint mousePosition = event.GetPosition();
	m_preMousePosition = mousePosition;
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
		if (m_selectedObject != nullptr)
		{
			wxPoint objPos1 = m_selectedObject->GetPosition();
			wxSize objSize = m_selectedObject->GetSize();
			wxPoint objPos2 = objPos1 + objSize;
			
			if (objPos1.x > objPos2.x)
			{
				int t = objPos1.x;
				objPos1.x = objPos2.x;
				objPos2.x = t;
			}
			if (objPos1.y > objPos2.y)
			{
				int t = objPos1.y;
				objPos1.y = objPos2.y;
				objPos2.y = t;
			}
			m_trackerDirection = Tracker::Direction::None;
			for (auto & tracker : m_selectedObject->GetTrackers())
			{
				if (tracker.IsHit(event.GetPosition()))
				{
					m_trackerDirection = tracker.GetDirection();
					break;
				}
			}
			if (m_trackerDirection == Tracker::Direction::None)
			{
				//만약에 트래커를 누른 상태가 아닌데, 선택한 도형의 위에 다른 도형이 있어 가렸을 경우, 다른 도형을 선택한다.
				m_selectedObject = HitTest(mousePosition);
			}
			
			Refresh(false);
		}
		//객체를 선택하는 과정
		if (m_selectedObject == nullptr)
		{
			m_selectedObject = HitTest(mousePosition);
			Refresh(false);
		}
		//NONE, 선택상태에서는 push_back를 해서는 안된다. 고로 하기 전에 return한다.
		return;
	}
	m_drawingObjects.push_back(m_selectedObject);
	Refresh(false);
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
		
		Refresh(false);
	}
	m_trackerDirection = Tracker::Direction::None;
}

void DrawingCanvas::OnMouseMove(wxMouseEvent & event)
{
	if (event.LeftIsDown())
	{
		if (m_selectedObject != nullptr)
		{
			//auto s = event.GetPosition() - m_selectedObject->GetPosition();
			if (m_drawType != DrawingObjectType::NONE)
			{
				//객체를 그릴 떄는 크기를 지정한다.
				m_selectedObject->SetPoint2(event.GetPosition());
			}
			else
			{
				//트래커를 누른 것이 아닐 때는 이동을 한다. 
				wxPoint delta = event.GetPosition() - m_preMousePosition;
				m_preMousePosition = event.GetPosition();
				wxPoint pos = m_selectedObject->GetPosition();
				wxSize size = m_selectedObject->GetSize();
				wxPoint mousePosition = event.GetPosition();
				switch (m_trackerDirection)
				{
				case Tracker::Direction::Pt1:
					m_selectedObject->SetPoint1(m_selectedObject->GetPoint1() + delta);
					break;
				case Tracker::Direction::Pt2:
					m_selectedObject->SetPoint2(m_selectedObject->GetPoint2() + delta);
					break;
				case Tracker::Direction::None:
					m_selectedObject->Move(delta);
					break;
				default:
					m_selectedObject->Stratch(m_trackerDirection, delta);
					break;
				}
					
			}
		}
		Refresh(false);
	}
	if (m_selectedObject != nullptr && m_drawType == DrawingObjectType::NONE)
	{
		this->SetCursor(*wxSTANDARD_CURSOR);
		for (auto & tracker : m_selectedObject->GetTrackers())
		{
			if (tracker.IsHit(event.GetPosition()))
			{
				switch (tracker.GetDirection())
				{
				case Tracker::Direction::NorthWest:
				case Tracker::Direction::SouthEast:
					this->SetCursor(wxStockCursor::wxCURSOR_SIZENWSE);
					break;
				case Tracker::Direction::North:
				case Tracker::Direction::South:
					this->SetCursor(wxStockCursor::wxCURSOR_SIZENS);
					break;
				case Tracker::Direction::West:
				case Tracker::Direction::East:
					this->SetCursor(wxStockCursor::wxCURSOR_SIZEWE);
					break;
				case Tracker::Direction::NorthEast:
				case Tracker::Direction::SouthWest:
					this->SetCursor(wxStockCursor::wxCURSOR_SIZENESW);
					break;
				default:
					this->SetCursor(wxStockCursor::wxCURSOR_SIZING);
				}
				break;
			}
		}
	}
}

void DrawingCanvas::OnRButtonDown(wxMouseEvent & event)
{
	if (m_drawType != DrawingObjectType::NONE)
	{
		return;
	}
	m_selectedObject = HitTest(event.GetPosition());
	if (m_selectedObject != nullptr)
	{
		wxMenu  menu;
		menu.Append(6001, wxT("속성"));
		menu.Append(6002, wxT("삭제"));
		menu.AppendSeparator();
		menu.Append(6003, wxT("위로"));
		menu.Append(6004, wxT("아래로"));
		Refresh(false);
		this->PopupMenu(&menu, event.GetPosition());
	}
	else
	{
		Refresh(false);
	}
}

void DrawingCanvas::OnClickDelete(wxCommandEvent & e)
{
	if (m_selectedObject == nullptr || m_drawType != DrawingObjectType::NONE)
	{
		return;
	}
	for (auto it = m_drawingObjects.begin(); it != m_drawingObjects.end(); it++)
	{
		if (*it == m_selectedObject)
		{
			m_drawingObjects.erase(it);
			delete m_selectedObject;
			m_selectedObject = nullptr;
			Refresh(false);
			return;
		}
	}
	
}
void DrawingCanvas::OnClickProperty(wxCommandEvent & e)
{
	auto * dialog =new PropertyDialog(this, m_selectedObject);
	if (dialog->ShowModal() == wxID_OK)
	{
		Line line = dialog->GetLine();
		Shape shape = dialog->GetShpae();
		/*object를 캐스팅을 시도한다*/

		RectangleObject * ractangleObject = dynamic_cast<RectangleObject*>(m_selectedObject);
		if (ractangleObject != nullptr)
		{
			ractangleObject->SetLine(line);
			ractangleObject->SetShape(shape);
			Refresh(false);
			delete dialog;
			return;
		}
		EllipseObject * ellipseObject = dynamic_cast<EllipseObject*>(m_selectedObject);
		if (ellipseObject != nullptr)
		{
			ellipseObject->SetLine(line);
			ellipseObject->SetShape(shape);
			Refresh(false);
			delete dialog;
			return;
		}
		LineObject * lineObject = dynamic_cast<LineObject*>(m_selectedObject);
		if (lineObject != nullptr)
		{
			lineObject->SetLine(line);
			Refresh(false);
			delete dialog;
			return;
		}
	}
	delete dialog;
}

void DrawingCanvas::OnClickMoveUpZindex(wxCommandEvent & e)
{
	if (m_drawingObjects.back() == m_selectedObject)
	{
		return;
	}
	for (auto it = m_drawingObjects.begin(); it != m_drawingObjects.end(); it++)
	{
		if (*it == m_selectedObject)
		{
			*it = *(it + 1);
			++it;
			*it = m_selectedObject;
			Refresh(false);
			return;
		}
	}
}

void DrawingCanvas::OnClickMoveDownZindex(wxCommandEvent & e)
{
	if (m_drawingObjects.front() == m_selectedObject)
	{
		return;
	}
	for (auto it = m_drawingObjects.begin(); it != m_drawingObjects.end(); it++)
	{
		if (*it == m_selectedObject)
		{
			*it = *(it - 1);
			--it;
			*it = m_selectedObject;
			Refresh(false);
			return;
		}
	}
}



