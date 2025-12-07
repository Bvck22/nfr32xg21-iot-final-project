#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <time.h>
#include <conio.h> // Để dùng hàm _kbhit() bắt phím nhấn

#define BUFFER_SIZE 256
#define TIMEOUT_MS 500 // Timeout đọc UART

// --- CÁC HÀM GIAO TIẾP SERIAL ---

HANDLE open_port(const char* device, unsigned long baud_rate) {
    printf("Dang ket noi toi %s voi toc do %lu...\n", device, baud_rate);
    HANDLE port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (port == INVALID_HANDLE_VALUE) return INVALID_HANDLE_VALUE;

    // Cấu hình Timeout
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50; 
    timeouts.ReadTotalTimeoutConstant = TIMEOUT_MS;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    
    if (!SetCommTimeouts(port, &timeouts)) {
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    // Cấu hình DCB
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(port, &dcb)) {
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }
    
    dcb.BaudRate = baud_rate;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    
    if (!SetCommState(port, &dcb)) {
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    return port;
}

// Hàm gửi chuỗi ký tự xuống EFR32
int send_string(HANDLE port, const char* str) {
    DWORD bytes_written;
    DWORD len = (DWORD)strlen(str);
    if (!WriteFile(port, str, len, &bytes_written, NULL)) {
        printf("Loi: Khong gui duoc du lieu!\n");
        return 0;
    }
    return 1;
}

// Hàm đọc một dòng dữ liệu (đến khi gặp \n hoặc timeout)
int read_line(HANDLE port, char* buffer, int max_len) {
    DWORD bytes_read;
    char c;
    int idx = 0;
    
    // Xóa buffer
    memset(buffer, 0, max_len);

    while (idx < max_len - 1) {
        if (ReadFile(port, &c, 1, &bytes_read, NULL)) {
            if (bytes_read == 0) {
                // Timeout, không có dữ liệu mới
                return 0; 
            }
            // Nếu gặp ký tự xuống dòng thì kết thúc chuỗi
            if (c == '\n') {
                buffer[idx] = '\0';
                return 1; // Đã đọc được 1 dòng
            }
            // Bỏ qua ký tự \r
            if (c != '\r') {
                buffer[idx++] = c;
            }
        } else {
            return -1; // Lỗi đọc
        }
    }
    buffer[idx] = '\0';
    return 1;
}

// --- HÀM LƯU DỮ LIỆU (DATABASE) ---
void save_to_database(const char* data) {
    // Mở file ở chế độ "append" (ghi nối tiếp)
    FILE* f = fopen("sensor_data.csv", "a");
    if (f == NULL) {
        printf("Loi: Khong the mo file database!\n");
        return;
    }

    // Lấy thời gian hiện tại
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Ghi vào file: Ngay gio, Du lieu
    fprintf(f, "%04d-%02d-%02d %02d:%02d:%02d, %s\n", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
            tm.tm_hour, tm.tm_min, tm.tm_sec, 
            data);
    
    fclose(f);
}

// --- MAIN MENU ---
int main() {
    char com_port[20];
    printf("Nhap cong COM (Vi du: COM3, COM6): ");
    scanf("%s", com_port);
    
    char device_path[30];
    sprintf(device_path, "\\\\.\\%s", com_port);

    HANDLE port = open_port(device_path, 115200);
    if (port == INVALID_HANDLE_VALUE) {
        printf("Khong the mo cong %s. Hay kiem tra lai ket noi va tat cac phan mem khac dang dung COM nay.\n", com_port);
        return -1;
    }

    int choice;
    char command_buffer[64];
    char rx_buffer[BUFFER_SIZE];
    int param;

    while (1) {
        printf("\n--- HE THONG QUAN LY IOT (EFR32) ---\n");
        printf("1. Cau hinh chu ky DO (Measurement Period)\n");
        printf("2. Cau hinh chu ky QUANG BA (Advertising Period)\n");
        printf("3. BAT DAU thu thap du lieu & Luu Database\n");
        printf("4. Thoat\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Nhap chu ky do (ms): ");
            scanf("%d", &param);
            sprintf(command_buffer, "MEAS %d\n", param); // Tạo lệnh đúng cú pháp
            send_string(port, command_buffer);
            printf("Da gui: %s", command_buffer);
            break;

        case 2:
            printf("Nhap chu ky quang ba (ms): ");
            scanf("%d", &param);
            sprintf(command_buffer, "ADV %d\n", param);
            send_string(port, command_buffer);
            printf("Da gui: %s", command_buffer);
            break;

        case 3:
            printf("\n--- DANG THU THAP DU LIEU ---\n");
            printf("Nhan phim bat ky de DUNG lai...\n");
            
            // Xóa bộ đệm input trước khi vào vòng lặp
            while (_kbhit()) _getch(); 

            while (1) {
                // 1. Kiểm tra nếu người dùng nhấn phím thì thoát
                if (_kbhit()) {
                    _getch(); // Đọc phím để xóa buffer
                    break;
                }

                // 2. Đọc dữ liệu từ UART
                int status = read_line(port, rx_buffer, BUFFER_SIZE);
                if (status == 1 && strlen(rx_buffer) > 0) {
                    // Hiển thị lên màn hình
                    printf("Nhan duoc: %s\n", rx_buffer);
                    
                    // 3. Lưu vào Database (File CSV)
                    save_to_database(rx_buffer);
                }
            }
            printf("\nDa dung thu thap.\n");
            break;

        case 4:
            CloseHandle(port);
            return 0;

        default:
            printf("Lua chon khong hop le.\n");
        }
    }
    
}