#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODE_LEN 6
#define STR_LEN 50
#define N_OBJECTS 8
#define N_STATS 6

typedef enum {add_ch, del_ch, add_eq, del_eq, stats_calc, end, error} command;
enum {hp, mp, atk, def, mag, spr};

typedef struct {
    int stat[N_STATS];
} stat_t;
typedef struct {
    char name[STR_LEN + 1];
    char type[STR_LEN + 1];
    stat_t modifier;
} object_t;
typedef struct {
    int inUso;
    object_t *objects_p[N_OBJECTS];
} equipmentTab_t;
typedef struct {
    char code[CODE_LEN + 1];
    char name[STR_LEN + 1];
    char class[STR_LEN + 1];
    equipmentTab_t equipment;
    stat_t stats;
} character_t;
typedef struct node NODE, *LINK;
struct node {
    character_t character;
    LINK nextCharacter;
};
typedef struct {
    LINK listHead;
    LINK listTail;
    int listLength;
} characterTab_t;
typedef struct {
    object_t *inventory;
    int inventoryLength;
    int maxInventroyLength;
} inventoryTab_t;


char *getCode(character_t *character);
character_t READcharacter(FILE *f);
command READcommand();
int existingCode(characterTab_t TABch, char *code);
object_t READobject(FILE *f);
LINK LISTcharacters_newNode(character_t new, LINK next);
LINK LISTcharacters_search(characterTab_t TABch, char *key);
void LISTcharacters_insertInTail(characterTab_t *TABch_p, character_t new);
void LISTcharacters_print(characterTab_t TABch, FILE *f);
void LISTcharacters_deleteNode(characterTab_t *TABch, char *delCode);
void inventory_print(inventoryTab_t TABin, FILE *f);


int main() {
    FILE *f_characters = fopen("characters.txt", "r");
    FILE *f_inventory = fopen("inventory.txt", "r");

    char key[CODE_LEN + 1];
    LINK character;
    character_t new;
    characterTab_t TABcharacters;
    command insertedCommand;
    int flag, input, i, j;
    inventoryTab_t TABinventory;
    stat_t modifiers;


    if (f_characters == NULL || f_inventory == NULL) {
        printf("Errore durante l' apetrtura dei file.\n");
        return EXIT_FAILURE;
    }

    //lettura inventario
    fscanf(f_inventory, "%d", &TABinventory.inventoryLength);
    TABinventory.inventory = (object_t *)malloc(TABinventory.inventoryLength * sizeof(object_t));
    for (i = 0; i < TABinventory.inventoryLength; i++) {
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
                for (i = 0; i < N_STATS; i++) {
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


char *getCode(character_t *character) {
    return &(character->code);
}
character_t READcharacter(FILE *f) {
    character_t new;

    if (f == stdin) printf("Inserisci il codice del personaggio: ");
    fscanf(f, "%s", new.code);
    if (f == stdin) printf("Inserisci il nome del personaggio: ");
    fscanf(f, "%s", new.name);
    if (f == stdin) printf("Inserisci la classe del personaggio: ");
    fscanf(f, "%s", new.class);
    if (f == stdin) printf("Inserisci la statistica hp del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[hp]);
    if (f == stdin) printf("Inserisci la statistica mp del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[mp]);
    if (f == stdin) printf("Inserisci la statistica atk del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[atk]);
    if (f == stdin) printf("Inserisci la statistica def del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[def]);
    if (f == stdin) printf("Inserisci la statistica mag del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[mag]);
    if (f == stdin) printf("Inserisci la statistica spr del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[spr]);

    return new;
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
int existingCode(characterTab_t TABch, char *code) {
    LINK x;

    for (x = TABch.listHead; x != NULL; x = x->nextCharacter) {
        if (strcmp(code, getCode(&(x->character))) == 0)
            return 1;
    }

    return 0;
}
object_t READobject(FILE *f) {
    object_t new;

    if (f == stdin) printf("Inserisci il nome del nuovo oggetto: ");
    fscanf(f, "%s", new.name);
    if (f == stdin) printf("Inserisci il tipo del nuovo oggetto: ");
    fscanf(f, "%s", new.type);
    if (f == stdin) printf("Inserisci il modificatore di hp di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[hp]);
    if (f == stdin) printf("Inserisci il modificatore di mp di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[mp]);
    if (f == stdin) printf("Inserisci il modificatore di atk di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[atk]);
    if (f == stdin) printf("Inserisci il modificatore di def di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[def]);
    if (f == stdin) printf("Inserisci il modificatore di mag di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[mag]);
    if (f == stdin) printf("Inserisci il modificatore di spr di questo oggetto: ");
    fscanf(f, "%d", &new.modifier.stat[spr]);

    return new;
}
LINK LISTcharacters_newNode(character_t new, LINK next) {
    LINK newNodeLink = (LINK)malloc(sizeof(NODE));
    int i;

    newNodeLink->character = new;
    newNodeLink->nextCharacter = next;
    for (i = 0; i < N_OBJECTS; i++)
        newNodeLink->character.equipment.objects_p[i] = NULL;

    return newNodeLink;
}
LINK LISTcharacters_search(characterTab_t TABch, char *key) {
    LINK x;

    for (x = TABch.listHead; strcmp(getCode(&(x->character)), key) != 0; x = x->nextCharacter);

    return x;
}
void LISTcharacters_insertInTail(characterTab_t *TABch_p, character_t new) {
    if (TABch_p->listLength == 0) {
        TABch_p->listHead = LISTcharacters_newNode(new, NULL);
        TABch_p->listTail = TABch_p->listHead;
        return;
    }
    TABch_p->listTail->nextCharacter = LISTcharacters_newNode(new, NULL);
    TABch_p->listTail = TABch_p->listTail->nextCharacter;
}
void LISTcharacters_print(characterTab_t TABch, FILE *f) {
    LINK x;

    for (x = TABch.listHead; x != NULL; x = x->nextCharacter)
        fprintf(f, "%s %s %s %d %d %d %d %d %d\n", x->character.code, x->character.name, x->character.class, x->character.stats.stat[hp], x->character.stats.stat[mp], x->character.stats.stat[atk], x->character.stats.stat[def], x->character.stats.stat[mag], x->character.stats.stat[spr]);
}
void LISTcharacters_deleteNode(characterTab_t *TABch, char *delCode) {
    LINK x, d;

    for (x = TABch->listHead; strcmp(delCode, getCode(&(x->nextCharacter->character))) != 0; x = x->nextCharacter);
    d = x->nextCharacter;
    x->nextCharacter = d->nextCharacter;
    free(d);
}
void inventory_print(inventoryTab_t TABin, FILE *f) {
    int i;
	for (i = 0; i < TABin.inventoryLength; i++)
        fprintf(f, "%d) %s %s %d %d %d %d %d %d\n", i+1, TABin.inventory[i].name, TABin.inventory[i].type, TABin.inventory[i].modifier.stat[hp], TABin.inventory[i].modifier.stat[mp], TABin.inventory[i].modifier.stat[atk], TABin.inventory[i].modifier.stat[def], TABin.inventory[i].modifier.stat[mag], TABin.inventory[i].modifier.stat[spr]);
}
