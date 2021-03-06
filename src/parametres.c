/* ************************************************************************** */
/*                                                                            */
/*     Copyright (C)    2000-2008 Cédric Auger (cedric@grisbi.org)            */
/*          2003-2009 Benjamin Drieu (bdrieu@april.org)                       */
/*            2008-2013 Pierre Biava (grisbi@pierre.biava.name)               */
/*          https://www.grisbi.org/                                            */
/*                                                                            */
/*  This program is free software; you can redistribute it and/or modify      */
/*  it under the terms of the GNU General Public License as published by      */
/*  the Free Software Foundation; either version 2 of the License, or         */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program; if not, write to the Free Software               */
/*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                            */
/* ************************************************************************** */

/**
 * \file parametres.c
 * we find here the configuration dialog
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "include.h"
#include <glib/gi18n.h>

/*START_INCLUDE*/
#include "parametres.h"
#include "accueil.h"
#include "affichage_liste.h"
#include "bet_config.h"
#include "bet_data.h"
#include "bet_finance_ui.h"
#include "categories_onglet.h"
#include "dialog.h"
#include "grisbi_app.h"
#include "grisbi_settings.h"
#include "gsb_automem.h"
#include "gsb_bank.h"
#include "gsb_currency_config.h"
#include "gsb_currency_link_config.h"
#include "gsb_data_account.h"
#include "gsb_data_partial_balance.h"
#include "gsb_data_payee.h"
#include "gsb_dirs.h"
#include "gsb_file.h"
#include "gsb_form.h"
#include "gsb_form_config.h"
#include "gsb_fyear_config.h"
#include "gsb_locale.h"
#include "gsb_payment_method_config.h"
#include "gsb_real.h"
#include "gsb_reconcile_config.h"
#include "gsb_reconcile_sort_config.h"
#include "gsb_regex.h"
#include "gsb_scheduler_list.h"
#include "gsb_transactions_list.h"
#include "import.h"
#include "imputation_budgetaire.h"
#include "menu.h"
#include "navigation.h"
#include "structures.h"
#include "tiers_onglet.h"
#include "traitement_variables.h"
#include "transaction_list.h"
#include "utils.h"
#include "utils_buttons.h"
#include "utils_dates.h"
#include "utils_files.h"
#include "utils_prefs.h"
#include "utils_real.h"
#include "utils_str.h"
#include "erreur.h"
/*END_INCLUDE*/

/*START_STATIC*/
static gboolean gsb_gui_delete_msg_toggled ( GtkCellRendererToggle *cell, gchar *path_str,
                        GtkTreeModel * model );
static gboolean gsb_gui_messages_toggled ( GtkCellRendererToggle *cell, gchar *path_str,
                        GtkTreeModel * model );
static void gsb_localisation_decimal_point_changed ( GtkComboBoxText *widget, gpointer user_data );
static gboolean gsb_localisation_format_date_toggle ( GtkToggleButton *togglebutton,
                        GdkEventButton *event,
                        gpointer user_data);
static void gsb_localisation_thousands_sep_changed ( GtkComboBoxText *widget, gpointer user_data );
static void gsb_localisation_update_affichage ( gint type_maj );
/*END_STATIC*/

GtkWidget *fenetre_preferences = NULL;


/*START_EXTERN*/
extern struct ConditionalMessage delete_msg[];
extern struct ConditionalMessage messages[];
/*END_EXTERN*/

/******************************************************************************/
/* Private functions                                                          */
/******************************************************************************/
/* ************************************************************************************************************** */
/* callback appelé quand on sélectionne un membre de la liste */
/* ************************************************************************************************************** */
/**
 * Creates the "Warning & Messages" tab.
 *
 * \returns A newly allocated vbox
 */
