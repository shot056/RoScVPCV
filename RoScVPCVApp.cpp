//---------------------------------------------------------------------------
//
// Name:        RoScVPCVApp.cpp
// Author:      sho
// Created:     2008/01/20 0:47:22
// Description: 
//
//---------------------------------------------------------------------------

#include "RoScVPCVApp.h"
#include "RoScVPCVDlg.h"

IMPLEMENT_APP(RoScVPCVDlgApp)

bool RoScVPCVDlgApp::OnInit()
{
    file.Open(wxT("./debug.log"), wxT("w"));
    wxLog *logger = new wxLogStderr(file.fp());
    wxLog::SetActiveTarget(logger);
    
    RoScVPCVDlg* dialog = new RoScVPCVDlg(NULL, -1, wxT("RoScVPCV"), wxDefaultPosition, wxDefaultSize);
    SetTopWindow(dialog);
    dialog->Show(true);
    
    int PacketLength[9999];
    int MaxNum = 0;
    if(!dialog->LoadPacketLength(PacketLength, MaxNum)) {
        wxMessageBox(wxT("PacketLengthが読み込めませんでした。"),
                     wxT("エラー"),
                     wxOK);
        return false;
    }
    thread = new CaptureThread(dialog);
    thread->SetPacketLength(PacketLength, MaxNum);
    if ( thread->Create() != wxTHREAD_NO_ERROR ) {
        wxMessageBox(wxT("スレッドが作成できませんでした。"),
                     wxT("エラー"),
                     wxOK);
        return false;
    }
    if( thread->Run() != wxTHREAD_NO_ERROR ) {
        wxMessageBox(wxT("スレッドが開始できませんでした。"),
                     wxT("エラー"),
                     wxOK);
        return false;
    }
    thread->StartCapture();
    return true;
}
 
int RoScVPCVDlgApp::OnExit()
{
    thread->StopCapture();
    thread->SetRoopFlag(false);
    while(thread->GetAlive() && thread->IsAlive());
    //    thread->Delete();
    //    delete thread;
    file.Close();
    return 0;
}
