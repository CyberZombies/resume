#include <windows.h>         // Подключение библиотеки Windows API для работы с функциями операционной системы.
#include <stdio.h>           // Подключение библиотеки для стандартного ввода/вывода (например, printf).
#include "buffer_library2.h" // Подключаем файл с функциями для работы с буфером
// Определение структуры Mouse для хранения координат мыши.
typedef struct
{
    int x; // Координата X.
    int y; // Координата Y.
} Mouse;

// Функция для получения текущей позиции курсора мыши.
POINT getMousePosition()
{
    POINT p;          // Создание переменной p типа POINT (структура Windows API для хранения координат).
    GetCursorPos(&p); // Вызов функции GetCursorPos для получения позиции курсора и сохранения её в p.
    return p;         // Возврат структуры p, содержащей координаты X и Y курсора.
}

// Функция для получения координаты X курсора мыши.
int find_out_mouse_coordinates_x()
{
    POINT mousePos = getMousePosition(); // Получение текущей позиции курсора.
    int x = mousePos.x;                  // Извлечение координаты X из структуры mousePos.
    return x;                            // Возврат координаты X.
}

// Функция для получения координаты Y курсора мыши.
int find_out_mouse_coordinates_y()
{
    POINT mousePos = getMousePosition(); // Получение текущей позиции курсора.
    int y = mousePos.y;                  // Извлечение координаты Y из структуры mousePos.
    return y;                            // Возврат координаты Y.
}

// Функция для эмуляции клика левой кнопкой мыши по заданным координатам.
void emulateMouseClick(int mouseX, int mouseY)
{
    // Перемещение курсора:
    SetCursorPos(mouseX, mouseY); // Вызов функции SetCursorPos для перемещения курсора в точку (mouseX, mouseY).

    // Эмуляция нажатия и отпускания левой кнопки:
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // Симуляция нажатия левой кнопки мыши.
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // Симуляция отпускания левой кнопки мыши.
}