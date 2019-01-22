#include <gtk/gtk.h>
#include <string.h>

enum {
  COL_NAME = 0,
  COL_PATH,
  NUM_COLS
} ;

GtkWidget       *list = NULL;
GtkListStore    *store = NULL;
GtkWidget       *entry_search = NULL;
GtkWidget       *entry_replace = NULL;
GtkWidget       *checkbox = NULL;
GRegex          *rx_name = NULL;
GRegex          *rx_path = NULL;
GRegex          *rx_replace = NULL;
GRegex          *rx_split = NULL;

int g_rename(const gchar *oldfilename, const gchar *newfilename);
void destroy(void);

/* ************************************************************************ *
 * UTILITY FUNCTIONS
 * ************************************************************************ */
gchar *replace_str(gchar *str, gchar *orig, gchar *rep)
{
    static gchar buffer[4096];
    gchar *p;

    strncpy(buffer, str, strlen(str));
    buffer[strlen(str)] = '\0';

    while((p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    {
        strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
        buffer[p-str] = '\0';

        sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));
        str = buffer;
    }
    return str;
}

/* ************************************************************************ *
 * POPULATING THE LIST (DRAG-N-DROP)
 * ************************************************************************ */
void view_onDragDataReceived(GtkWidget *wgt, GdkDragContext *context, int x, int y,
                        GtkSelectionData *seldata, guint info, guint time)
{
    GtkTreeIter   iter;
    gchar        **strings  = g_regex_split(rx_split, (gchar*)seldata->data, 0);
    gchar        *input     = NULL;
    int          index      = 0;

    while (strings[index] != NULL && strcmp(strings[index], "") != 0)
    {
        input = strings[index];
        gtk_list_store_append(GTK_LIST_STORE(store), &iter);
        gtk_list_store_set(GTK_LIST_STORE(store), &iter, COL_NAME,
            g_regex_replace(rx_name, input, strlen(input), 0, "\\1", 0, NULL), -1);
#ifdef WIN_32
        gtk_list_store_set(GTK_LIST_STORE(store), &iter, COL_PATH,
            replace_str(g_regex_replace(rx_path,
                                        input,
                                        strlen(input),
                                        0, "\\1", 0,
                                        NULL), "%20", " "), -1);
#else
        gtk_list_store_set(GTK_LIST_STORE(store), &iter, COL_PATH,
            g_regex_replace(rx_path, input, strlen(input), 0, "\\1", 0, NULL), -1);
#endif
        ++index;
    }
}

/* ************************************************************************ *
 * REMOVING ITEMS FROM THE LIST
 * ************************************************************************ */
gboolean get_reference_foreach (GtkTreeModel *model,
                GtkTreePath  *path,
                GtkTreeIter  *iter,
                GList       **rowref_list)
{
    *rowref_list = g_list_append(*rowref_list, gtk_tree_row_reference_new(model, path));
    return TRUE;
}

void clear_selected()
{
    GList *selected = NULL; /* references of selected items */
    GList *node;

    gtk_tree_selection_selected_foreach(gtk_tree_view_get_selection(GTK_TREE_VIEW(list)),
                                        (GtkTreeSelectionForeachFunc) get_reference_foreach,
                                        &selected);

    for ( node = selected;  node != NULL;  node = node->next )
    {
        GtkTreePath *path = gtk_tree_row_reference_get_path((GtkTreeRowReference*)node->data);

        if (path)
        {
            GtkTreeIter  iter;

            if (gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path))
            { gtk_list_store_remove(store, &iter); }

            gtk_tree_path_free(path);
        }
    }

    g_list_foreach(selected, (GFunc) gtk_tree_row_reference_free, NULL);
    g_list_free(selected);
    g_list_free(node);
    gtk_tree_view_columns_autosize(GTK_TREE_VIEW(list));
}

void clear_list()
{
    gtk_tree_selection_select_all(gtk_tree_view_get_selection(GTK_TREE_VIEW(list)));
    clear_selected();
}

/* ************************************************************************ *
 * PERFORMING THE SEARCH AND REPLACE
 * ************************************************************************ */
gboolean rename_foreach (GtkTreeModel *model,
                GtkTreePath  *path,
                GtkTreeIter  *iter)
{
    gchar   *_name, *_path, *_name2;

    gtk_tree_model_get (model, iter, COL_NAME, &_name, -1);
    gtk_tree_model_get (model, iter, COL_PATH, &_path, -1);

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox))) {
        _name2 = g_regex_replace(rx_replace, _name, strlen(_name), 0,
                             gtk_entry_get_text(GTK_ENTRY(entry_replace)), 0, NULL);
    } else {
        _name2 = replace_str(_name, (gchar*) gtk_entry_get_text(GTK_ENTRY(entry_search)),
                             (gchar*) gtk_entry_get_text(GTK_ENTRY(entry_replace)));
    }

    if (g_rename(g_strconcat(_path, _name, NULL), g_strconcat(_path, _name2, NULL)) == 0)
    { gtk_list_store_set (store, iter, COL_NAME, _name2, -1); }

    return FALSE;
}

void search_and_replace(void)
{
    rx_replace = g_regex_new(gtk_entry_get_text(GTK_ENTRY(entry_search)), G_REGEX_OPTIMIZE, 0, NULL);

    gtk_tree_model_foreach(GTK_TREE_MODEL(store),
                           (GtkTreeModelForeachFunc) rename_foreach,
                           NULL);

    gtk_tree_view_columns_autosize(GTK_TREE_VIEW(list));
}

