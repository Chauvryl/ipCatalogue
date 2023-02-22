#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour stocker l'adresse IP et le nom associé
struct addr_info {
    char ip_address[16];
    char name[256];
};

int main() {
    // Création d'un tableau de structures pour stocker plusieurs adresses IP
    struct addr_info addresses[100];
    int num_addresses = 0;

    // Demande à l'utilisateur une adresse IP et un nom associé
    char ip_address[16];
    char name[256];
    printf("Entrer une adresse IP: ");
    scanf("%s", ip_address);
    printf("Quel est le nom de votre adresse IP: ");
    scanf("%s", name);

    // Ajout de l'adresse IP et du nom associé au dictionnaire
    strcpy(addresses[num_addresses].ip_address, ip_address);
    strcpy(addresses[num_addresses].name, name);
    num_addresses++;

    // Affichage des adresses IP et des noms associés
    for (int i = 0; i < num_addresses; i++) {
        printf("%s: %s\n", addresses[i].ip_address, addresses[i].name);
    }

    return 0;
}