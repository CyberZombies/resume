#ifndef FILE_B_H // Директива препроцессора: Проверяет, определена ли константа FILE_B_H
#define FILE_B_H //  Директива препроцессора:  Определяет константу FILE_B_H, если она не была определена ранее

// Объявления функций (прототипы): 
void stopBruteForceAttack();                                   // Останавливает атаку методом перебора (брутфорс)
void emulateMouseClick(int mouseX, int mouseY);                  // Эмулирует клик мышью по заданным координатам (mouseX, mouseY)
void setInputFieldX(int mouseInputX);                           // Устанавливает координату X для поля ввода 
void setInputFieldY(int mouseInputY);                           // Устанавливает координату Y для поля ввода
void setMouseInputBufferX(int mouseInputX);                    // Устанавливает координату X для буфера ввода мыши 
void setMouseInputBufferY(int mouseInputY);                    // Устанавливает координату Y для буфера ввода мыши 
void setClickPointX(int mouseClickX);                          // Устанавливает координату X для точки клика 
void setClickPointY(int mouseClickY);                          // Устанавливает координату Y для точки клика
void setLanguageSwitchX(int setMouseBufferX);             // Устанавливает координату X для переключателя языка 
void setLanguageSwitchY(int languageSwitchBufferY);             // Устанавливает координату Y для переключателя языка 
void languageSwitchBufferX(int mouseBufferX);                        // Устанавливает координату X для буфера мыши 
void languageSwitchBufferY(int mouseBufferY);                        // Устанавливает координату Y для буфера мыши 
void setDelayBeforeBuffer(int delayBeforeBuffer);              // Устанавливает задержку перед выполнением действия, связанного с буфером 
void setDelayAfterBuffer(int delayAfterBuffer);                // Устанавливает задержку после выполнения действия, связанного с буфером 

// Определение функции: Перемещает курсор мыши в указанные координаты без выполнения клика
void moveCursorWithoutClick(int x, int y) {
    POINT point;                         // Объявление структуры POINT для хранения координат
    point.x = x;                        // Присваивание значения x полю x структуры point
    point.y = y;                        // Присваивание значения y полю y структуры point

    // Перемещение курсора: 
    if (SetCursorPos(x, y)) {         // Вызов функции Windows API SetCursorPos для перемещения курсора в координаты (x, y)
                                        // Условие:  Если SetCursorPos возвращает TRUE (успешно), то:
        HWND hwnd = GetForegroundWindow(); // Получение дескриптора активного окна с помощью GetForegroundWindow
        if (hwnd != NULL) {               // Проверка: Если hwnd не NULL (активное окно найдено), то:
            RECT rect;                     // Объявление структуры RECT для хранения координат окна
            GetWindowRect(hwnd, &rect);    // Получение координат активного окна с помощью GetWindowRect
            
            // Преобразование координат:
            point.x -= rect.left;         // Вычитание левой границы окна из координаты x точки, 
                                        // чтобы получить координаты относительно окна
            point.y -= rect.top;          // Вычитание верхней границы окна из координаты y точки
            
            // Отправка сообщения:
            PostMessage(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(point.x, point.y)); 
                                        // Отправка сообщения WM_MOUSEMOVE в активное окно,
                                        // чтобы уведомить его об изменении позиции курсора
        }
    } else {
        printf("Error.\n");              //  Вывод сообщения об ошибке в консоль, если SetCursorPos вернула FALSE (неудача)
    }
}

#endif // FILE_B_H  // Завершение блока условного включения 