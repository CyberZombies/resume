#include "bruteForceCombinations.c" // Подключаем файл с функцией перебора паролей
#include "pressing_setting.c"       // Подключаем файл с настройками нажатий
#include "buffer_library2.h"        // Подключаем файл с функциями для работы с буфером
#include <gtk/gtk.h>                // Подключаем библиотеку GTK+ для создания графического интерфейса
#include <stdlib.h>                 // Подключаем библиотеку стандартных функций C
#include <string.h>                 // Подключаем библиотеку для работы со строками
#include <pthread.h>                // Подключаем библиотеку для работы с потоками
#include <stdio.h>                  // Подключаем библиотеку для стандартного ввода-вывода
#include <stdbool.h>
char **selectedValues = NULL; // Массив для хранения выбранных символов для перебора
int selectedCount = 0;        // Количество выбранных символов
int activeCheckboxes = 0;     // Количество активных чекбоксов (выбранных символов)
bool SuccessfulRegistration = false;
// Функция, вызываемая при нажатии на кнопку "Инструкция"
void onInstructionClicked(GtkWidget *widget, gpointer data)
{
    // Создаем диалоговое окно с кнопкой "OK"
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Инструкция",     // Заголовок окна
        GTK_WINDOW(data), // Родительское окно
        GTK_DIALOG_MODAL, // Флаги окна (модальное)
        NULL              // Кнопки по умолчанию (NULL - нет кнопок по умолчанию)
    );

    // Получаем область содержимого диалогового окна
    GtkWidget *contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Создаем метку с текстом инструкции
    GtkWidget *label = gtk_label_new(
        "Перед началом использования коротенькая инструкция: эта чисто исследовательская программа,\n"
        "за всё зло во что её можно превратить, я Не несу ответсвенности. \n"
        "1)С начала нажми на кнопку \"установка нажатия.\"\n"
        "2)Выбери координаты для кнопок в соответсвии с названиями над их полями.\n"
        "(если что там указываются актуальные кооржинаты, для указания строки ввода указать её конец).\n"
        "3)Нажми на кнопку \"Задержка\".\n Где 1000 равна 1 секунде и выбери сколько будет длится ожидание проверки пароля и обязательно,\n для сохранения результата нажми \"ОК\"\n"
        "4)Нажми на кнопку \"Словарь\".\n"
        "(там надо отметить нужные символы, для удобства есть подборки символов).\n"
        "5)В главном окне ввести ЦИФРУ в поле ввода над кнопкой \"Вывести число\".\n"
        "(число должно обозначать минимально и максимально допустимую длину строки, \n"
        "перебор будет идти от меньшего к большему).\n"
        "6)В случае проблем или желание закрыть программу нажми на кнопку \"Отмена\".\n"
        "(выполнение перебора остановится и придётся запускать программу заново).\n"
        "7)Так же можно зайти/зарегистрироваться в локальный txt файл.\n"
        "(регистрация по сути бесполезная, но пока не войти в аккаунт, перебор не доступен).\n"
        "Эмулятор сам меняет язык, поддерживает все символы на клавиатуре, \n"
        "в случае баловства или не следованию инструкции придётся перезагружать ПК, удачи в ваших \"легальных\" целях!\n");

    // Добавляем метку в область содержимого диалогового окна
    gtk_container_add(GTK_CONTAINER(contentArea), label);

    // Добавляем кнопку "OK" в диалоговое окно
    gtk_dialog_add_button(GTK_DIALOG(dialog), "_OK", GTK_RESPONSE_ACCEPT);

    // Показываем все виджеты в диалоговом окне
    gtk_widget_show_all(dialog);

    // Запускаем диалоговое окно и ждем ответа пользователя
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Уничтожаем диалоговое окно после закрытия
    gtk_widget_destroy(dialog);
}

// Структура для передачи данных в поток перебора паролей
typedef struct
{
    int maxStringLength;   // Максимальная длина пароля для перебора
    int minStringLength;   // Минимальная длина пароля для перебора
    char **selectedValues; // Массив выбранных символов
    int activeCheckboxes;  // Количество активных чекбоксов
} ThreadData;
int mouseXInput = 0;          // Координата X поля ввода
int mouseYInput = 0;          // Координата Y поля ввода
int mouseXClick = 0;          // Координата X запуска
int mouseYClick = 0;          // Координата Y запуска
int mouseXLanguageSwitch = 0; // Координата X для смены языка
int mouseYLanguageSwitch = 0; // Координата Y для смены языка
int maxStringLength = 0;      // Глобальная переменная для хранения максимальной длины пароля
int minStringLength = 0;      // Глобальная переменная для хранения минимальной длины пароля
// Функция потока для перебора паролей
void *dictionaryThread(void *arg)
{
    ThreadData *data = (ThreadData *)arg; // Получаем данные из аргумента
    // Проверяем зарегестрирован ли пользователь
    int result = 1;
    if ((SuccessfulRegistration) && ((mouseXInput != 0) && (mouseYInput != 0) && (mouseXClick != 0) && (mouseYClick != 0) && (mouseXLanguageSwitch != 0) && (mouseYLanguageSwitch != 0)) && (maxStringLength > 0) && (minStringLength > 0) && (maxStringLength >= minStringLength))
    {
        // Вызываем функцию перебора паролей
        result = bruteForceCombinations(data->maxStringLength, data->minStringLength, data->selectedValues, data->activeCheckboxes);
    }
    else
    {
        if (((mouseXInput == 0) || (mouseYInput == 0) || (mouseXClick == 0) || (mouseYClick == 0) || (mouseXLanguageSwitch == 0) || (mouseYLanguageSwitch == 0)))
        {
            printf("Cursor coordinates are empty!\n");
        }
        else if ((maxStringLength < 0) || (minStringLength < 0) || (maxStringLength < minStringLength))
        {
            printf("The range of the string is incorrect!\n");
        }
        else
        {
            printf("Register or log in to your account!\n");
        }
    }
    // Обрабатываем результат перебора паролей
    if (result == 0)
    {
        printf("all OK\n"); // Выводим сообщение об успешном завершении
    }
    else
    {
        printf("Error\n"); // Выводим сообщение об ошибке
    }

    // Сбрасываем значения глобальных переменных
    selectedCount = 0;
    activeCheckboxes = 0;

    // Освобождаем память, выделенную для структуры данных
    free(data);
    pthread_exit(NULL); // Завершаем поток
}

