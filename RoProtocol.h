//---------------------------------------------------------------------------

#ifndef RoProtocolH
#define RoProtocolH
//---------------------------------------------------------------------------
// unsigned char = 1bytes 8bits
// short int     = 2byte 16bits
// unsigned int  = 4byte 32bits
// B = 1byte
// w = 2byte
// l = 4byte

// #define RO_MAX_CODE_NUM 0x0300
// #define MAX_CODE_NUM 0x0400

// int PacketLen[MAX_CODE_NUM];
// static short RoPacketLen[RO_MAX_CODE_NUM];

// bool SetPacketLengthTable (void) {
//     if( RO_MAX_CODE_NUM > MAX_CODE_NUM ) {
//         return false;
//     }
//     for ( int i = 0; i < RO_MAX_CODE_NUM; i ++ ) {
//         PacketLen[i] = RoPacketLen[i];
//     }
// }

typedef struct _RO_MESSAGE_TYPE {
    unsigned short int Type;     // 2byte 16bits
    short int Len;      // 2byte 16bits
} RO_MESSAGE_TYPE, *LP_RO_MESSAGE_TYPE;

/* R 0069 <len>.w <login ID1>.l <account ID>.l <login ID2>.l ?.30B <sex>.B
 * {<IP>.l <port>.w <server name>.20B <login users>.w <maintenance>.w <new>.w}.32B*
 *	login成功&鯖情報 */
typedef struct _RO_MESSAGE_0069 {
    short int Type;             // 2byte 16bits
    short int Len;              // 2byte 16bits _
    unsigned int LoginId;       // 4byte 32bits _
    unsigned int AccountId;     // 4byte 32bits _
//    unsigned int LoginId2;    // 4byte 32bits _
//    unsigned char Unknown[32];    // 1byte 8bits * 32 32byte 256 _ _ _ _ _ _ _ _
//    unsigned char Sex;            // 1byte 8bits
} RO_MESSAGE_0069, *LP_RO_MESSAGE_0069;

/* R 006b <len>.w <charactor select data>.106B*
 *	キャラセレ鯖接続成功&キャラクタデータ 
 *	<charactor select data> = <charactor ID>.l <base exp>.l <zeny>.l <job exp>.l <job level>.l ?.8B
 *	 <option>.l <karma>.l <manner>.l <status point>.w <HP>.w <MaxHP>.w <SP>.w <MaxSP>.w <speed>.w <class>.w <hair>.w
 *	 <weapon>.w <base level>.w <skill point>.w <head_bottom>.w <sheild>.w
 *	 <head option top>.w <head option mid>.w <hair color>.w
 *	 ?.w <name>.24B <STR>.B <AGI>.B <VIT>.B <INT>.B <DEX>.B <LUK>.B <charactor number>.B ?.B */
typedef struct _RO_MESSAGE_006B {
    short int Type;             // 2byte 16bits
    short int Len;              // 2byte 16bits _
} RO_MESSAGE_006B, *LP_RO_MESSAGE_006B;

