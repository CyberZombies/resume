// Подключение необходимых заголовочных файлов
#include "conclusion.c"
#include "buffer_library2.h"
#include <string.h> // Подключение функций для работы со строками (например, strlen, strcpy)

// Глобальные переменные для отслеживания состояния окна и курсора
HWND guestFocusedWindowHandle = NULL; // Дескриптор окна, на котором находится фокус ввода (NULL, если нет)
BOOL guestWindowChecked = FALSE;      // Флаг, указывающий, была ли проверена активность окна
BOOL guestExitProgram = FALSE;        // Флаг для сигнализации о завершении программы

// Глобальные переменные для состояния курсора и управления циклом перебора
int guestCursorState = 0; // Текущее состояние курсора (0 - неактивен, 1 - активен)
int guestStopAttack = 0;  // Флаг для остановки атаки перебором (0 - работает, 1 - остановлена)

// Глобальные переменные для координат мыши и задержек
// ПРИМЕЧАНИЕ: Некоторые имена переменных в этом разделе кажутся несогласованными и могут быть улучшены
int guestMouseInputX = 0, guestMouseInputY = 0;          // Координаты мыши для ввода
int guestMouseClickX = 0, guestMouseClickY = 0;          // Координаты мыши для кликов
int guestDelayBeforeInput = 0, guestDelayAfterInput = 0; // Задержки до и после действий
int guestMouseBufferX = 0, guestMouseBufferY = 0;        // Координаты мыши (возможно, для действий, связанных с буфером?)

// Функции для установки координат мыши для различных действий
void setInputFieldX(int mouse_x_input_b)
{
  guestMouseInputX = mouse_x_input_b; // Установка координаты X для поля ввода
}
void setInputFieldY(int mouse_y_input_b)
{
  guestMouseInputY = mouse_y_input_b; // Установка координаты Y для поля ввода
}
void setClickPointX(int mouse_x_click_b)
{
  guestMouseClickX = mouse_x_click_b; // Установка координаты X для событий клика
}
void setClickPointY(int mouse_y_click_b)
{
  guestMouseClickY = mouse_y_click_b; // Установка координаты Y для событий клика
}

// Функции для установки задержек для операций
void setDelayBeforeBuffer(int delay__before_b)
{
  guestDelayBeforeInput = delay__before_b; // Установка задержки перед действием (например, перед проверкой)
}
void setDelayAfterBuffer(int delay_after_b)
{
  guestDelayAfterInput = delay_after_b; // Установка задержки после действия
}

// Функции для установки координат мыши, связанные с переключением языка
void languageSwitchBufferX(int mouse_x_buf_b_b)
{
  guestMouseBufferX = mouse_x_buf_b_b; // Установка координаты X (буфер, связанный с переключением языка?)
}
void languageSwitchBufferY(int mouse_y_buf_b_b)
{
  guestMouseBufferY = mouse_y_buf_b_b; // Установка координаты Y (буфер, связанный с переключением языка?)
}

// Функция для проверки, активен ли курсор в данный момент (например, находится ли в поле ввода)
void isCursorActive()
{
  CURSORINFO cursorInfo;                  // Структура для хранения информации о курсоре
  cursorInfo.cbSize = sizeof(CURSORINFO); // Установка размера структуры

  // Получение информации о курсоре с помощью функции Windows API
  if (GetCursorInfo(&cursorInfo))
  {
    // Проверка, отображается ли курсор И является ли тип курсора I-образным (текстовый ввод)
    if (cursorInfo.flags & CURSOR_SHOWING && cursorInfo.hCursor == LoadCursor(NULL, IDC_IBEAM))
    {
      guestCursorState = 1; // Установка состояния курсора в активный (1)
    }
    else
    {
      guestCursorState = 0; // Установка состояния курсора в неактивный (0)
    }
  }
  else
  {
    guestCursorState = 0; // Установка состояния курсора в неактивный, если GetCursorInfo завершается неудачей
  }
}

// Структура для хранения комбинаций для атаки перебором
typedef struct
{
  size_t size; // Размер ключа (строки)
  int value;   // Числовое значение, связанное с ключом (возможно, индекс)
  char key[];  // Массив символов для хранения самого ключа (гибкий размер)
} BruteForceCombination;

// Функция для сигнализации об остановке атаки перебором
void stopBruteForceAttack()
{
  printf("Program stopped, start again\n"); // Вывод "Program stopped, start again" в консоль (для отладки/индикации)
  guestStopAttack = 1;                      // Установка глобального флага остановки в 1 для завершения цикла атаки
}

