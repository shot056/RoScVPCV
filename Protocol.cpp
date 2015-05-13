//---------------------------------------------------------------------------
#ifdef _MSC_VER
    #include "stdafx.h"
#endif


#pragma hdrstop

#include "Protocol.h"

//---------------------------------------------------------------------------

char *PROTOCOL_NAME[]={
    "Reserved",     //  0
    "ICMP",         //  1
    "IGMP",         //  2
    "GGP",          //  3
    "IP",           //  4
    "ST",           //  5
    "TCP",          //  6
    "UCL",          //  7
    "EGP",          //  8
    "IGP",          //  9
    "BBN-RCC-MON",  // 10
    "NVP-II",       // 11
    "PUP",          // 12
    "ARGUS",        // 13
    "EMCON",        // 14
    "XNET",         // 15
    "CHAOS",        // 16
    "UDP",          // 17
    "MUX",          // 18
    "DCN-MEAS",     // 19
    "HMP",          // 20
    "PRM",          // 21
    "XNS-IDP",      // 22
    "TRUNK-1",      // 23
    "TRUNK-2",      // 24
    "LEAF-1",       // 25
    "LEAF-2",       // 26
    "RDP",          // 27
    "IRTP",         // 28
    "ISO-TP4",      // 29
    "NETBLT",       // 30
    "MFE-NSP",      // 31
    "MERIT-INP",    // 32
    "SEP",          // 33
    "3PC",          // 34
    "IDPR",         // 35
    "XTP",          // 36
    "DDP",          // 37
    "IDPR-CMTP",    // 38
    "TP++",         // 39
    "IL",           // 40
    "SIP",          // 41
    "SDRP",         // 42
    "SIP-SR",       // 43
    "SIP-FRAG",     // 44
    "IDRP",         // 45
    "RSVP",         // 46
    "GRE",          // 47
    "MHRP",         // 48
    "BNA",          // 49
    "SIPP-ESP",     // 50
    "SIPP-AH",      // 51
    "I-NLSP",       // 52
    "SWIPE",        // 53
    "NHRP",         // 54
    "unknown",      // 55
    "unknown",      // 56
    "unknown",      // 57
    "unknown",      // 58
    "unknown",      // 59
    "unknown",      // 60
    "unknown",      // 61
    "CFTP",         // 62
    "unknown",      // 63
    "SAT-EXPAK",    // 64
    "KRYPTOLAN",    // 65
    "RVD",          // 66
    "IPPC",         // 67
    "unknown",      // 68
    "SAT-MON",      // 69
    "VISA",         // 70
    "IPCV",         // 71
    "CPNX",         // 72
    "CPHB",         // 73
    "WSN",          // 74
    "PVP",          // 75
    "BR-SAT-MON",   // 76
    "SUN-ND",       // 77
    "WB-MON",       // 78
    "WB-EXPAK",     // 79
    "ISO-IP",       // 80
    "VMTP",         // 81
    "SECURE-VMTP",  // 82
    "VINES",        // 83
    "TTP",          // 84
    "NSFNET-IGP",   // 85
    "DGP",          // 86
    "TCF",          // 87
    "IGRP",         // 88
    "OSPFIGP",      // 89
    "Sprite-RPC",   // 90
    "LARP",         // 91
    "MTP",          // 92
    "AX.25",        // 93
    "IPIP",         // 94
    "MICP",         // 95
    "SCC-SP",       // 96
    "ETHERIP",      // 97
    "ENCAP",        // 98
    "unknown",      // 98
    "GMTP"          // 99
    };


unsigned int _fastcall _uint(unsigned int p)
{
    return (p&0xff000000)>>24 | (p&0x00ff0000)>>8 | (p&0x0000ff00)<<8 | (p&0x000000ff)<<24;
}
    
