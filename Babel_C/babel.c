#include "babel.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

void pressEnter(){
    while(getchar() != '\n');
}

void refresh(){
    short int c;
    while((c = getchar()) != '\n' && c != EOF);
}

livre_t* init_livre(void){ /** Initialise un livre (alloue l'espace mémoire suffisant) */
    livre_t* livre = (livre_t*)malloc(sizeof(livre_t));
    return livre;
}

char carac(void){ // Récupère de manière aléatoire un caractère (alphabet minuscule +  espace, virgule, point, appostrophe)
    short int valeur = rand() % 30; // Aléatoire de 0 à 29 (30 exclu)
    if (valeur+1 < 27){ // on renvoie une lettre de l'alphabet
        return (char)valeur+97;
    }else{
        if(valeur == 27){
            return ' ';
        }else if(valeur == 28){
            return ',';
        }else if(valeur == 29){
            return '.';
        }else {
            return ' ';
        }
    }

}
char* bg_aleat(void){
    int al = rand() % 8;
    if(al == 0){
        return BG_JAUNE;
    }else if(al == 1){
        return BG_CYAN;
    }else if(al == 2){
        return BG_BLEU;
    }else if(al == 3){
        return BG_VERT;
    }else if(al == 4){
        return BG_MAGENTA;
    }else if(al == 5){
        return BG_BLANC;
    }else if(al == 6){
        return BG_NOIR;
    }else if(al == 7){
        return BG_ROUGE;
    }
}

bool est_identique(char modele [], char comparaison [], int taille){ /** Regarde si deux chaines de caractères sont identiques */
    for(int i = 0; i < taille; i++){
        if((int)comparaison[i] != (int)modele[i]){
            return false;
        }
    }
    return true;
}

char* bout_de_chaine(char str [], int start_point, int end_point, char chaine [NBCHAR]){ /** Récupère le bout d'une chaine de caractère (Inclu le point de départ mais pas le point d'arrivée) */
    int index = 0;
    for(int i = start_point; i < end_point; i++){
        chaine[index] = str[i];
        index++;
    }
    return chaine;
}

void init_livre_pages(livre_t* livre){ /** Ajouer les caractères dans le livre */
    for(int i = 0; i < NBPAGE; i++){
        for(int j = 0; j < NBLIGNE; j++){
            for(int k = 0; k < NBCHAR; k++){
                livre->contenu_book[i][j][k] = carac();
            }
        }
    }
}

void print_page(livre_t* livre, int page){ /** Permet d'afficher une page donné */
    system("clear");
    for(int i = 0; i < NBLIGNE; i++){
        for (int j = 0; j < NBCHAR; j++){
            printf("%c", livre->contenu_book[page][i][j]);
        }
        printf("\n");
    }
    if(page+1 < 10){
        printf("%s------------------------------- Page : 00%d / 410 ------------------------------%s\n\n", CYAN, page+1, RESET);
    }else if(page+1 < 100){
        printf("%s-------------------------------- Page : 0%d / 410 --------------------------------%s\n\n", CYAN, page+1, RESET);
    }else{
        printf("%s-------------------------------- Page : %d / 410 --------------------------------%s\n\n", CYAN, page+1, RESET);
    }
}