// Функция для подсчета количества элементов в массиве строк, завершающемся нулем
int getElementCount(char *keys[])
{
  int count = 0; // Инициализация счетчика элементов
  while (keys[count] != NULL)
  {          // Цикл до тех пор, пока не встретится нулевой указатель (конец массива)
    count++; // Увеличение счетчика для каждого ненулевого элемента
  }
  return count; // Возврат общего количества элементов
}

// Функция для инициализации массива значениями от 0 до num_elements - 1
void initializeValueArray(int values[], int num_elements)
{
  for (int i = 0; i < num_elements; i++)
  {
    values[i] = i; // Присвоение индекса цикла (i) в качестве значения для каждого элемента
  }
}

// Функция для сброса глобальных переменных (вероятно, используется до/после перебора)
void resetVariables()
{
  guestCursorState = 0; // Сброс состояния курсора в неактивный (0)
  guestStopAttack = 0;  // Сброс флага остановки в состояние "работает" (0)

  // Сброс координат мыши
  guestMouseInputX = 0;
  guestMouseInputY = 0;
  guestMouseClickX = 0;
  guestMouseClickY = 0;
  guestMouseBufferX = 0;
  guestMouseBufferY = 0;

  // Сброс задержек
  guestDelayBeforeInput = 0;
  guestDelayAfterInput = 0;
}

