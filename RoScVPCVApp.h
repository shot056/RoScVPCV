//---------------------------------------------------------------------------
//
// Name:        RoScVPCVApp.h
// Author:      sho
// Created:     2008/01/20 0:47:22
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __ROSCVPCVDLGApp_h__
#define __ROSCVPCVDLGApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

#include <wx/ffile.h>
#include <wx/log.h>
#include "CaptureThread.h"

class RoScVPCVDlgApp : public wxApp
{
public:
    bool OnInit();
    int OnExit();
private:
    CaptureThread* thread;
    wxFFile file;
};

#endif
