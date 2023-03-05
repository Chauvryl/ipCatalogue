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

void put_ip(char *adressIP, int masque) {
    int fic;
    char *name;

    switch (masque) {
        case 1:
            name = "../masque_8.txt";
            break;
        case 2:
            name = "../masque_16.txt";
            break;
        case 3:
            name = "../masque_24.txt";
            break;
        case 4:
            name = "../masque_32.txt";
            break;
        default:
            printf("Choix invalide.\n");
            return ;
    }
    fic = open(name, O_RDWR | O_APPEND);
    if (fic == -1) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return ;
    }
    int i = 0;
    while (adressIP[i])
        i++;
    write(fic, adressIP, i);
    write(fic, "\n", 1);
    close(fic);
}

void detect_ip(char *adresseIP) {
    int premierOctet;

    premierOctet = atoi(adresseIP);
    if (premierOctet >= 1 && premierOctet <= 126) {
        put_ip(adresseIP, 1);
        printf("L'adresse IP %s a été ajouté au fichier masque_8.\n", adresseIP);
    } else if (premierOctet >= 128 && premierOctet <= 191) {
        put_ip(adresseIP, 2);
        printf("L'adresse IP %s a été ajouté au fichier masque_16.\n", adresseIP);
    } else if (premierOctet >= 192 && premierOctet <= 223) {
        put_ip(adresseIP, 3);
        printf("L'adresse IP %s a été ajouté au fichier masque_24.\n", adresseIP);
    } else {
        put_ip(adresseIP, 4);
        printf("L'adresse IP %s a été ajouté au fichier masque_32.\n", adresseIP);
    }
}

// Fonction pour ajouter une adresse IP au tableau de lignes
char **ajout_ip(char** ip_array, int *ip_count) {
    printf("Nombre d'adresses IP dans le tableau : %d\n", *ip_count);

    char ip[MAX_LINE_LENGTH+1];
    char **cpy_tab = NULL;
    int i;

    printf("Entrez une adresse IP : ");
    scanf("%s", ip);

    while (check_ip_address(ip) != 0) {
        printf("Erreur, l'adresse renseignée n'est pas correcte, merci de renseigner une adresse IP correcte : ");
        scanf("%s", ip);
    }

    cpy_tab = malloc((*ip_count + 1) * sizeof(char*));

    if (cpy_tab == NULL) {
        fprintf(stderr, "Erreur : échec de l'allocation de mémoire pour le tableau d'adresses IP.\n");
        return NULL;
    }

    for (i = 0; i < *ip_count; i++) {
        int len = strlen(ip_array[i]);
        cpy_tab[i] = malloc((len+1) * sizeof(char));

        if (cpy_tab[i] == NULL) {
            fprintf(stderr, "Erreur : échec de l'allocation de mémoire pour l'adresse IP numéro %d.\n", i);
            return NULL;
        }

        strcpy(cpy_tab[i], ip_array[i]);
    }

    int len = strlen(ip);
    cpy_tab[i] = malloc((len+1) * sizeof(char) + 1);

    if (cpy_tab[i] == NULL) {
        fprintf(stderr, "Erreur : échec de l'allocation de mémoire pour l'adresse IP numéro %d.\n", i);
        return NULL;
    }

    cpy_tab[i][0] = '\0';
    strcat(cpy_tab[i], ip);
    detect_ip(ip);
    printf("L'adresse IP %s a été ajoutée au tableau.\n", ip);
    (*ip_count)++;
    return cpy_tab;
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

void afficher_ips(char** ip_array, int ip_count) {
    printf("\n");
    for (int j = 0; j < ip_count; j++) {
        if (ip_array[j] && ip_array[j][0] != '\n')
            printf("%s\n" ,ip_array[j]);
    }
    printf("\n\n");
}

char		**ft_get_tab(char const *s, char c)
{
	int		i;
	int		segement;
	char	**tab;

	segement = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			segement++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	if (!(tab = malloc(sizeof(*tab) * (segement + 1))))
		return (0);
	tab[segement] = 0;
	return (tab);
}

char		*ft_fill_tab(char const *s, char c, char *tab, int i)
{
	int		index;

	index = 0;
	while (s[i] && s[i] != c)
	{
		tab[index] = s[i];
		index++;
		i++;
	}
	return (tab);
}

int			ft_get_segement(char const *s, char c, int *i)
{
	int		segement;

	segement = 0;
	while (s[*i] && s[*i] != c)
	{
		segement++;
		*i = *i + 1;
	}
	return (segement);
}

char		**ft_split(char const *s, char c)
{
	int		i;
	int		line;
	int		segement;
	char	**tab;

	if (!s)
		return (0);
	if (!(tab = ft_get_tab(s, c)))
		return (0);
	i = 0;
	line = -1;
	while (s[i])
	{
		if (s[i] != c)
		{
			segement = ft_get_segement(s, c, &i);
			if (!(tab[++line] = malloc(sizeof(**tab) * (segement + 1))))
				return (0);
			tab[line] = ft_fill_tab(s, c, tab[line], i - segement);
			tab[line][segement] = '\0';
		}
		else
			i++;
	}
	return (tab);
}

char		*ft_allocmemory(char const *s1, char const *s2, char *copy)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		copy[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		copy[j] = s2[i];
		i++;
		j++;
	}
	copy[j] = '\0';
	return (copy);
}

