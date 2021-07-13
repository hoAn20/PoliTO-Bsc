#ifndef ELEMENT_H
#define ELEMENT_H

#define MAX_STR_LEN 100

enum {transition, acr_back, acr_front};
enum {back, front};
enum {yes, no};

typedef struct {
    char name[MAX_STR_LEN];
    int type;   //transition, back acrobatic, front acrobatic
    int dirIn;  //back, front
    int dirOut; //back, front
    int first;  //yes, no
    int last;   //yes, no
    int difficulty;
    float value;
} Element;

Element ELEMread(FILE *f);
int ELEMcompatibility(Element pred, Element succ);

#endif