void find_word_in_page(livre_t* livre, int page){ /** Regarde si il y a des occurences d'un mot / groupe de mot dans une page du livre */
    char search [NBCHAR]; /** Le mot / groupe de mot à rechercher dans une page */
    char compare [NBCHAR]; /** Le bout dont on doit vérifier si il est identique à search */
    int index_occurence = 0; /** Index permettant de remplir la liste du début des occurences et la liste de fin des occurences (=> est égal à leurs taille) */
    int occurence_begin [NBLIGNE * NBCHAR][2]; /** La position de début des occurences trouvés  2D : position par rapport à la ligne + position du 1er caractère */
    int occurence_end [NBLIGNE * NBCHAR][2]; /** La fin des occurences trouvés  2D : position par rapport à la ligne + position du 1er caractère */
    printf("Le mot / groupe de mot à rechercher : ");
    fgets(search, sizeof(search), stdin);
    printf("%d\n\n\n\n", strlen(search));
    system("clear");
    int taille = strlen(search)-1;
    int index_end = 0; /** Parcourir la liste indiquant la fin des occurences */
    for(int ligne = 0; ligne < NBLIGNE; ligne++){ // Parcours la page ligne par ligne
        for(int indice_char = 0; indice_char < NBCHAR-taille+1; indice_char++){ // Parcours la ligne caractère par caractère
            int k = 0; // Indice pour remplir le bout de chaine que l'on va comparer
            for(int caractere = indice_char; k < taille; caractere++){ // Permet d'initialiser le bout de caractère que l'on va vérifier 
                compare[k] = livre->contenu_book[page][ligne][caractere];
                k++;
            }
            if(est_identique(search, compare, taille)){ // Le bout de chaine comparer est identique à celui rechercher : on le note dans la liste des occurences
                occurence_begin[index_occurence][0] = ligne;
                occurence_begin[index_occurence][1] = indice_char;
                if(indice_char+taille >= NBCHAR){
                    // On ne place pas de fin d'occurence si l'occurence ce situe à la toute fin d'une ligne
                }else{
                    occurence_end[index_end][0] = ligne;                 //|____ On indique la position de fin de l'occurence (où elle se termine)
                    occurence_end[index_end][1] = indice_char + taille;
                    index_end++;
                }
                
                index_occurence++;
            }
            memset(compare, 0, sizeof(compare)); // Vider la liste
        }
    }

    ///////////////////////////////////////// Afficher la page en mettant en évidence les occurences trouvés /////////////////////////////////////////
    int index_begin = 0; /** Parcourir la liste indiquant le début des occurences */
    index_end = 0; /** Parcourir la liste indiquant la fin des occurences */
    for(int ligne = 0; ligne < NBLIGNE; ligne++){ // On parcour la page ligne par ligne
        for (int carac = 0; carac < NBCHAR; carac++){ // On parcour une ligne caractère par caractères
            if(index_begin < index_occurence && occurence_begin[index_begin][0] == ligne && occurence_begin[index_begin][1] == carac){ // On est dans un endroit où une occurence du mot chercher à été trouvé
                printf("%s%s%c", BG_JAUNE, NOIR, livre->contenu_book[page][ligne][carac]);
                index_begin ++;
                if(index_end < index_occurence && occurence_end[index_end][0] == ligne && occurence_end[index_end][1] == carac){ // Pour que le programme ne bug pas si 2 occurences se succèdent
                    index_end++;
                }
            }else if(occurence_end[index_end][0] == ligne && occurence_end[index_end][1] == carac){ // On a atteind la fin d'une occurence, on affiche normalement
                printf("%s%c", RESET, livre->contenu_book[page][ligne][carac]);
                index_end += (index_end+1 < index_occurence);
            }else{
                printf("%c", livre->contenu_book[page][ligne][carac]); // On est à un endroit quelconque dans la page (ni au début, dans ou à la fin d'une occurence) 
            }
        }
        printf("\n%s", RESET); // On saute une ligne après qu'une ligne de la page ait été affiché
    }
    if(page+1 < 10){ // Permet de faire un joli affichage si le numéro n'est qu'à 1 chiffre
        printf("%s------------------------------- Page : 00%d / 410 ------------------------------%s\n\n", CYAN, page+1, RESET);
    }else if(page+1 < 100){ // Permet de faire un joli affichage si le numéro n'est qu'à 2 chiffres
        printf("%s-------------------------------- Page : 0%d / 410 --------------------------------%s\n\n", CYAN, page+1, RESET);
    }else{ // Permet de faire un joli affichage si le numéro est à 3 chiffres
        printf("%s-------------------------------- Page : %d / 410 --------------------------------%s\n\n", CYAN, page+1, RESET);
    }
}

int action(void){ /** Demande une action à faire à l'utilisateur */
    int act = 0;
    while(act <= 0 || act > 8){
        printf("Que voulez-vous faire ? (1. Page suivante, 2. Page Précédente, 3. Première Page, 4. Dernière Page, 5. Rechercher dans la page, \n6. Choisir un autre livre, 7. Choisir une autre étagère, 8. Chosir un autre hexagone) : ");
        scanf("%d", &act);   
    }     
    refresh();
    return act;
}

void init_bibliotheque(biblioteque_babel_t* biblio){ /** Initialise (rempli) la bibliothèque de babel */
    float process = 0;
    printf("---------- Bienvenue dans la Bibliothèque de Babel ! ----------\n");
    printf("Initialisation... (Peut prendre quelques minutes...)\n");
    for(int hexagone = 0; hexagone < NBHEXAGONE; hexagone++){ // Hexagone par exagone
        for(int etagere = 0; etagere < NBETAGERE; etagere++){ // Etagère par étagère dans un hexagone
            for(int livre = 0; livre < NBLIVRE; livre++){ // Livre par livre dans une étagère
                srand(time(NULL));
                livre_t* bouqin = init_livre(); /** Livre qu'on initialise qui sera ajouter à la bibliothèque */
                init_livre_pages(bouqin);
                biblio->contenu_biblio[hexagone][etagere][livre] = bouqin;
            }
        }
    }
    system("clear");
}