GtkWidget *onglet_messages_and_warnings ( void )
{
    GtkWidget *vbox_pref, *paddingbox, *tip_checkbox, *tree_view, *sw;
    GtkWidget *paddinggrid;
    GtkTreeModel * model;
    GtkCellRenderer * cell;
    GtkTreeViewColumn * column;
    gchar *tmpstr;
	const gchar *filename;
    int i;
	gboolean is_loading;

	vbox_pref = new_vbox_with_title_and_icon ( _("Messages & warnings"), "gsb-warnings-32.png" );
	is_loading = grisbi_win_file_is_loading ();

    /* Tip of the day */
    paddingbox = new_paddingbox_with_title (vbox_pref, FALSE, _("Tip of the day"));

    /* Display or not tips */
    tip_checkbox = gsb_automem_checkbutton_blue_new ( _("Display tip of the day"), &(conf.show_tip), NULL, NULL );
    gtk_box_pack_start ( GTK_BOX ( paddingbox ), tip_checkbox, FALSE, FALSE, 0 );

    /* Warnings */
    paddinggrid = utils_prefs_paddinggrid_new_with_title (vbox_pref,
                                                          _("Display following warnings messages"));

    sw = utils_prefs_scrolled_window_new (NULL, GTK_SHADOW_IN, SW_COEFF_UTIL_PG, 400);
    gtk_grid_attach (GTK_GRID (paddinggrid), sw, 0, 0, 1, 1);

    /* create the model */
    model = GTK_TREE_MODEL(gtk_tree_store_new (5, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_RGBA));

    /* create the treeview */
    tree_view = gtk_tree_view_new();
	gtk_widget_set_name (tree_view, "tree_view");
    gtk_tree_view_set_model ( GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model) );
    g_object_unref (G_OBJECT(model));
    gtk_container_add (GTK_CONTAINER (sw), tree_view);

    cell = gtk_cell_renderer_toggle_new ();
    column = gtk_tree_view_column_new_with_attributes ("", cell, "active", 0, "cell-background-rgba", 4, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));
    g_signal_connect (cell, "toggled", G_CALLBACK (gsb_gui_messages_toggled), model);

    cell = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Message"),
													   cell,
													   "text", 1,
													   "foreground", 3,
													   "cell-background-rgba", 4,
													   NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));

	if (is_loading)
	{
		/* on récupère le nom du fichier */
		filename = grisbi_win_get_filename (NULL);

		/* remplit le modèle */
		for  ( i = 0; messages[i].name; i++ )
		{
			GtkTreeIter iter;

			if ( g_utf8_collate ( messages[i].name, "account-already-opened" ) == 0 )
				tmpstr = g_strdup_printf ( _(messages[i] . hint),
							g_path_get_basename (filename) );
			else if ( g_utf8_collate ( messages[i].name, "development-version" ) == 0 )
				tmpstr = g_strdup_printf ( _(messages[i] . hint), VERSION );
			else
				tmpstr = g_strdup ( _(messages[i] . hint) );

			gtk_tree_store_append (GTK_TREE_STORE (model), &iter, NULL);
			gtk_tree_store_set (GTK_TREE_STORE (model),
								&iter,
								0, !messages[i] . hidden,
								1, tmpstr,
								2, i,
								3, "#00000000ffff",
								-1);

			g_free ( tmpstr );
		}
		/* set column color */
		utils_set_tree_store_background_color (tree_view, 4);
	}
    /* Show everything */
    gtk_widget_show_all ( vbox_pref );

    if ( !gsb_data_account_get_accounts_amount () )
    {
        gtk_widget_set_sensitive (paddinggrid, FALSE);
    }

    return ( vbox_pref );
}


/**
 * Creates the "Delete messages" tab.
 *
 * \returns A newly allocated vbox
 */
