/*  Fichier qui permet l'�quilibrage des comptes */
/*     equilibrage.c */


/*     Copyright (C) 2000-2003  C�dric Auger */
/* 			cedric@grisbi.org */
/* 			http://www.grisbi.org */

/*     This program is free software; you can redistribute it and/or modify */
/*     it under the terms of the GNU General Public License as published by */
/*     the Free Software Foundation; either version 2 of the License, or */
/*     (at your option) any later version. */

/*     This program is distributed in the hope that it will be useful, */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*     GNU General Public License for more details. */

/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software */
/*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */


#include "include.h"
#include "structures.h"
#include "variables-extern.c"
#include "en_tete.h"




/* ********************************************************************************************************** */
GtkWidget *creation_fenetre_equilibrage ( void )
{
  GtkWidget *fenetre_equilibrage;
  GtkWidget *label;
  GtkWidget *table;
  GtkWidget *hbox;
  GtkWidget *bouton;  
  GtkWidget *separateur;
  GtkTooltips *tips;


  /* la fenetre est une vbox */

  fenetre_equilibrage = gtk_vbox_new ( FALSE, 5 );
  gtk_container_set_border_width ( GTK_CONTAINER ( fenetre_equilibrage ),
				   10 );
  gtk_widget_show ( fenetre_equilibrage );


  /* on met le nom du compte � �quilibrer en haut */
 
  label_equilibrage_compte = gtk_label_new ( "" );
  gtk_label_set_justify ( GTK_LABEL (label_equilibrage_compte ),
			  GTK_JUSTIFY_CENTER);
  gtk_box_pack_start ( GTK_BOX ( fenetre_equilibrage ),
		       label_equilibrage_compte,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( label_equilibrage_compte );


  separateur = gtk_hseparator_new();
  gtk_box_pack_start ( GTK_BOX ( fenetre_equilibrage ),
		       separateur,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( separateur );


  /* on cr�e le tooltips */

  tips = gtk_tooltips_new ();

  /*   sous le nom, on met le no de rapprochement, c'est une entr�e car il est modifiable */

  hbox = gtk_hbox_new ( FALSE,
			5 );
  gtk_box_pack_start ( GTK_BOX ( fenetre_equilibrage ),
		       hbox,
		       FALSE,
		       FALSE,
		       10);
  gtk_widget_show ( hbox );

  label = gtk_label_new ( COLON(_("Reconciliation reference")) );
  gtk_box_pack_start ( GTK_BOX ( hbox ),
		       label,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( label );

  entree_no_rapprochement = gtk_entry_new ();
  gtk_widget_set_usize ( entree_no_rapprochement,
			 50,
			 FALSE );
  gtk_tooltips_set_tip ( GTK_TOOLTIPS ( tips ),
			 entree_no_rapprochement,
			 _("If reconciliation reference ends in a digit, it is automatically incremented at each reconciliation."),
			 _("Reconciliation reference") );
  gtk_box_pack_start ( GTK_BOX ( hbox ),
		       entree_no_rapprochement,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( entree_no_rapprochement );


  separateur = gtk_hseparator_new();
  gtk_box_pack_start ( GTK_BOX ( fenetre_equilibrage ),
		       separateur,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( separateur );




  /* on met un premier tab en haut contenant dates et soldes des relev�s avec possibilit� de modif */
  /* du courant */

  table = gtk_table_new ( 3,
			  5,
			  FALSE );
  gtk_table_set_row_spacings ( GTK_TABLE ( table ),
			       5 );
  gtk_box_pack_start ( GTK_BOX ( fenetre_equilibrage ),
		       table,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( table );


  separateur = gtk_hseparator_new();
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      separateur,
			      0, 3,
			      1, 2 );
  gtk_widget_show ( separateur );


  separateur = gtk_hseparator_new();
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      separateur,
			      0, 3,
			      3, 4 );
  gtk_widget_show ( separateur );


  separateur = gtk_vseparator_new ();
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      separateur,
			      1, 2,
			      0, 5 );
  gtk_widget_show ( separateur );



  label = gtk_label_new ( _("Date") );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label,
			      0, 1,
			      0, 1);
  gtk_widget_show ( label );


  label = gtk_label_new ( _("Balance") );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label,
			      2, 3,
			      0, 1);
  gtk_widget_show ( label );



  label_ancienne_date_equilibrage = gtk_label_new ( "" );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label_ancienne_date_equilibrage,
			      0, 1,
			      2, 3 );
  gtk_widget_show ( label_ancienne_date_equilibrage );


  entree_ancien_solde_equilibrage = gtk_entry_new ( );
  gtk_widget_set_usize ( entree_ancien_solde_equilibrage,
			 50,
			 FALSE );
  gtk_signal_connect ( GTK_OBJECT ( entree_ancien_solde_equilibrage ),
		       "changed",
		       GTK_SIGNAL_FUNC ( modif_entree_solde_init_equilibrage ),
		       NULL );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      entree_ancien_solde_equilibrage,
			      2, 3,
			      2, 3 );
  gtk_widget_show ( entree_ancien_solde_equilibrage );



  entree_nouvelle_date_equilibrage = gtk_entry_new ();
  gtk_widget_set_usize ( entree_nouvelle_date_equilibrage,
			 50,
			 FALSE );
  gtk_signal_connect_after ( GTK_OBJECT ( entree_nouvelle_date_equilibrage ),
		       "focus-out-event",
		       GTK_SIGNAL_FUNC ( sortie_entree_date_equilibrage ),
		       NULL );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      entree_nouvelle_date_equilibrage,
			      0, 1,
			      4, 5 );
  gtk_widget_show ( entree_nouvelle_date_equilibrage );


  entree_nouveau_montant_equilibrage = gtk_entry_new ();
  gtk_widget_set_usize ( entree_nouveau_montant_equilibrage,
			 50,
			 FALSE );
  gtk_signal_connect ( GTK_OBJECT ( entree_nouveau_montant_equilibrage ),
		       "changed",
		       GTK_SIGNAL_FUNC ( modif_entree_solde_final_equilibrage ),
		       NULL );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      entree_nouveau_montant_equilibrage,
			      2, 3,
			      4, 5 );
  gtk_widget_show ( entree_nouveau_montant_equilibrage );




  separateur = gtk_hseparator_new();
  gtk_box_pack_start ( GTK_BOX ( fenetre_equilibrage ),
		       separateur,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( separateur );


  /*   la 2�me table contient le solde init, final, du pointage et l'�cart */

  table = gtk_table_new ( 5,
				      2,
				      FALSE );
  gtk_table_set_row_spacings ( GTK_TABLE ( table ),
			       5 );
  gtk_box_pack_start ( GTK_BOX ( fenetre_equilibrage ),
		       table,
		       FALSE,
		       FALSE,
		       15);
  gtk_widget_show ( table );



  label = gtk_label_new ( COLON(_("Initial balance")) );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label,
			      0, 1,
			      0, 1);
  gtk_widget_show ( label );


  label_equilibrage_initial = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC ( label_equilibrage_initial ),
			   1,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label_equilibrage_initial,
			      1, 2,
			      0, 1);
  gtk_widget_show ( label_equilibrage_initial );


  label = gtk_label_new ( COLON(_("Final balance")) );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label,
			      0, 1,
			      1, 2);
  gtk_widget_show ( label );


  label_equilibrage_final = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC ( label_equilibrage_final ),
			   1,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label_equilibrage_final,
			      1, 2,
			      1, 2);
  gtk_widget_show ( label_equilibrage_final );


  label = gtk_label_new ( COLON(_("Checking")) );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label,
			      0, 1,
			      2, 3);
  gtk_widget_show ( label );

  label_equilibrage_pointe = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC ( label_equilibrage_pointe ),
			   1,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label_equilibrage_pointe,
			      1, 2,
			      2, 3);
  gtk_widget_show ( label_equilibrage_pointe );


  separateur = gtk_hseparator_new();
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      separateur,
			      0, 2,
			      3, 4);
  gtk_widget_show ( separateur );


  label = gtk_label_new ( COLON(_("Variance")) );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label,
			      0, 1,
			      4, 5);
  gtk_widget_show ( label );


  label_equilibrage_ecart = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC ( label_equilibrage_ecart ),
			   1,
			   0.5 );
  gtk_table_attach_defaults ( GTK_TABLE ( table ),
			      label_equilibrage_ecart,
			      1, 2,
			      4, 5);
  gtk_widget_show ( label_equilibrage_ecart );



