#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "characters.h"
#include "inventory.h"

typedef enum {add_ch, del_ch, add_eq, del_eq, stats_calc, end, error} command;

command READcommand();

int main() {
    FILE *f_characters = fopen("pg.txt", "r");
    FILE *f_inventory = fopen("inventario.txt", "r");

    char key[CODE_LEN + 1];
    LINK character;
    character_t new;
    characterTab_t TABcharacters;
    command insertedCommand;
    int flag, input, j;
    inventoryTab_t TABinventory;
    stat_t modifiers;


    if (f_characters == NULL || f_inventory == NULL) {
        printf("Errore durante l' apetrtura dei file.\n");
        return EXIT_FAILURE;
    }

    //lettura inventario
    fscanf(f_inventory, "%d", &TABinventory.inventoryLength);
    TABinventory.inventory = (object_t *)malloc(TABinventory.inventoryLength * sizeof(object_t));
    for (int i = 0; i < TABinventory.inventoryLength; i++) {
        TABinventory.inventory[i] = READobject(f_inventory);
    }

    //lettura della lista di personaggi
    TABcharacters.listHead = NULL;
    TABcharacters.listLength = 0;
    while(!feof(f_characters)) {
        LISTcharacters_insertInTail(&TABcharacters, READcharacter(f_characters));
        TABcharacters.listLength++;
    }

    do {
        insertedCommand = READcommand();
        switch(insertedCommand) {
            case add_ch:
                printf(" - AGGIUNTA PERSONAGGIO -\n"
                       "Ecco i personaggi presenti: \n");
                LISTcharacters_print(TABcharacters, stdout);
                printf("\n");
                flag = 0;
                do {
                    if (flag)
                        printf("Inserire un personaggio con codice univoco.\n\n");
                    new = READcharacter(stdin);
                    flag = existingCode(TABcharacters, getCode(&new));
                } while(flag);
                LISTcharacters_insertInTail(&TABcharacters, new);
                break;
            case del_ch:
                printf(" - ELIMINAZIONE PERSONAGGIO -\n"
                       "Ecco i personaggi presenti: \n");
                LISTcharacters_print(TABcharacters, stdout);
                printf("\n");
                flag = 1;
                do {
                    if (!flag)
                        printf("Inserire un codice valido.\n\n");
                    printf("Inserisci il un codice da eliminare: ");
                    scanf("%s", key);
                    flag = existingCode(TABcharacters, key);
                } while(!flag);
                LISTcharacters_deleteNode(&TABcharacters, key);
                break;
            case add_eq:
                printf(" - AGGIUNTA EQUIPAGGIAMENTO -\n");
                flag = 1;
                do {
                    if (!flag)
                        printf("Inserire un codice valido.\n\n");
                    printf("Inserisci il codice del personaggio a cui vuoi aggiungere un oggetto: ");
                    scanf("%s", key);
                    flag = existingCode(TABcharacters, key);
                } while(!flag);
                character = LISTcharacters_search(TABcharacters, key);
                if (character->character.equipment.objects_p[0] != NULL) {
                    printf("\nEcco gli oggetti di cui dispone ad ora:\n");
                    for ( j = 0; j < N_OBJECTS && character->character.equipment.objects_p[j] != NULL; j++)
                        printf("%s %s\n", character->character.equipment.objects_p[j]->name, character->character.equipment.objects_p[j]->type);
                    if (j == N_OBJECTS - 1) {
                        printf("Il personaggio non ha spazio per altri oggetti.\n");
                        continue;
                    }
                } else
                    printf("Al momento non possiede oggetti.\n\n");
                printf("Ecco gli oggetti a tua dispoiszione:\n");
                inventory_print(TABinventory, stdout);
                printf("\n");
                printf("Inserisci il codice di un oggetto dalla lista: ");
                scanf("%d", &input);
                input--;
                j = 0;
                while(character->character.equipment.objects_p[j] != NULL) {
                    j++;
                }
                character->character.equipment.objects_p[j] = &(TABinventory.inventory[input]);
                break;
            case del_eq:
                printf(" - ELIMINAZIONE EQUIPAGGIAMENTO -\n");
                flag = 1;
                do {
                    if (!flag)
                        printf("Inserire un codice valido.\n\n");
                    printf("Inserisci il codice del personaggio a cui vuoi eliminare un oggetto: ");
                    scanf("%s", key);
                    flag = existingCode(TABcharacters, key);
                } while(!flag);
                character = LISTcharacters_search(TABcharacters, key);
                if (character->character.equipment.objects_p[0] != NULL) {
                    printf("Ecco gli oggetti di cui dispone ad ora:\n");
                    for (j = 0; j < N_OBJECTS && character->character.equipment.objects_p[j] != NULL; j++)
                        printf("%d) %s %s\n", j+1, character->character.equipment.objects_p[j]->name, character->character.equipment.objects_p[j]->type);
                    do {
                        printf("Inserisci il codice dell' oggetto da eliminare: ");
                        scanf("%d", &input);
                    } while(input < 1 || input > j);
                    input--;
                    character->character.equipment.objects_p[input] = NULL;
                    for (j = input; j < N_OBJECTS; j++)
                        character->character.equipment.objects_p[j] = character->character.equipment.objects_p[j+1];
                } else
                    printf("Al momento non possiede oggetti.\n\n");
                break;
            case stats_calc:
                printf(" - CALCOLO STATISTICHE -\n");
                flag = 1;
                do {
                    if (!flag)
                        printf("Inserire un codice valido.\n\n");
                    printf("Inserisci il codice del personaggio a cui vuoi eliminare un oggetto: ");
                    scanf("%s", key);
                    flag = existingCode(TABcharacters, key);
                } while(!flag);
                character = LISTcharacters_search(TABcharacters, key);
                for (int i = 0; i < N_STATS; i++) {
                    modifiers.stat[i] = 0;
                    for (j = 0; j < N_OBJECTS && character->character.equipment.objects_p[j]!=NULL; j++)
                        modifiers.stat[i] += character->character.equipment.objects_p[j]->modifier.stat[i];
                }
                printf("Le sue statistiche sono:\n"
                       "hp = %d\n"
                       "mp = %d\n"
                       "atk = %d\n"
                       "def = %d\n"
                       "mag = %d\n"
                       "spr = %d\n\n",
                       ((character->character.stats.stat[hp] + modifiers.stat[hp] > 1)?(character->character.stats.stat[hp] + modifiers.stat[hp]):1),
                       ((character->character.stats.stat[mp] + modifiers.stat[mp] > 1)?(character->character.stats.stat[mp] + modifiers.stat[mp]):1),
                       ((character->character.stats.stat[atk] + modifiers.stat[atk] > 1)?(character->character.stats.stat[atk] + modifiers.stat[atk]):1),
                       ((character->character.stats.stat[def] + modifiers.stat[def] > 1)?(character->character.stats.stat[def] + modifiers.stat[def]):1),
                       ((character->character.stats.stat[mag] + modifiers.stat[mag] > 1)?(character->character.stats.stat[mag] + modifiers.stat[mag]):1),
                       ((character->character.stats.stat[spr] + modifiers.stat[spr] > 1)?(character->character.stats.stat[spr] + modifiers.stat[spr]):1)
                );
                break;
            case end:
                printf("Fine esecuzione.\n\n");
                break;
            case error:
                printf("Inserire un comando valido.\n\n");
        }
    } while(insertedCommand != end);

    return 0;
}

command READcommand() {
    int ins;

    printf(" - MENU -\n"
           "1 inserire nuovo personaggio\n"
           "2 eliminare un presonaggio\n"
           "3 aggiungere un oggetto ad un personaggio\n"
           "4 eliminare un oggetto a un personaggio\n"
           "5 calcolare le statistiche effettive di un personaggio\n"
           "6 uscire\n\n"
           "Inserisci il comando desiderato: ");
    scanf("%d", &ins);
    switch(ins) {
        case 1:
            return add_ch;
        case 2:
            return del_ch;
        case 3:
            return add_eq;
        case 4:
            return del_eq;
        case 5:
            return stats_calc;
        case 6:
            return end;
        default:
            return error;
    }
}
