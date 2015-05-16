//---------------------------------------------------------------------------
//
// Name:        RoScVPCVDlg.cpp
// Author:      sho
// Created:     2008/01/20 0:47:22
// Description: RoScVPCVDlg class implementation
//
//---------------------------------------------------------------------------

#include "RoScVPCVDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

enum
{
    wxID_Close      = 2001,
    wxID_About      = 2002,
    wxID_Test       = 2003,
    wxID_ChangeSkin = 2004,
    wxID_StayOnTop  = 2005,
};

//----------------------------------------------------------------------------
// RoScVPCVDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(RoScVPCVDlg,wxDialog)
	////Manual Code Start
	EVT_MENU(wxID_Close, RoScVPCVDlg::OnQuit)
	EVT_MENU(wxID_Test, RoScVPCVDlg::Test)
	EVT_MENU(wxID_ChangeSkin, RoScVPCVDlg::ChangeSkin)
	EVT_MENU(wxID_StayOnTop, RoScVPCVDlg::ChangeStayOnTop)
	////Manual Code End

	EVT_CLOSE(RoScVPCVDlg::OnClose)
	EVT_LEFT_DOWN(RoScVPCVDlg::RoScVPCVDlgLeftDown)
	EVT_LEFT_UP(RoScVPCVDlg::RoScVPCVDlgLeftUP)
	EVT_RIGHT_DOWN(RoScVPCVDlg::RoScVPCVDlgRightDown)
	EVT_PAINT(RoScVPCVDlg::RoScVPCVDlgPaint)
	EVT_MOUSE_EVENTS(RoScVPCVDlg::RoScVPCVDlgMouseEvents)
END_EVENT_TABLE()
////Event Table End

RoScVPCVDlg::RoScVPCVDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
    // ウィンドウ移動モードをfalseに 
    wndMoveMode = false;

    PositionX = 0;
    PositionY = 0;

    // NICIndexを初期化
    NICIndex = -1;

    // スキンを初期化 
    // IniSkin = wxT('');

    // 本体設定の読み込み 
    if( !LoadMainIni() ) {
        wxMessageBox(wxT("設定の読み込みに失敗しました。"), wxT("エラー"), wxOK);
        Destroy();
    }
    else {
        //スキンの読み込み 
        if( !IniSkin.Length() ) {
            IniSkin = GetRegistrySkin();
        }
        else {
            if( !LoadSkin(IniSkin) ) {
                wxLogMessage(wxT("can not load ini skin"));
                IniSkin = GetRegistrySkin();
            }
        }
        
        if( IniSkin.Length() < 1 ) {
            wxMessageBox(wxT("スキンが見つかりません。"), wxT("エラー"), wxOK);
            Destroy();
        }
        else {
            if( !LoadSkin(IniSkin) ) {
                wxMessageBox(wxT("スキン「") + IniSkin + wxT("」が読み込めません。"), wxT("エラー"), wxOK);
                Destroy();
            }
            else {
                if( !LoadSkillsIni() ) {
                    wxMessageBox(wxT("スキルテーブルが読み込めません。\nSkills.iniを用意してください。"), wxT("エラー"), wxOK);
                    Destroy();
                }
                else {
                    LoadItemNameTable(&ItemId2Name);
                    for( int i = 0; i < 36; i ++ ) {
                        ItemList[i] = new ShortcutItem(i, &SkillId2Str, &SkillId2Name, &ItemId2Name);
                    }
                    CreateGUIControls();
                    WxPopupMenu->Append(wxID_StayOnTop, wxT("(&F)常に最前面に表\示"));
                    WxPopupMenu->Append(wxID_ChangeSkin, wxT("(&S)スキン変更")); 
                    WxPopupMenu->Append(wxID_Test, wxT("(&T)テスト")); 
                    WxPopupMenu->Append(wxID_Close, wxT("(&E)終了"));
                    TestCount = 0;
                }
            }
        }
    }
}

RoScVPCVDlg::~RoScVPCVDlg()
{
} 