// Массив значений для чекбоксов (символы для перебора)
char *checkboxValues[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    " ", ".", "'", "\\", ",", ";", "`", "-", "=", "[", "]", "!", "@", "#", "$", "%", "^", "&", "*", ":", "(", ")", "_", "+", "{", "}", "|", "/", "?", "~", "<", ">", "\"",
    "\u2116", // №
    "а", "б", "в", "г", "д", "е", "ё", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я",
    "А", "Б", "В", "Г", "Д", "Е", "Ё", "Ж", "З", "И", "Й", "К", "Л", "М", "Н", "О", "П", "Р", "С", "Т", "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я",
    "X"};

// Функция, вызываемая при нажатии на кнопку "Вывести число"
void onButtonClicked(GtkWidget *widget, gpointer data)
{
    // GtkWidget *entry = GTK_WIDGET(data);                      // Получаем виджет поля ввода
    // const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry)); // Получаем текст из поля ввода
    // value = atoi(text);                                       // Преобразуем текст в целое число
    g_print("%d-%d\n", minStringLength, maxStringLength); // Выводим значение в консоль

    // Создаем структуру для передачи данных в поток
    ThreadData *threadData = malloc(sizeof(ThreadData));
    threadData->maxStringLength = maxStringLength;   // Устанавливаем максимальную длину пароля
    threadData->minStringLength = minStringLength;   // Устанавливаем минимальную длину пароля
    threadData->selectedValues = selectedValues;     // Устанавливаем массив выбранных символов
    threadData->activeCheckboxes = activeCheckboxes; // Устанавливаем количество активных чекбоксов

    // Создаем новый поток
    pthread_t thread;
    pthread_create(&thread, NULL, dictionaryThread, threadData); // Запускаем поток
}

// Массив символов для чекбоксов (используется для создания меток)
const char *symbols[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    " ", ".", "'", "\\", ",", ";", "`", "-", "=", "[", "]", "!", "@", "#", "$", "%", "^", "&", "*", ":", "(", ")", "_", "+", "{", "}", "|", "/", "?", "~", "<", ">", "\"",
    "\u2116", // №
    "а", "б", "в", "г", "д", "е", "ё", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я",
    "А", "Б", "В", "Г", "Д", "Е", "Ё", "Ж", "З", "И", "Й", "К", "Л", "М", "Н", "О", "П", "Р", "С", "Т", "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я",
    "X",
    NULL};

// Функция, вызываемая при изменении состояния чекбокса
void checkboxToggled(GtkWidget *checkbox, gpointer data)
{
    int index = GPOINTER_TO_INT(data); // Получаем индекс символа из данных чекбокса

    // Проверяем, выбран ли чекбокс
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox)))
    {
        // Выделяем память для нового символа
        selectedValues = realloc(selectedValues, (selectedCount + 1) * sizeof(char *));
        if (selectedValues == NULL)
        {
            // Обработка ошибки выделения памяти
            return;
        }

        selectedValues[selectedCount++] = g_strdup(checkboxValues[index]); // Добавляем символ в массив
        activeCheckboxes++;                                                // Увеличиваем счетчик активных чекбоксов
    }
    else
    {
        // Удаляем символ из массива, если чекбокс снят
        for (int i = 0; i < selectedCount; i++)
        {
            if (selectedValues[i] != NULL && strcmp(selectedValues[i], checkboxValues[index]) == 0)
            {
                g_free(selectedValues[i]);
                // Сдвигаем элементы массива влево
                memmove(selectedValues + i, selectedValues + i + 1, (selectedCount - i - 1) * sizeof(char *));
                selectedCount--;    // Уменьшаем счетчик выбранных символов
                activeCheckboxes--; // Уменьшаем счетчик активных чекбоксов
                break;              // Выходим из цикла после удаления символа
            }
        }
    }
}
// Функция для выбора всех чекбоксов с цифрами
void selectDigits(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом
            // Получаем метку чекбокса
            const char *label = gtk_button_get_label(GTK_BUTTON(child->data));

            // Проверяем, является ли метка цифрой
            if (label[0] >= '0' && label[0] <= '9')
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов с буквами (английскими и русскими)
void selectLetters(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int checkboxIndex = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом

            // Проверяем, нужно ли выбрать текущий чекбокс (буквы)
            if ((checkboxIndex <= 66 && checkboxIndex != 0) || (checkboxIndex >= 101 && checkboxIndex <= 152))
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }

            checkboxIndex++; // Увеличиваем счетчик индекса
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов с русскими буквами
void selectRussianCharacters(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int checkboxIndex = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом

            // Проверяем, нужно ли выбрать текущий чекбокс (русские буквы)
            if (checkboxIndex <= 66 && checkboxIndex != 0)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }

            checkboxIndex++; // Увеличиваем счетчик индекса
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов с английскими буквами
void selectEnglishCharacters(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int checkboxIndex = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом

            // Проверяем, нужно ли выбрать текущий чекбокс (английские буквы)
            if (checkboxIndex >= 101 && checkboxIndex <= 152)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }

            checkboxIndex++; // Увеличиваем счетчик индекса
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов с заглавными русскими буквами
void selectRussianCharactersTop(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int checkboxIndex = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом

            // Проверяем, нужно ли выбрать текущий чекбокс (заглавные русские буквы)
            if (checkboxIndex <= 33 && checkboxIndex != 0)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }

            checkboxIndex++; // Увеличиваем счетчик индекса
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов со строчными русскими буквами
void selectRussianCharactersLower(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int checkboxIndex = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом

            // Проверяем, нужно ли выбрать текущий чекбокс (строчные русские буквы)
            if (checkboxIndex >= 34 && checkboxIndex <= 66)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }

            checkboxIndex++; // Увеличиваем счетчик индекса
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов с заглавными английскими буквами
void selectEnglishCharactersTop(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int checkboxIndex = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом

            // Проверяем, нужно ли выбрать текущий чекбокс (заглавные английские буквы)
            if (checkboxIndex >= 101 && checkboxIndex <= 126)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }

            checkboxIndex++; // Увеличиваем счетчик индекса
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов со строчными английскими буквами
void selectEnglishCharactersLower(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int checkboxIndex = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом

            // Проверяем, нужно ли выбрать текущий чекбокс (строчные английские буквы)
            if (checkboxIndex >= 129 && checkboxIndex <= 152)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }

            checkboxIndex++; // Увеличиваем счетчик индекса
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов со специальными символами
void selectCharacters(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int checkboxIndex = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        { // Проверяем, является ли элемент чекбоксом

            // Проверяем, нужно ли выбрать текущий чекбокс (специальные символы)
            if (checkboxIndex >= 67 && checkboxIndex <= 100)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            }

            checkboxIndex++; // Увеличиваем счетчик индекса
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка
}

// Функция для выбора всех чекбоксов
void selectAll(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    int index = 0; // Счетчик для индексов чекбоксов

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        {                                                                       // Проверяем, является ли элемент чекбоксом
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), TRUE); // Выбираем чекбокс
            index++;                                                            // Увеличиваем счетчик
        }
    }
    g_list_free(children); // Освобождаем память, выделенную для списка

    // Освобождаем память, выделенную для массива selectedValues
    if (selectedValues != NULL)
    {
        for (int i = 0; i < selectedCount; i++)
        {
            if (selectedValues[i] != NULL)
            {
                g_free(selectedValues[i]);
            }
        }
        free(selectedValues);
    }

    // Выделяем память для массива selectedValues и заполняем его значениями из checkboxValues
    selectedCount = index;
    selectedValues = calloc(sizeof(char *), selectedCount);
    for (int i = 0; i < selectedCount; i++)
    {
        selectedValues[i] = g_strdup(checkboxValues[i]);
    }

    activeCheckboxes = selectedCount - 1; // Устанавливаем количество активных чекбоксов
}

