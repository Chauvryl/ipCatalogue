#include <stdio.h>
#include <stdlib.h>
 
int affichageMenu(void)
{
    int choixMenu;
 
    printf("---Menu---\n\n");
    printf("1.BoumBadaBoum!\n");
    printf("2.BeDingBeDang!\n");
    printf("3.BAAAAAMMMMM!\n");
    printf("4.AieAieAie...Il va falloir tout ramasser!\n\n\n");
    printf("\nVotre choix?\n\n");
    scanf("%d", &choixMenu);
    return choixMenu;
}
int main(void)
{
    switch (affichageMenu())
    {
    case 1:
        printf("Vous avez choisis le BoumbadaBoum!");
        break;
    case 2:
        printf("Vous avez choisis le BeDingBeDang!");
        break;
    case 3:
        printf("Vous avez choisis le BAAAAAMMMMM!");
        break;
    case 4:
        printf("Vous n'auriez pas du choisir ca...");
        break;
    default:
        printf("Vous ne ferez rien du tout!");
        break;
    }
 
    system("PAUSE");
    return 0;
}