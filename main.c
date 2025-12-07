/***************************************************************************//**
 * @file
 * @brief main() function.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#include "em_usart.h"
#include "app.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT
#include <string.h> // Để dùng strncmp
#include <stdlib.h> // Để dùng atoi
#include <stdio.h>  // Để dùng sprintf
int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, processing task(s) will be created by
  // this call.
  sl_system_init();

  // Initialize the application. For example, create periodic timer(s) or
  // task(s) if the kernel is present.
  app_init();
  char my_input_buffer[100];
#if defined(SL_CATALOG_KERNEL_PRESENT)
  // Start the kernel. Task(s) created in app_init() will start running.
  sl_system_kernel_start();
#else // SL_CATALOG_KERNEL_PRESENT
  while (1) {
    // Do not remove this call: Silicon Labs components process action routine
    // must be called from the super loop.
    sl_system_process_action();

    // Application process.
    app_process_action();
    bsp_usart_print("User Input > ");


            bsp_usart_read_line(my_input_buffer);
            if (strncmp(my_input_buffer, "MEAS", 4) == 0) {
                // --- TRƯỜNG HỢP MEAS ---

                // Lấy giá trị số đi kèm.
                // &my_input_buffer[5] nghĩa là bỏ qua 5 ký tự đầu ("MEAS ") để lấy phần số "100"
                int value = atoi(&my_input_buffer[5]);

                // In ra để kiểm tra
                char msg[64];
                sprintf(msg, ">> Phat hien lenh MEAS. Gia tri: %d\r\n", value);
                bsp_usart_print(msg);

                // GỌI HÀM XỬ LÝ CỦA BẠN TẠI ĐÂY
                // Ví dụ: app_update_measurement_period(value);
            }

            // 2. Kiểm tra xem chuỗi có BẮT ĐẦU bằng chữ "ADV" không (so sánh 3 ký tự đầu)
            else if (strncmp(my_input_buffer, "ADV", 3) == 0) {
                // --- TRƯỜNG HỢP ADV ---

                // &my_input_buffer[4] nghĩa là bỏ qua 4 ký tự đầu ("ADV ") để lấy phần số
                int value = atoi(&my_input_buffer[4]);

                // In ra để kiểm tra
                char msg[64];
                sprintf(msg, ">> Phat hien lenh ADV. Gia tri: %d\r\n", value);
                bsp_usart_print(msg);

                // GỌI HÀM XỬ LÝ CỦA BẠN TẠI ĐÂY
                // Ví dụ: app_update_advertising_period(value);
            }

            // 3. Trường hợp lệnh không hợp lệ
            else {
                bsp_usart_print(">> Lenh khong hop le (Unknown Command)\r\n");
            }
            bsp_usart_print(my_input_buffer);
            // 5. Xử lý dữ liệu sau khi nhận xong
            // Ở đây mình in lại nội dung vừa nhận kèm độ dài (giống file mẫu của bạn)
            char output_msg[128];
//            sprintf(output_msg, "EFR32 Received: '%s' (Length: %d)\r\n",
//                    my_input_buffer,
//                    strlen(my_input_buffer));
//
//            bsp_usart_print(output_msg);
//            bsp_usart_print("----------------------------\r\n");



#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
    // Let the CPU go to sleep if the system allows it.
    sl_power_manager_sleep();
#endif
  }
#endif // SL_CATALOG_KERNEL_PRESENT
}
