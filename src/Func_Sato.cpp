#include "main.h"
#include "sato_lcd.h"
#include "Func_Sato.h"

#include <iostream>
#include <vector>
#include <memory>   // std::unique_ptr, std::make_unique 用

#include <thread>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/uart.h>
#include <stdio.h>

void Satofunc_01();
void Satofunc_02();
void Satofunc_03();
void Satofunc_04();
void Satofunc_05();
void Satofunc_06();
void Satofunc_07();
void Satofunc_08();
void Satofunc_09();
void Satofunc_10();
void Satofunc_11();
void Satofunc_12();
void Satofunc_13();

const struct device *get_my_i2c_device(void);
using namespace std; 


extern struct k_thread thread_a_data;
extern k_thread_stack_t thread_a_stack[];

extern struct k_thread thread_b_data;   
extern k_thread_stack_t thread_b_stack[]; 
extern void ThreadA(void);
extern void ThreadB(void);
extern LCD my_lcd;

//LCD my_lcd;
int msg_ = 0;
int end_flag_ = 0;

DataPacket received_packet;
K_MSGQ_DEFINE(sato_my_msgq_func, sizeof(DataPacket), 10, 4);
//extern "C" {
void my_msgq_func(void *p1, void *p2, void *p3) {
    char *thread_name = (char *)p1; // 引数1をキャストして使用
    

    while (true) {
        int ret = k_msgq_get(&sato_my_msgq_func, &received_packet, K_FOREVER);
        if (ret == 0) {
            switch (received_packet.reqest_no) {
            case 1:  Satofunc_01(); break;
            case 2:  Satofunc_02(); break;
            case 3:  Satofunc_03(); break;
            case 4:  Satofunc_04(); break;
            case 5:  Satofunc_05(); break;
            case 6:  Satofunc_06(); break;
            case 7:  Satofunc_07(); break;
            case 8:  Satofunc_08(); break;
            case 9:  Satofunc_09(); break;
            case 10: Satofunc_10(); break;
            case 11: Satofunc_11(); break;
            case 12: Satofunc_12(); break;
            case 13: Satofunc_13(); break;
            default: break;
            } // ← ここは switch の閉じ括弧

            printk("[Class RX] ID: %d, Value: %d\n", received_packet.sender_id, received_packet.value);
            //            std::cout << "[Class RX] ID: " << received_packet.sender_id 
            //                      << ", Value: " << received_packet.value << std::endl;
        }
    }
}
//}

