// gcc main_gui5-3.c -o lc3vm-gui `pkg-config --cflags --libs gtk+-3.0`
// ./lc3vm-gui


#include <gtk/gtk.h>

static void on_launch_program(GtkWidget *widget, gpointer data) {
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, "Launch Program button clicked.\n", -1);
}

static void on_status_machina(GtkWidget *widget, gpointer data) {
    // Implementare qui il codice per mostrare lo "Status" della macchina
    printf("Status Machina button clicked.\n");
}

static void on_info_machina(GtkWidget *widget, gpointer data) {
    // Implementare qui il codice per mostrare le "Info" sulla macchina
    printf("Info Machina button clicked.\n");
}

static void on_exit_callback(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
    // Funzione di "Exit" già implementata
}

static void on_compile(GtkWidget *widget, gpointer data) {
    // Implementare qui il codice per compilare (bottone "compile")
    printf("Compile button clicked.\n");
}

static void on_send_file(GtkWidget *widget, gpointer data) {
    // Implementare qui il codice per inviare il file
    /*
    La Gui consente di selezionare solo file con estensione obj
    In caso contrario restituisce un messaggio di errore a schermo
    */
    printf("Send File button clicked.\n");
}

static void on_file_selection(GtkFileChooserButton *filechooserbutton, gpointer data) {
    gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton));
    g_print("Selected file: %s\n", filename);
    g_free(filename);
}

static void on_send_file_open_window(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    GtkWidget *main_window = GTK_WIDGET(gtk_widget_get_toplevel(widget));

    dialog = gtk_file_chooser_dialog_new("Select File",
                                         GTK_WINDOW(main_window),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(dialog);

    gtk_file_chooser_set_select_multiple(file_chooser, TRUE);

    gtk_file_chooser_set_current_folder(file_chooser, g_get_home_dir());

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        // Esegui l'azione necessaria sui file selezionati
        GSList *filenames = gtk_file_chooser_get_filenames(file_chooser);
        for (GSList *node = filenames; node != NULL; node = g_slist_next(node)) {
            gchar *filename = node->data;

            // Verifica se il file ha l'estensione .obj
            gchar *extension = g_strrstr(filename, ".");
            if (extension && g_strcmp0(extension, ".obj") == 0) {
                // Esegui l'azione necessaria sul file
                g_print("Selected file: %s\n", filename);
                // Leggi il contenuto del file
                gchar *file_contents = NULL;
                gsize length;
                GError *error = NULL;
                gboolean success = g_file_get_contents(filename, &file_contents, &length, &error);
                if (success) {
                    // Aggiungi il contenuto del file al buffer di output
                    GtkTextView *output_text = GTK_TEXT_VIEW(data);
                    GtkTextBuffer *buffer = gtk_text_view_get_buffer(output_text);
                    gtk_text_buffer_set_text(buffer, file_contents, length);
                    g_free(file_contents);
                } else {
                    // Mostra un messaggio di errore se non è stato possibile leggere il file
                    g_print("Error reading file: %s\n", error->message);
                    g_error_free(error);
                }
            } else {
                // Mostra un messaggio di errore per i file non supportati
                GtkTextView *output_text = GTK_TEXT_VIEW(data);
                GtkTextBuffer *buffer = gtk_text_view_get_buffer(output_text);
                gtk_text_buffer_set_text(buffer, "Tipo di file non supportato.\n", -1);
            }

            g_free(filename);
        }
        g_slist_free(filenames);
    }

    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *header_bar;
    GtkWidget *paned;
    GtkWidget *output_window;
    GtkWidget *input_window;
    GtkWidget *bottom_hbox;
    GtkWidget *compile_button;
    GtkWidget *send_file_button;

    // Dichiarazioni delle variabili per i pulsanti
    GtkWidget *launch_program_button;
    GtkWidget *status_machina_button;
    GtkWidget *info_machina_button;
    GtkWidget *exit_button;
    GtkWidget *output_text;
    GtkTextBuffer *output_buffer;  // Dichiarazione di output_buffer

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "LC3VM GUI - v5.3.9");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(on_exit_callback), NULL);

    header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "LC3VM GUI - v5.3.9");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    output_text = gtk_text_view_new();
    output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_text));  // Assegna output_buffer

    launch_program_button = gtk_button_new_with_label("Launch Program");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), launch_program_button);
    g_signal_connect(launch_program_button, "clicked", G_CALLBACK(on_launch_program), output_buffer);

    status_machina_button = gtk_button_new_with_label("Status Machina");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), status_machina_button);
    g_signal_connect(status_machina_button, "clicked", G_CALLBACK(on_status_machina), NULL);

    info_machina_button = gtk_button_new_with_label("Info Machina");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), info_machina_button);
    g_signal_connect(info_machina_button, "clicked", G_CALLBACK(on_info_machina), NULL);

    exit_button = gtk_button_new_with_label("Exit");
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_callback), NULL);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), exit_button);

    compile_button = gtk_button_new_with_label("Compile");
    g_signal_connect(compile_button, "clicked", G_CALLBACK(on_compile), NULL);

    send_file_button = gtk_button_new_with_label("Select file");

    bottom_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(bottom_hbox), compile_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(bottom_hbox), send_file_button, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(vbox), bottom_hbox, FALSE, FALSE, 0);

    paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), paned, TRUE, TRUE, 0);

    GtkWidget *input_frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(input_frame), GTK_SHADOW_ETCHED_IN);
    gtk_paned_pack1(GTK_PANED(paned), input_frame, TRUE, FALSE);

    input_window = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *input_text = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(input_window), input_text);
    gtk_container_add(GTK_CONTAINER(input_frame), input_window);

    GtkWidget *output_frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(output_frame), GTK_SHADOW_ETCHED_IN);
    gtk_paned_pack2(GTK_PANED(paned), output_frame, TRUE, FALSE);

    output_window = gtk_scrolled_window_new(NULL, NULL);

    gtk_text_view_set_buffer(GTK_TEXT_VIEW(output_text), output_buffer);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output_text), FALSE);
    gtk_container_add(GTK_CONTAINER(output_window), output_text);
    gtk_container_add(GTK_CONTAINER(output_frame), output_window);

    GdkRGBA input_color, output_color;
    gdk_rgba_parse(&input_color, "#333333");
    gdk_rgba_parse(&output_color, "#444444");
    gtk_widget_override_background_color(input_text, GTK_STATE_FLAG_NORMAL, &input_color);
    gtk_widget_override_background_color(output_text, GTK_STATE_FLAG_NORMAL, &output_color);

    g_signal_connect(launch_program_button, "clicked", G_CALLBACK(on_launch_program), output_buffer);
    g_signal_connect(status_machina_button, "clicked", G_CALLBACK(on_status_machina), NULL);
    g_signal_connect(info_machina_button, "clicked", G_CALLBACK(on_info_machina), NULL);
    g_signal_connect(compile_button, "clicked", G_CALLBACK(on_compile), NULL);

    g_signal_connect(send_file_button, "clicked", G_CALLBACK(on_send_file_open_window), output_text);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

