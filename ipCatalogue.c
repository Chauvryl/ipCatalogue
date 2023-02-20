#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 18
#define MAX_LINE 300


 
int affichageMenu(void)
{
    int choixMenu;
 
    printf("---Menu---\n\n");
    printf("1.Ajouter une IPs\n");
    printf("2.Afficher les IPs par un masque\n");
    printf("3.Détail sur une IP donnée\n");
    printf("4.Supprimer une adresse ip\n");
    printf("5.Exit\n\n\n");
    printf("\nVotre choix?\n\n");
    scanf("%d", &choixMenu);
    return choixMenu;
}



int main()
{
    char* lines[MAX_LINE];
    char buffer[MAX_LINE_LENGTH];
    int i = 0;



    int fic;
    fic=open("ipCatalogue.txt", O_RDWR | O_APPEND);

    if (fic == -1){
        printf("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    int bits;

    while((bits = read( fic , buffer , MAX_LINE_LENGTH )) > 0 && i < MAX_LINE ){

        lines[i] = malloc(bits + 1);
        if (lines[i] == NULL) {
            printf("Erreur d'allocation mémoire.\n");
            exit(1);
        }
        strncpy(lines[i], buffer, bits);
        lines[i][bits] = '\0';
        i++;
    }



    // afficher les lignes stockées dans le tableau de chaînes de caractères
    for (int j = 0; j < i; j++) {
        printf("%s", lines[j]);
    }

    // libérer la mémoire allouée pour le tableau de chaînes de caractères
    for (int j = 0; j < i; j++) {
        free(lines[j]);
    }



    //     switch (affichageMenu())
    //     {
    //     case 1:
    //         printf("Merci de renseigné une IP à enregist");           
    //         break;
    //     case 2:
    //         printf("affichage par masque");
    //         break;
    //     case 3:
    //         printf("Détail IP");
    //         break;
    //     case 5:
    //         int suppIP;
    //         printf("Renseigné une adresse IP à supprimer");
    //         scanf("%d", &suppIP)
    //         adresseSupp(suppIP);
    //         break;
    //     case 6:
    //         printf("Au revoir !!!");
    //         break;
    //     default:
    //         printf("Vous ne ferez rien du tout!");
    //         break;
    // }

    return 0;
}