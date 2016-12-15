#include "tracker.h"
#include "drawingobject.h"
Tracker::Tracker(const wxPoint & pos, Tracker::Direction direction, DrawingObject * relativeObject)
{
	m_rect =
		wxRect(pos.x - Tracker::SIZE / 2,
			pos.y - Tracker::SIZE / 2,
			Tracker::SIZE , 
			Tracker::SIZE);
	m_direction = direction;
	m_relativeObjects = relativeObject;
}

void Tracker::DoDraw(wxDC & dc) const
{
	wxPen pen(wxColor(0, 0, 0), 1);
	wxBrush brush(wxColor(0xFF, 0xFF, 0xFF));

	dc.SetPen(pen);
	dc.SetBrush(brush);

	dc.DrawRectangle(m_rect);
}

void Tracker::SetPosition(const wxPoint & pos)
{
	m_rect =
		wxRect(pos.x - Tracker::SIZE / 2,
			pos.y - Tracker::SIZE / 2,
			Tracker::SIZE,
			Tracker::SIZE);
}

Tracker::Direction Tracker::GetDirection() const
{
	return m_direction;
}

bool Tracker::IsHit(const wxPoint & pos) const
{
	return m_rect.Contains(pos);
}
