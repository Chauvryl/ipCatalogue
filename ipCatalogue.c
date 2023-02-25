#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <arpa/inet.h>

#define MAX_LINE_LENGTH 18
#define MAX_LINE 300


// Définition des adresses IP spéciales sous forme de macros
#define LOOPBACK_IP "127.0.0.1"
#define BROADCAST_IP "255.255.255.255"
#define LINK_LOCAL_PREFIX "169.254.0.0"

// Cette fonction prend en entrée une chaîne de caractères représentant une adresse IP et vérifie son format et le nombre d'octets maximum
int check_ip_address(const char* str)
{
    regex_t regex;
    int reti;
    char msgbuf[100];

    // Le motif d'expression régulière pour la validation d'une adresse IP
    char* pattern = "^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";

    // Compilation du motif d'expression régulière
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Impossible de compiler l'expression régulière\n");
        return -1;
    }

    // Exécution du motif d'expression régulière sur la chaîne d'entrée
    reti = regexec(&regex, str, 0, NULL, 0);
    if (!reti) {
        printf("Adresse IP valide\n");
        return 0;
    }
    else if (reti == REG_NOMATCH) {
        printf("Adresse IP invalide\n");
        return -1;
    }
    else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Échec de la correspondance de l'expression régulière : %s\n", msgbuf);
        return -1;
    }

    // Libération de la mémoire utilisée pour stocker la structure de l'expression régulière compilée
    regfree(&regex);
}
 
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

int is_private_ipv4(struct in_addr addr) {
    // Vérifie si l'adresse appartient au bloc 10.0.0.0/8
    if ((ntohl(addr.s_addr) & 0xff000000) == 0x0a000000) {
        return 1;
    }
    // Vérifie si l'adresse appartient au bloc 172.16.0.0/12
    else if ((ntohl(addr.s_addr) & 0xfff00000) == 0xac100000) {
        return 1;
    }
    // Vérifie si l'adresse appartient au bloc 192.168.0.0/16
    else if ((ntohl(addr.s_addr) & 0xffff0000) == 0xc0a80000) {
        return 1;
    }
    // L'adresse n'est pas privée
    else {
        return 0;
    }
}

int is_special_ipv4(struct in_addr addr) {
    // Vérifie si l'adresse appartient au bloc 0.0.0.0/8
    if ((ntohl(addr.s_addr) & 0xff000000) == 0x00000000) {
        return 1;
    }
    // Vérifie si l'adresse appartient au bloc 127.0.0.0/8
    else if ((ntohl(addr.s_addr) & 0xff000000) == 0x7f000000) {
        return 1;
    }
    // Vérifie si l'adresse appartient au bloc 169.254.0.0/16
    else if ((ntohl(addr.s_addr) & 0xffff0000) == 0xa9fe0000) {
        return 1;
    }
    // Vérifie si l'adresse appartient au bloc 224.0.0.0/4
    else if ((ntohl(addr.s_addr) & 0xf0000000) == 0xe0000000) {
        return 1;
    }
    // Vérifie si l'adresse appartient au bloc 240.0.0.0/4
    else if ((ntohl(addr.s_addr) & 0xf0000000) == 0xf0000000) {
        return 1;
    }
    // L'adresse n'est pas spéciale
    else {
        return 0;
    }
}

int get_ipv4_type(struct in_addr addr) {
    // Vérifie si l'adresse est privée
    if (is_private_ipv4(addr)) {
        return 1;   // l'adresse IP est privée
    }
    // Vérifie si l'adresse est spéciale
    else if (is_special_ipv4(addr)) {
        return 2;   // l'adresse IP est spéciale
    }
    // L'adresse est publique
    else {
        return 0;
    }
}



char* detailIP(const char* ip)
{
    system("clear");
    struct in_addr addr;

    inet_aton(ip, &addr);

    printf("Adresse IP en décimal : %s\n", inet_ntoa(addr));

    printf("Voici l'adresse IP en Hexadecimal : 0x%02x%02x%02x%02x\n",
    addr.s_addr & 0xff,
    (addr.s_addr >> 8) & 0xff,
    (addr.s_addr >> 16) & 0xff,
    (addr.s_addr >> 24) & 0xff);

    unsigned char *ptr = (unsigned char *)&addr.s_addr;
    printf("Adresse IP en binaire : ");
    for (int i = 0; i < 4; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (ptr[i] >> j) & 1);
        }
        if (i < 3) {
            printf(".");
        }
    }
    printf("\n");
    printf("Adresse IP de type : ");
    switch (get_ipv4_type(addr))
    {
    case 0:
        printf("Publique\n");
        break;
    case 1:
        printf("privée\n");
        break;
    case 2:
        printf("Spéciale\n");
        break;
    }
        return 0;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////Main program //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    char addIPdetail[20];

    // afficher les lignes stockées dans le tableau de chaînes de caractères
    for (int j = 0; j < i; j++) {
        printf("%s", lines[j]);
    }
        switch (affichageMenu())
        {
        case 1:
            printf("Merci de renseigné une IP à enregisté");           
            break;
        case 2:
            printf("affichage par masque");
            break;
        case 3:
            
            printf("\nRenseigné une adresse IP pour plus de détail\n");
            scanf("%s",&addIPdetail);
            while (check_ip_address(addIPdetail)!=0){
                printf("\nErreur, l'adresse renseigné n'est pas correcte, merci de renseigné une adresse ip correcte : \n");
                scanf("%s",&addIPdetail);
            }
            
            detailIP(addIPdetail);

            break;
        case 5:
            // int suppIP;
            // printf("Renseigné une adresse IP à supprimer");
            // scanf("%d", &suppIP)
            // adresseSupp(suppIP);
            break;
        case 6:
            printf("Au revoir !!!");
            break;
        default:
            printf("Vous ne ferez rien du tout!");
            break;
    }


        // libérer la mémoire allouée pour le tableau de chaînes de caractères
    for (int j = 0; j < i; j++) {
        free(lines[j]);
    }

    return 0;
}