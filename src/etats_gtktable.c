/*  Fichier qui s'occupe d'afficher les �tats via une gtktable */
/*      etats_gtktable.c */

/*     Copyright (C) 2000-2002  C�dric Auger */
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

#include "etats_gtktable.h"

struct struct_etat_affichage gtktable_affichage = {
  gtktable_initialise,
  gtktable_affiche_titre,
  gtktable_affiche_separateur,
  gtktable_affiche_total_categories,
  gtktable_affiche_total_sous_categ,
  gtktable_affiche_total_ib,
  gtktable_affiche_total_sous_ib,
  gtktable_affiche_total_compte,
  gtktable_affiche_total_tiers,
  gtktable_affichage_ligne_ope,
  gtktable_affiche_total_partiel,
  gtktable_affiche_total_general,
  gtktable_affiche_categ_etat,
  gtktable_affiche_sous_categ_etat,
  gtktable_affiche_ib_etat,
  gtktable_affiche_sous_ib_etat,
  gtktable_affiche_compte_etat,
  gtktable_affiche_tiers_etat,
  gtktable_affiche_titre_revenus_etat,
  gtktable_affiche_titre_depenses_etat,
  gtktable_affiche_totaux_sous_jaccent,
  gtktable_affiche_titres_colonnes,
  gtktable_finish
};

gint gtktable_initialise ()
{
  /* on peut maintenant cr�er la table */
  /* pas besoin d'indiquer la hauteur, elle grandit automatiquement */

  table_etat = gtk_table_new ( 0,
			       nb_colonnes,
			       FALSE );
  gtk_table_set_col_spacings ( GTK_TABLE ( table_etat ),
			       5 );
  if ( GTK_BIN ( scrolled_window_etat ) -> child )
    gtk_container_remove ( GTK_CONTAINER ( scrolled_window_etat ),
			   GTK_BIN ( scrolled_window_etat ) -> child );

  gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW ( scrolled_window_etat ),
					  table_etat );
  gtk_widget_show ( table_etat );
  
  return 1;
}


gint gtktable_affiche_titre ( gint ligne )
{
  GtkWidget *label;
  label = gtk_label_new ( etat_courant -> nom_etat );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     0, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  return 1;
}


gint gtktable_affiche_separateur ( gint ligne )
{
  GtkWidget * separateur;
  separateur = gtk_hseparator_new ();
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     separateur,
		     0, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( separateur );

  return ligne + 1;
}