GtkWidget *onglet_delete_messages ( void )
{
    GtkWidget *vbox_pref, *tree_view, *sw;
    GtkWidget *paddinggrid;
    GtkTreeModel * model;
    GtkCellRenderer * cell;
    GtkTreeViewColumn * column;
    gchar *tmpstr;
    int i;

    vbox_pref = new_vbox_with_title_and_icon ( _("Messages before deleting"), "gtk-corbeille-32.png" );

    /* Delete messages */
    paddinggrid = utils_prefs_paddinggrid_new_with_title (vbox_pref, _("Display following messages") );

    sw = utils_prefs_scrolled_window_new (NULL, GTK_SHADOW_IN, SW_COEFF_UTIL_PG, 450);
	gtk_widget_set_vexpand (paddinggrid, TRUE);
    gtk_grid_attach (GTK_GRID (paddinggrid), sw, 0, 0, 1, 1);

    model = GTK_TREE_MODEL(gtk_tree_store_new (5, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_RGBA));

    tree_view = gtk_tree_view_new();
	gtk_widget_set_name (tree_view, "tree_view");
    gtk_tree_view_set_model ( GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model) );
    g_object_unref (G_OBJECT(model));
    gtk_container_add (GTK_CONTAINER (sw), tree_view);

    cell = gtk_cell_renderer_toggle_new ();
    column = gtk_tree_view_column_new_with_attributes ("", cell, "active", 0, "cell-background-rgba", 4, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));
    g_signal_connect (cell,
                        "toggled",
                        G_CALLBACK (gsb_gui_delete_msg_toggled),
                        model);

    cell = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Message"),
													   cell,
													   "text", 1,
													   "foreground", 3,
													   "cell-background-rgba", 4,
													   NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));

    for  ( i = 0; delete_msg[i].name; i++ )
    {
        GtkTreeIter iter;

        tmpstr = g_strdup ( _(delete_msg[i] . hint) );

        gtk_tree_store_append (GTK_TREE_STORE (model), &iter, NULL);
        gtk_tree_store_set (GTK_TREE_STORE (model),
							&iter,
							0, !delete_msg[i] . hidden,
							1, tmpstr,
							2, i,
							3, "#00000000ffff",
							-1);

        g_free ( tmpstr );
    }
	/* set column color */
	utils_set_tree_store_background_color (tree_view, 4);

    /* Show everything */
    gtk_widget_show_all ( vbox_pref );

    if ( !gsb_data_account_get_accounts_amount () )
    {
        gtk_widget_set_sensitive ( vbox_pref, FALSE );
    }

    return ( vbox_pref );
}


/**
 *
 *
 */
gboolean gsb_gui_messages_toggled ( GtkCellRendererToggle *cell, gchar *path_str,
                        GtkTreeModel * model )
{
    GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
    GtkTreeIter iter;
    gint position;

    /* Get toggled iter */
    gtk_tree_model_get_iter (GTK_TREE_MODEL(model), &iter, path);
    gtk_tree_model_get (GTK_TREE_MODEL(model), &iter, 2, &position, -1);

    messages[position] . hidden = !messages[position] . hidden;

    /* Set new value */
    gtk_tree_store_set (GTK_TREE_STORE (model), &iter, 0, ! messages[position] . hidden, -1);

    return TRUE;
}


/**
 *
 *
 */
gboolean gsb_gui_delete_msg_toggled ( GtkCellRendererToggle *cell, gchar *path_str,
                        GtkTreeModel * model )
{
    GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
    GtkTreeIter iter;
    gint position;

    /* Get toggled iter */
    gtk_tree_model_get_iter (GTK_TREE_MODEL(model), &iter, path);
    gtk_tree_model_get (GTK_TREE_MODEL(model), &iter, 2, &position, -1);

    delete_msg[position].hidden = !delete_msg[position].hidden;
    if ( delete_msg[position].hidden == 1 )
        delete_msg[position].default_answer = 1;

    /* Set new value */
    gtk_tree_store_set (GTK_TREE_STORE (model), &iter, 0, !delete_msg[position].hidden, -1);

    return TRUE;
}


/**
 *
 *
 *
 *
 * */
