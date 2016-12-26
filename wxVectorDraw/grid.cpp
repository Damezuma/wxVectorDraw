#include "grid.h"

CanvasGridManager::CanvasGridManager(int gap)
{
	m_gap = gap;
}

wxPoint CanvasGridManager::GetGridFitPoint(const wxPoint & pos)
{
	wxPoint res;
	res.x =(pos.x / m_gap) * m_gap;
	res.y = (pos.y / m_gap) * m_gap;
	return res;
}

void CanvasGridManager::Draw(wxDC & dc)
{
	wxPen pen(wxColor(0xBB, 0xBB, 0xBB), 1);
	wxSize size = dc.GetSize();
	
	wxPoint pt(0, 0);
	wxPoint pt2(0, 0);
	dc.SetPen(pen);
	for(int i = m_gap ; i < size.x; i+=m_gap)
	{
		pt2.x = i;
		pt2.y = size.y;
		pt.x = i;
		pt.y = 0;
		dc.DrawLine(pt, pt2);
	}
	pt.x = 0;
	pt.y = 0;
	for (int i = m_gap; i < size.y; i += m_gap)
	{
		pt2.y = i;
		pt2.x = size.x;
		pt.y = i;
		pt.x = 0;
		dc.DrawLine(pt, pt2);
	}
}