typedef struct _RO_MESSAGE_006B_CHARDATA {
    unsigned int CharId;        // 4byte 32bits _
    unsigned int BaseExp;       // 4byte 32bits _                                // OK
    unsigned int Zeny;          // 4byte 32bits _                                // OK
    unsigned int JobExp;        // 4byte 32bits _
    unsigned int JobLv;         // 4byte 32bits _                                // OK
    unsigned char unknown1[8];  // 1byte 8bits * 8 8byte 64bits _ _
    unsigned int option;        // 4byte 32bits _
    unsigned int karma;         // 4byte 32bits _
    unsigned int manner;        // 4byte 32bits _
    unsigned char unknown2[2];  // 1byte 8bits * 2 2byte 8bits
    short int HP;               // 2byte 16bits _                                // OK
    short int MaxHP;            // 2byte 16bits                                  // OK
    short int SP;               // 2byte 16bits _                                // OK
    short int MaxSP;            // 2byte 16bits                                  // OK
    short int Speed;            // 2byte 16bits _
    short int Class;            // 2byte 16bits                                  // OK
    short int Hair;             // 2byte 16bits _
    short int Unknown3;      // 2byte 16bits
    short int BaseLv;           // 2byte 16bits _
    short int SkillPoint;       // 2byte 16bits
    short int Unknown4;         // 2byte 16bits _
    short int Sheild;           // 2byte 16bits
    short int HeadTop;          // 2byte 16bits _
    short int HeadMid;          // 2byte 16bits
    short int HairColor;        // 2byte 16bits _
    short int Unknown5;         // 2byte 16bits
    unsigned char Name[24];     // 1byte 8bits * 24 24byte 136bits _ _ _ _ _ _
    unsigned char Str;          // 1byte 8bits
    unsigned char Agi;          // 1byte 8bits _
    unsigned char Vit;          // 1byte 8bits
    unsigned char Int;          // 1byte 8bits
    unsigned char Dex;          // 1byte 8bits
    unsigned char Luk;          // 1byte 8bits _
    unsigned char Number;       // 1byte 8bits
    unsigned char Unknown6;     // 1byte 8bits
} RO_MESSAGE_006B_CHARDATA, *LP_RO_MESSAGE_006B_CHARDATA; // 106byte

/* R 0071 <charactor ID>.l <map name>.16B <ip>.l <port>.w
 *	キャラクタ選択成功&マップ名&ゲーム鯖IP/port */
typedef struct _RO_MESSAGE_0071 {
    short int Type;             // 2byte 16bits
    unsigned int CharId;        // 4byte 32bits _
    unsigned char Map[16];      // 1byte 8bits * 16 16byte 128bits _ _ _ _
    unsigned int IP;            // 4byte 32bits _
    short int Port;             // 2byte 16bits
} RO_MESSAGE_0071, *LP_RO_MESSAGE_0071;

/* R 010f <len>.w {<skill ID>.w <target type>.w ?.w <lv>.w <sp>.w <range>.w <skill name>.24B <up>.B}.37B*
 *	スキル情報の塊。skill nameは一部流れて来ない物がある＞AL_PNEUMA,PR_SLOWPOISON等 
 *	target typeは0-パッシブ、1-敵、2-場所、4-即時発動、16-味方 
 *	lv=0 up=0の場合はリストに出してない? */
typedef struct _RO_MESSAGE_010F_SKILL {
    short int Id;                // 2byte 16bits
    short int TargType;          // 2byte 16bits
    short int unknown;           // 2byte 16bits
    short int Lv;                // 2byte 16bits
    short int Sp;                // 2byte 16bits
    short int Range;             // 2byte 16bits
    unsigned char Name[24];      // 1byte 8bits * 24 24byte 192bits
    unsigned char Up;            // 1byte 8bits
} RO_MESSAGE_010F_SKILL, *LP_RO_MESSAGE_010F_SKILL; // *37

typedef struct _RO_MESSAGE_010F {
    short int Type;             // 2byte 16bits
    short int Len;              // 2byte 16bits _
} RO_MESSAGE_010F, *LP_RO_MESSAGE_010F;

/* S 02B9 {<type>.B <ID>.l <Lv>.w}.7B*27
 *	typeは00:アイテム、01:スキル 
 *	ホットキー保存 */
typedef struct _RO_MESSAGE_02B9_SHORTCUT {
    //    unsigned char Flag;         // 1byte 8bits
    unsigned int Id;            // 4byte 32bits
    short int Lv;               // 2byte 16bits
} RO_MESSAGE_02B9_SHORTCUT, *LP_RO_MESSAGE_02B9_SHORTCUT; // *27

typedef struct _RO_MESSAGE_02B9 {
    short int Type;             // 2byte 16bits
    //    _RO_MESSAGE_02B9_SHORTCUT Shortcut[27];
} RO_MESSAGE_02B9, *LP_RO_MESSAGE_02B9;

#endif

