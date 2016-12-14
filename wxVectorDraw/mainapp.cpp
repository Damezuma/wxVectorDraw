#include <wx/wx.h>
#include "mainframe.h"
class wxVectorDrawApp : public wxApp
{
public:
	virtual bool OnInit() override
	{
		DrawMainFrame * frame = new DrawMainFrame();
		frame->Show();
		return true;
	}
};
IMPLEMENT_APP(wxVectorDrawApp);