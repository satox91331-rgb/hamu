//#include "main.h"
#include "sato_lcd.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/util.h>
#include <zephyr/arch/cpu.h> 
#include <stdint.h>
#include <stdio.h>


K_MSGQ_DEFINE(sato_my_msgq_lcd, sizeof(DataPacket), 10, 4);

extern "C" {
void my_thread_entry_lcd(void *p1, void *p2, void *p3) {
    char *thread_name = (char *)p1; // 引数1をキャストして使用
    
    while (true) {
        int ret = k_msgq_get(&sato_my_msgq_lcd, &received_packet, K_FOREVER);
        if (ret == 0) {
            printf("[Class RX] ID: %d, Value: %d\n", received_packet.sender_id, received_packet.value);
//            std::cout << "[Class RX] ID: " << received_packet.sender_id 

//                      << ", Value: " << received_packet.value << std::endl;
        }
    }
}
}
void lcd_init(LCD* lcd_inst, int address, uint8_t pin_sda, uint8_t pin_scl) {
    lcd_inst->addr = (address == -1) ? 0x27 : address;
    lcd_inst->pin_sda = pin_sda;
    lcd_inst->pin_scl = pin_scl;
    lcd_inst->i2c_type = 0;

        lcd_inst->addr = (address == -1) ? 0x27 : address;
    lcd_inst->pin_sda = pin_sda;
    lcd_inst->pin_scl = pin_scl;
    lcd_inst->i2c_type = 0;

    const struct device *i2c_dev = get_my_i2c_device();
    if (i2c_dev == NULL || !device_is_ready(i2c_dev)) {
        return;
    }

    k_msleep(50);
    send_byte(lcd_inst, 0x03, LCD_COMMAND_MODE, SLOW_DELAY);
    k_msleep(5);
    send_byte(lcd_inst, 0x03, LCD_COMMAND_MODE, FAST_DELAY);
    k_busy_wait(150);
    send_byte(lcd_inst, 0x03, LCD_COMMAND_MODE, FAST_DELAY);
    send_byte(lcd_inst, 0x02, LCD_COMMAND_MODE, FAST_DELAY);

    send_byte(lcd_inst, LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND_MODE, FAST_DELAY);
    send_byte(lcd_inst, LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND_MODE, FAST_DELAY);
    lcd_clear(lcd_inst);
    send_byte(lcd_inst, LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND_MODE, FAST_DELAY);
    // (※もし追加の初期化処理があればここに入ります)
}

void i2c_write_byte(LCD* lcd_inst, uint8_t val) {
    const struct device *i2c_dev = get_my_i2c_device();
    if (i2c_dev != NULL) {
        i2c_write(i2c_dev, &val, 1, lcd_inst->addr);
    }
}

void send_byte(LCD* lcd_inst, uint8_t val, uint8_t mode, uint32_t delay_us) {
    uint8_t high_nibble = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low_nibble = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(lcd_inst, high_nibble | 0x04);
    k_busy_wait(1); // ★ k_udelay から k_busy_wait に変更
    i2c_write_byte(lcd_inst, high_nibble & ~0x04);

    i2c_write_byte(lcd_inst, low_nibble | 0x04);
    k_busy_wait(1); // ★ k_udelay から k_busy_wait に変更
    i2c_write_byte(lcd_inst, low_nibble & ~0x04);

    k_busy_wait(delay_us); // ★ k_udelay から k_busy_wait に変更
}

void lcd_clear(LCD* lcd_inst) {
    send_byte(lcd_inst, 0x01, LCD_COMMAND_MODE, 2000);
}

void lcd_string(LCD* lcd_inst, const char *str) {
    while (*str) {
        send_byte(lcd_inst, *str++, LCD_DATA_MODE, 50);
    }
}

//const struct device *get_my_i2c_device(void) {
    // RP2040のI2Cデバイス(通常はI2C_0またはI2C_1)を取得して返します
//    return DEVICE_DT_GET(DT_NODELABEL(i2c0)); 
//}

