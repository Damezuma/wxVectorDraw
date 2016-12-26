#pragma once
#include<wx/wx.h>
class CanvasGridManager
{
public:
	CanvasGridManager(int gap = 16);
	int GetGap()
	{
		return m_gap;
	}
	void SetGap(int gap)
	{
		m_gap = gap;
	}
	wxPoint GetGridFitPoint(const wxPoint & pos);
	void Draw(wxDC & dc);
private:
	int m_gap;
};