// Функция, вызываемая при нажатии на кнопку "Отмена"
void stopPlay(GtkWidget *widget, gpointer data)
{
    stopBruteForceAttack(); // Останавливаем перебор паролей
}

// Функция для снятия выбора со всех чекбоксов
void deselectAll(GtkWidget *button, gpointer data)
{
    GtkGrid *grid = GTK_GRID(data);                                    // Получаем виджет сетки
    GList *children = gtk_container_get_children(GTK_CONTAINER(grid)); // Получаем список дочерних элементов сетки

    // Проходим по всем дочерним элементам сетки
    for (GList *child = children; child != NULL; child = child->next)
    {
        if (GTK_IS_CHECK_BUTTON(child->data))
        {                                                                        // Проверяем, является ли элемент чекбоксом
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child->data), FALSE); // Снимаем выбор с чекбокса
        }
    }

    g_list_free(children); // Освобождаем память, выделенную для списка

    // Освобождаем память, выделенную для массива selectedValues
    if (selectedValues != NULL)
    {
        for (int i = 0; i < selectedCount; i++)
        {
            if (selectedValues[i] != NULL)
            {
                g_free(selectedValues[i]);
            }
        }
        free(selectedValues);
    }

    selectedValues = NULL; // Обнуляем указатель на массив
    selectedCount = 0;     // Сбрасываем количество выбранных символов
    activeCheckboxes = 0;  // Сбрасываем количество активных чекбоксов
}

int delayBefore = 0; // Задержка перед проверкой пароля (в миллисекундах)
int delayAfter = 0;  // Задержка после проверки пароля (в миллисекундах)

