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
    
    // 右クリックメニュー表示 
    void RoScVPCVDlgRightDown(wxMouseEvent& event);
    
    // 右クリック->テスト 
    void Test(wxCommandEvent& WXUNUSED(event));
    // 右クリック->終了 
    void OnQuit(wxCommandEvent& WXUNUSED(event));
    // 右クリック->スキン変更 
    void ChangeSkin(wxCommandEvent& WXUNUSED(event));
    // 右クリック->常に最前面 
    void ChangeStayOnTop(wxCommandEvent& WXUNUSED(event));
    
    // ウィンドウ移動のため 
    void RoScVPCVDlgLeftDown(wxMouseEvent& event);
    void RoScVPCVDlgLeftUP(wxMouseEvent& event);
    void RoScVPCVDlgMouseEvents(wxMouseEvent& event);
    
    // RoScVPCV.iniをロードする 
    bool LoadMainIni();
    // RoScVPCV.iniをセーブする 
    bool SaveMainIni();

    // レジストリに登録されているスキンを取得する 
    wxString GetRegistrySkin();
    // スキンをロードする 
    bool LoadSkin(wxString Skin);
    // iniをロードする 
    bool LoadIni(MyStringHash *hash, wxString File, wxString Section);
    // iniをセーブする 
    bool SaveIni(MyStringHash *hash, wxString File, wxString Section); 
    // Skills.iniをロードする 
    bool LoadSkillsIni();
    // PacketLengthをロードする 
    bool LoadPacketLength(int *PacketLength, int &MaxNum);
    // アイテム名テーブルをロードする 
    bool LoadItemNameTable(MyStringHash *hash);
    
    // 場所移動 
    bool PosMove();
     
    // 画面描画 
    void RoScVPCVDlgPaint(wxPaintEvent& event);

    // スキルデータセット 
    bool SetShortcutItem(int item_count, int type, int id, int count);

    // アクティブスキルリセット 
    bool ResetActiveSkill();

    // アクティブスキルセット 
    bool SetActiveSkill(int id, int lv);

    // スキルがアクティブか返す 
    bool SkillIsActive(int id, int lv);

    // アイテムのアクティブ/ノンアクティブを設定しなおす 
    bool SetItemsActive();
     
    // NICの番号 
    int NICIndex;
    
    // ポジションX
    int PositionX;
    // ポジションY
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
    
    // 現在ウィンドウ移動中かどうか 
    bool wndMoveMode;
    // ウィンドウ移動のために 
    int last_x;
    int last_y;
    // ROのパス 
    wxString RoPath; 
    // スキンのパス 
    wxString SkinPath;
    // スキンイメージ 
    wxBitmap *bgbmp;
    // INIに設定されているスキン 
    wxString IniSkin;
    // ショートカットリスト 0-8 一列目 9-17 二列目 18-26 三列目 27-35 四列目 36-37 小さいの二つ
    ShortcutItem *ItemList[38];
    // テスト用カウンター 
    int TestCount;
    // スキルID => スキル英名 
    MyStringHash SkillId2Str;
    // スキルID => スキル和名 
    MyStringHash SkillId2Name;
    // アクティブスキルテーブル
    MyStringHash ActiveSkills;
    // アイテムID => アイテム和名 
    MyStringHash ItemId2Name;
};

#endif
