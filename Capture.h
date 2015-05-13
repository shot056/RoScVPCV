//---------------------------------------------------------------------------

#ifndef CaptureH
#define CaptureH

#include <winsock2.h>
//---------------------------------------------------------------------------

#define MAX_ADAPTER 10

#define MAX_RECV_SIZE 65535

// ��M�o�b�t�@�̃T�C�Y   

//---------------------------------------------------------------------------
// �ŗL�̃G���[�ԍ����`   
//---------------------------------------------------------------------------
#define ERROR_SOCKET_BUSY        0x2001
#define ERROR_WINSOCK2_INIT      0x2002
#define ERROR_WSASOCKET          0x2003
#define ERROR_ADDRESS_LIST_QUERY 0x2004
#define ERROR_BIND               0x2005
#define ERROR_SIO_RCVALL         0x2006
#define ERROR_SOCKET             0x2007

//---------------------------------------------------------------------------
// ���ׂĂ̊֐��́A�߂�l��FALSE�̎��G���[   
// GetLastError()�ŃG���[�̓��e���m�F���Ă�������   
//---------------------------------------------------------------------------


// WinSock�̏�����   
extern BOOL Startup(void);

//WinSock�̏I������   
extern BOOL Cleanup(void);

// �A�_�v�^�ꗗ�擾   
extern BOOL EnumAdapter(unsigned long AddressList[MAX_ADAPTER],int *max);

// �L���v�`���[�J�n   
extern BOOL Start(unsigned long ip_address);

// �L���v�`���[�I��   
extern BOOL Stop(void);

// �p�P�b�g��M   

extern BOOL Recv(unsigned char Buffer[MAX_RECV_SIZE],unsigned long *Len);


#endif