// Функция, вызываемая при нажатии на кнопку "OK" в окне настроек задержки
void onDelayOkClicked(GtkWidget *widget, gpointer data)
{
    // Преобразуем указатель data в массив указателей на виджеты
    GtkWidget **entries = (GtkWidget **)data;

    // Получаем виджеты полей ввода
    GtkWidget *beforeEntry = entries[0];
    GtkWidget *afterEntry = entries[1];

    // Считываем значения задержек из полей ввода
    delayBefore = atoi(gtk_entry_get_text(GTK_ENTRY(beforeEntry)));
    delayAfter = atoi(gtk_entry_get_text(GTK_ENTRY(afterEntry)));

    // Устанавливаем значения задержек в буфере
    setDelayBeforeBuffer(delayBefore);
    setDelayAfterBuffer(delayAfter);

    // Выводим значения задержек в консоль
    printf("Delay before inspection: %d\n", delayBefore);
    printf("Delay after verification: %d\n", delayAfter);

    // Закрываем окно настроек
    gtk_widget_destroy(gtk_widget_get_toplevel(widget));
}
void stringLengthOkClicked(GtkWidget *widget, gpointer data)
{
    // Преобразуем указатель data в массив указателей на виджеты
    GtkWidget **entries = (GtkWidget **)data;

    // Получаем виджеты полей ввода
    GtkWidget *minLengthEntry = entries[0];
    GtkWidget *maxLengthEntry = entries[1];

    // Считываем значения минимальной и максимальной длины строки из полей ввода
    minStringLength = atoi(gtk_entry_get_text(GTK_ENTRY(minLengthEntry)));
    maxStringLength = atoi(gtk_entry_get_text(GTK_ENTRY(maxLengthEntry)));

    // setMinStringLengthBuffer(minStringLength);
    // setMaxStringLengthBuffer(maxStringLength);

    // Выводим значения минимальной и максимальной длины строки в консоль
    printf("Minimal string length: %d\n", minStringLength);
    printf("Maximal string length: %d\n", maxStringLength);

    // Закрываем окно настроек
    gtk_widget_destroy(gtk_widget_get_toplevel(widget));
}
// Функция для создания и отображения окна настроек задержки
void delaySettings(GtkWidget *widget, gpointer data)
{
    // Создаем окно настроек
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Устанавливаем заголовок окна
    gtk_window_set_title(GTK_WINDOW(window), "Настройки задержки");

    // Устанавливаем размер окна
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    // Подключаем обработчик события "destroy" для закрытия окна
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), window);

    // Создаем сетку для размещения виджетов
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Создаем метку и поле ввода для задержки перед проверкой
    GtkWidget *beforeLabel = gtk_label_new("Перед проверкой (мс):");
    gtk_grid_attach(GTK_GRID(grid), beforeLabel, 0, 0, 1, 1);

    GtkWidget *beforeEntry = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(beforeEntry), GTK_INPUT_PURPOSE_NUMBER);
    gtk_grid_attach(GTK_GRID(grid), beforeEntry, 1, 0, 1, 1);

    // Создаем метку и поле ввода для задержки после проверки
    GtkWidget *afterLabel = gtk_label_new("После проверки (мс):");
    gtk_grid_attach(GTK_GRID(grid), afterLabel, 0, 1, 1, 1);

    GtkWidget *afterEntry = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(afterEntry), GTK_INPUT_PURPOSE_NUMBER);
    gtk_grid_attach(GTK_GRID(grid), afterEntry, 1, 1, 1, 1);

    // Создаем кнопку "OK"
    GtkWidget *okButton = gtk_button_new_with_label("OK");

    // Создаем массив для передачи указателей на поля ввода в функцию-обработчик
    static gpointer dataForOk[2];
    dataForOk[0] = beforeEntry;
    dataForOk[1] = afterEntry;

    // Подключаем обработчик нажатия на кнопку "OK"
    g_signal_connect(okButton, "clicked", G_CALLBACK(onDelayOkClicked), dataForOk);

    // Размещаем кнопку "OK" в сетке
    gtk_grid_attach(GTK_GRID(grid), okButton, 0, 2, 2, 1);

    // Отображаем все виджеты в окне
    gtk_widget_show_all(window);
}
void searchRanger(GtkWidget *widget, gpointer data)
{
    // Создаем окно настроек
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Устанавливаем заголовок окна
    gtk_window_set_title(GTK_WINDOW(window), "Диапозон перебора");

    // Устанавливаем размер окна
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    // Подключаем обработчик события "destroy" для закрытия окна
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), window);

    // Создаем сетку для размещения виджетов
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Создаем метку и поле ввода для минимальной длины строки
    GtkWidget *minLengthLabel = gtk_label_new("Минимальная длина строки:");
    gtk_grid_attach(GTK_GRID(grid), minLengthLabel, 0, 0, 1, 1);

    GtkWidget *minLengthEntry = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(minLengthEntry), GTK_INPUT_PURPOSE_NUMBER);
    gtk_grid_attach(GTK_GRID(grid), minLengthEntry, 1, 0, 1, 1);

    // Создаем метку и поле ввода для максимальной длины строки
    GtkWidget *maxLengthLabel = gtk_label_new("Максимальная длина строки:");
    gtk_grid_attach(GTK_GRID(grid), maxLengthLabel, 0, 1, 1, 1);

    GtkWidget *maxLengthEntry = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(maxLengthEntry), GTK_INPUT_PURPOSE_NUMBER);
    gtk_grid_attach(GTK_GRID(grid), maxLengthEntry, 1, 1, 1, 1);

    // Создаем кнопку "OK"
    GtkWidget *okButton = gtk_button_new_with_label("OK");

    // Создаем массив для передачи указателей на поля ввода в функцию-обработчик
    static gpointer dataForOk[2];
    dataForOk[0] = minLengthEntry;
    dataForOk[1] = maxLengthEntry;

    // Подключаем обработчик нажатия на кнопку "OK"
    g_signal_connect(okButton, "clicked", G_CALLBACK(stringLengthOkClicked), dataForOk);

    // Размещаем кнопку "OK" в сетке
    gtk_grid_attach(GTK_GRID(grid), okButton, 0, 2, 2, 1);

    // Отображаем все виджеты в окне
    gtk_widget_show_all(window);
}
GtkWidget *outputEntry = NULL;  // Поле вывода для координат поля ввода
GtkWidget *outputEntry1 = NULL; // Поле вывода для координат запуска
GtkWidget *outputEntry2 = NULL; // Поле вывода для координат смены языка

// Функция для получения координат мыши и установки их в поля ввода
void getMouseCoordinatesForInput(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    // Получаем координаты мыши
    mouseXInput = find_out_mouse_coordinates_x();
    mouseYInput = find_out_mouse_coordinates_y();

    // Устанавливаем координаты в соответствующие переменные
    setInputFieldX(mouseXInput);
    setInputFieldY(mouseYInput);
    setMouseInputBufferX(mouseXInput);
    setMouseInputBufferY(mouseYInput);

    // Обновляем текст в поле вывода
    if (outputEntry != NULL)
    {
        gchar *newText = g_strdup_printf("Координаты поля ввода: x=%d, y=%d", mouseXInput, mouseYInput);
        gtk_entry_set_text(GTK_ENTRY(outputEntry), newText);
        g_free(newText);
    }
}