/* on met les boutons */


  hbox = gtk_hbox_new ( TRUE,
					    5);
  gtk_box_pack_end ( GTK_BOX ( fenetre_equilibrage ),
		       hbox,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( hbox );
  
  bouton_ok_equilibrage = gtk_button_new_with_label (_("Valid") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton_ok_equilibrage),
			  GTK_RELIEF_NONE);
  gtk_signal_connect (GTK_OBJECT (bouton_ok_equilibrage),
		      "clicked",
		      (GtkSignalFunc) fin_equilibrage,
		      NULL );
  gtk_box_pack_start ( GTK_BOX ( hbox ),
		       bouton_ok_equilibrage,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( bouton_ok_equilibrage );

  
  bouton = gtk_button_new_with_label (_("Cancel") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton),
			  GTK_RELIEF_NONE);
  gtk_signal_connect ( GTK_OBJECT (bouton),
		       "clicked",
		       (GtkSignalFunc) annuler_equilibrage,
		       NULL );
  gtk_box_pack_start ( GTK_BOX ( hbox ),
		       bouton,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( bouton );


  separateur = gtk_hseparator_new();
  gtk_box_pack_end ( GTK_BOX ( fenetre_equilibrage ),
		       separateur,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( separateur );


  return ( fenetre_equilibrage );
}
/* ********************************************************************************************************** */





/* ********************************************************************************************************** */
void equilibrage ( void )
{
  GDate *date;

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  if ( !NB_OPE_COMPTE )
    {
      dialogue ( _("This account does not contain any transaction") );
      return;
    }


  /* efface le label propri�t�s du compte */

  gtk_widget_hide ( label_proprietes_operations_compte );

  /* calcule le montant des op�rations point�es */

  calcule_total_pointe_compte ( compte_courant );


  /* r�cup�re l'ancien no de rapprochement et essaie d'augmenter la partie num�rique */
  /*   si ne r�ussit pas, remet juste le nom de l'ancien */

  if ( DERNIER_NO_RAPPROCHEMENT )
    {
      GSList *liste_tmp;

      liste_tmp = g_slist_find_custom ( liste_no_rapprochements,
					GINT_TO_POINTER ( DERNIER_NO_RAPPROCHEMENT ),
					(GCompareFunc) recherche_no_rapprochement_par_no );

      if ( liste_tmp )
	{
	  struct struct_no_rapprochement *rapprochement;
	  gchar *pointeur_mobile;
	  gchar *pointeur_fin;
	  gchar *new_rap;

	  rapprochement = liste_tmp -> data;


	  /* on va cr�er une nouvelle chaine contenant la partie num�rique */

	  new_rap = g_strdup ( rapprochement -> nom_rapprochement );
	  pointeur_fin = new_rap + (strlen ( new_rap ) - 1) * sizeof (gchar);
	  pointeur_mobile = pointeur_fin;

	  while ( pointeur_mobile[0] > 47 && pointeur_mobile[0] < 58 && pointeur_mobile >= new_rap )
	    pointeur_mobile--;

	  if ( pointeur_mobile != pointeur_fin )
	    {
	      /* la fin du no de rapprochement est num�rique */

	      gchar *rempl_zero;
	      gchar *partie_num;
	      gint longueur;
	      gint nouvelle_longueur;

	      pointeur_mobile++;

	      partie_num = g_strdup ( pointeur_mobile );
	      pointeur_mobile[0] = 0;

	      longueur = strlen ( partie_num );

	      /* on incr�mente la partie num�rique */

	      partie_num = itoa ( atoi ( partie_num ) + 1 );

	      /* si la nouvelle partie num�rique est plus petite que l'ancienne, */
	      /* c'est que des 0 ont �t� shunt�s, on va les rajouter ici */
 
	      nouvelle_longueur = strlen ( partie_num );

	      if ( nouvelle_longueur < longueur )
		{
		  gint i;

		  rempl_zero = malloc ((longueur-nouvelle_longueur+1)*sizeof (gchar));

		  for ( i=0 ; i<longueur-nouvelle_longueur ; i++ )
		    rempl_zero[i]=48;

		  rempl_zero[longueur-nouvelle_longueur] = 0;
		}
	      else
		rempl_zero = "";

	      /* on  remet le tout ensemble */

	      new_rap = g_strconcat ( new_rap,
				      rempl_zero,
				      partie_num,
				      NULL );
	    }
	  else
	    new_rap = rapprochement -> nom_rapprochement;

	  gtk_entry_set_text ( GTK_ENTRY ( entree_no_rapprochement ),
			       new_rap );

	}
    }
  else
    gtk_entry_set_text ( GTK_ENTRY ( entree_no_rapprochement ),
			 "" );

  /* r�cup�re l'ancienne date et l'augmente d'1 mois et le met dans entree_nouvelle_date_equilibrage */

  if ( DATE_DERNIER_RELEVE )
    {
      date = g_date_new_dmy ( g_date_day ( DATE_DERNIER_RELEVE ),
			      g_date_month ( DATE_DERNIER_RELEVE ),
			      g_date_year ( DATE_DERNIER_RELEVE ));

      gtk_label_set_text ( GTK_LABEL ( label_ancienne_date_equilibrage ),
			   g_strdup_printf ( "%02d/%02d/%d", 
					     g_date_day ( date ),
					     g_date_month ( date ),
					     g_date_year ( date ) ));
      g_date_add_months ( date,
			  1 );

      gtk_entry_set_text ( GTK_ENTRY ( entree_ancien_solde_equilibrage ),
			   g_strdup_printf ("%4.2f", SOLDE_DERNIER_RELEVE ));
      gtk_widget_set_sensitive ( GTK_WIDGET ( entree_ancien_solde_equilibrage ),
				 FALSE );
    }
  else
    {
      time_t today;

      gtk_label_set_text ( GTK_LABEL ( label_ancienne_date_equilibrage ),
			   _("None") );

      time ( &today );
      date = g_date_new_dmy ( gmtime ( &today ) -> tm_mday,
			      gmtime ( &today ) -> tm_mon + 1,
			      gmtime ( &today ) -> tm_year + 1900 );

      gtk_entry_set_text ( GTK_ENTRY ( entree_ancien_solde_equilibrage ),
			   g_strdup_printf ("%4.2f", SOLDE_INIT ));
      gtk_widget_set_sensitive ( GTK_WIDGET ( entree_ancien_solde_equilibrage ),
				 TRUE );
    }



  gtk_entry_set_text ( GTK_ENTRY ( entree_nouvelle_date_equilibrage ),
		       g_strdup_printf ( "%02d/%02d/%d", 
					 g_date_day ( date ),
					 g_date_month ( date ),
					 g_date_year ( date ) ));



  gtk_entry_set_text ( GTK_ENTRY ( entree_nouveau_montant_equilibrage ),
		       "" );

  gtk_label_set_text ( GTK_LABEL ( label_equilibrage_compte ),
		       NOM_DU_COMPTE );


  etat.equilibrage = 1;

  if ( solde_final - solde_initial - operations_pointees )
    gtk_widget_set_sensitive ( bouton_ok_equilibrage,
			       FALSE );
  else
    gtk_widget_set_sensitive ( bouton_ok_equilibrage,
			       TRUE );


  /* affiche la liste en op� simplifi�es */

  ancien_nb_lignes_ope = NB_LIGNES_OPE;

  if ( NB_LIGNES_OPE != 1 )
    gtk_button_clicked ( GTK_BUTTON ( bouton_ope_1_lignes ));

  /* on vire les op�rations rapproch�es */

  etat.valeur_r_avant_rapprochement = AFFICHAGE_R;

  change_aspect_liste ( NULL,
			3 );


  /* classe la liste des op�s en fonction des types ou non */

  if ( TRI
       &&
       LISTE_TRI )
    LISTE_OPERATIONS = g_slist_sort ( LISTE_OPERATIONS,
				      (GCompareFunc) classement_sliste );


  remplissage_liste_operations ( compte_courant );

  gtk_notebook_set_page ( GTK_NOTEBOOK ( notebook_comptes_equilibrage ),
			  2 );


  /* la liste des op� prend le focus */

  gtk_widget_grab_focus ( GTK_WIDGET ( CLIST_OPERATIONS ));
}
/* ********************************************************************************************************** */



/* ********************************************************************************************************** */
void sortie_entree_date_equilibrage ( void )
{
  gchar *text;
  gint nb_parametres;
  GDate *date;
  gint date_releve_jour;
  gint date_releve_mois;
  gint date_releve_annee;


  text = (char *) gtk_entry_get_text ( GTK_ENTRY ( entree_nouvelle_date_equilibrage ) );

  if ( ( nb_parametres = sscanf ( text,
				  "%d/%d/%d",
				  &date_releve_jour,
				  &date_releve_mois,
				  &date_releve_annee))
       != 3 )
    {
      if ( !nb_parametres || nb_parametres == -1 )
	return;


      date = g_date_new ();
      g_date_set_time ( date,
			time(NULL));

      if ( nb_parametres == 1)
	date_releve_mois = g_date_month (date);

      date_releve_annee = g_date_year (date);

    }

  if ( g_date_valid_dmy ( date_releve_jour,
			  date_releve_mois,
			  date_releve_annee)
       == FALSE )
    {
      dialogue ( _("Error: invalid date") );
      return;
    };


  gtk_entry_set_text ( GTK_ENTRY ( entree_nouvelle_date_equilibrage ),
		       g_strdup_printf ( "%02d/%02d/%d", 
					 date_releve_jour,
					 date_releve_mois,
					 date_releve_annee ) );
	   




}
/* ********************************************************************************************************** */

				      
/* ********************************************************************************************************** */
void modif_entree_solde_init_equilibrage ( void )
{

  gtk_label_set_text ( GTK_LABEL ( label_equilibrage_initial ),
		       (char *) gtk_entry_get_text ( GTK_ENTRY ( entree_ancien_solde_equilibrage )) );

  solde_initial = g_strtod ( (char *) gtk_entry_get_text ( GTK_ENTRY ( entree_ancien_solde_equilibrage )),
			     NULL );


  if ( fabs ( solde_final - solde_initial - operations_pointees ) < 0.01 )
    {
      gtk_label_set_text ( GTK_LABEL ( label_equilibrage_ecart ),
			   g_strdup_printf ( "%4.2f",
					     0.0 ));
      gtk_widget_set_sensitive ( GTK_WIDGET ( bouton_ok_equilibrage ),
				 TRUE );
    }
  else
    {
      gtk_label_set_text ( GTK_LABEL ( label_equilibrage_ecart ),
			   g_strdup_printf ( "%4.2f",
					     solde_final - solde_initial - operations_pointees ));
      gtk_widget_set_sensitive ( GTK_WIDGET ( bouton_ok_equilibrage ),
				 FALSE );
    }

}
/* ********************************************************************************************************** */





				      
/* ********************************************************************************************************** */
void modif_entree_solde_final_equilibrage ( void )
{


  gtk_label_set_text ( GTK_LABEL ( label_equilibrage_final ),
		       (char *) gtk_entry_get_text ( GTK_ENTRY ( entree_nouveau_montant_equilibrage )) );

  solde_final = g_strtod ( (char *) gtk_entry_get_text ( GTK_ENTRY ( entree_nouveau_montant_equilibrage )),
			   NULL );

  if ( fabs ( solde_final - solde_initial - operations_pointees ) < 0.01 )
    {
      gtk_label_set_text ( GTK_LABEL ( label_equilibrage_ecart ),
			   g_strdup_printf ( "%4.2f",
					     0.0 ));
      gtk_widget_set_sensitive ( GTK_WIDGET ( bouton_ok_equilibrage ),
				 TRUE );
    }
  else
    {
      gtk_label_set_text ( GTK_LABEL ( label_equilibrage_ecart ),
			   g_strdup_printf ( "%4.2f",
					     solde_final - solde_initial - operations_pointees ));
      gtk_widget_set_sensitive ( GTK_WIDGET ( bouton_ok_equilibrage ),
				 FALSE );
    }

}
/* ********************************************************************************************************** */







/* ********************************************************************************************************** */
/* on annule l'�quilibrage */
/* ********************************************************************************************************** */

void annuler_equilibrage ( GtkWidget *bouton_ann,
			   gpointer data)
{

  gtk_notebook_set_page ( GTK_NOTEBOOK ( notebook_comptes_equilibrage ),
			  0 );

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  etat.equilibrage = 0;

  if ( ancien_nb_lignes_ope != NB_LIGNES_OPE )
    {
      if ( ancien_nb_lignes_ope == 4 )
	gtk_button_clicked ( GTK_BUTTON ( bouton_ope_4_lignes ));
      else
	{
	  if ( ancien_nb_lignes_ope == 3 )
	    gtk_button_clicked ( GTK_BUTTON ( bouton_ope_3_lignes ));
	  else
	    gtk_button_clicked ( GTK_BUTTON ( bouton_ope_2_lignes ));
	}
    }
	

  if ( etat.valeur_r_avant_rapprochement )
    change_aspect_liste ( NULL,
			  2 );

  LISTE_OPERATIONS = g_slist_sort ( LISTE_OPERATIONS,
				    (GCompareFunc) classement_sliste );


/*   gtk_clist_set_compare_func ( GTK_CLIST ( CLIST_OPERATIONS ), */
/* 			       (GtkCListCompareFunc) classement_liste_par_date ); */

  remplissage_liste_operations ( compte_courant );

  gtk_widget_show ( label_proprietes_operations_compte );

  focus_a_la_liste ();
}
/* ********************************************************************************************************** */





/* ********************************************************************************************************** */
/* fonction appel�e quand il y a un click dans la colonne des P, et si l'�quilibrage */
/* est en cours */
/* ********************************************************************************************************** */

void pointe_equilibrage ( int p_ligne )
{
  struct structure_operation *operation;
  gdouble montant;

  operation = gtk_clist_get_row_data ( GTK_CLIST ( CLIST_OPERATIONS ),
				       p_ligne );


  if ( operation == GINT_TO_POINTER ( -1 ) )
    return;


  if ( operation -> pointe == 2 )
    return;


  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;


  montant = calcule_montant_devise_renvoi ( operation -> montant,
					    DEVISE,
					    operation -> devise,
					    operation -> une_devise_compte_egale_x_devise_ope,
					    operation -> taux_change,
					    operation -> frais_change );

  if ( operation -> pointe )
    {
      operations_pointees = operations_pointees - montant;
      SOLDE_POINTE = SOLDE_POINTE - montant;

      operation -> pointe = 0;

      gtk_clist_set_text ( GTK_CLIST ( CLIST_OPERATIONS ),
			   p_ligne,
			   3,
			   " ");
    }
  else
    {
      operations_pointees = operations_pointees + montant;
      SOLDE_POINTE = SOLDE_POINTE + montant;

      operation -> pointe = 1;

      gtk_clist_set_text ( GTK_CLIST ( CLIST_OPERATIONS ),
			   p_ligne,
			   3,
			   "P");
    }

    
  /* si c'est une op� ventil�e, on recherche les op� filles pour leur mettre le m�me pointage que la m�re */

  if ( operation -> operation_ventilee )
    {
      /* p_tab est d�j� point� sur le compte courant */

      GSList *liste_tmp;

      liste_tmp = LISTE_OPERATIONS;

      while ( liste_tmp )
	{
	  struct structure_operation *ope_fille;

	  ope_fille = liste_tmp -> data;

	  if ( ope_fille -> no_operation_ventilee_associee == operation -> no_operation )
	    ope_fille -> pointe = operation -> pointe;

	      liste_tmp = liste_tmp -> next;
	}
    }


  gtk_label_set_text ( GTK_LABEL ( label_equilibrage_pointe ),
		       g_strdup_printf ("%4.2f",
					operations_pointees ));

  if ( fabs ( solde_final - solde_initial - operations_pointees ) < 0.01 )
    {
      gtk_label_set_text ( GTK_LABEL ( label_equilibrage_ecart ),
			   g_strdup_printf ( "%4.2f",
					     0.0 ));
      gtk_widget_set_sensitive ( GTK_WIDGET ( bouton_ok_equilibrage ),
				 TRUE );
    }
  else
    {
      gtk_label_set_text ( GTK_LABEL ( label_equilibrage_ecart ),
			   g_strdup_printf ( "%4.2f",
					     solde_final - solde_initial - operations_pointees ));
      gtk_widget_set_sensitive ( GTK_WIDGET ( bouton_ok_equilibrage ),
				 FALSE );
    }

  /* met le label du solde point� */

  gtk_label_set_text ( GTK_LABEL ( solde_label_pointe ),
		       g_strdup_printf ( _("Checked balance: %4.2f %s"),
					 SOLDE_POINTE,
					 devise_name ((struct struct_devise *)(g_slist_find_custom ( liste_struct_devises,
											 GINT_TO_POINTER ( DEVISE ),
											 (GCompareFunc) recherche_devise_par_no )-> data ))) );

  modification_fichier( TRUE );

}
/* ********************************************************************************************************** */




/* ********************************************************************************************************** */
void fin_equilibrage ( GtkWidget *bouton_ok,
		       gpointer data )
{
  GSList *pointeur_liste_ope;
  gchar *text;
  gint nb_parametres;
  GDate *date;
  gint date_releve_jour;
  gint date_releve_mois;
  gint date_releve_annee;


  if ( fabs ( solde_final - solde_initial - operations_pointees ) >= 0.01 )
    {
      dialogue ( _("There is a variance"));
      return;
    }


  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;


  /* r�cup�ration de la date */

  text = (char *) gtk_entry_get_text ( GTK_ENTRY ( entree_nouvelle_date_equilibrage ) );

  if ( ( nb_parametres = sscanf ( text,
				  "%d/%d/%d",
				  &date_releve_jour,
				  &date_releve_mois,
				  &date_releve_annee))
       != 3 )
    {
      if ( !nb_parametres || nb_parametres == -1 )
	{
	  dialogue ( _("Error: invalid date") );
	  return;
	}


      date = g_date_new ();
      g_date_set_time ( date,
			time(NULL));

      if ( nb_parametres == 1)
	date_releve_mois = g_date_month(date);

      date_releve_annee = g_date_year(date);

    }

  if ( !g_date_valid_dmy ( date_releve_jour,
			   date_releve_mois,
			   date_releve_annee))
    {
      dialogue ( _("Error: invalid date") );
      return;
    }

  DATE_DERNIER_RELEVE = g_date_new_dmy ( date_releve_jour,
					 date_releve_mois,
					 date_releve_annee );

  gtk_label_set_text ( GTK_LABEL ( label_releve ),
		       g_strdup_printf ( _("Last statement: %02d/%02d/%d"), 
					 date_releve_jour,
					 date_releve_mois,
					 date_releve_annee ));


  /*   r�cup�ration du no de rapprochement, */
  /*     s'il n'existe pas,on le cr�e */

  if ( strlen ( g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( entree_no_rapprochement )))))
    {
      struct struct_no_rapprochement *rapprochement;
      GSList *liste_tmp;
      gchar *rap_txt;

      rap_txt = g_strstrip ( (char *) gtk_entry_get_text ( GTK_ENTRY ( entree_no_rapprochement )));

      liste_tmp = g_slist_find_custom ( liste_no_rapprochements,
					rap_txt,
					(GCompareFunc) recherche_no_rapprochement_par_nom );

      if ( liste_tmp )
	{
	  /* le rapprochement existe d�j� */

	  rapprochement = liste_tmp -> data;
	  DERNIER_NO_RAPPROCHEMENT = rapprochement -> no_rapprochement;
	}
      else
	{
	  /* le rapprochement n'existe pas */

	  rapprochement = malloc ( sizeof ( struct struct_no_rapprochement ));
	  rapprochement -> no_rapprochement = g_slist_length ( liste_no_rapprochements ) + 1;
	  rapprochement -> nom_rapprochement = g_strdup ( rap_txt );

	  liste_no_rapprochements = g_slist_append ( liste_no_rapprochements,
						     rapprochement );

	  DERNIER_NO_RAPPROCHEMENT = rapprochement -> no_rapprochement;
	}
    }
  else
    DERNIER_NO_RAPPROCHEMENT = 0;