/* ************************************************************************ *
 * KEY PRESS HANDLERS
 * ************************************************************************ */
 gboolean key_press_event  (GtkWidget   *widget,
                               GdkEventKey *event,
                               gpointer     user_data)
{
    if(event->keyval == 65307)                                      /* Escape Key */
    { destroy(); }
    else if((GTK_WIDGET_HAS_FOCUS(entry_search) || GTK_WIDGET_HAS_FOCUS(entry_replace))
        && (event->keyval == 65293 || event->keyval == 65421))      /* Enter Keys */
    { search_and_replace(); }
    else if(GTK_WIDGET_HAS_FOCUS(list) && event->keyval == 65535)   /* Delete Key */
    { clear_selected(); }

    return FALSE;
}

/* ************************************************************************ *
 * DESTRUCTOR
 * ************************************************************************ */
void destroy(void)
{
    g_regex_unref(rx_name);
    g_regex_unref(rx_path);
    g_regex_unref(rx_split);
    if (rx_replace != NULL)
        g_regex_unref(rx_replace);
    gtk_main_quit();
}

/* ************************************************************************ *
 * WIDGETS & BASE SETUP
 * ************************************************************************ */
static GtkWidget *create_view_and_model (void)
{
    GtkCellRenderer     *renderer;
    GtkWidget           *view;

    view = gtk_tree_view_new ();

    /* --- Column #1 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                                -1,
                                                "File Name",
                                                renderer,
                                                "text", COL_NAME,
                                                NULL);

    /* --- Column #2 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                                -1,
                                                "Path",
                                                renderer,
                                                "text", COL_PATH,
                                                NULL);

    store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
    gtk_tree_view_set_model (GTK_TREE_VIEW (view), GTK_TREE_MODEL (store));

    /* Make tree view a destination for Drag'n'Drop */
    if (1)
    {
        enum {
            TARGET_STRING,
            TARGET_URL
        };

        static GtkTargetEntry targetentries[] =
        {
            { "STRING",        0, TARGET_STRING },
            { "text/plain",    0, TARGET_STRING },
            { "text/uri-list", 0, TARGET_URL },
        };

        gtk_drag_dest_set(view, GTK_DEST_DEFAULT_ALL, targetentries, 3,
                          GDK_ACTION_COPY|GDK_ACTION_MOVE|GDK_ACTION_LINK);

        g_signal_connect(view, "drag_data_received",
                         G_CALLBACK(view_onDragDataReceived), NULL);
    }

    return view;
}

int main (int argc, char *argv[])
{
    GtkWidget *win = NULL;
    GtkWidget *vbox = NULL;
    GtkWidget *hbox = NULL;
    GtkWidget *table = NULL;
    GtkWidget *scroll = NULL;
    GtkWidget *align = NULL;
    GtkWidget *label = NULL;
    GtkWidget *button = NULL;

    /* Initialize GTK+ */
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 500, 380);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_window_set_title (GTK_WINDOW (win), "Multi-File Renamer - v1.1.2"); /* Jul / 20 / 2008 */
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_NONE);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", destroy, NULL);
    gtk_signal_connect (GTK_OBJECT (win), "key_press_event",
                      GTK_SIGNAL_FUNC (key_press_event),
                      NULL);

    /* Form-wide Widgets */
    vbox = gtk_vbox_new (FALSE, 6);
    gtk_container_add (GTK_CONTAINER (win), vbox);

    /* First row of the Form */
    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    list = create_view_and_model();
    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(list)), GTK_SELECTION_MULTIPLE);
    gtk_container_add(GTK_CONTAINER(scroll), list);

    /* Second / Third rows of the Form */
    table = gtk_table_new(2, 2, FALSE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 6);
    gtk_table_set_col_spacings(GTK_TABLE(table), 6);
    gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, FALSE, 0);

    label = gtk_label_new("Search:");
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);
    entry_search = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), entry_search, 1, 2, 0, 1);

    label = gtk_label_new("Replace:");
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
    entry_replace = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), entry_replace, 1, 2, 1, 2);

    /* Fourth row of the Form */
    hbox = gtk_hbox_new(FALSE, 6);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    checkbox = gtk_check_button_new_with_label("Use Regex");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox), TRUE);
    gtk_box_pack_start(GTK_BOX(hbox), checkbox, FALSE, FALSE, 0);

    align = gtk_alignment_new(1, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(hbox), align, TRUE, TRUE, 0);

    button = gtk_button_new_with_label ("Clear List");
    g_signal_connect (button, "clicked", G_CALLBACK (clear_list), NULL);
    gtk_container_add(GTK_CONTAINER(align), button);

    button = gtk_button_new_with_label ("Search and Replace");
    g_signal_connect (button, "clicked", G_CALLBACK (search_and_replace), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    /* Complie Regex Patterns */
#ifdef WIN_32
    rx_name = g_regex_new(".*/([^/\r]+)\r?$", G_REGEX_OPTIMIZE, 0, NULL);
    rx_path = g_regex_new("file:///(.*?/)[^/]+$", G_REGEX_OPTIMIZE, 0, NULL);
#else
    rx_name = g_regex_new(".*/([^/]+)$", G_REGEX_OPTIMIZE, 0, NULL);
    rx_path = g_regex_new("file://(.*?/)[^/]+$", G_REGEX_OPTIMIZE, 0, NULL);
#endif
    rx_split = g_regex_new("\n", G_REGEX_OPTIMIZE, 0, NULL);

    /* Enter the main loop */
    gtk_widget_show_all (win);
    gtk_main ();

    return 0;
}
