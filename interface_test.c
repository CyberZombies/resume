#include <gtk/gtk.h>

void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_F1) {
        // Функция, которая будет вызываться при нажатии F1
        printf("CF F1\n");
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Обработка нажатия F1");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Подключаем обработчик события нажатия клавиши
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}