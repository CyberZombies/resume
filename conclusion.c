#include <windows.h>         // Подключение библиотеки Windows API
#include <time.h>            // Подключение библиотеки для работы со временем
#include <string.h>          // Подключение библиотеки для работы со строками
#include <locale.h>          // Подключение библиотеки для работы с локалями
#include <stdio.h>           // Подключение библиотеки для стандартного ввода/вывода
#include <math.h>            // Подключение библиотеки для математических функций
#include "buffer_library2.h" // Подключение пользовательской библиотеки

// Глобальные переменные:
int languageSwitchBufferX_buf = 0; // Координата X для клика, используемого при переключении языка
int languageSwitchBufferY_buf = 0; // Координата Y для клика, используемого при переключении языка
int mouseInputBufferX = 0;         // Координата X для буфера ввода мыши
int mouseInputBufferY = 0;         // Координата Y для буфера ввода мыши

// Функции для установки координат, используемых при переключении языка:
void setLanguageSwitchX(int mouseClickX)
{
  languageSwitchBufferX_buf = mouseClickX;
}
void setLanguageSwitchY(int mouseClickY)
{
  languageSwitchBufferY_buf = mouseClickY;
}

// Функции для установки координат буфера ввода мыши:
void setMouseInputBufferX(int mouseInputX)
{
  mouseInputBufferX = mouseInputX;
}
void setMouseInputBufferY(int mouseInputY)
{
  mouseInputBufferY = mouseInputY;
}

// Функция для проверки текущей раскладки клавиатуры:
int checkLanguage()
{
  HWND foregroundWindow = GetForegroundWindow();                     // Получение дескриптора активного окна
  DWORD threadId = GetWindowThreadProcessId(foregroundWindow, NULL); // Получение ID потока, владеющего окном
  HKL keyboardLayout = GetKeyboardLayout(threadId);                  // Получение текущей раскладки клавиатуры
  uintptr_t keyboardLayoutId = (uintptr_t)keyboardLayout & 0xFFFF;   // Извлечение нижних 16 бит (ID раскладки)
  Sleep(30);                                                         // Пауза 30 миллисекунд
  return keyboardLayoutId;                                           // Возврат ID раскладки
}