// Основная функция для выполнения атаки перебором комбинаций
int bruteForceCombinations(int phraseLength, int min, char *keys[], int active_checkboxes)
{
  guestStopAttack = 0; // Убеждаемся, что атака не остановлена в начале

  // Проверка входных данных: проверка недопустимых значений параметров
  if ((active_checkboxes == 0) || (phraseLength <= 0) || (guestDelayBeforeInput < 0) || (guestDelayAfterInput < 0))
  {
    if (active_checkboxes == 0)
    {
      printf("Select at least one character\n"); // Сообщение об ошибке, если не выбраны символы
    }
    if (phraseLength <= 0)
    {
      printf("The string must be greater than zero\n"); // Сообщение об ошибке, если длина фразы недействительна
    }
    if ((guestDelayBeforeInput < 0) || (guestDelayAfterInput < 0))
    {
      printf("Negative delay\n"); // Сообщение об ошибке, если задержки отрицательные
    }
    resetVariables(); // Сброс переменных перед возвратом
    return 1;         // Возврат 1 (или кода ошибки)
  }

  const size_t dict_size = active_checkboxes; // Установка размера словаря на основе выбранных символов

  // Динамическое выделение памяти для массива структур BruteForceCombination
  BruteForceCombination **bruteForceCombinations = malloc(sizeof(BruteForceCombination *) * dict_size);

  // Инициализация массива для хранения числовых значений для комбинаций
  int num_elements = getElementCount(keys);
  int values[num_elements];
  initializeValueArray(values, num_elements); // Заполнение массива values значениями от 0 до num_elements-1

  // Отладочный вывод: печать выбранных ключей (наборов символов)
  for (int i = 0; i < dict_size; i++)
  {
    printf("Symbols[%d] = %s\n", i, keys[i]);
  }

  // Заполнение массива bruteForceCombinations данными из 'keys'
  for (size_t i = 0; i < dict_size; i++)
  {
    if (keys[i] != NULL)
    {                                   // Проверка, действителен ли указатель ключа
      size_t key_len = strlen(keys[i]); // Получение длины строки ключа

      // Выделение памяти для структуры BruteForceCombination + строка ключа
      bruteForceCombinations[i] = malloc(sizeof(BruteForceCombination) + (key_len + 1) * sizeof(char));

      bruteForceCombinations[i]->size = key_len;       // Установка размера ключа
      bruteForceCombinations[i]->value = values[i];    // Установка числового значения ключа
      strcpy(bruteForceCombinations[i]->key, keys[i]); // Копирование строки ключа в структуру
    }
  }

  // Переменные для генерации и отслеживания перебора
  int bruteForceCombinationsItemNumber;   // Индекс для перебора bruteForceCombinations
  int allPossibleCombinations = 0;        // Общее количество комбинаций, которые будут сгенерированы
  int countedAllPossibleCombinations = 0; // Счетчик для отслеживания сгенерированных комбинаций

  // Динамическое выделение памяти для массива для хранения индексов символов
  int *phraseCharacterArray = malloc(phraseLength * sizeof(int));

  // Инициализация параметров перебора
  int limit = dict_size - 1;              // Верхний предел для индексов символов
  int reachingTheLimit = 0;               // Флаг для отслеживания достижения предела
  int ctik = 0;                           // Дополнительные переменные
  int remainingPasswords = 0;             // для показа числа всех паролей
  int toShowTheNumberOfPasswordsUsed = 0; // для показа числа использованных паролей
  // Инициализация phraseCharacterArray нулями
  for (int i = 0; i < phraseLength; i++)
  {
    phraseCharacterArray[i] = 0;
  }
  for (int i = 0; i < phraseLength; i++)
  {
    countedAllPossibleCombinations = pow(dict_size, i + 1);
    remainingPasswords = remainingPasswords + countedAllPossibleCombinations;
  }
  min--;
  // Вложенный цикл для генерации всех возможных комбинаций символов
  for (int i = min; i < phraseLength; i++)
  {
    // Внешний цикл: перебор позиций во фразе
    bruteForceCombinationsItemNumber = -1; // Сброс индекса комбинации
    allPossibleCombinations = 0;           // Сброс счетчика комбинаций

    // Внутренний цикл: генерация комбинаций для текущей позиции (i)
    do
    {
      allPossibleCombinations++;                              // Увеличение счетчика комбинаций
      countedAllPossibleCombinations = pow(dict_size, i + 1); // Вычисление ожидаемых комбинаций
      bruteForceCombinationsItemNumber++;                     // Переход к следующему индексу символа

      // Обработка переполнения индекса символа (перенос)
      if (bruteForceCombinationsItemNumber > limit)
      {
        bruteForceCombinationsItemNumber = 0; // Сброс в 0, если индекс превышает предел
      }

      // Установка символа в текущей позиции в phraseCharacterArray
      phraseCharacterArray[i] = bruteForceCombinations[bruteForceCombinationsItemNumber]->value;

      // Цикл для имитации кликов мыши и ввода
      do
      {
        if (guestStopAttack == 1)
        {               // Проверка, установлен ли флаг остановки
          printf("\n"); // Печать новой строки (для форматирования)
          return 1;     // Выход из функции, если атака должна быть остановлена
        }

        // Отладочный вывод: отображение координат клика мыши
        printf("Searching for an input field...\n");
        emulateMouseClick(guestMouseInputX, guestMouseInputY); // Имитация клика мыши в поле ввода
        Sleep(40);                                             // Пауза на 40 миллисекунд
        isCursorActive();                                      // Проверка, активен ли курсор
      } while (guestCursorState == 0); // Повтор до тех пор, пока курсор не станет активным
      printf("%d number of password options left!\n", remainingPasswords);
      remainingPasswords--;
      printf("%d password string length!\n", i + 1);
      // Цикл для ввода сгенерированной комбинации символов
      for (int k = 0; k < i + 1; k++)
      {
        if (guestStopAttack == 1)
        { // Снова проверка флага остановки
          printf("\n");
          return 1;
        }
        // Функция для имитации ввода строки (определение, вероятно, находится в другом месте)
        executeInput(bruteForceCombinations[phraseCharacterArray[k]]->key);
      }

      printf("\n"); // Печать новой строки

      Sleep(guestDelayBeforeInput);                            // Задержка перед кликом
      emulateMouseClick(guestMouseClickX, guestMouseClickY);   // Имитация клика мыши
      Sleep(guestDelayAfterInput);                             // Задержка после клика
      emulateMouseClick(guestMouseBufferX, guestMouseBufferY); // Имитация еще одного клика мыши

      // Логика для обработки переноса индекса символа при генерации комбинаций
      if (phraseCharacterArray[i] >= limit)
      {
        reachingTheLimit = 0;
        for (int k = i; k > -1; k--)
        {
          if (phraseCharacterArray[k] >= limit)
          {
            reachingTheLimit++;
          }
          else
          {
            k = -1;
          }
        }
        if (reachingTheLimit == i + 1)
        {
          for (int k = 0; k < i + 1; k++)
          {
            phraseCharacterArray[k] = 0;
          }
        }
        else
        {
          ctik = 0;
          for (int j = i + 1; ctik < reachingTheLimit; ctik++)
          {
            j--;
            if (phraseCharacterArray[j] >= limit)
            {
              if (((j != 0) || (i == 0)))
              {
                phraseCharacterArray[j] = 0;
              }
              if ((j != 0) && (phraseCharacterArray[j - 1] < limit))
              {
                phraseCharacterArray[j - 1]++;
              }
            }
          }
        }
      }
    } while (allPossibleCombinations != countedAllPossibleCombinations); // Продолжение до тех пор, пока не будут сгенерированы все комбинации
  }

  // Очистка выделенной памяти перед выходом из функции
  for (size_t i = 0; i < dict_size; i++)
  {
    free(bruteForceCombinations[i]); // Освобождение каждой структуры BruteForceCombination
  }
  free(bruteForceCombinations); // Освобождение массива структур
  free(phraseCharacterArray);   // Освобождение массива индексов символов

  resetVariables(); // Сброс глобальных переменных
  return 0;         // Возврат 0 для обозначения успешного завершения
}