/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les cat�gories sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gtktable_affiche_total_categories ( gint ligne )
{
  GtkWidget *separateur;
  GtkWidget *label;

  if ( etat_courant -> utilise_categ
       &&
       etat_courant -> affiche_sous_total_categ )
    {
      /* si rien n'est affich� en dessous de la cat�g, on */
      /* met le r�sultat sur la ligne de la cat�g */
      /* sinon on fait une barre et on met le r�sultat */

      if ( etat_courant -> afficher_sous_categ
	   ||
	   etat_courant -> utilise_ib
	   ||
	   etat_courant -> regroupe_ope_par_compte
	   ||
	   etat_courant -> utilise_tiers
	   ||
	   etat_courant -> afficher_opes )
	{
	  /* 	  si on affiche les op�s, on met les traits entre eux */

	  if ( etat_courant -> afficher_opes
	       &&
	       ligne_debut_partie != -1 )
	    {
	      gint i;
	      gint colonne;

	      colonne = 2;

	      for ( i=0 ; i<((nb_colonnes-2)/2) ; i++ )
		{
		  separateur = gtk_vseparator_new ();
		  gtk_table_attach ( GTK_TABLE ( table_etat ),
				     separateur,
				     colonne, colonne + 1,
				     ligne_debut_partie, ligne,
				     GTK_SHRINK | GTK_FILL,
				     GTK_SHRINK | GTK_FILL,
				     0, 0 );
		  gtk_widget_show ( separateur );

		  colonne = colonne + 2;
		}
	      ligne_debut_partie = -1;
	    }


	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols -1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  separateur = gtk_hseparator_new ();
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     separateur,
			     1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( separateur );

	  ligne++;

	  if ( nom_categ_en_cours )
	    label = gtk_label_new ( g_strconcat ( "Total ",
						  nom_categ_en_cours,
						  NULL ));
	  else
	    label = gtk_label_new ( "Total Cat�gorie : " );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_categ_etat,
						    devise_categ_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols -1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
      else
	{
	  ligne--;

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_categ_etat,
						    devise_categ_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}

    }

  montant_categ_etat = 0;
  nom_categ_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les sous_categ sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gtktable_affiche_total_sous_categ ( gint ligne )
{
  GtkWidget *separateur;
  GtkWidget *label;

  if ( etat_courant -> utilise_categ
       &&
       etat_courant -> afficher_sous_categ
       &&
       etat_courant -> affiche_sous_total_sous_categ )
    {
      /* si rien n'est affich� en dessous de la sous_categ, on */
      /* met le r�sultat sur la ligne de la ss categ */
      /* sinon on fait une barre et on met le r�sultat */

      if ( etat_courant -> utilise_ib
	   ||
	   etat_courant -> regroupe_ope_par_compte
	   ||
	   etat_courant -> utilise_tiers
	   ||
	   etat_courant -> afficher_opes )
	{
	  /* 	  si on affiche les op�s, on met les traits entre eux */

	  if ( etat_courant -> afficher_opes
	       &&
	       ligne_debut_partie != -1 )
	    {
	      gint i;
	      gint colonne;

	      colonne = 2;

	      for ( i=0 ; i<((nb_colonnes-2)/2) ; i++ )
		{
		  separateur = gtk_vseparator_new ();
		  gtk_table_attach ( GTK_TABLE ( table_etat ),
				     separateur,
				     colonne, colonne + 1,
				     ligne_debut_partie, ligne,
				     GTK_SHRINK | GTK_FILL,
				     GTK_SHRINK | GTK_FILL,
				     0, 0 );
		  gtk_widget_show ( separateur );

		  colonne = colonne + 2;
		}
	      ligne_debut_partie = -1;
	    }


	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  separateur = gtk_hseparator_new ();
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     separateur,
			     1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( separateur );

	  ligne++;

	  if ( nom_categ_en_cours
	       &&
	       nom_ss_categ_en_cours )
	    label = gtk_label_new ( g_strconcat ( "Total ",
						  nom_categ_en_cours,
						  " : ",
						  nom_ss_categ_en_cours,
						  NULL ));
	  else
	    label = gtk_label_new ( "Total Sous-cat�gories : " );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_sous_categ_etat,
						    devise_categ_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
      else
	{
	  ligne--;

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_sous_categ_etat,
						    devise_categ_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
    }

  montant_sous_categ_etat = 0;
  nom_ss_categ_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les ib sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gtktable_affiche_total_ib ( gint ligne )
{
  GtkWidget *separateur;
  GtkWidget *label;

  if ( etat_courant -> utilise_ib
       &&
       etat_courant -> affiche_sous_total_ib )
    {
      /* si rien n'est affich� en dessous de la ib, on */
      /* met le r�sultat sur la ligne de l'ib */
      /* sinon on fait une barre et on met le r�sultat */

      if ( etat_courant -> afficher_sous_ib
	   ||
	   etat_courant -> regroupe_ope_par_compte
	   ||
	   etat_courant -> utilise_tiers
	   ||
	   etat_courant -> afficher_opes )
	{
	  /* 	  si on affiche les op�s, on met les traits entre eux */

	  if ( etat_courant -> afficher_opes
	       &&
	       ligne_debut_partie != -1 )
	    {
	      gint i;
	      gint colonne;

	      colonne = 2;

	      for ( i=0 ; i<((nb_colonnes-2)/2) ; i++ )
		{
		  separateur = gtk_vseparator_new ();
		  gtk_table_attach ( GTK_TABLE ( table_etat ),
				     separateur,
				     colonne, colonne + 1,
				     ligne_debut_partie, ligne,
				     GTK_SHRINK | GTK_FILL,
				     GTK_SHRINK | GTK_FILL,
				     0, 0 );
		  gtk_widget_show ( separateur );

		  colonne = colonne + 2;
		}
	      ligne_debut_partie = -1;
	    }


	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  separateur = gtk_hseparator_new ();
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     separateur,
			     1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( separateur );

	  ligne++;

	  if ( nom_ib_en_cours )
	    label = gtk_label_new ( g_strconcat ( "Total ",
						  nom_ib_en_cours,
						  NULL ));
	  else
	    label = gtk_label_new ( "Total Imputations budg�taires : " );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_ib_etat,
						    devise_ib_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
      else
	{
	  ligne--;

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_ib_etat,
						    devise_ib_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
    }

  montant_ib_etat = 0;
  nom_ib_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les sous_ib sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gtktable_affiche_total_sous_ib ( gint ligne )
{
  GtkWidget *separateur;
  GtkWidget *label;

  if ( etat_courant -> utilise_ib
       &&
       etat_courant -> afficher_sous_ib
       &&
       etat_courant -> affiche_sous_total_sous_ib )
    {
      /* si rien n'est affich� en dessous de la sous ib, on */
      /* met le r�sultat sur la ligne de la sous ib */
      /* sinon on fait une barre et on met le r�sultat */

      if ( etat_courant -> regroupe_ope_par_compte
	   ||
	   etat_courant -> utilise_tiers
	   ||
	   etat_courant -> afficher_opes )
	{
	  /* 	  si on affiche les op�s, on met les traits entre eux */

	  if ( etat_courant -> afficher_opes
	       &&
	       ligne_debut_partie != -1 )
	    {
	      gint i;
	      gint colonne;

	      colonne = 2;

	      for ( i=0 ; i<((nb_colonnes-2)/2) ; i++ )
		{
		  separateur = gtk_vseparator_new ();
		  gtk_table_attach ( GTK_TABLE ( table_etat ),
				     separateur,
				     colonne, colonne + 1,
				     ligne_debut_partie, ligne,
				     GTK_SHRINK | GTK_FILL,
				     GTK_SHRINK | GTK_FILL,
				     0, 0 );
		  gtk_widget_show ( separateur );

		  colonne = colonne + 2;
		}
	      ligne_debut_partie = -1;
	    }


	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  separateur = gtk_hseparator_new ();
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     separateur,
			     1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( separateur );

	  ligne++;

	  if ( nom_ib_en_cours
	       &&
	       nom_ss_ib_en_cours )
	    label = gtk_label_new ( g_strconcat ( "Total ",
						  nom_ib_en_cours,
						  " : ",
						  nom_ss_ib_en_cours,
						  NULL ));
	  else
	    label = gtk_label_new ( "Total Sous-imputations budg�taires : " );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_sous_ib_etat,
						    devise_ib_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
      else
	{
	  ligne--;

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_sous_ib_etat,
						    devise_ib_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
    }

  montant_sous_ib_etat = 0;
  nom_ss_ib_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les compte sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gtktable_affiche_total_compte ( gint ligne )
{
  GtkWidget *separateur;
  GtkWidget *label;

  if ( etat_courant -> regroupe_ope_par_compte
       &&
       etat_courant -> affiche_sous_total_compte )
    {
      /* si rien n'est affich� en dessous du compte, on */
      /* met le r�sultat sur la ligne du compte */
      /* sinon on fait une barre et on met le r�sultat */

      if ( etat_courant -> utilise_tiers
	   ||
	   etat_courant -> afficher_opes )
	{
	  /* 	  si on affiche les op�s, on met les traits entre eux */

	  if ( etat_courant -> afficher_opes
	       &&
	       ligne_debut_partie != -1 )
	    {
	      gint i;
	      gint colonne;

	      colonne = 2;

	      for ( i=0 ; i<((nb_colonnes-2)/2) ; i++ )
		{
		  separateur = gtk_vseparator_new ();
		  gtk_table_attach ( GTK_TABLE ( table_etat ),
				     separateur,
				     colonne, colonne + 1,
				     ligne_debut_partie, ligne,
				     GTK_SHRINK | GTK_FILL,
				     GTK_SHRINK | GTK_FILL,
				     0, 0 );
		  gtk_widget_show ( separateur );

		  colonne = colonne + 2;
		}
	      ligne_debut_partie = -1;
	    }


	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  separateur = gtk_hseparator_new ();
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     separateur,
			     1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( separateur );

	  ligne++;

	  if ( nom_compte_en_cours )
	    label = gtk_label_new ( g_strconcat ( "Total ",
						  nom_compte_en_cours,
						  NULL ));
	  else
	    label = gtk_label_new ( "Total Compte : " );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_compte_etat,
						    devise_compte_en_cours_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
      else
	{
	  ligne--;

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_compte_etat,
						    devise_compte_en_cours_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
    }

  montant_compte_etat = 0;
  nom_compte_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les tiers sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gtktable_affiche_total_tiers ( gint ligne )
{
  GtkWidget *separateur;
  GtkWidget *label;

  if ( etat_courant -> utilise_tiers
       &&
       etat_courant -> affiche_sous_total_tiers )
    {
      /* si rien n'est affich� en dessous du tiers, on */
      /* met le r�sultat sur la ligne du tiers */
      /* sinon on fait une barre et on met le r�sultat */

      if ( etat_courant -> afficher_opes )
	{
	  /* 	  si on affiche les op�s, on met les traits entre eux */

	  if ( etat_courant -> afficher_opes
	       &&
	       ligne_debut_partie != -1 )
	    {
	      gint i;
	      gint colonne;

	      colonne = 2;

	      for ( i=0 ; i<((nb_colonnes-2)/2) ; i++ )
		{
		  separateur = gtk_vseparator_new ();
		  gtk_table_attach ( GTK_TABLE ( table_etat ),
				     separateur,
				     colonne, colonne + 1,
				     ligne_debut_partie, ligne,
				     GTK_SHRINK | GTK_FILL,
				     GTK_SHRINK | GTK_FILL,
				     0, 0 );
		  gtk_widget_show ( separateur );

		  colonne = colonne + 2;
		}
	      ligne_debut_partie = -1;
	    }


	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  separateur = gtk_hseparator_new ();
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     separateur,
			     1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( separateur );

	  ligne++;

	  if ( nom_tiers_en_cours )
	    label = gtk_label_new ( g_strconcat ( "Total ",
						  nom_tiers_en_cours,
						  NULL ));
	  else
	    label = gtk_label_new ( "Total Tiers : " );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_tiers_etat,
						    devise_tiers_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;

	  label = gtk_label_new ( "" );
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     1, GTK_TABLE ( table_etat ) -> ncols - 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
      else
	{
	  ligne--;

	  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
						    montant_tiers_etat,
						    devise_tiers_etat -> code_devise ));
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  ligne++;
	}
    }

  montant_tiers_etat = 0;
  nom_tiers_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gtktable_affichage_ligne_ope ( struct structure_operation *operation,
			   gint ligne )
{
  gint colonne;
  GtkWidget *label;


  if ( etat_courant -> afficher_opes )
    {
      /* on affiche ce qui est demand� pour les op�s */


      /* si les titres ne sont pas affich�s et qu'il faut le faire, c'est ici */

      if ( !titres_affiches
	   &&
	   etat_courant -> afficher_titre_colonnes
	   &&
	   etat_courant -> type_affichage_titres )
	ligne = gtktable_affichage . affiche_titres_colonnes ( ligne );

      colonne = 1;

      if ( etat_courant -> afficher_no_ope )
	{
	  label = gtk_label_new ( itoa ( operation -> no_operation ));
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     colonne, colonne + 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_date_ope )
	{
	  label = gtk_label_new ( g_strdup_printf  ( "%.2d/%.2d/%d",
						     operation -> jour,
						     operation -> mois,
						     operation -> annee ));
	  gtk_misc_set_alignment ( GTK_MISC ( label ),
				   0,
				   0.5 );
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     label,
			     colonne, colonne + 1,
			     ligne, ligne + 1,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( label );

	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_exo_ope )
	{
	  if ( operation -> no_exercice )
	    {
	      label = gtk_label_new ( ((struct struct_exercice *)(g_slist_find_custom ( liste_struct_exercices,
											GINT_TO_POINTER ( operation -> no_exercice ),
											(GCompareFunc) recherche_exercice_par_no )->data)) -> nom_exercice );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}


      if ( etat_courant -> afficher_tiers_ope )
	{
	  if ( operation -> tiers )
	    {
	      label = gtk_label_new ( ((struct struct_tiers *)(g_slist_find_custom ( liste_struct_tiers,
										     GINT_TO_POINTER ( operation -> tiers ),
										     (GCompareFunc) recherche_tiers_par_no )->data)) -> nom_tiers );
		      
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }

	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_categ_ope )
	{
	  gchar *pointeur;

	  pointeur = NULL;

	  if ( operation -> categorie )
	    {
	      struct struct_categ *categ;

	      categ = g_slist_find_custom ( liste_struct_categories,
					    GINT_TO_POINTER ( operation -> categorie ),
					    (GCompareFunc) recherche_categorie_par_no ) -> data;
	      pointeur = categ -> nom_categ;

	      if ( operation -> sous_categorie
		   &&
		   etat_courant -> afficher_sous_categ_ope )
		pointeur = g_strconcat ( pointeur,
					 " : ",
					 ((struct struct_sous_categ *)(g_slist_find_custom ( categ -> liste_sous_categ,
											     GINT_TO_POINTER ( operation -> sous_categorie ),
											     (GCompareFunc) recherche_sous_categorie_par_no ) -> data )) -> nom_sous_categ,
					 NULL );
	    }
	  else
	    {
	      /* si c'est un virement, on le marque, sinon c'est qu'il n'y a pas de categ */
	      /* ou que c'est une op� ventil�e, et on marque rien */

	      if ( operation -> relation_no_operation )
		{
		  /* c'est un virement */

		  p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> relation_no_compte;

		  if ( operation -> montant < 0 )
		    pointeur = g_strconcat ( "Virement vers ",
					     NOM_DU_COMPTE,
					     NULL );
		  else
		    pointeur = g_strconcat ( "Virement de ",
					     NOM_DU_COMPTE,
					     NULL );
		}
	    }

	  if ( pointeur )
	    {
	      label = gtk_label_new ( pointeur );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}
		


      if ( etat_courant -> afficher_ib_ope )
	{
	  if ( operation -> imputation )
	    {
	      struct struct_imputation *ib;
	      gchar *pointeur;

	      ib = g_slist_find_custom ( liste_struct_imputation,
					 GINT_TO_POINTER ( operation -> imputation ),
					 (GCompareFunc) recherche_imputation_par_no ) -> data;
	      pointeur = ib -> nom_imputation;

	      if ( operation -> sous_imputation
		   &&
		   etat_courant -> afficher_sous_ib_ope )
		pointeur = g_strconcat ( pointeur,
					 " : ",
					 ((struct struct_sous_imputation *)(g_slist_find_custom ( ib -> liste_sous_imputation,
												  GINT_TO_POINTER ( operation -> sous_imputation ),
												  (GCompareFunc) recherche_sous_imputation_par_no ) -> data )) -> nom_sous_imputation,
					 NULL );

	      label = gtk_label_new ( pointeur );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}


      if ( etat_courant -> afficher_notes_ope )
	{
	  if ( operation -> notes )
	    {
	      label = gtk_label_new ( operation -> notes );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_type_ope )
	{
	  GSList *pointeur;

	  p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> no_compte;

	  pointeur = g_slist_find_custom ( TYPES_OPES,
					   GINT_TO_POINTER ( operation -> type_ope ),
					   (GCompareFunc) recherche_type_ope_par_no );

	  if ( pointeur )
	    {
	      struct struct_type_ope *type;

	      type = pointeur -> data;

	      label = gtk_label_new ( type -> nom_type );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}


      if ( etat_courant -> afficher_cheque_ope )
	{
	  if ( operation -> contenu_type )
	    {
	      label = gtk_label_new ( operation -> contenu_type );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}


      if ( etat_courant -> afficher_pc_ope )
	{
	  if ( operation -> no_piece_comptable )
	    {
	      label = gtk_label_new ( operation -> no_piece_comptable );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_infobd_ope )
	{
	  if ( operation -> info_banque_guichet )
	    {
	      label = gtk_label_new ( operation -> info_banque_guichet );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_rappr_ope )
	{
	  GSList *pointeur;

	  pointeur = g_slist_find_custom ( liste_no_rapprochements,
					   GINT_TO_POINTER ( operation -> no_rapprochement ),
					   (GCompareFunc) recherche_no_rapprochement_par_no );

	  if ( pointeur )
	    {
	      struct struct_no_rapprochement *rapprochement;

	      rapprochement = pointeur -> data;
	      label = gtk_label_new ( rapprochement -> nom_rapprochement );
	      gtk_misc_set_alignment ( GTK_MISC ( label ),
				       0,
				       0.5 );
	      gtk_table_attach ( GTK_TABLE ( table_etat ),
				 label,
				 colonne, colonne + 1,
				 ligne, ligne + 1,
				 GTK_SHRINK | GTK_FILL,
				 GTK_SHRINK | GTK_FILL,
				 0, 0 );
	      gtk_widget_show ( label );
	    }
	  colonne = colonne + 2;
	}



      /* on affiche le montant au bout de la ligne */

      if ( devise_compte_en_cours_etat
	   &&
	   operation -> devise == devise_compte_en_cours_etat -> no_devise )
	label = gtk_label_new ( g_strdup_printf  ("%4.2f %s",
						  operation -> montant,
						  devise_compte_en_cours_etat -> code_devise ));
      else
	{
	  struct struct_devise *devise_operation;

	  devise_operation = g_slist_find_custom ( liste_struct_devises,
						   GINT_TO_POINTER ( operation -> devise ),
						   ( GCompareFunc ) recherche_devise_par_no ) -> data;
	  label = gtk_label_new ( g_strdup_printf  ("%4.2f %s",
						    operation -> montant,
						    devise_operation -> code_devise ));
	}

      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 GTK_TABLE ( table_etat ) -> ncols - 1, GTK_TABLE ( table_etat ) -> ncols,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      if ( ligne_debut_partie == -1 )
	ligne_debut_partie = ligne;

      ligne++;
    }
  return ( ligne );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
gint gtktable_affiche_total_partiel ( gdouble total_partie,
			     gint ligne,
			     gint type )
{
  GtkWidget *label;
  GtkWidget *separateur;

  /* 	  si on affiche les op�s, on met les traits entre eux */

  if ( etat_courant -> afficher_opes
       &&
       ligne_debut_partie != -1 )
    {
      gint i;
      gint colonne;

      colonne = 2;

      for ( i=0 ; i<((nb_colonnes-2)/2) ; i++ )
	{
	  separateur = gtk_vseparator_new ();
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     separateur,
			     colonne, colonne + 1,
			     ligne_debut_partie, ligne,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( separateur );

	  colonne = colonne + 2;
	}
      ligne_debut_partie = -1;
    }


  label = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     1, GTK_TABLE ( table_etat ) -> ncols - 1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  separateur = gtk_hseparator_new ();
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     separateur,
		     0, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( separateur );

  ligne++;

  if ( type )
    label = gtk_label_new ( "Total d�penses : " );
  else
    label = gtk_label_new ( "Total revenus : " );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     1, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
					    total_partie,
					    devise_generale_etat -> code_devise ));
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     GTK_TABLE ( table_etat ) -> ncols - 1, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  separateur = gtk_hseparator_new ();
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     separateur,
		     0, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( separateur );

  ligne++;

  label = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     1, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  nom_categ_en_cours = NULL;
  nom_ss_categ_en_cours = NULL;
  nom_ib_en_cours = NULL;
  nom_ss_ib_en_cours = NULL;
  nom_compte_en_cours = NULL;
  nom_tiers_en_cours = NULL;


  return ( ligne );
}
/*****************************************************************************************************/

/*****************************************************************************************************/
gint gtktable_affiche_total_general ( gdouble total_general,
			     gint ligne )
{
  GtkWidget *label;
  GtkWidget *separateur;

  /* 	  si on affiche les op�s, on met les traits entre eux */

  if ( etat_courant -> afficher_opes
       &&
       ligne_debut_partie != -1 )
    {
      gint i;
      gint colonne;

      colonne = 2;

      for ( i=0 ; i<((nb_colonnes-2)/2) ; i++ )
	{
	  separateur = gtk_vseparator_new ();
	  gtk_table_attach ( GTK_TABLE ( table_etat ),
			     separateur,
			     colonne, colonne + 1,
			     ligne_debut_partie, ligne,
			     GTK_SHRINK | GTK_FILL,
			     GTK_SHRINK | GTK_FILL,
			     0, 0 );
	  gtk_widget_show ( separateur );

	  colonne = colonne + 2;
	}
      ligne_debut_partie = -1;
    }

  label = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     1, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  separateur = gtk_hseparator_new ();
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     separateur,
		     0, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( separateur );

  ligne++;

  label = gtk_label_new ( "Total g�n�ral : " );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     1, GTK_TABLE ( table_etat ) -> ncols -1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  label = gtk_label_new ( g_strdup_printf ( "%4.2f %s",
					    total_general,
					    devise_generale_etat -> code_devise ));
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     GTK_TABLE ( table_etat ) -> ncols -1, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  separateur = gtk_hseparator_new ();
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     separateur,
		     0, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( separateur );

  ligne++;

  label = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     1, GTK_TABLE ( table_etat ) -> ncols,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  return ( ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
gint gtktable_affiche_categ_etat ( struct structure_operation *operation,
			  			  gchar *decalage_categ,
			  gint ligne )
{
  gchar *pointeur_char;
  GtkWidget *label;

  /* v�rifie qu'il y a un changement de cat�gorie */
  /* �a peut �tre aussi chgt pour virement, ventilation ou pas de categ */

  if ( etat_courant -> utilise_categ
       &&
       ( operation -> categorie != ancienne_categ_etat
	 ||
	 ( ancienne_categ_speciale_etat == 1
	   &&
	   !operation -> relation_no_operation )
	 ||
	 ( ancienne_categ_speciale_etat == 2
	   &&
	   !operation -> operation_ventilee )
	 ||
	 ( ancienne_categ_speciale_etat == 3
	   &&
	   ( operation -> operation_ventilee
	     ||
	     operation -> relation_no_operation ))))
    {

      /* lorsqu'on est au d�but de l'affichage de l'�tat, on n'affiche pas de totaux */

      if ( !debut_affichage_etat
	   &&
	   !changement_de_groupe_etat )
	{
	  /* on ajoute les totaux de tout ce qu'il y a derri�re la cat�gorie */

	  ligne = gtktable_affichage . affiche_totaux_sous_jaccent ( 1,
							     ligne );

	  /* on ajoute le total de la categ */

	  ligne = gtktable_affichage . affiche_total_categories ( ligne );
	}

      if ( operation -> categorie )
	{
	  nom_categ_en_cours = ((struct struct_categ *)(g_slist_find_custom ( liste_struct_categories,
									      GINT_TO_POINTER ( operation -> categorie ),
									      (GCompareFunc) recherche_categorie_par_no ) -> data )) -> nom_categ;
	  pointeur_char = g_strconcat ( decalage_categ,
					nom_categ_en_cours,
					NULL );
	  ancienne_categ_speciale_etat = 0;
	}

      else
	{
	  if ( operation -> relation_no_operation )
	    {
	      pointeur_char = g_strconcat ( decalage_categ,
					    "Virements",
					    NULL );
	      ancienne_categ_speciale_etat = 1;
	    }
	  else
	    {
	      if ( operation -> operation_ventilee )
		{
		  pointeur_char = g_strconcat ( decalage_categ,
						"Op�ration ventil�e",
						NULL );
		  ancienne_categ_speciale_etat = 2;
		}
	      else
		{
		  pointeur_char = g_strconcat ( decalage_categ,
						"Pas de cat�gorie",
						NULL );
		  ancienne_categ_speciale_etat = 3;
		}
	    }
	}

      label = gtk_label_new ( pointeur_char );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 0, 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      ligne++;

      ligne_debut_partie = ligne;
      denote_struct_sous_jaccentes ( 1 );

      ancienne_categ_etat = operation -> categorie;

      debut_affichage_etat = 0;
      changement_de_groupe_etat = 1;
    }

  return ( ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
gint gtktable_affiche_sous_categ_etat ( struct structure_operation *operation,
			       			       gchar *decalage_sous_categ,
			       gint ligne )
{
  gchar *pointeur_char;
  GtkWidget *label;

  if ( etat_courant -> utilise_categ
       &&
       etat_courant -> afficher_sous_categ
       &&
       operation -> categorie
       &&
       operation -> sous_categorie != ancienne_sous_categ_etat )
    {
      struct struct_categ *categ;

     /* lorsqu'on est au d�but de l'affichage de l'�tat, on n'affiche pas de totaux */

      if ( !debut_affichage_etat
	   &&
	   !changement_de_groupe_etat )
	{
	  /* on ajoute les totaux de tout ce qu'il y a derri�re la sous cat�gorie */

	  ligne = gtktable_affichage . affiche_totaux_sous_jaccent ( 2,
	ligne );

	  /* on ajoute le total de la sous categ */

	  ligne = gtktable_affichage . affiche_total_sous_categ ( ligne );
	}


      categ = g_slist_find_custom ( liste_struct_categories,
				    GINT_TO_POINTER ( operation -> categorie ),
				    (GCompareFunc) recherche_categorie_par_no ) -> data;

      if ( operation -> sous_categorie )
	{
	  nom_ss_categ_en_cours = ((struct struct_sous_categ *)(g_slist_find_custom ( categ->liste_sous_categ,
										      GINT_TO_POINTER ( operation -> sous_categorie ),
										      (GCompareFunc) recherche_sous_categorie_par_no ) -> data )) -> nom_sous_categ;
	  pointeur_char = g_strconcat ( decalage_sous_categ,
					nom_ss_categ_en_cours,
					NULL );
	}
      else
	{
	  if ( etat_courant -> afficher_pas_de_sous_categ )
	    pointeur_char = g_strconcat ( decalage_sous_categ,
					  "Pas de sous-cat�gorie",
					  NULL );
	  else
	    pointeur_char = "";
	}

      label = gtk_label_new ( pointeur_char );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 0,1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      ligne++;

      ligne_debut_partie = ligne;
      denote_struct_sous_jaccentes ( 2 );

     ancienne_sous_categ_etat = operation -> sous_categorie;

     debut_affichage_etat = 0;
     changement_de_groupe_etat = 1;
    }

  return ( ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gtktable_affiche_ib_etat ( struct structure_operation *operation,
		       		       gchar *decalage_ib,
		       gint ligne )
{
  gchar *pointeur_char;
  GtkWidget *label;

  /* mise en place de l'ib */


  if ( etat_courant -> utilise_ib
       &&
       operation -> imputation != ancienne_ib_etat )
    {
      /* lorsqu'on est au d�but de l'affichage de l'�tat, on n'affiche pas de totaux */

      if ( !debut_affichage_etat
	   &&
	   !changement_de_groupe_etat )
	{
	  /* on ajoute les totaux de tout ce qu'il y a derri�re l'ib */

	  ligne = gtktable_affichage . affiche_totaux_sous_jaccent ( 3,
	ligne );

	  /* on ajoute le total de l'ib */

	  ligne = gtktable_affichage . affiche_total_ib ( ligne );
	}
 
      if ( operation -> imputation )
	{
	  nom_ib_en_cours = ((struct struct_imputation *)(g_slist_find_custom ( liste_struct_imputation,
										GINT_TO_POINTER ( operation -> imputation ),
										(GCompareFunc) recherche_imputation_par_no ) -> data )) -> nom_imputation;
	  pointeur_char = g_strconcat ( decalage_ib,
					nom_ib_en_cours,
					NULL );
	}
      else
	pointeur_char = g_strconcat ( decalage_ib,
				      "Pas d'imputation budg�taire",
				      NULL );

      label = gtk_label_new ( pointeur_char );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 0, 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      ligne++;

      ligne_debut_partie = ligne;
      denote_struct_sous_jaccentes ( 3 );

      ancienne_ib_etat = operation -> imputation;

      debut_affichage_etat = 0;
      changement_de_groupe_etat = 1;
    }

  return ( ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gtktable_affiche_sous_ib_etat ( struct structure_operation *operation,
			    			    gchar *decalage_sous_ib,
			    gint ligne )
{
  gchar *pointeur_char;
  GtkWidget *label;


  /* mise en place de la sous_ib */


  if ( etat_courant -> utilise_ib
       &&
       etat_courant -> afficher_sous_ib
       &&
       operation -> imputation
       &&
       operation -> sous_imputation != ancienne_sous_ib_etat )
    {
      struct struct_imputation *imputation;

      /* lorsqu'on est au d�but de l'affichage de l'�tat, on n'affiche pas de totaux */

      if ( !debut_affichage_etat
	   &&
	   !changement_de_groupe_etat )
	{
	  /* on ajoute les totaux de tout ce qu'il y a derri�re la sous ib */

	  ligne = gtktable_affichage . affiche_totaux_sous_jaccent ( 4,
	ligne );

	  /* on ajoute le total de la sous ib */

	  ligne = gtktable_affichage . affiche_total_sous_ib ( ligne );
	}
 
      imputation = g_slist_find_custom ( liste_struct_imputation,
					 GINT_TO_POINTER ( operation -> imputation ),
					 (GCompareFunc) recherche_imputation_par_no ) -> data;

      if ( operation -> sous_imputation )
	{
	  nom_ss_ib_en_cours = ((struct struct_sous_imputation *)(g_slist_find_custom ( imputation->liste_sous_imputation,
											GINT_TO_POINTER ( operation -> sous_imputation ),
											(GCompareFunc) recherche_sous_imputation_par_no ) -> data )) -> nom_sous_imputation;
	  pointeur_char = g_strconcat ( decalage_sous_ib,
					nom_ss_ib_en_cours,
					NULL );
	}
      else
	{
	  if ( etat_courant -> afficher_pas_de_sous_ib )
	    pointeur_char = g_strconcat ( decalage_sous_ib,
					  "Pas de sous-imputation",
					  NULL );
	  else
	    pointeur_char = "";
	}

      label = gtk_label_new ( pointeur_char );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 0, 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      ligne++;

      ligne_debut_partie = ligne;
      denote_struct_sous_jaccentes ( 4 );

      ancienne_sous_ib_etat = operation -> sous_imputation;

      debut_affichage_etat = 0;
      changement_de_groupe_etat = 1;
    }

  return ( ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gtktable_affiche_compte_etat ( struct structure_operation *operation,
			   			   gchar *decalage_compte,
			   gint ligne )
{
  gchar *pointeur_char;
  GtkWidget *label;

  /* mise en place du compte */

  if ( etat_courant -> regroupe_ope_par_compte
       &&
       operation -> no_compte != ancien_compte_etat )
    {
      /* lorsqu'on est au d�but de l'affichage de l'�tat, on n'affiche pas de totaux */

      if ( !debut_affichage_etat
	   &&
	   !changement_de_groupe_etat )
	{
	  /* on ajoute les totaux de tout ce qu'il y a derri�re le compte */

	  ligne = gtktable_affichage . affiche_totaux_sous_jaccent ( 5,
	ligne );

	  /* on ajoute le total du compte */

	  ligne = gtktable_affichage . affiche_total_compte ( ligne );
	}
 
      p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> no_compte;

      pointeur_char = g_strconcat ( decalage_compte,
				    NOM_DU_COMPTE,
				    NULL );
      nom_compte_en_cours = NOM_DU_COMPTE;

      label = gtk_label_new ( pointeur_char );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 0, 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      ligne++;

      ligne_debut_partie = ligne;
      denote_struct_sous_jaccentes ( 5 );

      ancien_compte_etat = operation -> no_compte;

      debut_affichage_etat = 0;
      changement_de_groupe_etat = 1;
    }

  return ( ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gtktable_affiche_tiers_etat ( struct structure_operation *operation,
			  			  gchar *decalage_tiers,
			  gint ligne )
{
  gchar *pointeur_char;
  GtkWidget *label;

  /* affiche le tiers */

  if ( etat_courant -> utilise_tiers
       &&
       operation -> tiers != ancien_tiers_etat )
    {
      /* lorsqu'on est au d�but de l'affichage de l'�tat, on n'affiche pas de totaux */

      if ( !debut_affichage_etat
	   &&
	   !changement_de_groupe_etat )
	{
	  /* on ajoute les totaux de tout ce qu'il y a derri�re le tiers */

	  ligne = gtktable_affichage . affiche_totaux_sous_jaccent ( 6,
	ligne );

	  /* on ajoute le total du tiers */

	  ligne = gtktable_affichage . affiche_total_tiers ( ligne );
	}

      if ( operation -> tiers )
	{
	  nom_tiers_en_cours = ((struct struct_tiers *)(g_slist_find_custom ( liste_struct_tiers,
									      GINT_TO_POINTER ( operation -> tiers ),
									      (GCompareFunc) recherche_tiers_par_no ) -> data )) -> nom_tiers;
	  pointeur_char = g_strconcat ( decalage_tiers,
					nom_tiers_en_cours,
					NULL );
	}
      else
	pointeur_char = g_strconcat ( decalage_tiers,
				      "Pas de tiers",
				      NULL );

      label = gtk_label_new ( pointeur_char );
      gtk_misc_set_alignment ( GTK_MISC ( label ),
			       0,
			       0.5 );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 0, 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );


      ligne++;

      ligne_debut_partie = ligne;
      denote_struct_sous_jaccentes ( 6 );

      ancien_tiers_etat = operation -> tiers;

      debut_affichage_etat = 0;
      changement_de_groupe_etat = 1;
    }
  return ( ligne );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
gint gtktable_affiche_titre_revenus_etat ( gint ligne )
{
  GtkWidget *label;

  label = gtk_label_new ( "" );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     0, 1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;
  label = gtk_label_new ( "Revenus" );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     0, 1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  label = gtk_label_new ( "" );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     0, 1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  return ( ligne );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
gint gtktable_affiche_titre_depenses_etat ( gint ligne )
{
  GtkWidget *label;

  label = gtk_label_new ( "" );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     0, 1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  label = gtk_label_new ( "D�penses" );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     0, 1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;

  label = gtk_label_new ( "" );
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     label,
		     0, 1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( label );

  ligne++;


  return ( ligne );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* appel�e lors de l'affichage d'une structure ( cat�g, ib ... ) */
/* affiche le total de toutes les structures sous jaccentes */
/*****************************************************************************************************/

gint gtktable_affiche_totaux_sous_jaccent ( gint origine,
				   gint ligne )
{
  GList *pointeur_glist;

  /* on doit partir du bout de la liste pour revenir vers la structure demand�e */

  pointeur_glist = g_list_last ( etat_courant -> type_classement );


  while ( GPOINTER_TO_INT ( pointeur_glist -> data ) != origine )
    {
      switch ( GPOINTER_TO_INT ( pointeur_glist -> data ))
	{
	case 1:
	  ligne = gtktable_affichage . affiche_total_categories ( ligne );
	  break;

	case 2:
	  ligne = gtktable_affichage . affiche_total_sous_categ ( ligne );
	  break;

	case 3:
	  ligne = gtktable_affichage . affiche_total_ib ( ligne );
	  break;

	case 4:
	  ligne = gtktable_affichage . affiche_total_sous_ib ( ligne );
	  break;

	case 5:
	  ligne = gtktable_affichage . affiche_total_compte ( ligne );
	  break;

	case 6:
	  ligne = gtktable_affichage . affiche_total_tiers ( ligne );
	  break;
	}
      pointeur_glist = pointeur_glist -> prev;
    }

  return ( ligne );

}
/*****************************************************************************************************/



/*****************************************************************************************************/
gint gtktable_affiche_titres_colonnes ( gint ligne )
{
  gint colonne;
  GtkWidget *label;
  GtkWidget *separateur;

  colonne = 1;

  if ( etat_courant -> afficher_no_ope )
    {
      label = gtk_label_new ( "N�" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_date_ope )
    {
      label = gtk_label_new ( "Date" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_exo_ope )
    {
      label = gtk_label_new ( "Exercice" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_tiers_ope )
    {
      label = gtk_label_new ( "Tiers" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_categ_ope )
    {
      label = gtk_label_new ( "Cat�gorie" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_ib_ope )
    {
      label = gtk_label_new ( "Imputation budg�taire" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_notes_ope )
    {
      label = gtk_label_new ( "Notes" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_type_ope )
    {
      label = gtk_label_new ( "Type" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_cheque_ope )
    {
      label = gtk_label_new ( "Ch�que" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_pc_ope )
    {
      label = gtk_label_new ( "Pi�ce comptable" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_infobd_ope )
    {
      label = gtk_label_new ( "Info banque/guichet" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_rappr_ope )
    {
      label = gtk_label_new ( "Relev�" );
      gtk_table_attach ( GTK_TABLE ( table_etat ),
			 label,
			 colonne, colonne + 1,
			 ligne, ligne + 1,
			 GTK_SHRINK | GTK_FILL,
			 GTK_SHRINK | GTK_FILL,
			 0, 0 );
      gtk_widget_show ( label );

      colonne = colonne + 2;
    }


  ligne++;

  separateur = gtk_hseparator_new ();
  gtk_table_attach ( GTK_TABLE ( table_etat ),
		     separateur,
		     1, GTK_TABLE ( table_etat ) -> ncols - 1,
		     ligne, ligne + 1,
		     GTK_SHRINK | GTK_FILL,
		     GTK_SHRINK | GTK_FILL,
		     0, 0 );
  gtk_widget_show ( separateur );

  ligne++;

  titres_affiches = 1;

  return ( ligne );
}
/*****************************************************************************************************/


gint gtktable_finish ()
{
  /* Nothing to do in GTK */
}
