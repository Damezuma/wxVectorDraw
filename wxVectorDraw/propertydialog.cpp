#include "propertydialog.h"

PropertyDialog::PropertyDialog(wxWindow * parent, DrawingObject * object) : wxDialog(parent, wxID_ANY, wxT("속성"))
{
	ui_lineColorPicker = new wxColourPickerCtrl(this, wxID_ANY);
	ui_shapeFillColorPicker = new wxColourPickerCtrl(this, wxID_ANY);
	ui_lineThicknessSelecter = new wxSpinCtrl(this, wxID_ANY);

	wxFlexGridSizer * bSizer = new wxFlexGridSizer(3, 2, wxSize(0, 0));
	bSizer->AddGrowableCol(1);
	bSizer->Add(new wxStaticText(this, wxID_ANY, wxT("선 색상")));
	bSizer->Add(ui_lineColorPicker, 0, wxEXPAND);
	bSizer->Add(new wxStaticText(this, wxID_ANY, wxT("선 두께")));
	bSizer->Add(ui_lineThicknessSelecter, 0, wxEXPAND);
	bSizer->Add(new wxStaticText(this, wxID_ANY, wxT("면 색상")));
	bSizer->Add(ui_shapeFillColorPicker, 0, wxEXPAND);

	wxStdDialogButtonSizer * sizer = new wxStdDialogButtonSizer();
	sizer->AddButton(new wxButton(this, wxID_OK));
	sizer->AddButton(new wxButton(this, wxID_CANCEL));
	sizer->Realize();
	wxSizer * bsizer2 = new wxBoxSizer(wxVERTICAL);
	bsizer2->Add(bSizer, 0, wxEXPAND);
	bsizer2->Add(sizer, 0, wxEXPAND);

	SetSizer(bsizer2);

	Fit();
	Layout();

	/*object를 캐스팅을 시도한다*/
	RectangleObject * ractangleObject = dynamic_cast<RectangleObject*>(object);
	if (ractangleObject != nullptr)
	{
		Line line = ractangleObject->GetLine();
		ui_lineColorPicker->SetColour(line.GetColor());
		ui_lineThicknessSelecter->SetValue(line.GetThickness());

		Shape shape = ractangleObject->GetShape();
		ui_shapeFillColorPicker->SetColour(shape.GetColor());
		return;
	}
	EllipseObject * ellipseObject = dynamic_cast<EllipseObject*>(object);
	if (ellipseObject != nullptr)
	{
		Line line = ellipseObject->GetLine();
		ui_lineColorPicker->SetColour(line.GetColor());
		ui_lineThicknessSelecter->SetValue(line.GetThickness());

		Shape shape = ellipseObject->GetShape();
		ui_shapeFillColorPicker->SetColour(shape.GetColor());
		return;
	}
	LineObject * lineObject = dynamic_cast<LineObject*>(object);
	if (lineObject != nullptr)
	{
		Line line = lineObject->GetLine();
		ui_lineColorPicker->SetColour(line.GetColor());
		ui_lineThicknessSelecter->SetValue(line.GetThickness());

		ui_shapeFillColorPicker->Enable(false);
		return;
	}
}

Line PropertyDialog::GetLine()
{
	wxColor color = this->ui_lineColorPicker->GetColour();
	int thickness = this->ui_lineThicknessSelecter->GetValue();
	return Line(color,thickness);
}

Shape PropertyDialog::GetShpae()
{
	wxColor color = this->ui_shapeFillColorPicker->GetColour();
	return Shape(color);
}