// Функция для получения координат мыши и установки их в поля запуска
void getMouseCoordinatesForClick(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    // Получаем координаты мыши
    mouseXClick = find_out_mouse_coordinates_x();
    mouseYClick = find_out_mouse_coordinates_y();

    // Устанавливаем координаты в соответствующие переменные
    setClickPointX(mouseXClick);
    setClickPointY(mouseYClick);

    // Обновляем текст в поле вывода
    if (outputEntry1 != NULL)
    {
        gchar *newText = g_strdup_printf("Координаты курсора нажатия: x=%d, y=%d", mouseXClick, mouseYClick);
        gtk_entry_set_text(GTK_ENTRY(outputEntry1), newText);
        g_free(newText);
    }
}

// Функция для получения координат мыши и установки их в поля смены языка
void getMouseCoordinatesForLanguageSwitch(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    // Получаем координаты мыши
    mouseXLanguageSwitch = find_out_mouse_coordinates_x();
    mouseYLanguageSwitch = find_out_mouse_coordinates_y();

    // Устанавливаем координаты в соответствующие переменные
    setLanguageSwitchX(mouseXLanguageSwitch);
    setLanguageSwitchY(mouseYLanguageSwitch);
    languageSwitchBufferX(mouseXLanguageSwitch);
    languageSwitchBufferY(mouseYLanguageSwitch);

    // Обновляем текст в поле вывода
    if (outputEntry2 != NULL)
    {
        gchar *newText = g_strdup_printf("Координаты курсора смены языка: x=%d, y=%d", mouseXLanguageSwitch, mouseYLanguageSwitch);
        gtk_entry_set_text(GTK_ENTRY(outputEntry2), newText);
        g_free(newText);
    }
}

// Обработчик нажатия клавиши "1"
gboolean onKeyPress1(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    if (event->keyval == GDK_KEY_1)
    {
        GtkWidget *button = GTK_WIDGET(data);            // Получаем виджет кнопки
        getMouseCoordinatesForInput(button, NULL, NULL); // Вызываем функцию для получения координат мыши
        return TRUE;                                     // Обработали нажатие клавиши
    }
    return FALSE; // Не обработали нажатие клавиши
}

// Обработчик нажатия клавиши "2"
gboolean onKeyPress2(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    if (event->keyval == GDK_KEY_2)
    {
        GtkWidget *button = GTK_WIDGET(data);            // Получаем виджет кнопки
        getMouseCoordinatesForClick(button, NULL, NULL); // Вызываем функцию для получения координат мыши
        return TRUE;                                     // Обработали нажатие клавиши
    }
    return FALSE; // Не обработали нажатие клавиши
}

// Обработчик нажатия клавиши "F3"
gboolean onKeyPressF3(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    if (event->keyval == GDK_KEY_F3)
    {
        GtkWidget *stopButton = GTK_WIDGET(data); // Получаем виджет кнопки
        stopPlay(stopButton, NULL);               // Вызываем функцию для остановки перебора паролей
        return TRUE;                              // Обработали нажатие клавиши
    }
    return FALSE; // Не обработали нажатие клавиши
}

// Обработчик нажатия клавиши "F12"
gboolean onKeyPressF12(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    if (event->keyval == GDK_KEY_F12)
    {
        GtkWidget *button = GTK_WIDGET(data); // Получаем виджет кнопки
        onButtonClicked(button, NULL);        // Вызываем функцию для запуска перебора паролей
        return TRUE;                          // Обработали нажатие клавиши
    }
    return FALSE; // Не обработали нажатие клавиши
}

// Обработчик нажатия клавиши "3"
gboolean onKeyPress3(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    if (event->keyval == GDK_KEY_3)
    {
        GtkWidget *button = GTK_WIDGET(data);                     // Получаем виджет кнопки
        getMouseCoordinatesForLanguageSwitch(button, NULL, NULL); // Вызываем функцию для получения координат мыши
        return TRUE;                                              // Обработали нажатие клавиши
    }
    return FALSE; // Не обработали нажатие клавиши
}

// Функция для создания окна с настройками нажатий (координаты курсора)
void pressingSetting(GtkWidget *widget, gpointer data)
{
    // Создаем окно
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Устанавливаем заголовок окна
    gtk_window_set_title(GTK_WINDOW(window), "Координаты курсора");

    // Устанавливаем размер окна
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 100);

    // Подключаем обработчик события "destroy" для закрытия окна
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), window);

    // Создаем кнопки
    GtkWidget *button = gtk_button_new_with_label("Координаты курсора поля ввода (1)");
    GtkWidget *button1 = gtk_button_new_with_label("Координаты курсора запуска (2)");
    GtkWidget *button2 = gtk_button_new_with_label("Координаты курсора смены языка (3)");

    // Подключаем обработчики нажатия на кнопки
    g_signal_connect(button, "button-press-event", G_CALLBACK(getMouseCoordinatesForInput), NULL);
    g_signal_connect(button1, "button-press-event", G_CALLBACK(getMouseCoordinatesForClick), NULL);
    g_signal_connect(button2, "button-press-event", G_CALLBACK(getMouseCoordinatesForLanguageSwitch), NULL);

    // Создаем поля вывода
    outputEntry = gtk_entry_new();
    outputEntry1 = gtk_entry_new();
    outputEntry2 = gtk_entry_new();

    // Устанавливаем текст по умолчанию в поля вывода
    gtk_entry_set_text(GTK_ENTRY(outputEntry), "Координаты поля ввода: x=ПУСТО, y=ПУСТО");
    gtk_entry_set_text(GTK_ENTRY(outputEntry1), "Координаты курсора нажатия: x=ПУСТО, y=ПУСТО");
    gtk_entry_set_text(GTK_ENTRY(outputEntry2), "Координаты курсора смены языка: x=ПУСТО, y=ПУСТО");

    // Создаем вертикальный контейнер для размещения виджетов
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Добавляем кнопки и поля вывода в контейнер
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), outputEntry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), outputEntry1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), outputEntry2, TRUE, TRUE, 0);

    // Добавляем контейнер в окно
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Подключаем обработчики нажатия клавиш к окну
    g_signal_connect(window, "key-press-event", G_CALLBACK(onKeyPress1), button);
    g_signal_connect(window, "key-press-event", G_CALLBACK(onKeyPress2), button1);
    g_signal_connect(window, "key-press-event", G_CALLBACK(onKeyPress3), button2);

    // Показываем все виджеты в окне
    gtk_widget_show_all(window);
}

