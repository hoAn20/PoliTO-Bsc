#ifndef COMPUTER_H
#define COMPUTER_H

#define MAX_STR_LEN 30

typedef struct {
    char name[MAX_STR_LEN];
    char net[MAX_STR_LEN];
} Computer;

Computer COMPnew(char *name, char *net);
Computer COMPsetNull();

#endif
