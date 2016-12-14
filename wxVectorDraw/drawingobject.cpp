#include "drawingobject.h"

void RectangleObject::DoDraw(wxDC & dc) const
{
	wxPen pen(m_line.GetColor(), m_line.GetThickness());
	wxBrush brush(m_shape.GetColor());
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.DrawRectangle(m_position, m_size);
}

void LineObject::DoDraw(wxDC & dc) const
{
	wxPen pen(m_line.GetColor(), m_line.GetThickness());
	dc.SetPen(pen);
	wxPoint pt2 = m_position + m_size;
	dc.DrawLine(m_position, pt2);
}

void EllipseObject::DoDraw(wxDC & dc) const
{
	wxPen pen(m_line.GetColor(), m_line.GetThickness());
	wxBrush brush(m_shape.GetColor());
	dc.SetPen(pen);
	dc.SetBrush(brush);
	dc.DrawEllipse(m_position, m_size);
}
