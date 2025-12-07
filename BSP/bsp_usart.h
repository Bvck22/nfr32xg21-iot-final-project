/*
 * my_usart.h
 * Thư viện hỗ trợ hiển thị thông tin qua UART (VCOM)
 * Dựa trên tài liệu Lab 1 - EFR32xG21
 */

#ifndef BSP_BSP_USART_H_
#define BSP_BSP_USART_H_

#include "em_device.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_cmu.h"
#include <stdio.h>
#include <string.h>

// --- Cấu hình Pin theo Lab 1 [cite: 406-411] ---
#define BSP_TXPORT          gpioPortA
#define BSP_TXPIN           5
#define BSP_RXPORT          gpioPortA
#define BSP_RXPIN           6

// Pin kích hoạt VCOM (Virtual COM) để truyền lên PC [cite: 334]
#define BSP_ENABLE_PORT     gpioPortD
#define BSP_ENABLE_PIN      4
#define RX_BUFFER_SIZE      128
// --- Hàm chức năng ---

// Khởi tạo UART và GPIO
void bsp_usart_init(void);

// Gửi 1 ký tự (dùng cho printf)
void bsp_usart_sendchar(char c);

// Gửi một chuỗi ký tự (String)
void bsp_usart_print(const char *str);


char bsp_usart_receive_char(void);      // Nhận 1 ký tự (Blocking)

/* * Nhận cả một dòng lệnh (cho đến khi nhấn Enter).

buffer: Biến mảng để lưu dữ liệu.
max_len: Độ dài tối đa của mảng.*/
void bsp_usart_read_line(char *buffer, uint32_t max_len);
//uint8_t bsp_usart_receive(USART_TypeDef *usart);
#endif /* BSP_BSP_USART_H_ */
