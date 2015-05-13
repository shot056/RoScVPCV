//---------------------------------------------------------------------------
//
// Name:        ConfigDlg.cpp
// Author:      sho
// Created:     2008/01/20 2:00:47
// Description: ConfigDlg class implementation
//
//---------------------------------------------------------------------------

#include "ConfigDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// ConfigDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(ConfigDlg, wxDialog)
	////Manual Code Start
	////Manual Code End
	EVT_CLOSE(ConfigDlg::OnClose)
END_EVENT_TABLE()
////Event Table End

ConfigDlg::ConfigDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

ConfigDlg::~ConfigDlg()
{
} 

void ConfigDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start
	////GUI Items Creation End
}

void ConfigDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}