void RoScVPCVDlg::CreateGUIControls()
{
    //Do not add custom code between
    //GUI Items Creation Start and GUI Items Creation End.
    //wxDev-C++ designer will remove them.
    //Add the custom code before or after the blocks
    ////GUI Items Creation Start

	SetTitle(wxT("RoScVPCV"));
	SetIcon(wxNullIcon);
	SetSize(8,8,282,104);
	Center();
	

	WxPopupMenu = new wxMenu(wxT(""));
    ////GUI Items Creation End
}

void RoScVPCVDlg::OnClose(wxCloseEvent& event)
{
    bgbmp->CleanUpHandlers();
//    delete bgbmp;
//    delete SkillId2Str;
//    delete SkillId2Name;
    SaveMainIni();
    Destroy();
}

/// 常に最前面に表示 
void RoScVPCVDlg::ChangeStayOnTop(wxCommandEvent& WXUNUSED(event))
{
    if( GetWindowStyleFlag() != wxSTAY_ON_TOP ) {
        SetWindowStyleFlag(wxSTAY_ON_TOP);
    }
    else {
        SetWindowStyleFlag(524289);
    }
}

// スキン選択ダイアログ 
void RoScVPCVDlg::ChangeSkin(wxCommandEvent& WXUNUSED(event))
{
    
    if(wxDir::Exists(RoPath + wxT("\\skin\\"))) {
        wxDir dir(RoPath + wxT("\\skin\\"));
        wxString skin;
        dir.GetFirst(&skin);
        bool flg = true;
        int dir_count = 0;
        while(flg) {
//            skins.Add(&skin);
//            wxMessageBox(skin,wxT("msg"), wxOK);
            dir_count ++;
            flg = dir.GetNext(&skin);
        }
        if(dir_count) {
            wxString skins[dir_count];
            int i = 0;
            dir.GetFirst(&skin);
            
            flg = true;
            while(flg) {
                skins[i] = skin;
                i ++;
                flg = dir.GetNext(&skin);
            }
            int index = wxGetSingleChoiceIndex(wxT("スキンを選択してください。"),
                                               wxT("スキン"),
                                               WXSIZEOF(skins),
                                               skins);
            if( index >= 0 ) {
                if( !LoadSkin(skins[index]) ) {
                    wxMessageBox(wxT("スキン 「") + skins[index] + wxT("」が読み込めません。"),
                                 wxT("エラー"), wxOK);
                }
                else {
                    IniSkin = skins[index];
                }
                this->Refresh(false);
            }
        }
    }
    //    wxString skin = wxGet;
    
}

bool RoScVPCVDlg::LoadIni(MyStringHash *hash, wxString File, wxString Section)
{
    wxLogMessage(wxString::Format(wxT("Load INI : ") + File + wxT(" : ") + Section));
    if(!wxFile::Exists(File)) {
        return false;
    }
    DWORD dwstrlen;
    int size_count = 1;
    bool flg = true;
    {
        while(flg) {
            char keystr[1024 * size_count];
            dwstrlen = GetPrivateProfileSection(Section.c_str(), keystr, sizeof(keystr), File.c_str());
            if(dwstrlen != (sizeof(keystr) - 2) ) {
                flg = false;
            }
            else {
                size_count ++;
            }
        }
    }

    char keystr[1024 * size_count];
    char *pkey;

    dwstrlen = GetPrivateProfileSection(Section.c_str(), keystr, sizeof(keystr), File.c_str());
    pkey = keystr;

    while(*pkey != '\0' ) {
        char view[64];
        char *ptemp;
        ptemp = pkey;
        strcpy(view, ptemp);
        pkey += strlen(ptemp);
        wxString tstr;
        tstr.sprintf("%s", view);
        int sep_pos = tstr.Find(wxT("="));
        wxString id = tstr.Mid(0,sep_pos);
        wxString str = tstr.Mid(sep_pos + 1);

        hash->operator[](id) = str;
//        SkillId2Str[id] = str;

        pkey ++;
    }
    return true;
}

