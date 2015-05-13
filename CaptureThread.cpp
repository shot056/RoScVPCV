#include <cstdlib>
#include "CaptureThread.h"

CaptureThread::CaptureThread(RoScVPCVDlg* dlg)
{
    p_dlg = dlg;
    alive = true;
    MaxCodeNum = 0;
}

CaptureThread::~CaptureThread()
{

}

void *CaptureThread::Entry()
{
    wxLogMessage(wxT("Entry"));
    roop_flag = true;
    unsigned char Packet[MAX_RECV_SIZE];
    unsigned long Len;
    PacketBuffer *Buffer = new PacketBuffer;
    Buffer->CryptedFlag02A3 = false;
    Buffer->CryptedFlag02BD = false;
    Buffer->ReceivedFlag02BD = false;
    
    while(roop_flag) {
        if(capture_flag) {
            wxThread::Sleep(0);
            if( Recv(Packet, &Len) ) {
                if( Len != 0 ) {
                    ParsePacket(Packet, Len, Buffer);
                }
                else {
                    wxThread::Sleep(100);
                }
            }
            else {
                wxString msg;
                msg.sprintf(wxT("�p�P�b�g�̎�M�Ɏ��s���܂����B( ERROR = %lu )"), GetLastError());
                wxLogMessage(msg);
//                roop_flag = false;
            }
        }
        else {
            wxThread::Sleep(1000);
        }
    }
    //    delete Buffer;
    if( capture_flag && !Stop() ) {
        wxMessageBox(wxT("���ł�Socket���N���[�Y���Ă��܂��B"), wxT("�G���["), wxOK);
    }
    Cleanup();
    return NULL;
}

void CaptureThread::OnExit()
{
    alive = false;
}

bool CaptureThread::StartCapture()
{
    wxLogMessage(wxT("StartCapture"));
    if(!Startup()) {
        wxMessageBox(wxT("WinSock2�̏������Ɏ��s���܂����B"), wxT("�G���["), wxOK);
        return false;
    }
    unsigned long AddressList[MAX_ADAPTER];
    int Max = 0;

    if( !EnumAdapter(AddressList, &Max) ) {
        DWORD Error = GetLastError();
        wxString msg;
        switch(Error) {
        case ERROR_SOCKET_BUSY :
            msg = wxT("Socket���g�p���ł��B");
            break;
        case ERROR_WINSOCK2_INIT :
            msg = wxT("WinSock2�̏��������������Ă��܂���B");
            break;
        case ERROR_WSASOCKET :
            msg = wxT("WSASocket�ɂ��RAW Socket�̍쐬�Ɏ��s���܂����B");
        case ERROR_ADDRESS_LIST_QUERY :
            msg = wxT("�A�_�v�^�ꗗ�̎擾(ADDRESS_LIST_QUERY)�̏������Ɏ��s���܂����B");
        default:
            msg = wxT("�����s���̃G���[���������܂����B");
        }
        wxMessageBox(msg, wxT("�G���["), wxOK);
        return false;
    }
    int Index = 0;
    if( p_dlg->NICIndex < 0 ) {
        if( Max > 1 ) {
            wxString Addrs[Max];
            for( int i = 0; i < Max; i ++ ) {
                unsigned char *p = (unsigned char *)&AddressList[i];
                Addrs[i] = wxString::Format(wxT("%d.%d.%d.%d"), *(p+0), *(p+1), *(p+2), *(p+3));
            }
            Index = wxGetSingleChoiceIndex(wxT("�g�p����A�_�v�^��I�����Ă��������B"),
                                           wxT("�A�_�v�^"),
                                           WXSIZEOF(Addrs),
                                           Addrs );
        }
        p_dlg->NICIndex = Index;
    }
    else {
        Index = p_dlg->NICIndex;
    }
    
    if( Index == -1 or Max <= Index ) {
        wxMessageBox(wxT("�������I�����Ă��������B"), wxT("�G���["), wxOK);
        return false;
    }
    
    unsigned long ip_address = AddressList[Index];
    {
        unsigned char *p = (unsigned char *)&ip_address;
        wxLogMessage(wxString::Format(wxT("Address : %d.%d.%d.%d"), *(p+0), *(p+1), *(p+2), *(p+3)));
    }
    if( !Start(ip_address) ) {
        DWORD Error = GetLastError();
        wxString msg;
        switch( Error ) {
        case ERROR_SOCKET_BUSY :
            msg = wxT("Socket���g�p���ł��B");
            break;
        case ERROR_WSASOCKET :
            msg = wxT("WSASocket�ɂ��RAW Socket�̍쐬�Ɏ��s���܂����B");
            break;
        case ERROR_BIND :
            msg = wxT("Socket�̃o�C���h�Ɏ��s���܂����B");
            break;
        case ERROR_SIO_RCVALL :
            msg = wxT("�v���~�X�L���X���[�h�̐ݒ�Ɏ��s���܂����B");
        default :
            msg = wxT("�����s���̃G���[���������܂����B");
        }
        wxMessageBox(msg, wxT("�G���["), wxOK);
        return false;
    }
    capture_flag = true;
    wxLogMessage(wxT("\tSuccess"));
    return true;
}

