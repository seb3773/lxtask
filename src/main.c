/* $Id: main.c 2350 2007-01-13 10:12:31Z nick $
 *
 * Copyright (c) 2006 Johannes Zellner, <webmaster@nebulon.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glib.h>
#include <signal.h>
#include <unistd.h>



#include <gdk/gdk.h>


#ifdef ENABLE_NLS
#include <libintl.h>
#endif

#include "types.h"
#include "interface.h"
#include "functions.h"

GtkStatusIcon *global_status_icon;

GtkWidget *main_window;

GArray *task_array;
gint tasks;
uid_t own_uid;

gchar *config_file;

gboolean show_user_tasks;
gboolean show_root_tasks;
gboolean show_other_tasks;
gboolean show_full_path;

gboolean show_cached_as_free; /* Show memory used Cache as free memory */

gboolean full_view;


gint win_width;
gint win_height;
gint refresh_interval;
guint rID;

int page_size;


void on_menu_item_clicked(GtkMenuItem *menu_item, gpointer user_data) {
    g_print("Menu item clicked\n");
}
void on_status_icon_popup_menu(GtkStatusIcon *status_icon, gpointer user_data) {
    GtkWidget *menu;
    GtkWidget *menu_item;

    // Créer le menu
    menu = gtk_menu_new();

    // Ajouter des éléments au menu
    menu_item = gtk_menu_item_new_with_label("Item 1");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    g_signal_connect(menu_item, "activate", G_CALLBACK(on_menu_item_clicked), NULL);

    menu_item = gtk_menu_item_new_with_label("Quit");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    g_signal_connect(menu_item, "activate", G_CALLBACK(on_menu_item_clicked), NULL);

    // Afficher le menu
    gtk_widget_show_all(menu);

    // Afficher le menu à la position de l'icône de statut
    gtk_menu_popup(GTK_MENU(menu), NULL, NULL, gtk_status_icon_position_menu, status_icon, 0, gtk_get_current_event_time());
}




int main (int argc, char *argv[])
{

#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif

#if !GTK_CHECK_VERSION(3,0,0)
    gtk_set_locale ();
#endif
    gtk_init (&argc, &argv);

    page_size=sysconf(_SC_PAGESIZE)>>10;
    own_uid = getuid();

    config_file = g_build_filename(g_get_user_config_dir(), "lxtask.conf", NULL);
    load_config();

    task_array = g_array_new (FALSE, FALSE, sizeof (struct task));
    tasks = 0;

    main_window = create_main_window ();
    gtk_widget_show (main_window);

// Créer une icône de statut
//    GtkStatusIcon *status_icon = gtk_status_icon_new_from_stock(GTK_STOCK_HOME);
    GtkWidget *image = gtk_image_new_from_file("/usr/share/lxtask/icons/cpu0.png");

    // Créer la barre d'état
    global_status_icon = gtk_status_icon_new();

// global_status_icon = status_icon;
g_signal_connect(G_OBJECT(global_status_icon), "activate", G_CALLBACK(on_status_icon_popup_menu), NULL);


    if(!refresh_task_list())
        return 0;

    rID = g_timeout_add_seconds(refresh_interval, (gpointer) refresh_task_list, NULL);





//g_signal_connect(G_OBJECT(status_icon), "activate", G_CALLBACK(on_button_quit_clicked), NULL);


    gtk_main ();

    return 0;
}