/* met tous les P � R */

  p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

  pointeur_liste_ope = LISTE_OPERATIONS;

  while ( pointeur_liste_ope )
    {
      struct structure_operation *operation;

      operation = pointeur_liste_ope -> data;

      if ( operation -> pointe == 1 )
	{
	  operation -> pointe = 2;
	  operation -> no_rapprochement = DERNIER_NO_RAPPROCHEMENT;
	}

      pointeur_liste_ope = pointeur_liste_ope -> next;
    }



/* on r�affiche la liste */

  modification_fichier( TRUE );

  SOLDE_DERNIER_RELEVE = solde_final;


  gtk_notebook_set_page ( GTK_NOTEBOOK ( notebook_comptes_equilibrage ),
			  0 );

  etat.equilibrage = 0;

  if ( ancien_nb_lignes_ope != NB_LIGNES_OPE )
    {
      if ( ancien_nb_lignes_ope == 4 )
	gtk_button_clicked ( GTK_BUTTON ( bouton_ope_4_lignes ));
      else
	{
	  if ( ancien_nb_lignes_ope == 3 )
	    gtk_button_clicked ( GTK_BUTTON ( bouton_ope_3_lignes ));
	  else
	    gtk_button_clicked ( GTK_BUTTON ( bouton_ope_2_lignes ));
	}
    }
	
  if ( etat.valeur_r_avant_rapprochement )
    change_aspect_liste ( NULL,
			  2 );

  LISTE_OPERATIONS = g_slist_sort ( LISTE_OPERATIONS,
				    (GCompareFunc) classement_sliste );

