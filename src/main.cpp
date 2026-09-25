// =========================================================================
// 1. すべてのインクルード文をファイルの「一番上」に集めます（外側に出す）
// =========================================================================
// main.cpp の最上部（1行目から）
//#include "main.h"

#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/gpio.h> // GPIO制御に必須
#include <zephyr/drivers/uart.h> // uart_poll_in に必須
#include <zephyr/drivers/i2c.h>  // I2C制御に必須

// ハードウェアヘッダーをすべて読み込んだ「後」に独自のヘッダーを読み込みます
#include "sato_lcd.h"
#include "Func_Sato.h"

void my_thread_entry_main(void *p1, void *p2, void *p3);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
std::string input_buffer = "";
const struct device *const uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
Sato_func<int>* functest = new Sato_func<int>();    

LCD my_lcd;

// 遅延時間などの定数定義（エラー90行目の解決）
//#define FAST_DELAY      5
#define SLEEP_TIME_MS   500
// =========================================================================
// 2. インクルードがすべて「終わった後」に、C言語規格の関数定義だけを囲みます
// =========================================================================
extern "C" {
    const struct device *get_my_i2c_device(void) {
        // 元々動いていた本物のI2Cデバイス取得ロジック（一切変えない）
        return DEVICE_DT_GET(DT_NODELABEL(i2c0)); 
    }
}

/*
extern "C" {
extern void my_msgq_func(void *p1, void *p2, void *p3);
}
extern "C" {
extern void my_thread_entry_lcd(void *p1, void *p2, void *p3);
}
extern void my_thread_entry_main(void *p1, void *p2, void *p3);
*/
// =========================================================================
// I2C / LCD 周りの処理
// =========================================================================

/*
void SimpleStack_test(){

    string x;

    printk("文字列：");
    scanf("%d", &x);

    SimpleStack<char> s(x.length());	// スタックsの容量は文字列xの長さ

    try {
        for (int i = 0; i < x.length(); i++)	// 先頭文字から順にプッシュ
            s.push(x[i]);

        for (int i = 0; i < x.length(); i++) {
            char c = s.pop();					// ポップ（逆順に得られる）
            cout << c;
        }
        cout << '\n';
    }
    catch (const SimpleStack<char>::Overflow&) {
        cout << "\a満杯の<char>スタックにプッシュしようとしました。\n";
    }
    catch (const SimpleStack<char>::Empty&) {
        cout << "\a空の<char>スタックからポップしようとしました。\n";
    }
};
*/
// =========================================================================
// メイン関数
// =========================================================================int main(void)
#define STACK_SIZE 4096
#define THREAD_PRIORITY 5
struct k_thread my_thread_data_lcd;
struct k_thread my_thread_data_func;
struct k_thread my_thread_data_main;
K_THREAD_STACK_DEFINE(my_thread_stack_main, STACK_SIZE);
K_THREAD_STACK_DEFINE(my_thread_stack_func, STACK_SIZE);
K_THREAD_STACK_DEFINE(my_thread_stack_lcd, STACK_SIZE);
//K_MSGQ_DEFINE(my_shared_msgq, sizeof(DataPacket), 10, 4);

