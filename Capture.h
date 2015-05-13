//---------------------------------------------------------------------------

#ifndef CaptureH
#define CaptureH

#include <winsock2.h>
//---------------------------------------------------------------------------

#define MAX_ADAPTER 10

#define MAX_RECV_SIZE 65535

// 受信バッファのサイズ   

//---------------------------------------------------------------------------
// 固有のエラー番号を定義   
//---------------------------------------------------------------------------
#define ERROR_SOCKET_BUSY        0x2001
#define ERROR_WINSOCK2_INIT      0x2002
#define ERROR_WSASOCKET          0x2003
#define ERROR_ADDRESS_LIST_QUERY 0x2004
#define ERROR_BIND               0x2005
#define ERROR_SIO_RCVALL         0x2006
#define ERROR_SOCKET             0x2007

//---------------------------------------------------------------------------
// すべての関数は、戻り値がFALSEの時エラー   
// GetLastError()でエラーの内容を確認してください   
//---------------------------------------------------------------------------


// WinSockの初期化   
extern BOOL Startup(void);

//WinSockの終了処理   
extern BOOL Cleanup(void);

// アダプタ一覧取得   
extern BOOL EnumAdapter(unsigned long AddressList[MAX_ADAPTER],int *max);

// キャプチャー開始   
extern BOOL Start(unsigned long ip_address);

// キャプチャー終了   
extern BOOL Stop(void);

// パケット受信   

extern BOOL Recv(unsigned char Buffer[MAX_RECV_SIZE],unsigned long *Len);


#endif