/*   gtk_clist_set_compare_func ( GTK_CLIST ( CLIST_OPERATIONS ), */
/* 			       (GtkCListCompareFunc) classement_liste_par_date ); */


  remplissage_liste_operations ( compte_courant );

  gtk_widget_show ( label_proprietes_operations_compte );

/*   on redonne le focus � la liste */

  gtk_widget_grab_focus ( GTK_WIDGET ( CLIST_OPERATIONS ) );
}
/* ********************************************************************************************************** */


/* ********************************************************************************************************** */
gint recherche_no_rapprochement_par_nom ( struct struct_no_rapprochement *rapprochement,
					  gchar *no_rap )
{

  return ( strcmp ( rapprochement -> nom_rapprochement,
		    no_rap ));

}
/* ********************************************************************************************************** */


/* ********************************************************************************************************** */
gint recherche_no_rapprochement_par_no ( struct struct_no_rapprochement *rapprochement,
					 gint *no_rap )
{

  return ( !(rapprochement -> no_rapprochement == GPOINTER_TO_INT ( no_rap )));

}
/* ********************************************************************************************************** */



/* ********************************************************************************************************** */
void calcule_total_pointe_compte ( gint no_compte )
{
  GSList *pointeur_liste_ope;

  p_tab_nom_de_compte_variable =  p_tab_nom_de_compte + no_compte;

  pointeur_liste_ope = LISTE_OPERATIONS;
  operations_pointees = 0;

  while ( pointeur_liste_ope )
    {
      struct structure_operation *operation;

      operation = pointeur_liste_ope -> data;

      /* on ne prend en compte l'op� que si c'est pas une op� de ventil */

      if ( operation -> pointe == 1
	   &&
	   !operation -> no_operation_ventilee_associee )
	{
	  gdouble montant;

	  montant = calcule_montant_devise_renvoi ( operation -> montant,
						    DEVISE,
						    operation -> devise,
						    operation -> une_devise_compte_egale_x_devise_ope,
						    operation -> taux_change,
						    operation -> frais_change );

	  operations_pointees = operations_pointees + montant;
	}

      pointeur_liste_ope = pointeur_liste_ope -> next;
    }

  gtk_label_set_text ( GTK_LABEL ( label_equilibrage_pointe ),
		       g_strdup_printf ( "%4.2f", 
					 operations_pointees ));
}
/* ********************************************************************************************************** */


