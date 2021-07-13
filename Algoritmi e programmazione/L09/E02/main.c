#include <stdio.h>
#include <stdlib.h>

#include "character.h"
#include "inventory.h"
#include "chlist.h"

enum {addCh, delCh, equipObj, unequipObj, calcStats, end, error};

int selectAction();

int main() {
    int selectedAction;
    char code[CODE_LEN+1];
    ChList characterList = CHLISTnew();
    Inventory inventory = INVENTORYnew();
    Character character;

    CHLISTreadFromFile(characterList);
    INVENTORYread(inventory);

    do {
        selectedAction = selectAction();
        switch(selectedAction) {
            case addCh:
                do {
                    printf("Inserisci il codice del personaggio: ");
                    scanf("%s", code);
                    character = CHLISTsearchByCode(characterList, code);
                } while(character != NULL);
                CHLISTinsertInTailCharacter(characterList, CHARACTERnewFromKeyboard(code));

                CHLISTprint(characterList);
                break;
            case delCh:
                do {
                    printf("Inserisci il codice del personaggio: ");
                    scanf("%s", code);
                } while(CHLISTsearchByCode(characterList, code) == NULL);
                CHLISTremoveCharacter(characterList, code);

                CHLISTprint(characterList);
                break;
            case equipObj:
                do {
                    printf("Inserisci il codice del personaggio: ");
                    scanf("%s", code);
                    character = CHLISTsearchByCode(characterList, code);
                } while(character == NULL);
                CHARACTERaddEquipment(character, inventory);
                break;
            case unequipObj:
                do {
                    printf("Inserisci il codice del personaggio: ");
                    scanf("%s", code);
                    character = CHLISTsearchByCode(characterList, code);
                } while(character == NULL);
                CHARACTERdelEquipment(character, inventory);
                break;
            case calcStats:
                do {
                    printf("Inserisci il codice del personaggio: ");
                    scanf("%s", code);
                    character = CHLISTsearchByCode(characterList, code);
                } while(character == NULL);
                CHARACTERcalculateEffectiveStats(character, inventory);
                STATprint(CHARACTERgetEffectiveStat(character));
                break;
            case end:
                CHLISTfree(characterList);
                INVENTORYfree(inventory);
                printf("Esecuzione terminata.\n\n");
                break;
            case error:
                printf("ERRORE: inserire un comando valido.\n\n");
        }
    } while(selectedAction!= end);

    return 0;
}

int selectAction() {
    int action;

    printf(" - MENU' -\n"
           "1) aggiungere un personaggio alla lista\n"
           "2) togliere un personaggio dalla lista\n"
           "3) equipaggiare un personaggio di un oggetto\n"
           "4) privare un personaggio di un oggetto\n"
           "5) calcolare le statistiche effettive\n"
           "6) terminare il programma\n\n"
           "Inserire il numero dell'azione desiderata: ");
    scanf("%d", &action);
    if (action <= 0 || action > 6)
        return error;
    return (action - 1);
}