char		*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*copy;

	if (!s1)
		return (0);
	if (!s2)
		return (0);
	i = 0;
	j = 0;
	copy = NULL;
	i = strlen(s1);
	j = strlen(s2);
	copy = malloc(sizeof(*copy) * (i + j + 1));
	if (copy == NULL)
		return (NULL);
	copy = ft_allocmemory(s1, s2, copy);
	return (copy);
}

void menu_filtre() {
    int choix;
    int fic;
    char *name;
    
    printf("Choisissez la taille du masque : \n");
    printf("1. 8 bits\n");
    printf("2. 16 bits\n");
    printf("3. 24 bits\n");
    printf("4. 32 bits\n");
    scanf("%d", &choix);
    
    switch (choix) {
        case 1:
            name = "masque_8.txt";
            break;
        case 2:
            name = "masque_16.txt";
            break;
        case 3:
            name = "masque_24.txt";
            break;
        case 4:
            name = "masque_32.txt";
            break;
        default:
            printf("Choix invalide.\n");
            return ;
    }
    fic = open(name, O_RDWR | O_APPEND);
    if (fic == -1)
    {
        printf("erreur lors de l'ouverture du fichier\n");
        return ;
    }

    int bits;
    char *tmp = NULL;
    char *join = NULL;
    char buffer[MAX_LINE_LENGTH];
    while((bits = read( fic , buffer , MAX_LINE_LENGTH )) > 0){
        buffer[bits] = '\0';
        if (join)
        {
            tmp = strdup(join);
            free(join);
            join = ft_strjoin(tmp, buffer);
            free(tmp);
        }
        else
            join = strdup(buffer);
    }
    if (join){
        printf("\n");
        printf("%s\n",join);
    }
    close(fic);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////Main program //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    char **lines = NULL; // tableau de pointeurs vers les lignes lues du fichier
    char buffer[MAX_LINE_LENGTH]; // tableau temporaire pour stocker chaque ligne lue
    int i = 0; // compteur pour parcourir le tableau de lignes
    // Ouverture du fichier en mode lecture/écriture avec ajout à la fin
    int fic;
    fic = open("ipCatalogue.txt", O_RDWR | O_APPEND);

    if (fic == -1){
        printf("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    int bits; // variable pour stocker le nombre de bits lus depuis le fichier
    char *tmp = NULL;
    char *join = NULL;

    // Lecture des lignes du fichier tant que le nombre maximum de lignes n'est pas atteint
    // et tant qu'il y a encore des lignes à lire
    while((bits = read( fic , buffer , MAX_LINE_LENGTH )) > 0){
        // Allocation dynamique de mémoire pour stocker chaque ligne lue
        buffer[bits] = '\0';
        if (join)
        {
            tmp = strdup(join);
            free(join);
            join = ft_strjoin(tmp, buffer);
            free(tmp);
        }
        else
            join = strdup(buffer);
    }
    if (join){
        lines = ft_split(join, '\n');
        free(join);
        i = 0;
        while (lines[i])
            i++;
    }
    // Exemple de traitement à effectuer sur les données lues :
    // création d'un tableau de caractères pour stocker une adresse IP
    char *addIPdetail = NULL;

    // afficher les lignes stockées dans le tableau de chaînes de caractères
    // for (int j = 0; j < i; j++) {
    //     printf("%s", lines[j]);
    // }



    int choix = 0;
    int valeur = i;

    while(choix != 6){


        printf("---Menu---\n\n");
        printf("1.Ajouter une IP\n");
        printf("2.Afficher les IPs enregistrées\n");
        printf("3.Supprimer une IP\n");
        printf("4.Filtrer par masque\n");
        printf("5.Détail sur une IP\n");
        printf("6.EXIT\n\n\n");
        printf("\nVotre choix?\n\n");
        scanf("%d", &choix);



        switch (choix)
        {

            case 1: //ajout IP
                lines = ajout_ip(lines,&valeur);
                // valeur=valeur+1;
                break;

            case 2: //affichage IP enregistré
                printf("\nVoici les IPs enregistrées\n");
                afficher_ips(lines,valeur);

                break;

            case 3: // suppression IP
                printf("\nVoici les IPs enregistrées, saisissez l'adresse IP à supprimer : \n");
                afficher_ips(lines,valeur);
                char ip_to_delete[MAX_LINE_LENGTH];
                scanf("%s", ip_to_delete);

                // Parcours du tableau de lignes pour trouver l'adresse IP à supprimer
                for (int j = 0; j < valeur; j++) {
                    while (!lines[j] && j < valeur)
                        j++;
                    if (strcmp(lines[j], ip_to_delete) == 0) {
                        free(lines[j]); // libération de la mémoire allouée pour cette ligne
                        lines[j][0] = '\n'; // assignation de NULL à l'élément supprimé pour éviter les erreurs de pointeurs
                        printf("L'adresse IP %s a été supprimée du tableau.\n", ip_to_delete);
                        break;
                    }
                }
                break;


            case 4: // filtre par masque
                menu_filtre();
                break;

            case 5: //détail sur une IP

                printf("\nRenseigné une adresse IP pour plus de détail\n");
                addIPdetail = malloc(sizeof(char*) * 20);
                scanf("%s",addIPdetail);
                while (check_ip_address(addIPdetail)!=0){
                    printf("\nErreur, l'adresse renseigné n'est pas correcte, merci de renseigné une adresse ip correcte : \n");
                    scanf("%s",addIPdetail);
                }
                detailIP(addIPdetail);
                free(addIPdetail);

                break;
            case 6:
                printf("\n\nAu revoir !!!\n\n");
                break;
            default:
                printf("\nErreur lors de la saisie du menu\n");
                break;

        }

    }
    fic = open("ipCatalogue.txt", O_RDONLY | O_WRONLY | O_TRUNC);
    if (fic == -1) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return (0);
    }
    int y = 0;
    if (i > 0)
    {   
        while (lines[y][0] == '\n')
            y++;
        int z;
        while (lines[y]){
            while (lines[y][0] == '\n')
                y++;
            z = 0;
            while (lines[y][z])
                z++;
            write(fic, lines[y], z);
            write(fic, "\n", 1);
            y++;
        }
    }
    close(fic);
        // libérer la mémoire allouée pour le tableau de chaînes de caractères
    for (int j = 0; j < i; j++) {
        free(lines[j]);
    }
    return 0;
}