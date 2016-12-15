#pragma once
#include<wx/wx.h>
#include "tracker.h"
enum DrawingObjectType : unsigned long
{
	NONE,
	RECTANGLE,
	ELLIPSE,
	LINE
};

class DrawingObject
{
public:
	DrawingObject(const wxPoint & position) :m_point1(position), m_point2(position){}
	virtual  ~DrawingObject() { }
	void SetPosition(const wxPoint& pos)
	{
		wxPoint virtualPosition = GetPosition();
		wxPoint delta = pos - virtualPosition;
		m_point1 += delta;
		m_point2 += delta;
		OnUpdatePosition();
	}
	void Move(const wxPoint & delta)
	{
		m_point1 += delta;
		m_point2 += delta;
		OnUpdatePosition();
	}
	void Stratch(Tracker::Direction direction, const wxPoint & delta);
	virtual wxSize GetSize() const
	{
		wxPoint s = m_point1 - m_point2;
		return wxSize(abs(s.x),abs(s.y));
	}
	wxPoint GetPosition() const
	{
		wxPoint virtualPosition;
		virtualPosition.x = (m_point1.x < m_point2.x) ? m_point1.x : m_point2.x;
		virtualPosition.y = (m_point1.y < m_point2.y) ? m_point1.y : m_point2.y;
		return virtualPosition;
	}
	void SetPoint1(const wxPoint & pt)
	{
		m_point1 = pt;
		OnUpdatePosition(); 
	}
	void SetPoint2(const wxPoint & pt)
	{
		m_point2 = pt;
		OnUpdatePosition();
	}
	wxPoint GetPoint1() const
	{
		return m_point1;
	}
	wxPoint GetPoint2() const
	{
		return m_point2;
	}
	virtual bool IsHit(const wxPoint & pos);
	virtual void DoDraw(wxDC & dc) const = 0;
	virtual const std::vector<Tracker>& GetTrackers() = 0;
protected:
	virtual void OnUpdatePosition() = 0;
	wxPoint m_point1;
	wxPoint m_point2;
};
class Line
{
public:
	Line(const wxColor & color, int thickness) : m_color(color), m_thickness(thickness)
	{

	}
	wxColor GetColor() const
	{
		return m_color;
	}
	int GetThickness() const
	{ 
		return m_thickness;
	}
protected:
	wxColor m_color;
	int m_thickness;
};
class Shape
{
public:
	Shape(const wxColor & color) : m_color(color)
	{

	}
	wxColor GetColor() const
	{
		return m_color;
	}
protected:
	wxColor m_color;
};
class RectangleObject : public DrawingObject
{
public:
	RectangleObject(const wxPoint & position, const Line & line, const Shape & shape);
	~RectangleObject() { m_trackers.clear(); }
	void SetLine(const Line & line)
	{
		m_line = line;
	}
	Line GetLine() const
	{
		return m_line;
	}
	void SetShape(const Shape & shape)
	{
		m_shape = shape;
	}
	Shape GetShape() const
	{
		return m_shape;
	}
	const std::vector<Tracker>& GetTrackers() override;
	virtual void DoDraw(wxDC & dc) const override;
protected:
	virtual void OnUpdatePosition() override;
	std::vector<Tracker> m_trackers;
	Line m_line;
	Shape m_shape;
};
class EllipseObject : public DrawingObject
{
public:
	EllipseObject(const wxPoint & position, const Line & line, const Shape & shape);
	~EllipseObject() { m_trackers.clear(); }
	void SetLine(const Line & line)
	{
		m_line = line;
	}
	Line GetLine() const
	{
		return m_line;
	}
	void SetShape(const Shape & shape)
	{
		m_shape = shape;
	}
	Shape GetShape() const
	{
		return m_shape;
	}
	const std::vector<Tracker>& GetTrackers() override;
	virtual void DoDraw(wxDC & dc) const override;
protected:
	virtual void OnUpdatePosition() override;
	std::vector<Tracker> m_trackers;
	Line m_line;
	Shape m_shape;
};
class LineObject : public DrawingObject
{
public:
	LineObject(const wxPoint & position, const Line & line);
	~LineObject() { m_trackers.clear(); }
	void SetLine(const Line & line)
	{
		m_line = line;
	}
	Line GetLine() const
	{
		return m_line;
	}
	const std::vector<Tracker>& GetTrackers() override;
	virtual void DoDraw(wxDC & dc) const override;
protected:
	virtual void OnUpdatePosition() override;
	std::vector<Tracker> m_trackers;
	Line m_line;
};
