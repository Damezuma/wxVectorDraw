#pragma once
#include<wx/wx.h>
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
	DrawingObject(const wxPoint & position) :m_position(position), m_size(0,0){}
	~DrawingObject(){}
	void SetPosition(const wxPoint& pos)
	{
		m_position = pos;
	}
	virtual void SetSize(const wxSize& size)
	{
		m_size = size;
	}
	virtual wxSize GetSize() const
	{
		return m_size;
	}
	wxPoint GetPosition() const
	{
		return m_position;
	}
	virtual void DoDraw(wxDC & dc) const = 0;
protected:
	wxSize m_size;
	wxPoint m_position; 
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
	RectangleObject(const wxPoint & position, const Line & line, const Shape & shape) : DrawingObject(position),m_line(line),m_shape(shape)
	{

	}
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
	virtual void DoDraw(wxDC & dc) const override;
protected:
	Line m_line;
	Shape m_shape;
};
class EllipseObject : public DrawingObject
{
public:
	EllipseObject(const wxPoint & position, const Line & line, const Shape & shape) : DrawingObject(position), m_line(line), m_shape(shape)
	{

	}
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
	virtual void DoDraw(wxDC & dc) const override;
protected:
	Line m_line;
	Shape m_shape;
};
class LineObject : public DrawingObject
{
public:
	LineObject(const wxPoint & position, const Line & line) : DrawingObject(position), m_line(line)
	{

	}
	void SetLine(const Line & line)
	{
		m_line = line;
	}
	Line GetLine() const
	{
		return m_line;
	}
	virtual void DoDraw(wxDC & dc) const override;
protected:
	Line m_line;
};