// Структура для хранения данных для входа/регистрации
typedef struct
{
    GtkWidget *usernameEntry; // Поле ввода имени пользователя
    GtkWidget *passwordEntry; // Поле ввода пароля
    GtkWidget *outputEntry;   // Поле вывода результата
} LoginData;

// Функция для проверки существования пользователя в файле
int checkUserExists(const char *username, const char *password)
{
    FILE *file = fopen("users.txt", "r"); // Открываем файл "users.txt" для чтения
    if (file != NULL)
    {                   // Проверяем, удалось ли открыть файл
        char line[256]; // Буфер для чтения строки из файла
        while (fgets(line, sizeof(line), file))
        { // Читаем строки из файла до конца
            // Проверяем, содержит ли строка имя пользователя и пароль
            if (strstr(line, "Имя пользователя:") && strstr(line, username))
            {
                char *passwordLine = fgets(line, sizeof(line), file); // Читаем следующую строку (пароль)
                if (passwordLine && strstr(passwordLine, "Пароль:") && strstr(passwordLine, password))
                {
                    fclose(file); // Закрываем файл
                    return 1;     // Пользователь найден
                }
            }
        }
        fclose(file); // Закрываем файл
    }
    return 0; // Пользователь не найден
}

// Функция для входа пользователя
void loginUser(GtkWidget *button, gpointer data)
{
    // Получаем данные для входа/регистрации
    LoginData *loginData = (LoginData *)data;

    // Получаем имя пользователя и пароль из полей ввода
    const char *username = gtk_entry_get_text(GTK_ENTRY(loginData->usernameEntry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(loginData->passwordEntry));

    // Проверяем, существует ли пользователь
    if (checkUserExists(username, password))
    {
        printf("You are now a qualified user!\n");
        SuccessfulRegistration = true;
        gtk_entry_set_text(GTK_ENTRY(loginData->outputEntry), "Вход успешен!"); // Выводим сообщение об успешном входе
    }
    else
    {

        gtk_entry_set_text(GTK_ENTRY(loginData->outputEntry), "Неверное имя пользователя или пароль."); // Выводим сообщение об ошибке
    }
}

// Функция для регистрации пользователя
void registerUser(GtkWidget *button, gpointer data)
{
    // Получаем данные для входа/регистрации
    LoginData *loginData = (LoginData *)data;

    // Получаем имя пользователя и пароль из полей ввода
    const char *username = gtk_entry_get_text(GTK_ENTRY(loginData->usernameEntry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(loginData->passwordEntry));

    // Проверяем, существует ли уже пользователь с таким именем
    if (checkUserExists(username, password))
    {
        gtk_entry_set_text(GTK_ENTRY(loginData->outputEntry), "Пользователь с таким именем уже существует.");
        return;
    }

    // Открываем файл "users.txt" для добавления данных
    FILE *file = fopen("users.txt", "a");
    if (file != NULL)
    {                                                                                  // Проверяем, удалось ли открыть файл
        fprintf(file, "Имя пользователя: %s\nПароль: %s\n\n", username, password);     // Записываем данные пользователя в файл
        fclose(file);                                                                  // Закрываем файл
        gtk_entry_set_text(GTK_ENTRY(loginData->outputEntry), "Регистрация успешна!"); // Выводим сообщение об успешной регистрации
    }
    else
    {
        // Если не удалось открыть файл для добавления, пытаемся создать новый файл
        file = fopen("users.txt", "w");
        if (file != NULL)
        {                                                                                  // Проверяем, удалось ли создать файл
            fprintf(file, "Имя пользователя: %s\nПароль: %s\n\n", username, password);     // Записываем данные пользователя в файл
            fclose(file);                                                                  // Закрываем файл
            gtk_entry_set_text(GTK_ENTRY(loginData->outputEntry), "Регистрация успешна!"); // Выводим сообщение об успешной регистрации
        }
        else
        {
            gtk_entry_set_text(GTK_ENTRY(loginData->outputEntry), "Ошибка при сохранении данных."); // Выводим сообщение об ошибке
        }
    }
}

// Функция для создания и отображения окна входа/регистрации
void showLoginWindow(GtkWidget *button, gpointer data)
{
    // Создаем окно входа/регистрации
    GtkWidget *loginWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Устанавливаем заголовок окна
    gtk_window_set_title(GTK_WINDOW(loginWindow), "Вход/Регистрация");

    // Устанавливаем размер окна
    gtk_window_set_default_size(GTK_WINDOW(loginWindow), 300, 125);

    // Устанавливаем позицию окна по центру экрана
    gtk_window_set_position(GTK_WINDOW(loginWindow), GTK_WIN_POS_CENTER);

    // Подключаем обработчик события "destroy" для закрытия окна
    g_signal_connect(loginWindow, "destroy", G_CALLBACK(gtk_widget_destroy), loginWindow);

    // Создаем метки и поля ввода для имени пользователя и пароля
    GtkWidget *usernameLabel = gtk_label_new("Имя пользователя:");
    GtkWidget *usernameEntry = gtk_entry_new();
    GtkWidget *passwordLabel = gtk_label_new("Пароль:");
    GtkWidget *passwordEntry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(passwordEntry), FALSE); // Скрываем вводимый пароль

    // Создаем метку и поле вывода результата
    GtkWidget *outputLabel = gtk_label_new("Результат:");
    GtkWidget *outputEntry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(outputEntry), FALSE); // Делаем поле вывода нередактируемым

    // Создаем кнопки "Войти" и "Зарегистрироваться"
    GtkWidget *loginButton = gtk_button_new_with_label("Войти");
    GtkWidget *registerButton = gtk_button_new_with_label("Зарегистрироваться");

    // Создаем структуру для передачи данных в функции обработки нажатия кнопок
    LoginData *loginData = malloc(sizeof(LoginData));
    loginData->usernameEntry = usernameEntry;
    loginData->passwordEntry = passwordEntry;
    loginData->outputEntry = outputEntry;

    // Подключаем обработчики нажатия на кнопки
    g_signal_connect(loginButton, "clicked", G_CALLBACK(loginUser), loginData);
    g_signal_connect(registerButton, "clicked", G_CALLBACK(registerUser), loginData);

    // Создаем сетку для размещения виджетов
    GtkWidget *grid = gtk_grid_new();

    // Размещаем виджеты в сетке
    gtk_grid_attach(GTK_GRID(grid), usernameLabel, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), usernameEntry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), passwordLabel, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), passwordEntry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), outputLabel, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), outputEntry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), loginButton, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), registerButton, 1, 3, 1, 1);

    // Добавляем сетку в окно
    gtk_container_add(GTK_CONTAINER(loginWindow), grid);

    // Отображаем все виджеты в окне
    gtk_widget_show_all(loginWindow);
}