// Функция для эмуляции нажатия клавиши:
void emulateKeystroke(char password[], int index, int isUpperCase)
{
  INPUT input;                    // Структура для хранения информации о вводе
  int keyboardLayoutId = 0;       // ID текущей раскладки клавиатуры
  int byteCombinationToCheck = 0; // Комбинация байтов для проверки
  int byteCycleIndex = 0;         // Индекс цикла байтов
  int powerOfTen = 0;             // Степень числа 10 (для вычислений)
  int shiftPressed = 0;           // Флаг, указывающий, нажата ли клавиша Shift

  input.type = INPUT_KEYBOARD;             // Установка типа ввода: клавиатура
  char currentSymbol = password[index];    // Текущий символ из пароля
  int russianIndex = (char)currentSymbol;  // Индекс символа в русской кодировке
  int keyboardIndex = (char)currentSymbol; // Индекс символа в кодировке клавиатуры

  printf("keyboard_index=%d\n", keyboardIndex); // Вывод индекса символа в консоль

  // Цикл для вычисления комбинации байтов :
  for (int i = sizeof(currentSymbol) * 8 - 1; i >= 0; i--)
  {
    powerOfTen = pow(10, byteCycleIndex);
    byteCombinationToCheck = byteCombinationToCheck + ((currentSymbol >> i) & 1) * powerOfTen;
    byteCycleIndex++;
  }

  Sleep(40);                                                                    // Пауза 40 миллисекунд
  moveCursorWithoutClick(languageSwitchBufferX_buf, languageSwitchBufferY_buf); // Перемещение курсора к точке переключения языка

  // Установка раскладки клавиатуры:
  if ((russianIndex <= -65) && (russianIndex >= -128) || (russianIndex == -30) ||
      (password[index] == '?') || (password[index] == '/') || (password[index] == '\"'))
  {
    // Установка русской раскладки:
    do
    {
      PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04190419); // Отправка сообщения о смене раскладки
      keyboardLayoutId = checkLanguage();                                           // Проверка текущей раскладки
    } while (keyboardLayoutId != 1049); // Повтор, пока не будет установлена русская раскладка
  }
  else
  {
    // Установка английской раскладки:
    do
    {
      PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 1, 0x04090409); // Отправка сообщения о смене раскладки
      keyboardLayoutId = checkLanguage();                                           // Проверка текущей раскладки
    } while (keyboardLayoutId != 1033); // Повтор, пока не будет установлена английская раскладка
  }

  moveCursorWithoutClick(mouseInputBufferX, mouseInputBufferY); // Возврат курсора в позицию буфера ввода

  // Определение виртуального кода клавиши (wVk) для нажатия:
  if ((password[index] >= 97) && (password[index] <= 122))
  {                                           // Английская строчная буква:
    keyboardIndex = (char)currentSymbol - 32; // Вычисление индекса заглавной буквы
    input.ki.wVk = keyboardIndex;             // Установка виртуального кода клавиши
  }
  else if ((password[index] >= 65) && (password[index] <= 90))
  {                               // Английская заглавная буква:
    input.ki.wVk = keyboardIndex; // Установка виртуального кода клавиши
    shiftPressed = 1;             // Установка флага нажатия Shift
  }
  else if ((password[index] >= '0') && (password[index] <= '9'))
  {                               // Цифра:
    input.ki.wVk = keyboardIndex; // Установка виртуального кода клавиши
  }
  else if (((russianIndex <= -65) && (russianIndex >= -128) && (byteCombinationToCheck != 1101001)))
  { // Русская буква (кроме Ё):
    // Определение виртуального кода клавиши для русской буквы (с использованием switch-case):
    switch (russianIndex)
    {
    case -65:
      input.ki.wVk = 0x47;
      break;
    case -66:
      input.ki.wVk = 0x4A;
      break;
    case -67:
      input.ki.wVk = 0x59;
      break;
    case -68:
      input.ki.wVk = 0x56;
      break;
    case -69:
      input.ki.wVk = 0x4B;
      break;
    case -70:
      input.ki.wVk = 0x52;
      break;
    case -71:
      input.ki.wVk = 0x51;
      break;
    case -72:
      input.ki.wVk = 0x42;
      break;
    case -73:
      input.ki.wVk = 0x50;
      break;
    case -74:
      input.ki.wVk = 0xBA;
      break;
    case -75:
      input.ki.wVk = 0x54;
      break;
    case -76:
      input.ki.wVk = 0x4C;
      break;
    case -77:
      input.ki.wVk = 0x55;
      break;
    case -78:
      input.ki.wVk = 0x44;
      break;
    case -79:
      input.ki.wVk = 0xBC;
      break;
    case -80:
      input.ki.wVk = 0x46;
      break;
    case -111:
      if (isUpperCase == 0)
      {
        input.ki.wVk = 0xC0;
      }
      else
      {
        input.ki.wVk = 0xBC;
        shiftPressed = 1;
      }
      break;
    case -113:
      input.ki.wVk = 0x5A;
      break;
    case -114:
      input.ki.wVk = 0xBE;
      break;
    case -115:
      input.ki.wVk = 0xDE;
      break;
    case -116:
      input.ki.wVk = 0x4D;
      break;
    case -117:
      input.ki.wVk = 0x53;
      break;
    case -118:
      input.ki.wVk = 0xDD;
      break;
    case -119:
      input.ki.wVk = 0x4F;
      break;
    case -120:
      input.ki.wVk = 0x49;
      break;
    case -121:
      input.ki.wVk = 0x58;
      break;
    case -122:
      input.ki.wVk = 0x57;
      break;
    case -123:
      input.ki.wVk = 0xDB;
      break;
    case -124:
      input.ki.wVk = 0x41;
      break;
    case -125:
      input.ki.wVk = 0x45;
      break;
    case -126:
      input.ki.wVk = 0x4E;
      break;
    case -127:
      if (isUpperCase == 0)
      {
        input.ki.wVk = 0x43;
      }
      else
      {
        input.ki.wVk = 0xC0;
        shiftPressed = 1;
      }
      break;
    case -128:
      input.ki.wVk = 0x48;
      break;
    case -81:
      input.ki.wVk = 0x5A;
      shiftPressed = 1;
      break;
    case -82:
      input.ki.wVk = 0xBE;
      shiftPressed = 1;
      break;
    case -83:
      input.ki.wVk = 0xDE;
      shiftPressed = 1;
      break;
    case -84:
      input.ki.wVk = 0x4D;
      shiftPressed = 1;
      break;
    case -85:
      input.ki.wVk = 0x53;
      shiftPressed = 1;
      break;
    case -86:
      input.ki.wVk = 0xDD;
      shiftPressed = 1;
      break;
    case -87:
      input.ki.wVk = 0x4F;
      shiftPressed = 1;
      break;
    case -88:
      input.ki.wVk = 0x49;
      shiftPressed = 1;
      break;
    case -89:
      input.ki.wVk = 0x58;
      shiftPressed = 1;
      break;
    case -90:
      input.ki.wVk = 0x57;
      shiftPressed = 1;
      break;
    case -91:
      input.ki.wVk = 0xDB;
      shiftPressed = 1;
      break;
    case -92:
      input.ki.wVk = 0x41;
      shiftPressed = 1;
      break;
    case -93:
      input.ki.wVk = 0x45;
      shiftPressed = 1;
      break;
    case -94:
      input.ki.wVk = 0x4E;
      shiftPressed = 1;
      break;
    case -95:
      input.ki.wVk = 0x43;
      shiftPressed = 1;
      break;
    case -96:
      input.ki.wVk = 0x48;
      shiftPressed = 1;
      break;
    case -97:
      input.ki.wVk = 0x47;
      shiftPressed = 1;
      break;
    case -98:
      input.ki.wVk = 0x4A;
      shiftPressed = 1;
      break;
    case -99:
      input.ki.wVk = 0x59;
      shiftPressed = 1;
      break;
    case -100:
      input.ki.wVk = 0x56;
      shiftPressed = 1;
      break;
    case -101:
      input.ki.wVk = 0x4B;
      shiftPressed = 1;
      break;
    case -102:
      input.ki.wVk = 0x52;
      shiftPressed = 1;
      break;
    case -103:
      input.ki.wVk = 0x51;
      shiftPressed = 1;
      break;
    case -104:
      input.ki.wVk = 0x42;
      shiftPressed = 1;
      break;
    case -105:
      input.ki.wVk = 0x50;
      shiftPressed = 1;
      break;
    case -106:
      input.ki.wVk = 0xBA;
      shiftPressed = 1;
      break;
    case -107:
      input.ki.wVk = 0x54;
      shiftPressed = 1;
      break;
    case -108:
      input.ki.wVk = 0x4C;
      shiftPressed = 1;
      break;
    case -109:
      input.ki.wVk = 0x55;
      shiftPressed = 1;
      break;
    case -110:
      input.ki.wVk = 0x44;
      shiftPressed = 1;
      break;
    case -112:
      input.ki.wVk = 0x46;
      shiftPressed = 1;
      break;
    }
  }
  else
  {
    // Определение виртуального кода клавиши для специальных символов:
    switch (password[index])
    {
    case ' ':
      input.ki.wVk = 0x20;
      break;
    case '.':
      input.ki.wVk = 0xBE;
      break;
    case '\'':
      input.ki.wVk = 0xDE;
      break;
    case '\\':
      input.ki.wVk = 0xDC;
      break;
    case ',':
      input.ki.wVk = 0xBC;
      break;
    case ';':
      input.ki.wVk = 0xBA;
      break;
    case '`':
      input.ki.wVk = 0xC0;
      break;
    case '-':
      input.ki.wVk = 0xBD;
      break;
    case '=':
      input.ki.wVk = 0xBB;
      break;
    case '[':
      input.ki.wVk = 0xDB;
      break;
    case ']':
      input.ki.wVk = 0xDD;
      break;
    }
  }

  if ((password[index] == '!') || (password[index] == '@') || (password[index] == '#') ||
      (password[index] == '$') || (password[index] == '%') || (password[index] == '^') ||
      (password[index] == '&') || (password[index] == '*') || (password[index] == '(') ||
      (password[index] == ')') || (password[index] == '_') || (password[index] == '+') ||
      (password[index] == ':') || (password[index] == '{') || (password[index] == '}') ||
      (password[index] == '|') || (password[index] == '/') || (password[index] == '?') ||
      (password[index] == '~') || (password[index] == '<') || (password[index] == '>') ||
      (password[index] == '\"') || (russianIndex == -30))
  {
    switch (password[index])
    {
    case '!':
      input.ki.wVk = 0x31;
      break;
    case '@':
      input.ki.wVk = 0x32;
      break;
    case '#':
      input.ki.wVk = 0x33;
      break;
    case '$':
      input.ki.wVk = 0x34;
      break;
    case '%':
      input.ki.wVk = 0x35;
      break;
    case '^':
      input.ki.wVk = 0x36;
      break;
    case '&':
      input.ki.wVk = 0x37;
      break;
    case '*':
      input.ki.wVk = 0x38;
      break;
    case ':':
      input.ki.wVk = 0xBA;
      break;
    case '(':
      input.ki.wVk = 0x39;
      break;
    case ')':
      input.ki.wVk = 0x30;
      break;
    case '_':
      input.ki.wVk = 0xBD;
      break;
    case '+':
      input.ki.wVk = 0xBB;
      break;
    case '{':
      input.ki.wVk = 0xDB;
      break;
    case '}':
      input.ki.wVk = 0xDD;
      break;
    case '|':
      input.ki.wVk = 0xDC;
      break;
    case '/':
      input.ki.wVk = 0xDC;
      break;
    case '?':
      input.ki.wVk = 0x37;
      break;
    case '~':
      input.ki.wVk = 0xC0;
      break;
    case '<':
      input.ki.wVk = 0xBC;
      break;
    case '>':
      input.ki.wVk = 0xBE;
      break;
    case '\"':
      input.ki.wVk = 0x32;
      break;
    default:
      input.ki.wVk = 0x33;
      break; // №-3 байта, костыль
    }
    shiftPressed = 1;
  }

  // Эмуляция нажатия клавиши:
  if (shiftPressed == 1) // проверяет нужен ли Shift
  {
    keybd_event(VK_SHIFT, 0, 0, 0); // Нажатие Shift
  }
  keybd_event(input.ki.wVk, 0, 0, 0);               // Нажатие клавиши
  keybd_event(input.ki.wVk, 0, KEYEVENTF_KEYUP, 0); // Отпускание клавиши
  if (shiftPressed == 1)
  {                                               // проверяет нужен ли Shift
    keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0); // Отпускание Shift
  }
}

// Функция для эмуляции ввода строки:
void executeInput(char word[])
{
  for (int i = 0; i <= strlen(word) - 1; i++)
  {
    int isUpperCase = 0; // Флаг для обозначения заглавных букв

    char currentSymbol = word[i];    // Текущий символ
    int index = (char)currentSymbol; // Индекс символа в кодировке

    // Проверка на наличие символов, управляющих регистром:
    if (((index == -48) || (index == -47)))
    {      //  -48: заглавные, -47: строчные (отсев ложных символов)
      i++; // Переход к следующему символу
      if (index == -48)
      {
        isUpperCase = 1; // Установка флага заглавных букв
      }
    }

    if ((index != 0))
    {                                         // Пропуск символов с индексом 0
      emulateKeystroke(word, i, isUpperCase); // Эмуляция нажатия клавиши
    }
    if ((index == -30))
    {            // Пропуск двух символов после -30 (для выявления ключевых символов)
      i = i + 2; // Переход к символу после пропущенных
    }
  }
}