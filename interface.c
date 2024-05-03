#include "dictionary.c"
#include <gtk/gtk.h>
#include <stdlib.h>

// Функция, вызываемая при нажатии кнопки
void on_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data);
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    int value = atoi(text);
    g_print("%d\n", value);
    dictionary(value);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Создание главного окна
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Ввод числа");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 150);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Создание вертикального контейнера-бокса
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Создание окна ввода
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(entry), GTK_INPUT_PURPOSE_NUMBER);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    // Создание кнопки
    GtkWidget *button = gtk_button_new_with_label("Вывести число");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), entry);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    // Отображение виджетов и запуск главного цикла
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
