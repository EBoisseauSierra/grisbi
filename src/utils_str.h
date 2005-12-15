#ifndef _UTILS_STR_H
#define _UTILS_STR_H (1)
/* START_INCLUDE_H */
#include "utils_str.h"
/* END_INCLUDE_H */


/* START_DECLARATION */
gchar *get_line_from_string ( gchar *string );
gchar * gsb_string_escape_underscores ( gchar * orig );
GSList *gsb_string_get_list_from_string ( const gchar *string,
					  gchar *delimiter );
gchar * gsb_string_truncate ( gchar * string );
gchar * latin2utf8 ( char * inchar);
gchar *limit_string ( gchar *string,
		      gint length );
gint my_strcasecmp ( gchar *chaine_1,
		     gchar *chaine_2 );
gchar *my_strdelimit ( const gchar *string,
		       gchar *delimiters,
		       gchar *new_delimiters );
gchar *my_strdup ( const gchar *string );
gint my_strncasecmp ( gchar *chaine_1,
		      gchar *chaine_2,
		      gint longueur );
double my_strtod ( const char *nptr, const char **endptr );
gint utils_str_atoi ( const gchar *chaine );
gchar *utils_str_itoa ( gint integer );
/* END_DECLARATION */
#endif