bool RoScVPCVDlg::SaveIni(MyStringHash *hash, wxString File, wxString Section)
{
    wxLogMessage(wxString::Format(wxT("Save INI : ") + File + wxT(" : ") + Section));
    
    MyStringHash::iterator it;
    for( it = hash->begin(); it != hash->end(); ++it ) {
        wxString key = it->first, value = it->second;
//        wxMessageBox(key + wxT(" => ") + value + wxT("\n") + key + wxT(" => ") + hash->operator[](key),
//                     wxT("msg"), wxOK);
        WritePrivateProfileString(Section.c_str(), key.c_str(), value.c_str(), File.c_str());
    }
    return true;
}

bool RoScVPCVDlg::LoadMainIni()
{
    MyStringHash MainSetting;
    bool rt;
    wxString MainIniFile = wxT(".\\RoScVPCV.ini");
    rt = LoadIni(&MainSetting, MainIniFile.c_str(), wxT("RoScVPCV"));
    
    // ロードできなかったら作る 
    if( !rt ) {
        MainSetting[wxT("RoPath")] = wxT("C:\\Program Files\\Gravity\\RagnarokOnline");
        wxLogMessage(wxT("can not load main ini... generate"));
        SaveIni(&MainSetting, MainIniFile.c_str(), wxT("RoScVPCV"));
    }
    if(MainSetting[wxT("NICIndex")].Length()) {
        long val;
        MainSetting[wxT("NICIndex")].ToLong(&val);
        NICIndex = (int)val;
    }
    if(MainSetting[wxT("Skin")].Length()) {
        IniSkin = MainSetting[wxT("Skin")];
    }
    RoPath = MainSetting[wxT("RoPath")];
    wxLogMessage(wxT("RoPath = ") + RoPath);
    {
        long val = 0;
        MainSetting[wxT("StayOnTop")].ToLong(&val);
        if( (int)val != 0 ) {
            SetWindowStyleFlag(wxSTAY_ON_TOP);
        }
    }
    {
        long val_x = 0;
        long val_y = 0;
        MainSetting[wxT("PosX")].ToLong(&val_x);
        MainSetting[wxT("PosY")].ToLong(&val_y);
        PositionX = val_x;
        PositionY = val_y;
    }
    return true;
}

bool RoScVPCVDlg::SaveMainIni()
{
    MyStringHash MainSetting;
    MainSetting[wxT("RoPath")] = RoPath;
    MainSetting[wxT("NICIndex")] = wxString::Format(wxT("%d"), NICIndex);
    MainSetting[wxT("Skin")] = IniSkin;
    
    if( GetWindowStyleFlag() == wxSTAY_ON_TOP ) {
        MainSetting[wxT("StayOnTop")] = wxT("1");
    }
    else {
        MainSetting[wxT("StayOnTop")] = wxT("0");
    }
    MainSetting[wxT("PosX")] = wxString::Format(wxT("%d"), PositionX);
    MainSetting[wxT("PosY")] = wxString::Format(wxT("%d"), PositionY);
    
    wxString MainIniFile = wxT(".\\RoScVPCV.ini");
    SaveIni(&MainSetting, MainIniFile.c_str(), wxT("RoScVPCV"));
    return true;
}

bool RoScVPCVDlg::LoadSkillsIni()
{
    bool rt;
    rt = LoadIni(&SkillId2Str, wxT(".\\Skills.ini"), wxT("ID2STR"));
    rt = LoadIni(&SkillId2Name, wxT(".\\Skills.ini"), wxT("ID2NAME"));
    return rt;
}

bool RoScVPCVDlg::LoadItemNameTable(MyStringHash *hash)
{
    wxLogMessage(wxT("LoadItemNameTable"));
    if(!wxFile::Exists(wxT(".\\idnum2itemdisplaynametable.txt"))) {
        return false;
    }
    wxTextFile file(wxT(".\\idnum2itemdisplaynametable.txt"));
    
    if(!file.Open()) {
        wxMessageBox(wxT("can not open .\\idnum2itemdisplaynametable.txt"), wxT("error"), wxOK);
        return false;
    }
    wxString s;
    wxRegEx regex(wxT("[0-9]+#[^#]+#"));
    for ( s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine() ) {
//        wxString line;
//        line.sprintf(wxT("%d : %d : %s"), file.GetCurrentLine(), s.Length(), s.c_str());
//        wxMessageBox(line, wxT(""), wxOK);
        if(regex.Matches(s)) {
            int sharp_pos = s.Find(wxT("#"));
            wxString id = s.Mid(0, sharp_pos);
            wxString name = s.Mid(sharp_pos + 1, s.Length() - (sharp_pos + 1 + 1));
            hash->operator[](id) = name;
            /*
            wxString line;
            line.sprintf(wxT("%d : %d : %s : %s : %s"), file.GetCurrentLine(), s.Length(), id.c_str(), name.c_str(), s.c_str());
            wxMessageBox(line, wxT(""), wxOK);
            */
        }
    }
    return true; 
}