bool CaptureThread::StopCapture()
{
    wxLogMessage(wxT("StopCapture"));
    capture_flag = false;
    SetRoopFlag(false);
    return true;
}

void CaptureThread::SetRoopFlag(bool f)
{
    roop_flag = f;
}

bool CaptureThread::GetAlive()
{
    return alive;
}

void CaptureThread::ParsePacket(unsigned char *Packet, unsigned long Len, PacketBuffer * Buffer) {
    unsigned char *p = Packet;

    LP_PROTOCOL_IP pIpHdr = (LP_PROTOCOL_IP)p;

    // TCP�������� 
    if( pIpHdr->Protocol == 6 ) {
        // & �̓r�b�g���Z�q �����̃r�b�g��1�������Ƃ��̂�1�ɂȂ� 
        // 0x0F��00001111�Ȃ̂ŁA 
        // Ver_Len�̃r�b�g��0100XXXX�Ȃ̂ŁA0100�͑S��0�� XXXX��1�����������������c�� 
        int IpHdrLen = (pIpHdr->Ver_Len & 0x0F)*4; // IP�w�b�_�̒��� 
        LP_PROTOCOL_TCP pTcpHdr = (LP_PROTOCOL_TCP)(p + IpHdrLen);
        
        // SYN�Ƃ��̓o�C�o�C 
        if( (pTcpHdr->FLG & TH_SYN) != 0 ||
            (pTcpHdr->FLG & TH_FIN) != 0 ||
            (pTcpHdr->FLG & TH_RST) != 0 ) {
            return;
        }

        // RO�̃T�[�o����Ȃ������疳�� 
        if( isRoServer(pIpHdr->SrcIp, htons(pTcpHdr->SrcPort)) ) {
            // ���x��TCP�w�b�_�̕������|�C���^���ړ����Ă�� 
            // pTcpHdr->Offset���g���͂� 
//            printf("SrcPort=%05d : DstPort=%05d : ", htons(pTcpHdr->SrcPort), htons(pTcpHdr->DstPort)); 
            // ����ǂ�Offset�̑O4�r�b�g�����c������ 
//            printf("Of=%d : ", (pTcpHdr->Offset & ~0x0F)); 
            // URG�t���O�H 0x20(00100000)�ł����H 
//            printf("URGFLG=%d : ", (pTcpHdr->FLG & 0x20)); 
//            printf("Urg=%d : ", pTcpHdr->Urgent); 
//            printf("Len=%d : ", ntohs(pIpHdr->TotalLen)); 
            int TcpHdrLen = (pTcpHdr->Offset & ~0x0F) / 4 + pTcpHdr->Urgent;
            // �ڑ��悪�ς���� 
            if(Buffer->IpAddr != pIpHdr->SrcIp || Buffer->Port != htons(pTcpHdr->SrcPort)) {
                Buffer->IpAddr = pIpHdr->SrcIp;
                Buffer->Port = htons(pTcpHdr->SrcPort);
                Buffer->Len = 0;
                Buffer->CryptedFlag02A3 = false;
                Buffer->CryptedFlag02BD = false;
                Buffer->ReceivedFlag02BD = false;
                if( isRoLoginServer(Buffer->IpAddr, Buffer->Port) ) {
//                    printf("Login Server\n");
                }
                else if( isRoCharServer(Buffer->IpAddr, Buffer->Port) ) {
//                    printf("Char Server\n");
                }
                else if( isRoMapServer(Buffer->IpAddr, Buffer->Port) ) {
//                    printf("Map Server\n");
                }
                else {
//                    printf("Unknown Server\n");
                }
            }
            if( !Buffer->CryptedFlag02A3 ) {
                ParseNormalPacket(Packet + IpHdrLen + TcpHdrLen,
                                  Len - IpHdrLen - TcpHdrLen,
                                  Buffer);
            }
            else {
                ParseCryptedPacket(Packet + IpHdrLen + TcpHdrLen,
                                  Len - IpHdrLen - TcpHdrLen,
                                  Buffer);
            }
        }
    }
}