GtkWidget *gsb_config_date_format_chosen ( GtkWidget *parent, gint sens )
{
    GtkWidget *hbox, *paddingbox;
    GtkWidget *button_1, *button_2, *button_3, *button_4;
    gchar *format_date;

    button_1 = gtk_radio_button_new_with_label ( NULL, "dd/mm/yyyy" );
    format_date = g_strdup ( "%d/%m/%Y" );
    g_object_set_data_full ( G_OBJECT ( button_1 ),
                        "pointer",
                        format_date,
                        g_free );

    button_2 = gtk_radio_button_new_with_label ( gtk_radio_button_get_group (
                        GTK_RADIO_BUTTON ( button_1 ) ),
						"mm/dd/yyyy" );
    format_date = g_strdup ( "%m/%d/%Y" );
    g_object_set_data_full ( G_OBJECT ( button_2 ),
                        "pointer",
                        format_date,
                        g_free );

    button_3 = gtk_radio_button_new_with_label ( gtk_radio_button_get_group (
                        GTK_RADIO_BUTTON ( button_1 ) ),
                        "dd.mm.yyyy" );
    format_date = g_strdup ( "%d.%m.%Y" );
    g_object_set_data_full ( G_OBJECT ( button_3 ),
                        "pointer",
                        format_date,
                        g_free );

	button_4 = gtk_radio_button_new_with_label ( gtk_radio_button_get_group (
                        GTK_RADIO_BUTTON ( button_1 ) ),
                        "yyyy-mm-dd." );
    format_date = g_strdup ( "%Y-%m-%d" );
    g_object_set_data_full ( G_OBJECT ( button_4 ),
                        "pointer",
                        format_date,
                        g_free );

    if ( sens == GTK_ORIENTATION_VERTICAL )
    {
        paddingbox = new_paddingbox_with_title ( parent, FALSE, _("Choose the date format") );
        gtk_box_pack_start ( GTK_BOX ( paddingbox ), button_1, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( paddingbox ), button_2, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( paddingbox ), button_3, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( paddingbox ), button_4, FALSE, FALSE, 0 );
    }
    else
    {
        paddingbox = new_paddingbox_with_title ( parent, FALSE, _("Choose the date format") );
        hbox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, MARGIN_PADDING_BOX );
        gtk_box_pack_start ( GTK_BOX ( paddingbox ), hbox, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( hbox ), button_1, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( hbox ), button_2, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( hbox ), button_3, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( hbox ), button_4, FALSE, FALSE, 0);
    }

    format_date = gsb_date_get_format_date ( );
    if ( format_date )
    {
        if ( strcmp ( format_date, "%m/%d/%Y" ) == 0 )
            gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button_2 ), TRUE );
        else if ( strcmp ( format_date, "%d.%m.%Y" ) == 0 )
            gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button_3 ), TRUE );
        else if ( strcmp ( format_date, "%Y-%m-%d" ) == 0 )
            gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( button_4 ), TRUE );

        g_free ( format_date );
    }

    g_signal_connect ( G_OBJECT ( button_1 ),
                        "button-release-event",
                        G_CALLBACK ( gsb_localisation_format_date_toggle ),
                        GINT_TO_POINTER ( sens ) );
    g_signal_connect ( G_OBJECT ( button_2 ),
                        "button-release-event",
                        G_CALLBACK ( gsb_localisation_format_date_toggle ),
                        GINT_TO_POINTER ( sens ) );
    g_signal_connect ( G_OBJECT ( button_3 ),
                        "button-release-event",
                        G_CALLBACK ( gsb_localisation_format_date_toggle ),
                        GINT_TO_POINTER ( sens ) );
    g_signal_connect ( G_OBJECT ( button_4 ),
                        "button-release-event",
                        G_CALLBACK ( gsb_localisation_format_date_toggle ),
                        GINT_TO_POINTER ( sens ) );
    return paddingbox;
}


/**
 *
 *
 *
 *
 * */
gboolean gsb_localisation_format_date_toggle ( GtkToggleButton *togglebutton,
                        GdkEventButton *event,
                        gpointer user_data)
{
    const gchar *format_date;
	GrisbiWinRun *w_run;

	w_run = grisbi_win_get_w_run ();

    format_date = g_object_get_data ( G_OBJECT ( togglebutton ), "pointer" );
    gsb_date_set_format_date ( format_date );
	gsb_regex_init_variables ();

	if (grisbi_win_file_is_loading () && !w_run->new_account_file)
		gsb_localisation_update_affichage ( 0 );

    return FALSE;
}


/**
 *
 *
 *
 *
 * */
