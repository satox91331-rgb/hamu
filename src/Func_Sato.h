#ifndef SATO_FUNC_H
#define SATO_FUNC_H
#include <vector>
#include <memory>
//#define FAST_DELAY 5  // main.cppの定義より前に必要だったためここに配置します
inline int baibai(int val) {
    return val * 2;
}


#include <zephyr/kernel.h>
#include <syscall_list.h>
#include <zephyr/drivers/i2c.h>


// C++の標準ヘッダー
#include <string>
#include <stdexcept>
#include <cstdio>
#include "main.h"

extern int baibai(int val);
extern void SimpleStack_test();


// 1. グローバル変数の宣言
extern DataPacket received_packet;

// 2. メッセージキューの共有宣言（名前を my_msgq にします）
extern struct k_msgq sato_my_msgq_func;
#ifdef __cplusplus
extern "C" {
#endif

// 3. 受信スレッド（関数）のプロトタイプ宣言（ my_msgq_func ）
void my_msgq_func(void *p1, void *p2, void *p3);
#ifdef __cplusplus
}
#endif

class Animal {
protected:
    std::string name;

public:
    Animal(std::string n) : name(n) {}

    void eat() const {
        printk("%s が食事をしました。\n", name.c_str());
    }
};

// 派生クラス（子クラス）: Animalクラスを継承
class Dog : public Animal {
public:
    // 派生クラスのコンストラクタで基底クラスのコンストラクタを呼び出す
    Dog(std::string n) : Animal(n) {}

    void bark() const {
        printk("%s が「バウバウ！」と鳴きました。\n", name.c_str());
    }
};

class Wanko {
public:
    virtual ~Wanko() = default;

    virtual void gei_() const {
        printk("...\n");
    }
};
class Ote : public Wanko {
public:
    void gei_() const override {
        printk("お手！\n");
    }
};

class Osuwari : public Wanko {
public:
    void gei_() const override {
        printk("お座り！\n");
    }
};
class Vector2 {
public:
    float x;
    float y;

    bool operator==(const Vector2& other) const {
        return (x == other.x) && (y == other.y);
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2{x + other.x, y + other.y};
    }
}; // ← 最後の閉じ括弧

class BankAccount {
private:
    std::string owner;
    int balance;
public:

    BankAccount(std::string o, int initial_balance) : owner(o), balance(initial_balance) {}
    void deposit(int amount) {
        balance += amount;
    }
    int getBalance() const {
        return balance;
    }
    void printAccountInfo() const {
        printk ("口座名義:%s , 残高:%d\n" ,owner.c_str() ,balance );
    }
};

class ZephyrThread {
private:
    struct k_thread thread_data;
    k_thread_stack_t *stack_ptr;
    size_t stack_size;
public:
    // コンストラクタ
    ZephyrThread(k_thread_stack_t *stack, size_t size, void (*entry)(void)) 
        : stack_ptr(stack), stack_size(size) 
    {
        k_thread_create(
            &thread_data, stack_ptr, stack_size,
            [](void *p1, void *p2, void *p3) {
                auto func = reinterpret_cast<void (*)(void)>(p1);
                func();
            },
            reinterpret_cast<void *>(entry), nullptr, nullptr,
            K_LOWEST_APPLICATION_THREAD_PRIO, 0, K_NO_WAIT
        );
    }
    void join() {
        k_thread_join(&thread_data, K_FOREVER);
    }
}; 

template <typename T>
class Sato_func {
public:
    // Satofunc_Main の定義（中身）
private:
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
    void Satofunc_Main(int funcno) {
//        lcd_init(&my_lcd, 0x27, 4, 5);
    }     // ← ★ここが Satofunc_Main 関数の閉じ括弧（追加）


}; // ← クラス
inline void SimpleStack_test() {
    // テスト用のダミー処理（空でOKです）
}
#endif