void print_etagere_hexa(void){
    system("clear");
    printf("                          _________________________   \n");
    printf("                     1.  | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s |  \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("                         | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s |  \n",bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("                         | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s |  \n",bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("                         | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s |  \n",bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("         _______________ |_________________________| ______________   \n");
    printf("        / %s__%s %s__%s %s__%s %s__%s  /                             \\  %s__%s %s__%s %s__%s %s__%s \\ \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("       / %s__%s %s__%s %s__%s %s__%s  /                               \\  %s__%s %s__%s %s__%s %s__%s \\ \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("      / %s__%s %s__%s %s__%s %s__%s  /                                 \\  %s__%s %s__%s %s__%s %s__%s \\ \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("     / %s__%s %s__%s %s__%s %s__%s  /                                   \\  %s__%s %s__%s %s__%s %s__%s \\ \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("    / %s__%s %s__%s %s__%s %s__%s  /  2.                             3.  \\  %s__%s %s__%s %s__%s %s__%s \\ \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("   / %s__%s %s__%s %s__%s %s__%s  /                                       \\  %s__%s %s__%s %s__%s %s__%s \\ \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("  / %s__%s %s__%s %s__%s %s__%s  /                                         \\  %s__%s %s__%s %s__%s %s__%s \\ \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf(" / %s__%s %s__%s %s__%s %s__%s  /                                           \\  %s__%s %s__%s %s__%s %s__%s \\ \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("/______________/                                             \\______________\\ \n");
    printf("        _________________________         _________________________ \n");
    printf("       | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s |       | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s | \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("       | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s |       | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s | \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("       | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s |       | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s | \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("       | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s |       | %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s %s__%s | \n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("       |_________________________|       |_________________________| \n");
    printf("                 4.                                       5.\n");

}

void print_hexgone(void){
    system("clear");
    printf(" ____       ____       ____ \n");
    printf("//1.\\\\     //2.\\\\     //3.\\\\ \n");
    printf("\\\\__//     \\\\__//     \\\\__//\n \n");
    printf(" ____       ____       ____ \n");
    printf("//4.\\\\     //5.\\\\     //6.\\\\ \n");
    printf("\\\\__//     \\\\__//     \\\\__// \n \n");
}

void print_etagere(void){
    system("clear");
    printf(" ________________________________\n");
    printf("| %s01%s  %s02%s  %s03%s  %s04%s  %s05%s  %s06%s  %s07%s  %s08%s |\n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("| %s09%s  %s10%s  %s11%s  %s12%s  %s13%s  %s14%s  %s15%s  %s16%s |\n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("| %s17%s  %s18%s  %s19%s  %s20%s  %s21%s  %s22%s  %s23%s  %s24%s |\n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("| %s25%s  %s26%s  %s27%s  %s28%s  %s29%s  %s30%s  %s31%s  %s32%s |\n", bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET, bg_aleat(), RESET);
    printf("|________________________________|\n");
}

int choice_hexagone(void){
    int action = 0;
    while(0 >= action || action > NBHEXAGONE){
        printf("Quel hexagone souhaitez-vous parcourir ? ");
        scanf("%d", &action);
    }
    return action-1;
}

int choice_etagere(void){
    int action = 0;
    while(0 >= action || action > NBETAGERE){
        printf("Quelle étagère souhaitez-vous parcourir ? ");
        scanf("%d", &action);
    }
    return action-1;
}

int choice_livre(void){
    int action = 0;
    while(0 >= action || action > NBLIVRE){
        printf("Quel livre souhaitez-vous prendre ? ");
        scanf("%d", &action);
    }
    return action-1;
}

int main(void)
{
    // print_etagere();
    biblioteque_babel_t babel;
    init_bibliotheque(&babel);
    print_hexgone();
    int hexagone = choice_hexagone();
    print_etagere_hexa();
    int etagere = choice_etagere();
    print_etagere();
    int livre = choice_livre();
    int page = 0; /** Indice pour savoir à quelle page on est */
    int howDo = 0; /** Savoir quelle action faire */
    print_page(babel.contenu_biblio[hexagone][etagere][livre], page);
    while (true)
    {
        howDo = action();
        if(howDo == 1){ // On affiche la page suivante
            page += (page+1 < 410);
            print_page(babel.contenu_biblio[hexagone][etagere][livre], page);
        }else if(howDo == 2){ // On affiche la page précédente
            page -= (page > 0);
            print_page(babel.contenu_biblio[hexagone][etagere][livre], page);
        }else if(howDo == 3){ // On affiche la 1ère page
            page = 0;
            print_page(babel.contenu_biblio[hexagone][etagere][livre], page);
        }else if(howDo == 4){ // On affiche la dernière page
            page = NBPAGE-1;
            print_page(babel.contenu_biblio[hexagone][etagere][livre], page);
        }else if(howDo == 5){ // On cherche une occurence d'un mot / groupe de mot / lettre dans la page
            find_word_in_page(babel.contenu_biblio[hexagone][etagere][livre], page);
        }else if(howDo == 6){ // On choisit un autre livre
            print_etagere();
            livre = choice_livre();
            page = 0;
            print_page(babel.contenu_biblio[hexagone][etagere][livre], page);
        }else if(howDo == 7){ // On choisit une autre étagère (donc aussi un autre livre)
            print_etagere_hexa();
            etagere = choice_etagere();
            print_etagere();
            livre = choice_livre();
            page = 0;
            print_page(babel.contenu_biblio[hexagone][etagere][livre], page);
        }else if(howDo == 8){ // On choisit un autre hexagone (donc aussi une autre étagère + un autre livre par extension)
            print_hexgone();
            hexagone = choice_hexagone();
            print_etagere_hexa();
            etagere = choice_etagere();
            print_etagere();
            livre = choice_livre();
            page = 0;
            print_page(babel.contenu_biblio[hexagone][etagere][livre], page);
        }
    }
    return EXIT_SUCCESS;
}