GtkWidget *gsb_config_number_format_chosen ( GtkWidget *parent, gint sens )
{
    GtkWidget *hbox, *paddingbox, *label;
    GtkWidget *dec_hbox, *dec_sep, *thou_hbox, *thou_sep;
    GtkSizeGroup *size_group;
    gchar *mon_decimal_point;
    gchar *mon_thousands_sep;

    size_group = gtk_size_group_new ( GTK_SIZE_GROUP_HORIZONTAL );

    dec_hbox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
    label = gtk_label_new ( _("Decimal point: ") );
    utils_labels_set_alignment ( GTK_LABEL ( label ), MISC_LEFT, MISC_VERT_CENTER );
    gtk_size_group_add_widget ( GTK_SIZE_GROUP ( size_group ), label );
    gtk_box_pack_start ( GTK_BOX ( dec_hbox ), label, FALSE, FALSE, 0 );

    dec_sep = gtk_combo_box_text_new_with_entry ( );
    gtk_editable_set_editable ( GTK_EDITABLE ( gtk_bin_get_child ( GTK_BIN ( dec_sep ) ) ), FALSE );
    gtk_entry_set_width_chars ( GTK_ENTRY ( gtk_bin_get_child ( GTK_BIN ( dec_sep ) ) ), 5 );
    gtk_combo_box_text_append_text ( GTK_COMBO_BOX_TEXT ( dec_sep ), "." );
    gtk_combo_box_text_append_text ( GTK_COMBO_BOX_TEXT ( dec_sep ), "," );
    gtk_box_pack_start ( GTK_BOX ( dec_hbox ), dec_sep, FALSE, FALSE, 0 );

    thou_hbox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
    label = gtk_label_new (_("Thousands separator: ") );
    utils_labels_set_alignment ( GTK_LABEL ( label ), MISC_LEFT, MISC_VERT_CENTER );
    gtk_size_group_add_widget ( GTK_SIZE_GROUP ( size_group ), label );
    gtk_box_pack_start ( GTK_BOX ( thou_hbox ), label, FALSE, FALSE, 0 );

    thou_sep = gtk_combo_box_text_new_with_entry ( );
    gtk_editable_set_editable ( GTK_EDITABLE ( gtk_bin_get_child ( GTK_BIN ( thou_sep ) ) ), FALSE );
    gtk_entry_set_width_chars ( GTK_ENTRY ( gtk_bin_get_child ( GTK_BIN ( thou_sep ) ) ), 5 );
    gtk_combo_box_text_append_text ( GTK_COMBO_BOX_TEXT ( thou_sep ), "' '" );
    gtk_combo_box_text_append_text ( GTK_COMBO_BOX_TEXT ( thou_sep ), "." );
    gtk_combo_box_text_append_text ( GTK_COMBO_BOX_TEXT ( thou_sep ), "," );
    gtk_combo_box_text_append_text ( GTK_COMBO_BOX_TEXT ( thou_sep ), "''" );

    gtk_box_pack_start ( GTK_BOX ( thou_hbox ), thou_sep, FALSE, FALSE, 0 );

    if ( sens == GTK_ORIENTATION_VERTICAL )
    {
        paddingbox = new_paddingbox_with_title ( parent, FALSE, _("Choose the decimal and thousands separator") );
        gtk_box_pack_start ( GTK_BOX ( paddingbox ), dec_hbox, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( paddingbox ), thou_hbox, FALSE, FALSE, 0 );
    }
    else
    {
        paddingbox = new_paddingbox_with_title ( parent, FALSE, _("Decimal and thousands separator") );
        hbox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
        gtk_box_set_homogeneous ( GTK_BOX ( hbox ), TRUE );
        gtk_box_pack_start ( GTK_BOX ( paddingbox ), hbox, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( hbox ), dec_hbox, FALSE, FALSE, 0 );
        gtk_box_pack_start ( GTK_BOX ( hbox ), thou_hbox, FALSE, FALSE, 0 );
    }

    mon_decimal_point = gsb_locale_get_mon_decimal_point ( );
    if ( strcmp ( mon_decimal_point, "," ) == 0 )
        gtk_combo_box_set_active ( GTK_COMBO_BOX ( dec_sep ), 1 );
    else
        gtk_combo_box_set_active ( GTK_COMBO_BOX ( dec_sep ), 0 );
    g_free ( mon_decimal_point );

    mon_thousands_sep = gsb_locale_get_mon_thousands_sep ( );
    if ( mon_thousands_sep == NULL )
        gtk_combo_box_set_active ( GTK_COMBO_BOX ( thou_sep ), 3 );
    else if ( strcmp ( mon_thousands_sep, "." ) == 0 )
        gtk_combo_box_set_active ( GTK_COMBO_BOX ( thou_sep ), 1 );
    else if ( strcmp ( mon_thousands_sep, "," ) == 0 )
        gtk_combo_box_set_active ( GTK_COMBO_BOX ( thou_sep ), 2 );
    else
        gtk_combo_box_set_active ( GTK_COMBO_BOX ( thou_sep ), 0 );

    if ( mon_thousands_sep )
        g_free ( mon_thousands_sep );

    g_object_set_data ( G_OBJECT ( dec_sep ), "separator", thou_sep );
    g_object_set_data ( G_OBJECT ( thou_sep ), "separator", dec_sep );

    g_signal_connect ( G_OBJECT ( dec_sep ),
                        "changed",
                        G_CALLBACK ( gsb_localisation_decimal_point_changed ),
                        GINT_TO_POINTER ( sens ) );
    g_signal_connect ( G_OBJECT ( thou_sep ),
                        "changed",
                        G_CALLBACK ( gsb_localisation_thousands_sep_changed ),
                        GINT_TO_POINTER ( sens ) );

    return paddingbox;
}


