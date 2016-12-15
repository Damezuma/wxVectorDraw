#pragma once
#include <wx/wx.h>
#include <wx/vector.h>
class DrawingObject;
class Tracker
{
public:
	enum Direction
	{
		None,
		NorthWest,
		North,
		NorthEast,
		West,
		East,
		SouthWest,
		South,
		SouthEast,
		Pt1,
		Pt2
	};
	Tracker(const wxPoint & pos, Tracker::Direction direction,DrawingObject * relativeObject);
	void DoDraw(wxDC & dc) const;
	void SetPosition(const wxPoint& pos);
	Tracker::Direction GetDirection()  const;
	bool IsHit(const wxPoint & pos) const;
	
private:
	static const int SIZE = 8;
	wxRect m_rect;
	Tracker::Direction m_direction;
	DrawingObject* m_relativeObjects;
};
