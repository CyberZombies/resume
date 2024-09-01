#include <windows.h>         // Подключение библиотеки Windows API
#include <time.h>            // Подключение библиотеки для работы со временем
#include <string.h>          // Подключение библиотеки для работы со строками
#include <locale.h>          // Подключение библиотеки для работы с локалями
#include <stdio.h>           // Подключение библиотеки для стандартного ввода/вывода
#include <math.h>            // Подключение библиотеки для математических функций
#include <windows.h>

int main() {
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_RETURN; // Код клавиши Enter
Sleep(10000);



    for (int i = 0; i < 1000; i++) {
        Sleep(1000);

        input.ki.dwFlags = 0;
        SendInput(1, &input, sizeof(INPUT)); // Нажатие клавиши

        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT)); // Отпускание клавиши
    }

    return 0;
}