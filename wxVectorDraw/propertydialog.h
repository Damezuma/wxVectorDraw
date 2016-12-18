#pragma once
#include<wx/wx.h>
#include<wx/clrpicker.h>
#include<wx/spinctrl.h>
#include "drawingobject.h"


class PropertyDialog : public wxDialog
{
public:
	PropertyDialog(wxWindow * parent, DrawingObject * object);
	Line GetLine();
	Shape GetShpae();
private:
	wxColourPickerCtrl * ui_lineColorPicker;
	wxColourPickerCtrl * ui_shapeFillColorPicker;
	wxSpinCtrl * ui_lineThicknessSelecter;
};