int CaptureThread::ParseNormalPacket(unsigned char *inPacket, unsigned long inLen, PacketBuffer *Buffer) {
    unsigned char LocalBuffer[MAX_RECV_SIZE];
    unsigned char *Packet;
    unsigned long Len;
    int RoLen;

    if( inLen == 0 ) {
        return PACKET_STATE_END;
    }

    if( Buffer->Len + inLen > MAX_RECV_SIZE ) {
        Buffer->Len = 0;
        return PACKET_STATE_ERROR;
    }

    Packet = LocalBuffer;

    // �o�b�t�@���ƍ��񕪂������t���� 
    if( Buffer->Len > 0 ) {
        memcpy(Packet, Buffer->Packet, Buffer->Len);
    }
    memcpy(Packet + Buffer->Len, inPacket, inLen);
    Len = Buffer->Len + inLen;
    Buffer->Len = 0;

    // ���܂��Ă�p�P�b�g���؂��܂ł܂킷 
    while(1) {
        // 2�ȉ��͐�Ε����H 
        if( Len < 2 ) {
            memcpy(Buffer->Packet, Packet, Len);
            Buffer->Len = Len;
            return PACKET_STATE_CONTINUE;
        }

        LP_RO_MESSAGE_TYPE roType = (LP_RO_MESSAGE_TYPE)Packet;
        //        wxLogMessage(wxString::Format(wxT("Type = %04X"), roType->Type));
        if( roType->Type >= MaxCodeNum || (RoLen = (int)PacketLengthTable[roType->Type]) == 0 ) {
            // �Ȃ񂼂���H ����Ȃ��p�P�b�g�������� 
            return PACKET_STATE_ERROR;
        }
        
        //        wxLogMessage(wxString::Format(wxT("Len = %d : RoLen = %d"), Len, RoLen));
        if( RoLen == -1 ) {
            // 4�o�C�g�ȉ��͕����H 
            if( Len < 4 ) {
                memcpy(Buffer->Packet, Packet, Len);
                Buffer->Len = Len;
                return PACKET_STATE_CONTINUE;
            }
            // -1���Ă��Ƃ̓p�P���璷���ǂނ� 
            RoLen = roType->Len;
        }

        // �p�P���e�[�u�����Z�����番������ 
        if( (int)Len < RoLen ) {
            memcpy(Buffer->Packet, Packet, Len);
            Buffer->Len = Len;
            return PACKET_STATE_CONTINUE;
        }

        switch(roType->Type) {
        case 0x02A3 : {
            // 02A3�Í����̑Ώ� 
            break;
        }
        case 0x02BD : {
            // 02BD�Í����̑Ώ� 
            break;
        }
        default : {
            UsePacket(roType->Type, Packet);
        }
        }

        // �p�P�I���[ 
        if( RoLen == (int)Len ) {
            return PACKET_STATE_END;
        }
        Packet += RoLen;
        Len -= RoLen;
    }
}

int CaptureThread::ParseCryptedPacket(unsigned char *inPacket, unsigned long inLen, PacketBuffer *Buffer) {
    return 0;
}