bool RoScVPCVDlg::LoadPacketLength(int *PacketLength, int &Max)
{
    wxLogMessage(wxT("LoadPacketLength"));
    if(!wxFile::Exists(wxT(".\\PacketLength.txt"))) {
        return false;
    }
    wxTextFile file(wxT(".\\PacketLength.txt"));

    if(!file.Open()) {
        wxMessageBox(wxT("can not open .\\PacketLength.txt"), wxT("error"), wxOK);
        return false;
    }
    wxString s;
    wxRegEx regex(wxT("[-0-9]+"));
    for( s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine() ) {
        if(s.Length() > 0 && s.Find(wxT("#")) != 0) {
            s.Replace(wxT(" "),wxT(""));
            wxStringTokenizer tkz(s,",");
            while(tkz.HasMoreTokens()) {
                wxString token = tkz.GetNextToken();
//                wxMessageBox(wxString::Format(wxT("%d : %s"), i, token.c_str()), wxT(""), wxOK);
                long val;
                token.ToLong(&val);
                *(PacketLength + Max) = (int)val;
                Max ++;
            }
        }
    }
    return true;
}

// テスト用関数 
void RoScVPCVDlg::Test(wxCommandEvent& WXUNUSED(event))
{
    /*
    MyStringHash::iterator it;
    for( it = SkillId2Str.begin(); it != SkillId2Str.end(); ++it ) {
        wxString key = it->first, value = it->second;
        wxMessageBox(key + wxT(" => ") + value + wxT("\n") + key + wxT(" => ") + SkillId2Name[key],
                     wxT("msg"), wxOK);
    }
    */
    /*
    int PacketLength[999];
    int MaxNum = 0;
    if(!LoadPacketLength(PacketLength, MaxNum)) {
        wxMessageBox(wxT("can not load PacketLength"), wxT(""), wxOK);
        return;
    }
    wxMessageBox(wxString::Format(wxT("MaxNum = %d"), MaxNum), wxT(""), wxOK);
    for(int i = 0; i < MaxNum; i ++ ) {
        wxString str;
        str.sprintf("%d = %d", i, PacketLength[i]);
        wxMessageBox(str, wxT(""), wxOK);
    }
    */
    
    wxString msgstr;
    msgstr.sprintf(wxT("タイプを入力してください。(%d)"), TestCount);
    wxString stype = wxGetTextFromUser(msgstr, wxT("テキスト入力"), wxT(""));
    long type;
    if(stype.ToLong(&type, 10)) {
        if(type == 0) {
            msgstr.sprintf(wxT("アイテムIDを入力してください。(%d)"), TestCount);
        }
        else if(type == 1) {
            msgstr.sprintf(wxT("スキルIDを入力してください。(%d)"), TestCount);
        }
        else {
            wxMessageBox(wxT("0か1を入力してください。"), wxT("エラー"), wxOK)   ;
            return;
        }
        wxString sid = wxGetTextFromUser(msgstr, wxT("テキスト入力"), wxT("")); 
        long id;
        if(sid.ToLong(&id, 10)) {
            long count;
            if(type == 0) {
                msgstr.sprintf(wxT("個数を入力してください。(%d)"), TestCount);
            }
            else if(type == 1) {
                msgstr.sprintf(wxT("スキルLvを入力してください。(%d)"), TestCount);
            }
            wxString scount = wxGetTextFromUser(msgstr, wxT("テキスト入力"), wxT(""));
            if(!scount.ToLong(&count, 10)) {
                wxMessageBox(sid + wxT("はlongに変換できませんでした。"), wxT("エラー"), wxOK);
                return;
            }
            
            //            if(ItemList[TestCount]->SetItem(type, id, count)) {
            if(SetShortcutItem(TestCount, type, id, count)) {
                TestCount ++;
                if(TestCount > 26) {
                    TestCount = 0;
                }
            }
            this->Refresh(false);
        }
        else {
            wxMessageBox(sid + wxT("はlongに変換できませんでした。"), wxT("エラー"), wxOK);
        }
    }
    else {
        wxMessageBox(stype + wxT("はlongに変換できませんでした。"), wxT("エラー"), wxOK); 
    }
    
}