//    template <typename T>
//    class Sato_func {
//    private:
        void Satofunc_01(){
            Dog myDog("ponta");
            myDog.bark();
            printk ("Result:OK!!\n" ); 
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.1");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");
        }

        void Satofunc_02(){
            std::vector<std::unique_ptr<Wanko>> wankos;
            wankos.push_back(std::unique_ptr<Ote>(new Ote()));
            wankos.push_back(std::unique_ptr<Osuwari>(new Osuwari()));            
            for (const auto& wanko : wankos) {
                wanko->gei_();
            }
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.2");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");
            printk ("Result:OK!!\n" );
        };
        void Satofunc_03(){

            Vector2 p1 = { 1, 2 };
            Vector2 p2 = { 3, 4 };
            printk("p1: (%f, %f)\n", (double)p1.x, (double)p1.y);
            printk("p2: (%f, %f)\n", (double)p2.x, (double)p2.y);

                    // オーバーロードした + 演算子を使用
            Vector2 p3 = p1 + p2;
            printk("p3: (%f, %f)\n", (double)p3.x, (double)p3.y);

                    // オーバーロードした == 演算子を使用
            if (p1 == p2) {
                printk("p1 and p2 are equal.\n");
            }
            else {
                printk("p1 and p2 are not equal.\n"); // こちらが実行されるはず。。
            }
            printk("Result:OK!!\n");
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.3");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");
        };
        void Satofunc_04(){
            // スタック領域をクラス（ヘッダー）内部で安全に確保するための、Zephyrの標準マクロ
            // もしすでにマクロが外にある場合は、関数の内側で以下のように定義するとエラーになりません
            #define STACK_SIZE_ 1024
            static K_THREAD_STACK_DEFINE(my_th_a_stack, STACK_SIZE_);
            static K_THREAD_STACK_DEFINE(my_th_b_stack, STACK_SIZE_);
            static struct k_thread th_a_data;
            static struct k_thread th_b_data;

            // ★スレッドの起動関数（ThreadA / ThreadB）の中身がまだ無い場合、
            //   以下のようにインライン（ラムダ式）で直接処理を書いてしまうのが一番安全です
            k_tid_t tid_a = k_thread_create(
                &th_a_data, my_th_a_stack, K_THREAD_STACK_SIZEOF(my_th_a_stack),
                [](void *p1, void *p2, void *p3) {
                    while (1) {
                        printk("[Thread A] Running...\n");
                        k_msleep(1000);
                    }
                }, NULL, NULL, NULL, 5, 0, K_NO_WAIT
            );

            k_tid_t tid_b = k_thread_create(
                &th_b_data, my_th_b_stack, K_THREAD_STACK_SIZEOF(my_th_b_stack),
                [](void *p1, void *p2, void *p3) {
                    while (1) {
                        printk("[Thread B] Running...\n");
                        k_msleep(1500);
                    }
                }, NULL, NULL, NULL, 5, 0, K_NO_WAIT
            );

            printk("マルチスレッド（ThreadA / ThreadB）を起動しました。3秒間実行します...\n");
            k_msleep(3000); // 3秒間泳がせる

            // テストのために一時的にスレッドを止めてメニューに戻る処理
            k_thread_abort(tid_a);
            k_thread_abort(tid_b);
            printk("マルチスレッドテスト終了。\n");
            printk ("Result:OK!!\n" );            //printk ("OK!!\n" );
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.4");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");

        };
        void Satofunc_05(){
            double num1, num2;
            char op;

            printk("\n--- 簡易電卓 ---\n");
            printk("計算式を入力してください (例: 5 + 3) : ");
            scanf("%lf %c %lf", &num1, &op, &num2);

            switch (op) {
            case '+': printk("結果: %f\n", num1 + num2);; break;
            case '-': printk("結果: %f\n", num1 - num2); break;
            case '*': printk("結果: %f\n", num1 - num2); break;
            case '/':
                if (num2 != 0){ printk("結果: %f\n", num1 / num2);
                    lcd_clear(&my_lcd);
                    send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
                    lcd_string(&my_lcd, "ReqNo.5");

                    send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
                    lcd_string(&my_lcd, "Result:OK");
                }
                else printk("エラー: 0で割ることはできません。\n");
                break;
            default: printk("エラー: 無効な演算子です。\n");;
            };
        };
        void Satofunc_06(){
            BankAccount myAccount("satou", 1000);
            myAccount.deposit(500);
            myAccount.printAccountInfo();

            printk("現在の値: %d \n", myAccount.getBalance());
            printk ("Result:OK!!\n" );            //printk ("OK!!\n" );
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.6");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");
        
        };
        void Satofunc_07(){
            printk("テンプレート実施前の値: 11 \n");
            printk("テンプレート実施後の値: %d \n", baibai(11));
            printk ("Result:OK!!\n" );            //printk ("OK!!\n" );
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.7");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");
        };
        void Satofunc_08(){
            std::vector<int> vec;

            vec.push_back(10);
            vec.push_back(20);
            for (int val : vec) printk("%d ", val);
            printk ("Result:OK!!\n" );            //printk ("OK!!\n" );
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.8");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");
        };
        void Satofunc_09(){
            SimpleStack_test();
            printk ("Result:OK!!\n" ); 
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.9");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");

        };
        void Satofunc_10(){
            printk("[LOG] 10番: スタッククラステンプレートを実行します。\n");
            
            MyStack<int>* s = new MyArrayStack<int>();

            int x = 100; 
            s->push(x);
            printk("[LOG] スタックに数値 %d を push しました。\n", x);
            
            int y = 200;
            s->push(y);
            printk("[LOG] スタックに数値 %d を push しました。\n", y);

            printk ("Result:OK!!\n" );
            delete s;
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.10");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");

        };
        class A { public: virtual ~A() {} };
        class B { public: virtual ~B() {} };
        class C : public A, public B {};

        void Satofunc_11(){
            C c;
            A* ptr_a = &c;		// ptr_aはcを指す
            B* ptr_b = &c;		// ptr_bはcを指す

            printk("ptr_a = %p\n", (void*)ptr_a);   // cを指すptr_aの値を表示
            printk("ptr_b = %p\n", (void*)ptr_b);	// cを指すptr_bの値を表示
            printk("&c    = %p\n", (void*)&c);		// cへのポインタ値を表示
            printk ("Result:    OK!!\n" );
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.11");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");

        };
        void Satofunc_12(){
            class MyClass {
            public:
                MyClass() {
                    printk("コンストラクタが呼び出されました。\n");
                }
                ~MyClass() {
                    printk("デストラクタが呼び出されました。\n");
                }
            };
            printk ("Result:OK!!\n" );
            MyClass obj; // コンストラクタが呼び出される    
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.12");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");
        };
        void Satofunc_13(){
            std::vector<int> vec;
            // 要素の追加
            vec.push_back(10);
            vec.push_back(20);
            vec.push_back(30);
            // 要素の表示
            printk("ベクトルの要素: ");
            for (int val : vec) {
                printk("%d ", val);
            }
            printk("\n");
            // 要素の削除
            vec.pop_back(); // 最後の要素を削除
            // 削除後の要素の表示
            printk("削除後のベクトルの要素: ");
            for (int val : vec) {
                printk("%d ", val);
            }
            printk("\n");
            printk ("Result:OK!!\n" );
            lcd_clear(&my_lcd);
            send_byte(&my_lcd, (uint8_t)0x80, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "ReqNo.13");

            send_byte(&my_lcd, (uint8_t)0xC0, (uint8_t)LCD_COMMAND_MODE, (uint32_t)FAST_DELAY);
            lcd_string(&my_lcd, "Result:OK");
        };
//    };

void Chuusyou_test() {
    MyStack<int>* s = new MyArrayStack<int>();

    while (1) {
        int menu;
        printk("(1)プッシュ (2)ポップ (0)終了：");
        scanf("%d", &menu);

        if (menu == 0) break;

        switch (menu) {
            int x;
            //--- プッシュ ---//
        case 1: printk("データ：");
            scanf("%d", &x);
        if (s->isFull()) { 
            printk("スタックが満杯です。\n");
            break; // ループを抜けるなどの処理
        } else {
            s->push(10);
        }
        break;

            //--- ポップ ---//
        case 2:
            if (s->isEmpty()) {
            printk("スタックは空です。\n");
        } else {
            // 変数 x の型に合わせて pop して出力する（例は int 型の場合）
            int x = s->pop(); 
            printk("ポップしたデータは%dです。\n", x);
        }
              break;
        }
        if (menu == 0) break;
    }
    delete s;
}
void Satofunc_Main(int);
//    void Satofunc_01();
//    void Satofunc_02();
//    void Satofunc_03();
//    void Satofunc_04();
//    void Satofunc_05();
//    void Satofunc_06();
//    void Satofunc_07();
//    void Satofunc_08();
//    void Satofunc_09();
//    void Satofunc_10();
//    void Satofunc_11();
//    void Satofunc_12();
 //   void Satofunc_13();



template class Sato_func<int>;