#ifndef WORKLIB_H
#define WORKLIB_H

/*****************************************/
#include <stdbool.h>

#define CPF_SIZE 12
#define CNPJ_SIZE 14
#define NAME_SIZE 200
#define DATE_SIZE 11
#define POTENCIA 1

#define CONSUMERS "CONSUMERS"
#define ARCHIVE_CONSUMERS "C:\\Users\\Admin\\Desktop\\Developer\\ws-apc-remake\\after\\archives\\consumers.txt"
#define BACKUP_ARCHIVE_CONSUMERS "C:\\Users\\Admin\\Desktop\\Developer\\ws-apc-remake\\after\\archives\\backups\\b-consumers.txt"

#define COMPANIES "COMPANIES"
#define ARCHIVE_COMPANIES "C:\\Users\\Admin\\Desktop\\Developer\\ws-apc-remake\\after\\archives\\companies.txt"
#define BACKUP_ARCHIVE_COMPANIES "C:\\Users\\Admin\\Desktop\\Developer\\ws-apc-remake\\after\\archives\\backups\\b-companies.txt"

#define CONTRACTS "CONTRACTS"
#define ARCHIVE_CONTRACTS "C:\\Users\\Admin\\Desktop\\Developer\\ws-apc-remake\\after\\archives\\contracts.txt"
#define BACKUP_ARCHIVE_CONTRACTS "C:\\Users\\Admin\\Desktop\\Developer\\ws-apc-remake\\after\\archives\\backups\\b-contracts.txt"

typedef struct _consumer CONSUMER;
typedef struct _company COMPANY;
typedef struct _contract CONTRACT;
typedef struct _list List;
typedef struct _node Node;

List *createList(const char *);

void loadFromArchive(List *, char *);
void updateArchive(const List *, const List *);
void printList(const List *, char *);
void destroyList(List **);

void printMenu();
void newCompany(List *);
void newConsumer(List *);
void newContract();
void viewCompany();
void viewConsumer();
void deleteCompany();
void deleteConsumer();
void cleanScreen();
int compareStrings(char *, char*);
bool checkDate(const char *);
bool checkCPF(const char *);
bool checkCNPJ(const char *);
bool checkConsumer(char *, const List *);
bool checkCompany(char *, const List *);

/*****************************************/
#endif