#ifndef SATO_LCD_H
#define SATO_LCD_H

#include <zephyr/kernel.h>
#include <stdint.h>
#include "main.h"
// ====================================================
// 1. C++ テンプレートクラス
// ====================================================
#ifdef __cplusplus
extern "C" {
#endif
extern struct k_msgq sato_my_msgq_lcd;
extern DataPacket received_packet;

void my_msgq_lcd(void *p1, void *p2, void *p3);
#ifdef __cplusplus
}
#endif

template <typename T>
class MyStack {
protected:
    int size = 0;
    int capacity = 10;
public:
    virtual ~MyStack() {}
    virtual bool isFull() { return this->size >= this->capacity; }
    virtual bool isEmpty() { return this->size == 0; }
    virtual void push(T val) = 0;
    virtual T pop() = 0;
};

template <typename T>
class MyArrayStack : public MyStack<T> {
private:
    int top = 0;
    T data[100]; // 配列の実体を定義
public:
    void push(T val) override {
        if (top >= 100) return;
        data[top++] = val;
    }
    T pop() override {
        if (top <= 0) return T();
        return data[--top];
    }
};

// ====================================================
// 2. C言語用宣言 (sato_lcd.cpp の本物の型に完全に一致させます)
// ====================================================
#ifdef __cplusplus
extern "C" {
#endif

#define LCD_COMMAND_MODE  0x00
#define LCD_DATA_MODE     0x01
#define LCD_BACKLIGHT     0x08
#define FAST_DELAY 80
#define SLOW_DELAY 2000
#define LCD_CLEARDISPLAY 0x01
#define LCD_ENABLE_PIN 0x04
#define LCD_COMMAND_MODE 0x00
#define LCD_CHAR_MODE 0x01
#define LCD_BACKLIGHT 0x08
#define LCD_FUNCTIONSET 0x20
#define LCD_2LINE 0x08
#define LCD_DISPLAYCONTROL 0x08
#define LCD_DISPLAYON 0x04
#define LCD_ENTRYMODESET 0x04
#define LCD_ENTRYLEFT 0x02

typedef struct {
    int addr; 
    uint8_t pin_sda;
    uint8_t pin_scl;
    int i2c_type;
} LCD;

extern const struct device *get_my_i2c_device(void);
void lcd_init(LCD *lcd_inst, int address, uint8_t pin_sda, uint8_t pin_scl);
void send_byte(LCD *lcd_inst, uint8_t val, uint8_t mode, uint32_t delay_us); 
void lcd_clear(LCD *lcd_inst); 
void lcd_string(LCD *lcd_inst, const char *str);

#ifdef __cplusplus
}
#endif

#endif // SATO_LCD_