/**
 *
 *
 *
 */
GtkWidget * tab_display_reconciliation ( void )
{
  GtkWidget *onglet;
  GtkWidget *hbox;
  GtkWidget *frame;
  GtkWidget *scrolled_window;
  gchar *titres[2] = { _("Accounts"),
		       _("Default") };
  gint i;
  GtkWidget *vbox;
  GtkWidget *hbox2;
  GtkWidget *menu;
  GtkWidget *item;
  GtkWidget *label;
  GtkWidget *bouton;
  GtkWidget *vbox2, *vbox_pref;

  vbox_pref = new_vbox_with_title_and_icon ( _("Reconciliation"),
					     "reconciliation.png" );

  frame = gtk_frame_new ( _("Reconciliation: sort transactions") );
  gtk_box_pack_start ( GTK_BOX ( vbox_pref ),
		       frame,
		       TRUE,
		       TRUE,
		       0 );
  gtk_widget_show ( frame );

  /* on met une vbox dans la frame */

  vbox2 = gtk_vbox_new ( FALSE,
			 5 );
  gtk_container_set_border_width ( GTK_CONTAINER ( vbox2 ),
				   5 );
  gtk_container_add ( GTK_CONTAINER ( frame ),
		      vbox2 );
  gtk_widget_show ( vbox2 );

  /*   la partie du haut : tri par date ou par type */

  bouton_type_tri_date = gtk_radio_button_new_with_label ( NULL,
							   _("Sort by date") );
  gtk_widget_set_sensitive ( bouton_type_tri_date,
			     FALSE );
  gtk_toggle_button_set_active ( GTK_TOGGLE_BUTTON ( bouton_type_tri_date ),
				 TRUE );
  gtk_signal_connect ( GTK_OBJECT ( bouton_type_tri_date ),
		       "toggled",
		       GTK_SIGNAL_FUNC ( modif_tri_date_ou_type ),
		       NULL );
  gtk_box_pack_start ( GTK_BOX ( vbox2 ),
		       bouton_type_tri_date,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( bouton_type_tri_date );

  bouton_type_tri_type = gtk_radio_button_new_with_label ( gtk_radio_button_group ( GTK_RADIO_BUTTON ( bouton_type_tri_date )),
							   _("Sort by method of payment") );
  gtk_widget_set_sensitive ( bouton_type_tri_type,
			     FALSE );
  gtk_box_pack_start ( GTK_BOX ( vbox2 ),
		       bouton_type_tri_type,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( bouton_type_tri_type );


  /* la partie du milieu est une hbox avec les types */

  hbox = gtk_hbox_new ( FALSE,
			 5 );
  gtk_box_pack_start ( GTK_BOX ( vbox2 ),
		       hbox,
		       TRUE,
		       TRUE,
		       0 );
  gtk_widget_show ( hbox );


  /* mise en place de la liste qui contient les types class�s */

  scrolled_window = gtk_scrolled_window_new ( NULL, NULL );
  gtk_box_pack_start ( GTK_BOX ( hbox ),
		       scrolled_window,
		       TRUE,
		       TRUE,
		       0);
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_window ),
				   GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC);
  gtk_widget_show ( scrolled_window );


  type_liste_tri = gtk_clist_new ( 1 );
  gtk_widget_set_sensitive ( type_liste_tri,
			     FALSE );
  gtk_clist_set_column_auto_resize ( GTK_CLIST ( type_liste_tri ) ,
				     0,
				     TRUE );
  gtk_clist_set_reorderable ( GTK_CLIST ( type_liste_tri ),
			      TRUE );
  gtk_clist_set_use_drag_icons ( GTK_CLIST ( type_liste_tri ),
				 TRUE );
  gtk_container_add ( GTK_CONTAINER ( scrolled_window ),
		      type_liste_tri );
  gtk_signal_connect ( GTK_OBJECT ( type_liste_tri ),
		       "select_row",
		       (GtkSignalFunc ) selection_type_liste_tri,
		       NULL );
  gtk_signal_connect ( GTK_OBJECT ( type_liste_tri ),
		       "unselect_row",
		       (GtkSignalFunc ) deselection_type_liste_tri,
		       NULL );
  gtk_signal_connect_after ( GTK_OBJECT ( type_liste_tri ),
			     "row_move",
			     GTK_SIGNAL_FUNC ( save_ordre_liste_type_tri ),
			     NULL );
  gtk_widget_show ( type_liste_tri );

      
  /* on place ici les fl�ches sur le c�t� de la liste */

  vbox_fleches_tri = gtk_vbutton_box_new ();
  gtk_widget_set_sensitive ( vbox_fleches_tri,
			     FALSE );
  gtk_box_pack_start ( GTK_BOX ( hbox ),
		       vbox_fleches_tri,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( vbox_fleches_tri );

  bouton = gtk_button_new_from_stock (GTK_STOCK_GO_UP);
  gtk_button_set_relief ( GTK_BUTTON ( bouton ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect ( GTK_OBJECT ( bouton ),
		       "clicked",
		       (GtkSignalFunc ) deplacement_type_tri_haut,
		       NULL );
  gtk_container_add ( GTK_CONTAINER ( vbox_fleches_tri ),
		      bouton );
  gtk_widget_show ( bouton );

  bouton = gtk_button_new_from_stock (GTK_STOCK_GO_DOWN);
  gtk_button_set_relief ( GTK_BUTTON ( bouton ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect ( GTK_OBJECT ( bouton ),
		       "clicked",
		       (GtkSignalFunc ) deplacement_type_tri_bas,
		       NULL);
  gtk_container_add ( GTK_CONTAINER ( vbox_fleches_tri ),
		      bouton );
  gtk_widget_show ( bouton );

  /* la partie du bas contient des check buttons */

  bouton_type_neutre_inclut = gtk_check_button_new_with_label ( _("Include the mixed methods of payment in the incomes/outgoings") );
  gtk_widget_set_sensitive ( bouton_type_neutre_inclut,
			     FALSE );
  gtk_signal_connect ( GTK_OBJECT ( bouton_type_neutre_inclut ),
		       "toggled",
		       GTK_SIGNAL_FUNC ( inclut_exclut_les_neutres ),
		       NULL );
  gtk_box_pack_start ( GTK_BOX ( vbox2 ),
		       bouton_type_neutre_inclut,
		       FALSE,
		       FALSE,
		       0);
  gtk_widget_show ( bouton_type_neutre_inclut );

  return vbox_pref;
}
