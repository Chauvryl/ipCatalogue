#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IP_LENGTH 16 // Longueur maximale d'une adresse IP

void afficher_ips(char** ip_array, int ip_count) {
    printf("Voici les adresses IP enregistrees :\n");
    for (int i = 0; i < ip_count; i++) {
        printf("%d - %s\n", i+1, ip_array[i]);
    }
}

int main() {
    char** ip_array = NULL; // Initialisation du tableau dynamique d'adresses IP
    int ip_count = 0; // Nombre d'adresses IP dans le tableau
    
    while (1) {
        printf("Que voulez-vous faire ?\n");
        printf("1 - Ajouter une adresse IP\n");
        printf("2 - Afficher les adresses IP\n");
        printf("3 - Supprimer une adresse IP\n");
        printf("4 - Quitter le programme\n");
        
        char choix;
        printf("Entrez votre choix (1/2/3/4) : ");
        scanf(" %c", &choix);
        
        if (choix == '1') {
            char* ip = malloc(MAX_IP_LENGTH * sizeof(char)); // Allouer de la memoire pour une nouvelle adresse IP
            
            printf("Entrez une adresse IP : ");
            scanf("%s", ip);
            
            ip_array = realloc(ip_array, (ip_count + 1) * sizeof(char*)); // Reallouer la memoire pour le tableau d'adresses IP
            ip_array[ip_count] = ip;
            ip_count++;
            
            printf("L'adresse IP %s a ete ajoutee au tableau.\n", ip);
        }
        else if (choix == '2') {
            afficher_ips(ip_array, ip_count);
        }
        else if (choix == '3') {
            afficher_ips(ip_array, ip_count);
            if (ip_count > 0) {
                int choix_ip;
                printf("Entrez le numero de l'adresse IP à supprimer : ");
                scanf("%d", &choix_ip);
                
                if (choix_ip > 0 && choix_ip <= ip_count) {
                    char* ip = ip_array[choix_ip - 1];
                    
                    free(ip);
                    
                    for (int i = choix_ip - 1; i < ip_count - 1; i++) {
                        ip_array[i] = ip_array[i + 1];
                    }
                    
                    ip_count--;
                    ip_array = realloc(ip_array, ip_count * sizeof(char*));
                    printf("L'adresse IP %s a ete supprimee du tableau.\n", ip);
                }
                else {
                    printf("Numero d'adresse IP invalide.\n");
                }
            }
            else {
                printf("Aucune adresse IP enregistree.\n");
            }
        }
        else if (choix == '4') {
            break;
        }
        else {
            printf("Choix invalide. Veuillez choisir entre 1, 2, 3, ou 4.\n");
        }
    }
    
    for (int i = 0; i < ip_count; i++) {
        free(ip_array[i]); // Liberer la memoire pour chaque adresse IP
    }
    free(ip_array); // Liberer la memoire pour le tableau d'adresses IP
    
    return 0;
}
