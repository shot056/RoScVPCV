//---------------------------------------------------------------------------

#ifndef ProtocolH
#define ProtocolH
//---------------------------------------------------------------------------

#define TH_OFF(th) (((th)-> th_off & 0xf0) >> 4)
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80

typedef struct _PROTOCOL_ETHER{
    unsigned char Mac[2][6];
    short int Type;
} PROTOCOL_ETHER, *LP_PROTOCOL_ETHER;

typedef struct _PROTOCOL_ARP{
    short int HwType;           // 2byte 16bits
    short int Type;             // 2byte 16bits
    unsigned char HwLen;        // 1byte 8bits
    unsigned char ProtoLen;     // 1byte 8bits
    short int Code;             // 2byte 16bits
    unsigned char SrcMac[6];    // 1byte 8bits * 6 6byte 48bits
    unsigned int SrcIp;         // 4byte 32bits
    unsigned char DstMac[6];    // 1byte 8bits * 6 6byte 48bits
    unsigned int DstIp;         // 4byte 32bits
} PROTOCOL_ARP, *LP_PROTOCOL_ARP;

typedef struct _PROTOCOL_IP{
    unsigned char Ver_Len;      // 1byte 8bits
    unsigned char TOS;          // 1byte 8bits
    short int TotalLen;         // 2byte 16bits
    short int Ident;            // 2byte 16bits
    short int Flags;            // 2byte 16bits
    unsigned char TTL;          // 1byte 8bits
    unsigned char Protocol;     // 1byte 8bits
    short int CheckSum;         // 2byte 16bits
    unsigned int SrcIp;         // 4byte 32bits
    unsigned int DstIp;         // 4byte 32bits
    unsigned char *option;      // 1byte 8bits
} PROTOCOL_IP, *LP_PROTOCOL_IP;

typedef struct _PROTOCOL_TCP{
    short int SrcPort;          // 2byte 16bits
    short int DstPort;          // 2byte 16bits
    unsigned int Squence;       // 4byte 32bits
    unsigned int Ack;           // 4byte 32bits
    unsigned char Offset;       // 1byte 8bits
    unsigned char FLG;          // 1byte 8bits
    short int Window;           // 2byte 16bits
    short int CheckSum;         // 2byte 16bits
    short int Urgent;           // 2byte 16bits
    unsigned char Option;       // 1byte 8bits
} PROTOCOL_TCP, *LP_PROTOCOL_TCP;

typedef struct _PROTOCOL_UDP{
    short int Port[2];          // 2byte 16bits * 2 4byte 32bits
    short int UdpLen;           // 2byte 16bits
    short int CheckSum;         // 2byte 16bits
} PROTOCOL_UDP, *LP_PROTOCOL_UDP;

typedef struct _PROTOCOL_ICMP{
    unsigned char Type;         // 1byte 8bits
    unsigned char Code;         // 1byte 8bits
    short int CheckSum;         // 2byte 16bits
} PROTOCOL_ICMP, *LP_PROTOCOL_ICMP;




extern unsigned int _fastcall _uint(unsigned int p);


extern char *PROTOCOL_NAME[];

#define PACKET_STATE_END      0
#define PACKET_STATE_CONTINUE 1
#define PACKET_STATE_ERROR    -1

#endif