int main(void)
{
    struct k_thread my_thread_data_lcd;
    struct k_thread my_thread_data_func;
    struct k_thread my_thread_data_main;

    if (!device_is_ready(uart_dev)) {
        //printk ("UART device not ready!" );
        delete functest; 
        return 0;
    }


    // LEDの初期化
    if (!gpio_is_ready_dt(&led)) {
        delete functest;
        return 0;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    k_msleep(100);
    
    lcd_init(&my_lcd, 0x27, 4, 5);

    const struct device *i2c_dev = get_my_i2c_device();
    if (i2c_dev != NULL && device_is_ready(i2c_dev)) {
         lcd_clear(&my_lcd);
        send_byte(&my_lcd, 0x80, LCD_COMMAND_MODE, (uint32_t )FAST_DELAY); 
        lcd_string(&my_lcd, "Mystar Study Start");

        send_byte(&my_lcd, 0xC0, LCD_COMMAND_MODE, (uint32_t )FAST_DELAY); 
        lcd_string(&my_lcd, "Hamutarou");
    }

        gpio_pin_toggle_dt(&led);
        k_msleep(SLEEP_TIME_MS);
    k_thread_create(
        &my_thread_data_func,
        my_thread_stack_func,  // ← 定義したスタックを指定
        K_THREAD_STACK_SIZEOF(my_thread_stack_func), // ← そのサイズを計算して指定
        my_msgq_func,
        NULL, NULL, NULL,
        THREAD_PRIORITY, 0, K_NO_WAIT
    );

    k_thread_create(
        &my_thread_data_lcd,
        my_thread_stack_lcd,  // ← 定義したスタックを指定
        K_THREAD_STACK_SIZEOF(my_thread_stack_lcd), // ← そのサイズを計算して指定
        my_thread_entry_lcd,
        NULL, NULL, NULL,
        THREAD_PRIORITY, 0, K_NO_WAIT
    );

    k_thread_create(
        &my_thread_data_main,
        my_thread_stack_main,  // ← 定義したスタックを指定
        K_THREAD_STACK_SIZEOF(my_thread_stack_main), // ← そのサイズを計算して指定
        my_thread_entry_main,
        NULL, NULL, NULL,
        THREAD_PRIORITY, 0, K_NO_WAIT
    );
    int32_t count = 500;
    while(1){
//        DataPacket packet;
//        packet.sender_id = 2; // タスクBのID
//        packet.value = count++;

//        k_msgq_put(&sato_my_msgq_func, &packet, K_NO_WAIT);
//        k_msgq_put(&sato_my_msgq_lcd, &packet, K_NO_WAIT);
        k_msleep(2500); // 2.5秒待機
        k_msleep(1000);

    }    
    return 0;
}    

void my_thread_entry_main(void *p1, void *p2, void *p3) {
    int reqno = 0;
    // メインの機能選択ループ
    while (reqno != 99) {
        // 起動時や選択後の無駄な5秒待ちを削除し、すぐにメニューを表示
/*
        printk ("\nWhat functions do you want to implement?\n" );
        printk ("1  : Inheritance\n" );
        printk ("2  : polymorphism\n" );
        printk ("3  : Operator overloading\n" );
        printk ("4  : Multithreading\n" );
        printk ("5  : calculator\n" );
        printk ("6  : capsule\n" );
        printk ("7  : Class template\n" );
        printk ("8  : Array class template\n" );
        printk ("9  : Stack class template\n" );
        printk ("10 : Abstract class template\n" );
        printk ("11 : Multiple inheritance\n" );
        printk ("12 : Constructor, destructor\n" );
        printk ("13 : Vector library\n" );
        printk ("99 : Exit" );
*/
        printk(u8"実施したい機能は?\r\n");
        printk(u8"1  : 継承\r\n");
        printk(u8"2  : 多態性\r\n");
        printk(u8"3  : 演算子のオーバロード\r\n");
        printk(u8"4  : マルチスレッド\r\n");
        printk(u8"5  : 電卓\r\n");
        printk(u8"6  : カプセル\r\n");
        printk(u8"7  : クラステンプレート\r\n");
        printk(u8"8  : 配列クラステンプレート\r\n");
        printk(u8"9  : スタッククラステンプレート\r\n");
        printk(u8"10 : 抽象クラステンプレート\r\n");
        printk(u8"11 : 多重継承\r\n");
        printk(u8"12 : コンストラクタ、デストラクタ\r\n");
        printk(u8"13 : ベクトルライブラリ\r\n" );

        printk(u8"99 : 終了\r\n");

        // キーボード入力待ち受けループ
        while (true) {
            printk ("\nEnter No: " );
            input_buffer.clear();

            // 1行（改行が来るまで）の入力を待ち受けるループ
            while (true) {
                unsigned char c;
                // 1文字読み込み（データがあれば0を返す）
                if (uart_poll_in(uart_dev, &c) == 0) {
                    // エンターキー（改行）が押されたら1行入力終了
                    if (c == '\r' || c == '\n') {
                        break;
                    } else if (c == 0x7f || c == '\b') {
                        // バックスペースが押されたら1文字消す
                        if (!input_buffer.empty()) {
                            //printk ("\b \b" (std::flush;
                            input_buffer.pop_back();
                        }
                    } else {
                        // 入力された文字を画面にエコーバックし、バッファに貯める
                        //printk (c (std::flush;
                        input_buffer += c;
                    }
                }
                // CPUを占有しないようにわずかに待つ (ここで正常にIdleに入ります)
                k_sleep(K_MSEC(10));
            }

            // --- sscanf による解析 ---
            int input = 0;
            if (sscanf(input_buffer.c_str(), "%d", &input) == 1) {
                //printk ("\n--> input: " (input );
                reqno = input;
                break; // 入力待ちループを抜ける
            } else {
                //printk ("\n--> Invalid input! Please try again." );
            }
        }
        DataPacket packet;
        packet.sender_id = 2; // タスクBのID
        packet.value = reqno;
        packet.reqest_no = reqno;
        packet.Result = 0;

        k_msgq_put(&sato_my_msgq_func, &packet, K_NO_WAIT);
        k_msleep(5000);

//        functest->Satofunc_Main(reqno);

    }
    // 終了用、またはLEDのバックグラウンド無限ループ
//    while (true) {
//        gpio_pin_toggle_dt(&led);
//        k_msleep(SLEEP_TIME_MS);
//    }

    return;
}
//#pragma GCC pop_options