void CaptureThread::UsePacket(short int Type, unsigned char *Packet) {
    //    wxLogMessage(wxString::Format(wxT("UsePacket : %04X"), Type));
    switch(Type) {
//     case 0x069 : {

//     }
//     case 0x006B : {

//     }
//     case 0x0071 : {

//     }
        // �V���[�g�J�b�g���X�g 
        case 0x02B9 : {
//            LP_RO_MESSAGE_02B9 msg = (LP_RO_MESSAGE_02B9)Packet;
            wxLogMessage(wxT("case 0x02B9"));
            Packet += 2;
            for( int i = 0; i < 27; i ++ ) {
                unsigned char *Flag = (unsigned char *)Packet;
                Packet += 1;
                LP_RO_MESSAGE_02B9_SHORTCUT shortcut = (LP_RO_MESSAGE_02B9_SHORTCUT)Packet;
                wxLogMessage(wxString::Format(wxT("    %d: Flag=%d : ID=%d : Lv=%d"), i, (int)Flag[0], (int)(shortcut->Id), (int)(shortcut->Lv)));
                p_dlg->SetShortcutItem(i, (int)Flag[0], (int)(shortcut->Id), (int)(shortcut->Lv));
                Packet += 6;
            }
            p_dlg->Refresh(false);
            break;
        }
        // �X�L�����X�g 
        case 0x010F : {
            wxLogMessage(wxT("case 0x010F"));
            LP_RO_MESSAGE_010F skill_header = (LP_RO_MESSAGE_010F)Packet;
            wxLogMessage(wxString::Format(wxT("\tLength: %d"), skill_header->Len));
            Packet += 4;
            p_dlg->ResetActiveSkill();
            for( int i = 0; ( i * 37 ) < ( skill_header->Len - 4 ); i ++ ) {
                LP_RO_MESSAGE_010F_SKILL skill = (LP_RO_MESSAGE_010F_SKILL)Packet;
                /*
                wxLogMessage(wxString::Format(wxT("  Id:% 4u TargType:% 2u unknown:%u Lv:% 2u Sp:% 3u Range:% 2u Name:% 15s Up:%u"),
                                              (short)skill->Id,
                                              (short)skill->TargType,
                                              (short)skill->unknown,
                                              (short)skill->Lv,
                                              (short)skill->Sp,
                                              (short)skill->Range,
                                              skill->Name,
                                              (unsigned short)skill->Up
                                              ));
                */
                if( (short)skill->Lv != 0 ) {
                    p_dlg->SetActiveSkill( (short)(skill->Id), (short)(skill->Lv) );
                }
                Packet += 37;
            }
            p_dlg->SetItemsActive();
            p_dlg->Refresh(false);
            break;
        }
    }
    return;
}

void CaptureThread::SetPacketLength(int *PacketLenTable, int MaxNum) {
    for( int i = 0; i < MaxNum; i ++ ) {
        PacketLengthTable[i] = *(PacketLenTable + i);
    }
    MaxCodeNum = MaxNum;
    wxLogMessage(wxString::Format(wxT("MaxCodeNum = %d (%x)"), MaxCodeNum, MaxCodeNum));
}

bool CaptureThread::isRoServer(unsigned int Ip, short int Port) {
    if(isRoLoginServer(Ip, Port) || isRoCharServer(Ip, Port) || isRoMapServer(Ip, Port)) {
        return true;
    }
    else {
        return false;
    }
}


bool CaptureThread::isRoLoginServer(unsigned int Ip, short int Port) {
    if(Port == 6900) {
        return true;
    }
    else {
        return false;
    }
}
bool CaptureThread::isRoCharServer(unsigned int Ip, short int Port) {
    if(Port == 6121) {
        return true;
    }
    else {
        return false;
    }
}
bool CaptureThread::isRoMapServer(unsigned int Ip, short int Port) {
    if(Port == 5121) {
        return true;
    }
    else {
        return false;
    }
}


// void CaptureThread::ParsePacket(unsigned char *Buffer, unsigned long Len)
// {

//     unsigned char *p = Buffer;
//     unsigned int Type = 0x0800;

//     if(Type == 0x0800) {
//         LP_PROTOCOL_IP protocol_ip = (LP_PROTOCOL_IP)p;

//         p += (protocol_ip->Ver_Len & 0x0F)*4;

//         if( protocol_ip->Protocol == 6 ) {
//             unsigned char *ip;
//             ip = (unsigned char *)&(protocol_ip->Ip[0]);
//             bool go_next_flg = false;
//             if(*(ip+0) == 61 && *(ip+1) == 215 && *(ip+2) == 212 && *(ip+3) == 5) {
//                 go_next_flg = true;
//                 //                printf("Login Server\n");
//             }
//             else if(*(ip+0) == 211 && *(ip+1) == 13 && *(ip+2) == 235 && *(ip+3) == 163) {
//                 go_next_flg = true;
//                 //                printf("Char Server\n");
//             }

//             LP_PROTOCOL_TCP protocol_tcp = (LP_PROTOCOL_TCP)p;
//             if(htons(protocol_tcp->SrcPort) == 5121) {
//                 go_next_flg = true;
//                 //                printf("Map Server\n");
//             }
//             else if(htons(protocol_tcp->SrcPort) == 6900) {
//                 go_next_flg = true;
//                 //                printf("Login Server\n");
//             }
//             if( go_next_flg ) {
//                 ip = (unsigned char *)&(protocol_ip->Ip[1]);
//                 int move_size = (protocol_tcp->Offset & ~0x0F) / 4 + protocol_tcp->Urgent;
//                 p += move_size;

