#ifndef CAPTURETHREAD_H
#define CAPTURE_THREAD_H

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
	#include <wx/thread.h>
#endif

#include "Capture.h"
#include "Protocol.h"
#include "RoProtocol.h"
#include "RoScVPCVDlg.h"


typedef struct PacketBuffer {
    unsigned int IpAddr;
    short Port;
    bool CryptedFlag02A3;
    bool CryptedFlag02BD;
    bool ReceivedFlag02BD;
    unsigned char Packet[MAX_RECV_SIZE];
    unsigned long Len;
};

class CaptureThread : public wxThread
{
public:
    CaptureThread(RoScVPCVDlg* dlg);
    ~CaptureThread();
    virtual void * Entry();
    void OnExit();
    bool StartCapture();
    bool StopCapture();
    void SetRoopFlag(bool f);
    bool GetAlive();
    bool isRoServer(unsigned int Ip, short int Port);
    bool isRoLoginServer(unsigned int Ip, short int Port);
    bool isRoCharServer(unsigned int Ip, short int Port);
    bool isRoMapServer(unsigned int Ip, short int Port);
    void ParsePacket(unsigned char *Packet, unsigned long Len, PacketBuffer *Buffer);
    int ParseNormalPacket(unsigned char *Packet, unsigned long Len, PacketBuffer *Buffer);
    int ParseCryptedPacket(unsigned char *Packet, unsigned long Len, PacketBuffer *Buffer);
    void UsePacket(short int Type, unsigned char *Packet);
    void SetPacketLength(int *PacketLenTable, int MaxNum);
private:
    RoScVPCVDlg* p_dlg;
    bool alive;
    bool roop_flag;
    bool capture_flag;
    int MaxCodeNum;
    int PacketLengthTable[9999];
};

#endif // CAPTURE_THREAD_H