/**
 *
 *
 *
 *
 * */
void gsb_localisation_decimal_point_changed ( GtkComboBoxText *widget, gpointer user_data )
{
    GtkWidget *combo_box;
    GtkWidget *entry;
    gchar *str_capital;
    const gchar *text;

    text = gtk_combo_box_text_get_active_text ( widget );
    combo_box = g_object_get_data ( G_OBJECT ( widget ), "separator" );

    if ( g_strcmp0 ( text, "," ) == 0 )
    {
        gsb_locale_set_mon_decimal_point ( "," );

        if ( g_strcmp0 ( gtk_combo_box_text_get_active_text ( GTK_COMBO_BOX_TEXT ( combo_box ) ), "," ) == 0 )
        {
            gsb_locale_set_mon_thousands_sep ( " " );
            gtk_combo_box_set_active ( GTK_COMBO_BOX ( combo_box ), 0 );
        }
    }
    else
    {
        gsb_locale_set_mon_decimal_point ( "." );
        if ( g_strcmp0 ( gtk_combo_box_text_get_active_text ( GTK_COMBO_BOX_TEXT ( combo_box ) ), "." ) == 0 )
        {
            gsb_locale_set_mon_thousands_sep ( "," );
            gtk_combo_box_set_active ( GTK_COMBO_BOX ( combo_box ), 2 );
        }
    }

    /* on sort si on est dans l'assistant de sélection d'un nouveau fichier */
    if ( GPOINTER_TO_INT ( user_data ) == GTK_ORIENTATION_HORIZONTAL )
        return;

    /* reset capital */
    entry = bet_finance_ui_get_capital_entry ( );
    str_capital = utils_real_get_string_with_currency ( gsb_real_double_to_real (
                    etat.bet_capital ),
                    etat.bet_currency,
                    FALSE );

    gtk_entry_set_text ( GTK_ENTRY ( entry ), str_capital );
    g_free ( str_capital );

    gsb_localisation_update_affichage ( 1 );
}


/**
 *
 *
 *
 *
 * */
void gsb_localisation_thousands_sep_changed ( GtkComboBoxText *widget, gpointer user_data )
{
    GtkWidget *combo_box;
    GtkWidget *entry;
    gchar *str_capital;
    const gchar *text;

    text = gtk_combo_box_text_get_active_text ( widget );
    combo_box = g_object_get_data ( G_OBJECT ( widget ), "separator" );

    if ( g_strcmp0 ( text, "' '" ) == 0 )
    {
        gsb_locale_set_mon_thousands_sep ( " " );
    }
    else if ( g_strcmp0 ( text, "." ) == 0 )
    {

        gsb_locale_set_mon_thousands_sep ( "." );
        if ( g_strcmp0 ( gtk_combo_box_text_get_active_text ( GTK_COMBO_BOX_TEXT ( combo_box ) ), "." ) == 0 )
        {
            gsb_locale_set_mon_decimal_point ( "," );
            gtk_combo_box_set_active ( GTK_COMBO_BOX ( combo_box ), 1 );
        }
    }
    else if ( g_strcmp0 ( text, "," ) == 0 )
    {

        gsb_locale_set_mon_thousands_sep ( "," );
        if ( g_strcmp0 ( gtk_combo_box_text_get_active_text ( GTK_COMBO_BOX_TEXT ( combo_box ) ), "," ) == 0 )
        {
            gsb_locale_set_mon_decimal_point ( "." );
            gtk_combo_box_set_active ( GTK_COMBO_BOX ( combo_box ), 0 );
        }
    }
    else
        gsb_locale_set_mon_thousands_sep ( NULL );

    /* on sort si on est dans l'assistant de sélection d'un nouveau fichier */
    if ( GPOINTER_TO_INT ( user_data ) == GTK_ORIENTATION_HORIZONTAL )
        return;

    /* reset capital */
    entry = bet_finance_ui_get_capital_entry ( );
    str_capital = utils_real_get_string_with_currency ( gsb_real_double_to_real (
                    etat.bet_capital ),
                    etat.bet_currency,
                    FALSE );

    gtk_entry_set_text ( GTK_ENTRY ( entry ), str_capital );
    g_free ( str_capital );

    gsb_localisation_update_affichage ( 1 );
}