bool RoScVPCVDlg::SetShortcutItem(int item_count, int type, int id, int count)
{
    if( item_count > 38) {
        return false;
    }
    bool isActive = false;
    // こっちはアイテム 
    if( type == 0 ) {
        isActive = true;
    }
    // こっちはスキル 
    else if( type == 1 ) {
        isActive = SkillIsActive( id, count );
    }
    return ItemList[item_count]->SetItem(type, id, count, isActive);
}


// アクティブスキルリセット 
bool RoScVPCVDlg::ResetActiveSkill()
{
    ActiveSkills.clear();
    return true;
}

// アクティブスキルセット 
bool RoScVPCVDlg::SetActiveSkill(int id, int lv)
{
    wxString IdStr;
    IdStr.sprintf(wxT("%d"), id);
    wxLogMessage(wxT("SetActive : ID:% 4d Lv:% 2d IdStr:%s Name:%s"),
                 id,
                 lv,
                 IdStr.c_str(),
                 SkillId2Name[wxString::Format(wxT("%d"),id)].c_str());
    ActiveSkills[IdStr] = wxString::Format(wxT("%d"), lv);
    return true;
}

// スキルがアクティブか調べる
bool RoScVPCVDlg::SkillIsActive(int id, int lv)
{
    wxString IdStr;
    IdStr.sprintf(wxT("%d"), id);
    long max_lv;
    if( ActiveSkills.find( IdStr ) != ActiveSkills.end() ) {
        ActiveSkills[IdStr].ToLong(&max_lv);
        if( lv <= (int)max_lv ) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool RoScVPCVDlg::SetItemsActive()
{
    for( int i = 0; i < 36; i ++ ) {
        bool active = false;
        if( ItemList[i]->GetType() == 0 ) {
            active = true;
        }
        else if( ItemList[i]->GetType() == 1 ) {
            active = SkillIsActive( ItemList[i]->GetItemID(), ItemList[i]->GetCount() );
        }
        ItemList[i]->SetActiveFlag(active);
    }
    return true;
}

// 右クリックからの終了 
void RoScVPCVDlg::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    /* TODO (#1#): Implement RoScVPCVDlg::OnQuit() */
    Close(true);
}

// レジストリに登録されているスキンを取得する 
wxString RoScVPCVDlg::GetRegistrySkin()
{
    // HKEY_LOCAL_MACHINE\SOFTWARE\Gravity Soft\Ragnarok\Option\SKINNAME
    HKEY hkey;
    DWORD result;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Gravity Soft\\Ragnarok\\Option",
                   0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &result);
    
    TCHAR data[1024] = "";
    DWORD dataSize = (DWORD)sizeof(data);
    DWORD dataTypeSize;
    RegQueryValueEx(hkey, "SKINNAME", NULL, &dataTypeSize, (LPBYTE)&data, &dataSize);
    RegCloseKey(hkey);
    
    wxString Skin = data;
    if( Skin.Length() < 1 ) {
        Skin = wxT("default");
    }
    wxLogMessage(wxT("Get Skin Name : ") + Skin);
    return Skin;
}

