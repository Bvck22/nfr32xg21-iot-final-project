/*
 * my_usart.c
 */

#include <BSP/bsp_usart.h>

void bsp_usart_init(void) {
    // 1. Cấp Clock cho GPIO và USART0
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART0, true);

    // 2. Cấu hình GPIO cho TX và RX [cite: 414-415]
    // TX là Output (PushPull), RX là Input
    GPIO_PinModeSet(BSP_TXPORT, BSP_TXPIN, gpioModePushPull, 1);
    GPIO_PinModeSet(BSP_RXPORT, BSP_RXPIN, gpioModeInput, 0);

    // 3. Kích hoạt VCOM để giao tiếp với PC [cite: 334, 416]
    // Chân PD04 cần được kéo lên mức cao
    GPIO_PinModeSet(BSP_ENABLE_PORT, BSP_ENABLE_PIN, gpioModePushPull, 1);

    // 4. Cấu hình thông số UART [cite: 423-449]
    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
    init.enable       = usartEnable;
    init.baudrate     = 115200;          // Baudrate phổ biến cho debug
    init.oversampling = usartOVS16;
    init.databits     = usartDatabits8;
    init.parity       = usartNoParity;
    init.stopbits     = usartStopbits1;
    init.mvdis        = false;
    init.prsRxEnable  = false;
    init.hwFlowControl = usartHwFlowControlNone;

    // 5. Route chân TX/RX vào module USART0 [cite: 452-456]
    // EFR32 cần map chân vật lý vào module ngoại vi
    GPIO->USARTROUTE[0].TXROUTE = (BSP_TXPORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
                                | (BSP_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
    GPIO->USARTROUTE[0].RXROUTE = (BSP_RXPORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
                                | (BSP_RXPIN << _GPIO_USART_RXROUTE_PIN_SHIFT);

    // Kích hoạt Route [cite: 457-459]
    GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;

    // 6. Khởi tạo USART0 với cấu hình trên [cite: 461]
    USART_InitAsync(USART0, &init);

}

// Hàm gửi 1 ký tự sử dụng thư viện em_usart [cite: 463]
void bsp_usart_send_char(char c) {
    USART_Tx(USART0, c);
}

// Hàm gửi cả chuỗi ký tự (tiện lợi hơn gửi từng byte)
void bsp_usart_print(const char *str) {
    while (*str) {
        bsp_usart_send_char(*str++);
    }
}
char bsp_usart_receive_char(void) {
    return USART_Rx(USART0); // Hàm này của Silicon Labs tự động chờ (blocking)
}
void bsp_usart_read_line(char *buffer, uint32_t max_len) {
    uint32_t index = 0;
    char c;

    // Xóa buffer trước khi dùng
    memset(buffer, 0, max_len);

    while (1) {
        // Nhận 1 ký tự
        c = bsp_usart_receive_char();

        // [QUAN TRỌNG] Echo: Gửi lại ký tự lên màn hình để người dùng thấy mình gõ gì
        bsp_usart_send_char(c);

        // Kiểm tra ký tự Enter (\r hoặc \n)
        if (c == '\r' || c == '\n') {
            buffer[index] = '\0'; // Kết thúc chuỗi
            bsp_usart_print("\r\n"); // Xuống dòng thẩm mỹ
            return; // Thoát hàm
        }
        // Kiểm tra Backspace (xóa lùi - tùy chọn nâng cao cho terminal)
        else if (c == '\b' || c == 127) {
            if (index > 0) {
                index--;
                buffer[index] = '\0';
                // Xóa hiển thị trên màn hình: Lùi lại, in khoảng trắng, lùi lại
                bsp_usart_print("\b \b");
            }
        }
        // Lưu ký tự vào buffer nếu còn chỗ
        else {
            if (index < (max_len - 1)) {
                buffer[index] = c;
                index++;
            }
        }
    }
}
