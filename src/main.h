#ifndef SATO_MAIN_H
#define SATO_MAIN_H
// =========================================================================
// 1. すべてのインクルード文をファイルの「一番上」に集めます（外側に出す）
// =========================================================================
// main.cpp の最上部（1行目から）
// ハードウェアヘッダーをすべて読み込んだ「後」に独自のヘッダーを読み込みます
#include <cstdint> 
struct DataPacket {
    uint32_t sender_id;
    int32_t  value;
    int32_t  reqest_no;
    int32_t  Result;
};
// =========================================================================
// 2. インクルードがすべて「終わった後」に、C言語規格の関数定義だけを囲みます
// =========================================================================
extern "C" {
extern void my_thread_entry_func(void *p1, void *p2, void *p3);
extern void my_thread_entry_lcd(void *p1, void *p2, void *p3);
extern void my_thread_entry_main(void *p1, void *p2, void *p3);
}

#endif
//#pragma GCC pop_options