// 引数->default->足元の順でスキンをロードする 
bool RoScVPCVDlg::LoadSkin(wxString Skin)
{
    wxLogMessage(wxT("Load Skin : ") + RoPath + wxT("\\skin\\") + Skin + wxT("\\basic_interface\\shortitem_bg.bmp"));
    // RoPath + \skin\ + skin + \basic_interface\shortitem_bg.bmp
    
    wxString tSkinPath = RoPath + wxT("\\skin\\") + Skin + wxT("\\basic_interface\\shortitem_bg.bmp");

//    wxMessageBox(SkinPath, wxT("SkinPath"), wxOK);
    wxBitmap bmp;
    if(! bmp.LoadFile(tSkinPath, wxBITMAP_TYPE_BMP)) {
        tSkinPath = RoPath + wxT("\\skin\\default\\basic_interface\\shortitem_bg.bmp");
        if(! bmp.LoadFile(tSkinPath, wxBITMAP_TYPE_BMP)) {
            tSkinPath = wxString(wxT(".\\shortitem_bg.bmp"));
            if(! bmp.LoadFile(tSkinPath, wxBITMAP_TYPE_BMP)) {
                return false;
            }
        }
    }
    SkinPath = tSkinPath;
    
    bgbmp = new wxBitmap;
    if ( !bgbmp->LoadFile(SkinPath, wxBITMAP_TYPE_BMP) ) {
        return false;
    }
    wxMask *bgmask = new wxMask(*bgbmp, wxColor(0xff,0x00,0xff));
    bgbmp->SetMask(bgmask);
    
    return true;
}

// 右クリックメニューを呼び出し 
void RoScVPCVDlg::RoScVPCVDlgRightDown(wxMouseEvent& event)
{
    // insert your code here

    PopupMenu(WxPopupMenu, event.GetX(), event.GetY() );
}

// ウィンドウ移動モードOn 
void RoScVPCVDlg::RoScVPCVDlgLeftDown(wxMouseEvent& event)
{
    // insert your code here
    wndMoveMode = true;
    last_x = event.GetX();
    last_y = event.GetY();
}

// ウィンドウ移動モードOFF 
void RoScVPCVDlg::RoScVPCVDlgLeftUP(wxMouseEvent& event)
{
    // insert your code here
    wndMoveMode = false;
}

// マウスの移動を検地。ウィンドウ移動モードだったらウィンドウを動かす 
void RoScVPCVDlg::RoScVPCVDlgMouseEvents(wxMouseEvent& event)
{
    // insert your code here
    if(wndMoveMode) {
        wxWindow *win = (wxWindow*) event.GetEventObject();
        int wnd_x;
        int wnd_y;
        win->GetPosition(&wnd_x, &wnd_y);
        int x = wnd_x + event.GetX() - last_x;
        int y = wnd_y + event.GetY() - last_y;
        PositionX = x;
        PositionY = y;
        win->Move(x, y);
    }
}

// 描画関数 
void RoScVPCVDlg::RoScVPCVDlgPaint(wxPaintEvent& event)
{
    // insert your code here
    wxPaintDC dc(this);
    dc.SetFont(wxFont(8,wxDEFAULT,wxNORMAL,wxNORMAL,false));
    
    dc.DrawBitmap(*bgbmp, 0,   0,  true);
    dc.DrawBitmap(*bgbmp, 0,   34, true);
    dc.DrawBitmap(*bgbmp, 0,   68, true);
    dc.DrawBitmap(*bgbmp, 0,   102, true);
    
    dc.SetTextForeground(wxColour(0x77, 0x77, 0x77));
    for( int x = -1; x < 2; x ++ ) {
        for( int y = -1; y < 2; y ++ ) {
            dc.DrawText(wxT("4"), 268 + x, 13 + y);
            dc.DrawText(wxT("3"), 268 + x, 47 + y);
            dc.DrawText(wxT("2"), 268 + x, 81 + y);
            dc.DrawText(wxT("1"), 268 + x, 115 + y);
        }
    }
    dc.SetTextForeground(wxColour(0xff, 0xff, 0xff));
    dc.DrawText(wxT("4"), 268, 13);
    dc.DrawText(wxT("3"), 268, 47);
    dc.DrawText(wxT("2"), 268, 81);
    dc.DrawText(wxT("1"), 268, 115);
    
    for( int i = 0; i < 36; i ++ ) {
        ItemList[i]->Draw(&dc);
    }
}







