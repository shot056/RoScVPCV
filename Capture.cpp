//---------------------------------------------------------------------------
#ifdef _MSC_VER
    #include "stdafx.h"
#endif
#include "Capture.h"
//---------------------------------------------------------------------------



// Winsock初期化用構造体
WSADATA wsd;
// WinSock2が使用可能かどうかのフラグ
static BOOL WinSock2 = FALSE;
// ソケットハンドル
static SOCKET sock=-1;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// WinSockの初期化
//---------------------------------------------------------------------------
BOOL Startup(void)
{
    //Ver2.0 でWinSockを初期化する
    if(0==WSAStartup(MAKEWORD(2,0),&wsd))
        WinSock2 = TRUE;
    return WinSock2;
}
//---------------------------------------------------------------------------
//WinSockの終了処理
//---------------------------------------------------------------------------
BOOL Cleanup(void)
{
    if(WinSock2){
        WinSock2 = FALSE;
        if(0==WSACleanup())
            return TRUE;
        return FALSE;
    }
    return TRUE;
}
//---------------------------------------------------------------------------
// アダプタの一覧取得
//---------------------------------------------------------------------------
BOOL EnumAdapter(unsigned long AddressList[MAX_ADAPTER],int *max)
{
    BOOL Ret=FALSE;

    char Buffer[1024];
    SOCKET_ADDRESS_LIST *slist;
    *max=0;

    // WinSock2の初期化が完了していない場合は、処理を継続できない
    if(!WinSock2){
        SetLastError(ERROR_WINSOCK2_INIT);
        return FALSE;
    }

    // ソケットが使用中の場合は、処理を継続できない
    if(sock!=-1){
        SetLastError(ERROR_SOCKET_BUSY);
        return Ret;
    }

    if( INVALID_SOCKET != (sock = socket(AF_INET,SOCK_RAW,IPPROTO_IP))){

        DWORD d=0;
        if(0 == WSAIoctl(sock,SIO_ADDRESS_LIST_QUERY,NULL,0,Buffer,1024,&d,NULL,NULL)){
            slist = (SOCKET_ADDRESS_LIST *)Buffer;
            for(int i=0;i<slist->iAddressCount && (*max)<MAX_ADAPTER;i++){
                sockaddr *so = (sockaddr*)(slist->Address[i].lpSockaddr);
                if(so->sa_family==AF_INET){
                    sockaddr_in *s = (sockaddr_in*)(slist->Address[i].lpSockaddr);
                    AddressList[(*max)] = s->sin_addr.s_addr;
                    (*max)++;
                }
            }
            Ret=TRUE;
        }else{
            SetLastError(ERROR_ADDRESS_LIST_QUERY);
        }
    }else{
        SetLastError(ERROR_WSASOCKET);
    }
    if(sock!=-1)
        closesocket(sock);
    sock=-1;

    return Ret;
}

//---------------------------------------------------------------------------
//キャプチャー開始
//---------------------------------------------------------------------------
BOOL Start(unsigned long ip_address)
{
    BOOL Ret=FALSE;
    unsigned long optval;
    SOCKADDR_IN   addr_in;

    // WinSock2の初期化が完了していない場合は、処理を継続できない
    if(!WinSock2){
        SetLastError(ERROR_WINSOCK2_INIT);
        return FALSE;
    }

    // ソケットが使用中の場合は、処理を継続できない
    if(sock!=-1){
        SetLastError(ERROR_SOCKET_BUSY);
        return FALSE;
    }

    //SOCKETの生成
    if( INVALID_SOCKET == (sock = socket(AF_INET,SOCK_RAW,IPPROTO_IP))){
        SetLastError(ERROR_WSASOCKET);
        goto end;
    }

    //SOCKET のデータセット
    addr_in.sin_addr.s_addr=ip_address;
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(0);

    //bind
    if(bind(sock,(SOCKADDR *)&addr_in,sizeof(addr_in))==SOCKET_ERROR) {
//        DWORD e = WSAGetLastError();
        WSAGetLastError();
        SetLastError(ERROR_BIND);
        goto end;
    }


    // プロミスキャスモードの\設定
    #define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
    //#define SIO_RCVALL  0x98000001
    optval=1; //PROMISC
    if(0!=ioctlsocket(sock,SIO_RCVALL,&optval)){
        SetLastError(ERROR_SIO_RCVALL);
        goto end;
    }

    // 非ブロッキングモードの\設定
    unsigned long arg;
    arg = 1L;
    if(0!=ioctlsocket(sock,FIONBIO,&arg)){
        SetLastError(WSAGetLastError());
        goto end;
    }

    Ret=TRUE;
end:
    if(!Ret && sock!=-1)
        closesocket(sock);
    return Ret;
}
//---------------------------------------------------------------------------
// キャプチャー終了
//---------------------------------------------------------------------------
BOOL Stop(void)
{
    // WinSock2の初期化が完了していない場合は、処理を継続できない
    if(!WinSock2){
        SetLastError(ERROR_WINSOCK2_INIT);
        return FALSE;
    }

    // ソケットの準備ができていない場合は、処理を継続できない
    if(sock==-1){
        SetLastError(ERROR_SOCKET);
        return FALSE;
    }else{
        closesocket(sock);
        sock=-1;
        return TRUE;
    }
}


//---------------------------------------------------------------------------
// パケット受信
//---------------------------------------------------------------------------
BOOL Recv(unsigned char Buffer[MAX_RECV_SIZE],unsigned long *Len)
{
    // WinSock2の初期化が完了していない場合は、処理を継続できない
    if(!WinSock2){
        SetLastError(ERROR_WINSOCK2_INIT);
        return FALSE;
    }

    // ソケットの準備ができていない場合は、処理を継続できない
    if(sock==-1){
        SetLastError(ERROR_SOCKET);
        return FALSE;
    }

    if(SOCKET_ERROR==(*Len = recv(sock,(char *)Buffer,MAX_RECV_SIZE,0))){
        DWORD Error = WSAGetLastError();
        if(Error==WSAEWOULDBLOCK){
            *Len=0;
            return TRUE;
        }else{
            SetLastError(Error);
            return FALSE;
        }
    }
    return TRUE;
}