// Функция для создания и отображения окна выбора символов для перебора
void showSymbolWindow(GtkWidget *button, gpointer data)
{
    // Создаем окно выбора символов
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Устанавливаем заголовок окна
    gtk_window_set_title(GTK_WINDOW(window), "Символы");

    // Устанавливаем размер окна (автоматический)
    gtk_window_set_default_size(GTK_WINDOW(window), -1, -1);

    // Устанавливаем позицию окна по центру экрана
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Подключаем обработчик события "destroy" для закрытия окна
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroy), window);

    // Создаем сетку для размещения чекбоксов
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE); // Устанавливаем одинаковую ширину столбцов

    int row = 0, col = 0; // Индексы строки и столбца для размещения чекбоксов

    // Создаем чекбоксы для каждого символа в массиве symbols
    for (int i = 0; symbols[i] != NULL; i++)
    {
        GtkWidget *checkbox = gtk_check_button_new_with_label(symbols[i]);                      // Создаем чекбокс с меткой
        g_signal_connect(checkbox, "toggled", G_CALLBACK(checkboxToggled), GINT_TO_POINTER(i)); // Подключаем обработчик изменения состояния
        gtk_grid_attach(GTK_GRID(grid), checkbox, col, row, 1, 1);                              // Размещаем чекбокс в сетке
        col++;                                                                                  // Увеличиваем индекс столбца
        if (col > 6)
        {            // Если достигли конца строки
            col = 0; // Сбрасываем индекс столбца
            row++;   // Переходим на следующую строку
        }
    }

    // Создаем кнопки для управления выбором чекбоксов
    GtkWidget *selectAllButton = gtk_button_new_with_label("Выбрать всё");
    GtkWidget *deselectAllButton = gtk_button_new_with_label("Отменить всё");
    GtkWidget *selectDigitsButton = gtk_button_new_with_label("Цифры");
    GtkWidget *selectLettersButton = gtk_button_new_with_label("Буквы");
    GtkWidget *selectRussianCharactersButton = gtk_button_new_with_label("Русские буквы");
    GtkWidget *selectEnglishCharactersButton = gtk_button_new_with_label("Английские буквы");
    GtkWidget *selectRussianCharactersTopButton = gtk_button_new_with_label("Русские буквы (заглавные)");
    GtkWidget *selectRussianCharactersLowerButton = gtk_button_new_with_label("Русские буквы (строчные)");
    GtkWidget *selectEnglishCharactersTopButton = gtk_button_new_with_label("Английские буквы (заглавные)");
    GtkWidget *selectEnglishCharactersLowerButton = gtk_button_new_with_label("Английские буквы (строчные)");
    GtkWidget *selectSpecialCharactersButton = gtk_button_new_with_label("Специальные символы");

    // Подключаем обработчики нажатия на кнопки
    g_signal_connect(selectAllButton, "clicked", G_CALLBACK(selectAll), grid);
    g_signal_connect(deselectAllButton, "clicked", G_CALLBACK(deselectAll), grid);
    g_signal_connect(selectDigitsButton, "clicked", G_CALLBACK(selectDigits), grid);
    g_signal_connect(selectLettersButton, "clicked", G_CALLBACK(selectLetters), grid);
    g_signal_connect(selectRussianCharactersButton, "clicked", G_CALLBACK(selectRussianCharacters), grid);
    g_signal_connect(selectEnglishCharactersButton, "clicked", G_CALLBACK(selectEnglishCharacters), grid);
    g_signal_connect(selectRussianCharactersTopButton, "clicked", G_CALLBACK(selectRussianCharactersTop), grid);
    g_signal_connect(selectRussianCharactersLowerButton, "clicked", G_CALLBACK(selectRussianCharactersLower), grid);
    g_signal_connect(selectEnglishCharactersTopButton, "clicked", G_CALLBACK(selectEnglishCharactersTop), grid);
    g_signal_connect(selectEnglishCharactersLowerButton, "clicked", G_CALLBACK(selectEnglishCharactersLower), grid);
    g_signal_connect(selectSpecialCharactersButton, "clicked", G_CALLBACK(selectCharacters), grid);

    // Создаем горизонтальные боксы для размещения кнопок
    GtkWidget *buttonsBox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(buttonsBox1), selectAllButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttonsBox1), deselectAllButton, TRUE, TRUE, 0);

    GtkWidget *buttonsBox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(buttonsBox2), selectDigitsButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttonsBox2), selectLettersButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttonsBox2), selectSpecialCharactersButton, TRUE, TRUE, 0);

    GtkWidget *buttonsBox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(buttonsBox3), selectRussianCharactersButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttonsBox3), selectEnglishCharactersButton, TRUE, TRUE, 0);

    GtkWidget *buttonsBox4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(buttonsBox4), selectRussianCharactersTopButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttonsBox4), selectEnglishCharactersTopButton, TRUE, TRUE, 0);

    GtkWidget *buttonsBox5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(buttonsBox5), selectRussianCharactersLowerButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttonsBox5), selectEnglishCharactersLowerButton, TRUE, TRUE, 0);

    // Размещаем боксы с кнопками в сетке
    gtk_grid_attach(GTK_GRID(grid), buttonsBox1, 0, row + 1, 7, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonsBox2, 0, row + 2, 7, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonsBox3, 0, row + 3, 7, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonsBox4, 0, row + 4, 7, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonsBox5, 0, row + 5, 7, 1);

    deselectAll(deselectAllButton, grid); // Снимаем выбор со всех чекбоксов по умолчанию

    // Отображаем все виджеты в окне
    gtk_widget_show_all(window);
}