/**
 * met à jour l'affichage suite à modification des données de localisation
 *
 *\param type_maj 0 = ELEMENT_DATE 1 =  ELEMENT_CREDIT && ELEMENT_DEBIT
 *
 * */
void gsb_localisation_update_affichage ( gint type_maj )
{
    gint current_page;

    current_page = gsb_gui_navigation_get_current_page ( );

    /* update home page */
    if ( current_page == GSB_HOME_PAGE )
        mise_a_jour_accueil ( TRUE );
    else
        run.mise_a_jour_liste_comptes_accueil = TRUE;

    /* update sheduled liste */
    gsb_scheduler_list_fill_list ( gsb_scheduler_list_get_tree_view ( ) );
    gsb_scheduler_list_set_background_color ( gsb_scheduler_list_get_tree_view ( ) );
    if ( current_page == GSB_SCHEDULER_PAGE )
        gsb_scheduler_list_select (-1);

    /* update transaction liste */
    if ( type_maj == 0 )
    {
        transaction_list_update_element ( ELEMENT_DATE );
    }
    else
    {
        transaction_list_update_element ( ELEMENT_CREDIT );
        transaction_list_update_element ( ELEMENT_DEBIT );
        gsb_transactions_list_update_tree_view ( gsb_gui_navigation_get_current_account ( ), FALSE );
    }

    /* update home page */
    if ( current_page == GSB_ACCOUNT_PAGE )
    {
        gint account_number;
        gint account_current_page;
        KindAccount kind;

        account_number = gsb_gui_navigation_get_current_account ( );
        account_current_page = gtk_notebook_get_current_page ( GTK_NOTEBOOK ( grisbi_win_get_account_page () ) );

        kind = gsb_data_account_get_kind ( account_number );
        switch ( kind )
        {
            case GSB_TYPE_BALANCE:
                break;
            case GSB_TYPE_BANK:
            case GSB_TYPE_CASH:
                if ( account_current_page == 1 || account_current_page == 2 )
                {
                    gsb_data_account_set_bet_maj ( account_number, BET_MAJ_ALL );
                    bet_data_update_bet_module ( account_number, -1 );
                }
                break;
            case GSB_TYPE_LIABILITIES:
                if ( account_current_page == 3 )
                    bet_finance_update_amortization_tab ( account_number );
                break;
            case GSB_TYPE_ASSET:
                break;
        }
    }

    /* update payees, categories and budgetary lines */
    if ( current_page == GSB_PAYEES_PAGE )
        payees_fill_list ( );
    else if ( current_page == GSB_CATEGORIES_PAGE )
        categories_fill_list ( );
    else if ( current_page == GSB_BUDGETARY_LINES_PAGE )
        budgetary_lines_fill_list ( );

    /* update simulator page */
    if ( current_page == GSB_SIMULATOR_PAGE )
        bet_finance_ui_switch_simulator_page ( );

}


/**
 * Warns that there is no coming back if password is forgotten when
 * encryption is activated.
 *
 * \param checkbox  Checkbox that triggered event.
 * \param data      Unused.
 *
 * \return          FALSE
 */
gboolean gsb_gui_encryption_toggled ( GtkWidget * checkbox, gpointer data )
{
#ifdef HAVE_SSL
	GrisbiWinRun *w_run;

	w_run = grisbi_win_get_w_run ();
    if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON (checkbox)))
    {
        dialog_message ( "encryption-is-irreversible" );
        w_run->new_crypted_file = TRUE;
    }
	else
		w_run->new_crypted_file = FALSE;
#else
	gchar *text = _("This build of Grisbi does not support encryption.\n"
					"Please recompile Grisbi with OpenSSL encryption enabled.");
	gchar *hint;

	hint = g_strdup_printf ( _("Could not encrypt file"));
	dialogue_error_hint ( text, hint );
	g_signal_handlers_block_by_func (G_OBJECT (checkbox),
									 G_CALLBACK (gsb_gui_encryption_toggled),
									 data);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), FALSE);

	g_free ( hint );
	g_signal_handlers_unblock_by_func (G_OBJECT (checkbox),
									   G_CALLBACK (gsb_gui_encryption_toggled),
									   data);

#endif
    return FALSE;
}

/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
