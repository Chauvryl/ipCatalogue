#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    int choix;
    FILE *file;
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
            return 1;
    }
    
    file=fopen(name,"rt");

    while((name=fgetc(file))!=EOF){
        printf("%c",name);
    }

    fclose(file);
    return 0;
}
