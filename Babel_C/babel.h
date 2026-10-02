#ifndef BABEL_H
#define BABEL_H
#define NBHEXAGONE 6 // Le nombre d'hexagones que contient la bibliothèque
#define NBETAGERE 5 // Le nombre d'étagères par hexagone
#define NBLIVRE 32 // Le nombre de livres par étagère
#define NBPAGE 410 // Le nombre de pages par livre
#define NBLIGNE 40 // Le nombre de ligne par page
#define NBCHAR 79 // Le nombre de caractère par page (-1)
#define JAUNE     "\033[33m"
#define RESET     "\033[0m"
#define BG_JAUNE   "\033[43m"
#define BG_VERT    "\033[42m"
#define BG_BLEU    "\033[44m"
#define BG_CYAN    "\033[46m"
#define BG_ROUGE    "\033[41m"
#define BG_MAGENTA   "\033[45m"
#define BG_BLANC   "\033[47m"
#define BG_NOIR   "\033[40m"
#define NOIR      "\033[30m"
#define ROUGE     "\033[31m"
#define CYAN      "\033[36m"

typedef struct livre_struct{
    char contenu_book [NBPAGE][NBLIGNE][NBCHAR];
}livre_t;

typedef struct biblioteque_babel_struct{
    livre_t* contenu_biblio [NBHEXAGONE][NBETAGERE][NBLIVRE];
}biblioteque_babel_t;
/**
 * " ____       ____       ____ \n"
 * "//1.\\     //2.\\     //3.\\ \n"
 * "\\__//     \\__//     \\__//\n \n"
 * 
 * " ____       ____       ____ \n"
 * "//4.\\     //5.\\     //6.\\ \n"
 * "\\__//     \\__//     \\__// \n \n"
 *  
*/


#endif