//                 LP_RO_MESSAGE_TYPE ro_message_type = (LP_RO_MESSAGE_TYPE)p;
//                 //                short int typei = htons(ro_message_type->Type);
//                 switch (ro_message_type->Type) {
//                     /*
//                 case 0x0069: // ���O�C���f�[�^ 
//                     LP_RO_MESSAGE_0069 ro_msg_0069 = (LP_RO_MESSAGE_0069)p;
//                     printf("Login Success : Len=%d LoginID1=%d AccountID=%d\n",
//                            ro_msg_0069->Len, ro_msg_0069->LoginId, ro_msg_0069->AccountId);
//                     break;
//                 case 0x006B : // �L�����f�[�^ 
//                     printf("Len=%d\n", ro_msg_006B->Len);
//                     p += 4;
//                     for(int i = 106; i < ro_msg_006B->Len; i += 106) {
//                         LP_RO_MESSAGE_006B_CHARDATA ro_char = (LP_RO_MESSAGE_006B_CHARDATA)p;
//                         printf("CharId=%d : BExp=%d JExp=%d Zeny=%d\n\t%s BLv.%d JLv.%d Class.%d HP=%d(%d) SP=%d(%d)\nSTR=%d AGI=%d VIT=%d INT=%d DEX=%d LUK=%d\n",
//                                ro_char->CharId, ro_char->BaseExp, ro_char->JobExp, ro_char->Zeny,
//                                ro_char->Name, ro_char->BaseLv, ro_char->JobLv,
//                                ro_char->Class, ro_char->HP, ro_char->MaxHP, ro_char->SP, ro_char->MaxSP,
//                                ro_char->Str, ro_char->Agi, ro_char->Vit, ro_char->Int, ro_char->Dex, ro_char->Luk);
//                         p += 106;
//                     }
//                     break;
//                 case 0x0071 : // �L�����I�ڑ��f�[�^ 
//                     p += 2;
//                     LP_RO_MESSAGE_0071 ro_msg_0071 = (LP_RO_MESSAGE_0071)p;
//                     unsigned char *ip;
//                     ip = (unsigned char *)&(ro_msg_0071->IP);
//                     unsigned char *charid = (unsigned char *)&(ro_msg_0071->CharId);
//                     unsigned char *map = (unsigned char *)&(ro_msg_0071->Map);
//                     unsigned char *port = (unsigned char *)&(ro_msg_0071->Port);
//                     printf("CharId=%d Map=%s IP=%d.%d.%d.%d Port=%d\n",
//                            ro_msg_0071->CharId, ro_msg_0071->Map, *(ip+0), *(ip+1), *(ip+2), *(ip+3), ro_msg_0071->Port);
//                     break;
//                     */
//                 case 0x02B9 : // �V���g�J�f�[�^ 
//                     p += 2;
//                     for(int i = 0; i < 27; i ++) {
//                         unsigned char *Flag = (unsigned char *)p;
//                         p += 1;
//                         LP_RO_MESSAGE_02B9_SHORTCUT ro_shortcut = (LP_RO_MESSAGE_02B9_SHORTCUT)p;
//                         int skill_type = Flag[0];
//                         int skill_id = ro_shortcut->Id;
//                         int skill_lv = ro_shortcut->Lv;
//                         p_dlg->SetShortcutItem(i, skill_type, skill_id, skill_lv);
//                         /*
//                         printf("%d-%d Type:%d ID:%d Lv:%d\n",
//                                (i % 9) + 1,
//                                i - (9 * i % 9),
//                                Flag[0], ro_shortcut->Id, ro_shortcut->Lv);
                        
//                         unsigned char *sId = (unsigned char *)&(ro_shortcut->Id);
//                         unsigned char *sLv = (unsigned char *)&(ro_shortcut->Lv);
                        
//                         printf("\tFlag=");
//                         printf("%02X : %d", Flag[0], Flag[0]);
//                         printf("\n");
//                         printf("\tID=");
//                         for(int j = 0; j < 8; j ++) {
//                             printf("%02X ", sId[j]);
//                         }
//                         printf("\n");
//                         printf("\tLv=");
//                         for(int j = 0; j < 2; j ++) {
//                             printf("%02X ", sLv[j]);
//                         }
//                         printf("\n");
//                         */
//                         p += 6;
//                     }
//                     p_dlg->Refresh(false);
//                     break;
//                 }
//             }
//         }
//     }
// }
