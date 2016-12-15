#include "drawingobject.h"
RectangleObject::RectangleObject(const wxPoint & position, const Line & line, const Shape & shape) : DrawingObject(position), m_line(line), m_shape(shape)
{
	std::vector<Tracker> res;
	wxRect rc;
	wxPoint pt2 = GetPosition() + GetSize();
	wxPoint pt1 = GetPosition();
	int left = (pt1.x < pt2.x) ? pt1.x : pt2.x;
	int top = (pt1.y < pt2.y) ? pt1.y : pt2.y;
	int right = (pt1.x < pt2.x) ? pt2.x : pt1.x;
	int bottom = (pt1.y < pt2.y) ? pt2.y : pt1.y;
	int middle = (top + bottom) / 2;
	int center = (left + right) / 2;
	res.push_back(Tracker(wxPoint(left, top), Tracker::Direction::NorthWest, this));
	res.push_back(Tracker(wxPoint(center, top), Tracker::Direction::North, this));
	res.push_back(Tracker(wxPoint(right, top), Tracker::Direction::NorthEast, this));

	res.push_back(Tracker(wxPoint(left, middle), Tracker::Direction::West, this));
	res.push_back(Tracker(wxPoint(right, middle), Tracker::Direction::East, this));

	res.push_back(Tracker(wxPoint(left, bottom), Tracker::Direction::SouthWest, this));
	res.push_back(Tracker(wxPoint(center, bottom), Tracker::Direction::South, this));
	res.push_back(Tracker(wxPoint(right, bottom), Tracker::Direction::SouthEast, this));
	m_trackers = std::move(res);
}
const std::vector<Tracker>& RectangleObject::GetTrackers()
{
	return m_trackers;
}
void RectangleObject::DoDraw(wxDC & dc) const
{
	wxPen pen(m_line.GetColor(), m_line.GetThickness());
	wxBrush brush(m_shape.GetColor());
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.DrawRectangle(GetPosition(), GetSize());
}
void RectangleObject::OnUpdatePosition()
{
	wxPoint pt2 = GetPosition() + GetSize();
	wxPoint pt1 = GetPosition();
	int left = (pt1.x < pt2.x) ? pt1.x : pt2.x;
	int top = (pt1.y < pt2.y) ? pt1.y : pt2.y;
	int right = (pt1.x < pt2.x) ? pt2.x : pt1.x;
	int bottom = (pt1.y < pt2.y) ? pt2.y : pt1.y;
	int middle = (top + bottom) / 2;
	int center = (left + right) / 2;
	for (auto & tracker : m_trackers)
	{
		switch (tracker.GetDirection())
		{
		case Tracker::Direction::NorthWest:
			tracker.SetPosition(wxPoint(left, top));
			break;
		case Tracker::Direction::North:
			tracker.SetPosition(wxPoint(center, top));
			break;
		case Tracker::Direction::NorthEast:
			tracker.SetPosition(wxPoint(right, top));
			break;
		case Tracker::Direction::West:
			tracker.SetPosition(wxPoint(left, middle));
			break;
		case Tracker::Direction::East:
			tracker.SetPosition(wxPoint(right, middle));
			break;
		case Tracker::Direction::SouthWest:
			tracker.SetPosition(wxPoint(left, bottom));
			break;
		case Tracker::Direction::South:
			tracker.SetPosition(wxPoint(center, bottom));
			break;
		case Tracker::Direction::SouthEast:
			tracker.SetPosition(wxPoint(right, bottom));
			break;
		}
	}
}
LineObject::LineObject(const wxPoint & position, const Line & line) : DrawingObject(position), m_line(line)
{
	m_trackers = { Tracker(m_point1,Tracker::Direction::Pt1,this),Tracker(m_point2,Tracker::Direction::Pt2,this) };
}
const std::vector<Tracker>& LineObject::GetTrackers()
{
	return  m_trackers;
}
void LineObject::DoDraw(wxDC & dc) const
{
	wxPen pen(m_line.GetColor(), m_line.GetThickness());
	dc.SetPen(pen);
	dc.DrawLine(m_point1, m_point2);
}

void LineObject::OnUpdatePosition()
{
	for (auto & tracker : m_trackers)
	{
		switch (tracker.GetDirection())
		{
		case Tracker::Direction::Pt1:
			tracker.SetPosition(m_point1);
			break;
		case Tracker::Direction::Pt2:
			tracker.SetPosition(m_point2);
			break;
		}
	}
}

