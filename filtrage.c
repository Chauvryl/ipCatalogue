void masqueIP(char *adresseIP) {
    int premierOctet;
    int masque;

    sscanf(adresseIP, "%d", &premierOctet);
    if (premierOctet >= 1 && premierOctet <= 126) {
        masqueIP(adresseIP, 8)
        printf("L'adresse IP %s a été ajouté au fichier masque_8.\n", adresseIP);
    } else if (premierOctet >= 128 && premierOctet <= 191) {
        masqueIP(adresseIP, 16)
        printf("L'adresse IP %s a été ajouté au fichier masque_16.\n", adresseIP);
    } else if (premierOctet >= 192 && premierOctet <= 223) {
        masqueIP(adresseIP, 24)
        printf("L'adresse IP %s a été ajouté au fichier masque_24.\n", adresseIP);
    } else {
        masqueIP(adresseIP, 32)
        printf("L'adresse IP %s a été ajouté au fichier masque_32.\n", adresseIP);
    }
}

void masqueIP(char *adresseIP, masque) {
    int IP;
    char*f="masque_"+masque
    FILE *fichier = fopen(f, "w");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return 1;
    }
    fprintf(fichier, "%d", adresseIP);
    fclose(fichier);
    return 0;
}