// Функция для входа пользователя

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv); // Инициализируем библиотеку GTK+
    printf("You have to register, it s useless, but you have to!\n");
    // Создаем главное окно
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "BruteForce Attack");        // Устанавливаем заголовок окна
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);            // Устанавливаем размер окна
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // Подключаем обработчик закрытия окна

    // Создаем сетку для размещения виджетов
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Создаем кнопку "Задержка"
    GtkWidget *delayButton = gtk_button_new_with_label("Задержка");
    g_signal_connect(delayButton, "clicked", G_CALLBACK(delaySettings), NULL); // Подключаем обработчик нажатия
    gtk_grid_attach(GTK_GRID(grid), delayButton, 0, 0, 2, 1);                  // Размещаем кнопку в сетке

    // Создаем кнопку "Инструкция"
    GtkWidget *instructionButton = gtk_button_new_with_label("Инструкция");
    g_signal_connect(instructionButton, "clicked", G_CALLBACK(onInstructionClicked), window); // Подключаем обработчик нажатия
    gtk_grid_attach(GTK_GRID(grid), instructionButton, 0, 2, 1, 1);                           // Размещаем кнопку в сетке

    // Создаем кнопку "Установка нажатия"
    GtkWidget *pressingSettingButton = gtk_button_new_with_label("Установка нажатия");
    g_signal_connect(pressingSettingButton, "clicked", G_CALLBACK(pressingSetting), NULL); // Подключаем обработчик нажатия
    gtk_grid_attach(GTK_GRID(grid), pressingSettingButton, 1, 2, 1, 1);                    // Размещаем кнопку в сетке

    // Создаем кнопку "Регистрация/Вход"
    GtkWidget *loginButton = gtk_button_new_with_label("Регистрация/Вход");
    gtk_widget_set_hexpand(loginButton, TRUE);                                   // Разрешаем кнопке расширяться по горизонтали
    gtk_widget_set_vexpand(loginButton, TRUE);                                   // Разрешаем кнопке расширяться по вертикали
    g_signal_connect(loginButton, "clicked", G_CALLBACK(showLoginWindow), NULL); // Подключаем обработчик нажатия
    gtk_grid_attach(GTK_GRID(grid), loginButton, 0, 4, 1, 2);                    // Размещаем кнопку в сетке

    // Создаем поле ввода для максимальной длины пароля
    GtkWidget *searchRange = gtk_button_new_with_label("Диапозон перебора");
    g_signal_connect(searchRange, "clicked", G_CALLBACK(searchRanger), NULL); // Подключаем обработчик нажатия
    gtk_grid_attach(GTK_GRID(grid), searchRange, 1, 3, 1, 1);                 // Размещаем поле ввода в сетке

    // Создаем кнопку "Вывести число F12"
    GtkWidget *startButton = gtk_button_new_with_label("Вывести число (F12)");
    g_signal_connect(startButton, "clicked", G_CALLBACK(onButtonClicked), NULL); // Подключаем обработчик нажатия
    gtk_grid_attach(GTK_GRID(grid), startButton, 1, 4, 1, 1);                    // Размещаем кнопку в сетке

    // Создаем кнопку "Отмена (F3)"
    GtkWidget *stopButton = gtk_button_new_with_label("Отмена (F3)");
    g_signal_connect(stopButton, "clicked", G_CALLBACK(stopPlay), NULL); // Подключаем обработчик нажатия
    gtk_grid_attach(GTK_GRID(grid), stopButton, 1, 5, 1, 1);             // Размещаем кнопку в сетке

    // Создаем кнопку "Словарь"
    GtkWidget *dictionaryButton = gtk_button_new_with_label("Словарь");
    gtk_widget_set_size_request(dictionaryButton, 100, 50);                            // Устанавливаем размер кнопки
    g_signal_connect(dictionaryButton, "clicked", G_CALLBACK(showSymbolWindow), NULL); // Подключаем обработчик нажатия
    gtk_grid_attach(GTK_GRID(grid), dictionaryButton, 0, 3, 1, 1);                     // Размещаем кнопку в сетке

    // Отображаем все виджеты в окне
    gtk_widget_show_all(window);

    // Запускаем главный цикл GTK+
    gtk_main();

    // Освобождаем память, выделенную для массива selectedValues, после выхода из главного цикла
    if (selectedValues != NULL)
    {
        for (int i = 0; i < selectedCount; i++)
        {
            if (selectedValues[i] != NULL)
            {
                g_free(selectedValues[i]);
            }
        }
        free(selectedValues);
    }

    return 0; // Возвращаем 0, сигнализируя об успешном завершении программы
}