EllipseObject::EllipseObject(const wxPoint & position, const Line & line, const Shape & shape) : DrawingObject(position), m_line(line), m_shape(shape)
{
	std::vector<Tracker> res;
	wxRect rc;
	wxPoint pt2 = GetPosition() + GetSize();
	wxPoint pt1 = GetPosition();
	int left = (pt1.x < pt2.x) ? pt1.x : pt2.x;
	int top = (pt1.y < pt2.y) ? pt1.y : pt2.y;
	int right = (pt1.x < pt2.x) ? pt2.x : pt1.x;
	int bottom = (pt1.y < pt2.y) ? pt2.y : pt1.y;
	int middle = (top + bottom) / 2;
	int center = (left + right) / 2;
	res.push_back(Tracker(wxPoint(left, top), Tracker::Direction::NorthWest, this));
	res.push_back(Tracker(wxPoint(center, top), Tracker::Direction::North, this));
	res.push_back(Tracker(wxPoint(right, top), Tracker::Direction::NorthEast, this));

	res.push_back(Tracker(wxPoint(left, middle), Tracker::Direction::West, this));
	res.push_back(Tracker(wxPoint(right, middle), Tracker::Direction::East, this));

	res.push_back(Tracker(wxPoint(left, bottom), Tracker::Direction::SouthWest, this));
	res.push_back(Tracker(wxPoint(center, bottom), Tracker::Direction::South, this));
	res.push_back(Tracker(wxPoint(right, bottom), Tracker::Direction::SouthEast, this));
	m_trackers = std::move(res);
}

const std::vector<Tracker>& EllipseObject::GetTrackers()
{
	return m_trackers;
}

void EllipseObject::DoDraw(wxDC & dc) const
{
	wxPen pen(m_line.GetColor(), m_line.GetThickness());
	wxBrush brush(m_shape.GetColor());
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.DrawEllipse(GetPosition(), GetSize());
}

void EllipseObject::OnUpdatePosition()
{
	wxPoint pt2 = GetPosition() + GetSize();
	wxPoint pt1 = GetPosition();
	int left = (pt1.x < pt2.x) ? pt1.x : pt2.x;
	int top = (pt1.y < pt2.y) ? pt1.y : pt2.y;
	int right = (pt1.x < pt2.x) ? pt2.x : pt1.x;
	int bottom = (pt1.y < pt2.y) ? pt2.y : pt1.y;
	int middle = (top + bottom) / 2;
	int center = (left + right) / 2;
	for (auto & tracker : m_trackers)
	{
		switch (tracker.GetDirection())
		{
		case Tracker::Direction::NorthWest:
			tracker.SetPosition(wxPoint(left, top));
			break;
		case Tracker::Direction::North:
			tracker.SetPosition(wxPoint(center, top));
			break;
		case Tracker::Direction::NorthEast:
			tracker.SetPosition(wxPoint(right, top));
			break;
		case Tracker::Direction::West:
			tracker.SetPosition(wxPoint(left, middle));
			break;
		case Tracker::Direction::East:
			tracker.SetPosition(wxPoint(right, middle));
			break;
		case Tracker::Direction::SouthWest:
			tracker.SetPosition(wxPoint(left, bottom));
			break;
		case Tracker::Direction::South:
			tracker.SetPosition(wxPoint(center, bottom));
			break;
		case Tracker::Direction::SouthEast:
			tracker.SetPosition(wxPoint(right, bottom));
			break;
		}
	}
}

void DrawingObject::Stratch(Tracker::Direction direction, const wxPoint & delta)
{
	wxPoint pt2 = GetPosition() + GetSize();
	wxPoint pt1 = GetPosition();
	int left = (pt1.x < pt2.x) ? pt1.x : pt2.x;
	int top = (pt1.y < pt2.y) ? pt1.y : pt2.y;
	int right = (pt1.x < pt2.x) ? pt2.x : pt1.x;
	int bottom = (pt1.y < pt2.y) ? pt2.y : pt1.y;

	switch (direction)
	{
	case Tracker::Direction::NorthWest:
		left += delta.x;
		top += delta.y;
		break;
	case Tracker::Direction::North:
		top += delta.y;
		break;
	case Tracker::Direction::NorthEast:
		top += delta.y;
		right += delta.x;
		break;
	case Tracker::Direction::West:
		left += delta.x;
		break;
	case Tracker::Direction::East:
		right += delta.x;
		break;
	case Tracker::Direction::SouthWest:
		bottom += delta.y;
		left += delta.x;
		break;
	case Tracker::Direction::South:
		bottom += delta.y;
		break;
	case Tracker::Direction::SouthEast:
		bottom += delta.y;
		right += delta.x;
		break;
	}

	m_point1.x = left;
	m_point1.y = top;
	m_point2.x = right;
	m_point2.y = bottom;

	OnUpdatePosition();
}

bool DrawingObject::IsHit(const wxPoint & pos)
{
	return false;
}
