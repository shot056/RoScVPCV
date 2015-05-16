//---------------------------------------------------------------------------
//
// Name:        RoScVPCVDlg.h
// Author:      sho
// Created:     2008/01/20 0:47:22
// Description: RoScVPCVDlg class declaration
//
//---------------------------------------------------------------------------

#ifndef __ROSCVPCVDLG_h__
#define __ROSCVPCVDLG_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif
#include <wx/config.h>
#include <wx/dir.h>
#include <wx/regex.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/log.h>

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/menu.h>
////Header Include End

////Dialog Style Start
#undef RoScVPCVDlg_STYLE
#define RoScVPCVDlg_STYLE wxDIALOG_NO_PARENT
////Dialog Style End
#include "ShortcutItem.h"

//WX_DECLARE_STRING_HASH_MAP( wxString, MyStringHash );

class RoScVPCVDlg : public wxDialog
{
private:
    DECLARE_EVENT_TABLE();
    
public:
    RoScVPCVDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("RoScVPCV"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = RoScVPCVDlg_STYLE);
    virtual ~RoScVPCVDlg();
    
    // �E�N���b�N���j���[�\�� 
    void RoScVPCVDlgRightDown(wxMouseEvent& event);
    
    // �E�N���b�N->�e�X�g 
    void Test(wxCommandEvent& WXUNUSED(event));
    // �E�N���b�N->�I�� 
    void OnQuit(wxCommandEvent& WXUNUSED(event));
    // �E�N���b�N->�X�L���ύX 
    void ChangeSkin(wxCommandEvent& WXUNUSED(event));
    // �E�N���b�N->��ɍőO�� 
    void ChangeStayOnTop(wxCommandEvent& WXUNUSED(event));
    
    // �E�B���h�E�ړ��̂��� 
    void RoScVPCVDlgLeftDown(wxMouseEvent& event);
    void RoScVPCVDlgLeftUP(wxMouseEvent& event);
    void RoScVPCVDlgMouseEvents(wxMouseEvent& event);
    
    // RoScVPCV.ini�����[�h���� 
    bool LoadMainIni();
    // RoScVPCV.ini���Z�[�u���� 
    bool SaveMainIni();

    // ���W�X�g���ɓo�^����Ă���X�L�����擾���� 
    wxString GetRegistrySkin();
    // �X�L�������[�h���� 
    bool LoadSkin(wxString Skin);
    // ini�����[�h���� 
    bool LoadIni(MyStringHash *hash, wxString File, wxString Section);
    // ini���Z�[�u���� 
    bool SaveIni(MyStringHash *hash, wxString File, wxString Section); 
    // Skills.ini�����[�h���� 
    bool LoadSkillsIni();
    // PacketLength�����[�h���� 
    bool LoadPacketLength(int *PacketLength, int &MaxNum);
    // �A�C�e�����e�[�u�������[�h���� 
    bool LoadItemNameTable(MyStringHash *hash);
    
    // �ꏊ�ړ� 
    bool PosMove();
     
    // ��ʕ`�� 
    void RoScVPCVDlgPaint(wxPaintEvent& event);

    // �X�L���f�[�^�Z�b�g 
    bool SetShortcutItem(int item_count, int type, int id, int count);

    // �A�N�e�B�u�X�L�����Z�b�g 
    bool ResetActiveSkill();

    // �A�N�e�B�u�X�L���Z�b�g 
    bool SetActiveSkill(int id, int lv);

    // �X�L�����A�N�e�B�u���Ԃ� 
    bool SkillIsActive(int id, int lv);

    // �A�C�e���̃A�N�e�B�u/�m���A�N�e�B�u��ݒ肵�Ȃ��� 
    bool SetItemsActive();
     
    // NIC�̔ԍ� 
    int NICIndex;
    
    // �|�W�V����X
    int PositionX;
    // �|�W�V����Y
    int PositionY;

    
private:
    //Do not add custom control declarations between 
    //GUI Control Declaration Start and GUI Control Declaration End.
    //wxDev-C++ will remove them. Add custom code after the block.
    ////GUI Control Declaration Start
    wxMenu *WxPopupMenu;
    ////GUI Control Declaration End
    
private:
    //Note: if you receive any error with these enum IDs, then you need to
    //change your old form code that are based on the #define control IDs.
    //#defines may replace a numeric value for the enum names.
    //Try copy and pasting the below block in your old form header files.
    enum
        {
            ////GUI Enum Control ID Start
            ////GUI Enum Control ID End
            ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
        };
    
private:
    void OnClose(wxCloseEvent& event);
    void CreateGUIControls();
    
    // ���݃E�B���h�E�ړ������ǂ��� 
    bool wndMoveMode;
    // �E�B���h�E�ړ��̂��߂� 
    int last_x;
    int last_y;
    // RO�̃p�X 
    wxString RoPath; 
    // �X�L���̃p�X 
    wxString SkinPath;
    // �X�L���C���[�W 
    wxBitmap *bgbmp;
    // INI�ɐݒ肳��Ă���X�L�� 
    wxString IniSkin;
    // �V���[�g�J�b�g���X�g 0-8 ���� 9-17 ���� 18-26 �O��� 27-35 �l��� 36-37 �������̓��
    ShortcutItem *ItemList[38];
    // �e�X�g�p�J�E���^�[ 
    int TestCount;
    // �X�L��ID => �X�L���p�� 
    MyStringHash SkillId2Str;
    // �X�L��ID => �X�L���a�� 
    MyStringHash SkillId2Name;
    // �A�N�e�B�u�X�L���e�[�u��
    MyStringHash ActiveSkills;
    // �A�C�e��ID => �A�C�e���a�� 
    MyStringHash ItemId2Name;
};

#endif
