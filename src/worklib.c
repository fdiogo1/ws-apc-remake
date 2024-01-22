#include "worklib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*****************************************************/

struct _consumer
{
    char name[NAME_SIZE];
    char cpf[CPF_SIZE];
};

struct _company
{
    char name[NAME_SIZE];
    char cnpj[CNPJ_SIZE];
    float potency;
    char founded[DATE_SIZE];
};

struct _contract
{
    CONSUMER consumer;
    COMPANY company;
    float power;
};

struct _list
{
    char listName[200];
    int quantity;
    Node *begin;
    Node *end;
};

struct _node
{
    CONSUMER *consumers;
    COMPANY *companies;
    Node *next;
    Node *prev;
};

int compareStrings(char *text1, char *text2)
{
    char *aux1 = (char *)calloc(strlen(text1) + 1, sizeof(char));
    char *aux2 = (char *)calloc(strlen(text2) + 1, sizeof(char));

    int answer;

    for (int i = 0; i <= strlen(text1); i++)
    {
        aux1[i] = tolower(text1[i]);
    }

    for (int i = 0; i <= strlen(text2); i++)
    {
        aux2[i] = tolower(text2[i]);
    }

    if (strcmp(aux1, aux2) == 0)
        answer = 1;
    else
        answer = 0;

    free(aux1);
    free(aux2);

    return answer;
}

bool checkDate(const char *date)
{
    if (strlen(date) != 10) return false;
    
    char dayString[3] = {date[0], date[1]};
    char monthString[3] = {date[3], date[4]};
    char yearString[5] = {date[6], date[7], date[8], date[9]};


    int day = atoi(dayString);
    int month = atoi(monthString);
    int year = atoi(yearString);

    if (day < 1 || day > 31) return false;

    if (month < 1 || month > 12) return false;

    if (year < 1900 || year > 2100) return false;

    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            if (day > 29)
            {
                return false;
            }
        }
        else
        {
            if (day > 28)
            {
                return false;
            }
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30)
        {
            return false;
        }
    }

    return true;
}

/*****************************************************/

/****************** DATA STRUCTURES ******************/

List *createList(const char *name)
{
    List *new = (List *)calloc(1, sizeof(List));
    strcpy(new->listName, name);
    return new;
}

bool isEmpty(const List *list)
{
    return list->quantity == 0;
}

Node *createConsumerNode(CONSUMER p)
{
    Node *new = (Node *)calloc(1, sizeof(Node));
    new->consumers = (CONSUMER *)calloc(1, sizeof(CONSUMER));
    strcpy(new->consumers->cpf, p.cpf);
    strcpy(new->consumers->name, p.name);
    return new;
}

Node *createCompanyNode(COMPANY p)
{
    Node *new = (Node*) calloc(1, sizeof(Node));
    new->companies = (COMPANY *)calloc(1, sizeof(COMPANY));
    strcpy(new->companies->name, p.name);
    strcpy(new->companies->cnpj, p.cnpj);
    strcpy(new->companies->founded, p.founded);
    new->companies->potency = p.potency;
    return new;
}

void insertConsumerNode(List *list, CONSUMER p)
{
    Node *newNode = createConsumerNode(p);
    if (isEmpty(list))
    {
        list->begin = list->end = newNode;
    }
    else
    {
        newNode->prev = list->end;
        list->end->next = newNode;
        list->end = newNode;
    }
    list->quantity++;
}

void insertCompanyNode(List *list, COMPANY p)
{
    Node *newNode = createCompanyNode(p);
    if (isEmpty(list))
    {
        list->begin = list->end = newNode;
    }
    else
    {
        newNode->prev = list->end;
        list->end->next = newNode;
        list->end = newNode;
    }
    list->quantity++;
}

void loadFromArchive(List *list, char *type)
{
    if (compareStrings(type, CONSUMERS))
    {
        FILE *archive = fopen(ARCHIVE_CONSUMERS, "rt");
        CONSUMER read;
        strcpy(read.name, "null");

        if (archive != NULL)
        {
            while (!feof(archive))
            {
                fscanf(archive, " %[^\n]s", read.name);
                fscanf(archive, "%s", read.cpf);
                if (!compareStrings(read.name, "null"))
                    insertConsumerNode(list, read);
                strcpy(read.name, "null");
            }

        }
        else
        {
            fprintf(stderr, "Error: Something went wrong with the archive 'consumers.txt'!\n");
            exit(EXIT_FAILURE);
        }
        fclose(archive);
        rename(ARCHIVE_CONSUMERS, BACKUP_ARCHIVE_CONSUMERS);
        remove(ARCHIVE_CONSUMERS);
    }
    else if (compareStrings(type, COMPANIES))
    {
        FILE *archive = fopen(ARCHIVE_COMPANIES, "rt");
        COMPANY read;
        strcpy(read.name, "null");

        if (archive != NULL)
        {
            while (!feof(archive))
            {
                fscanf(archive, " %[^\n]s", read.name);
                fscanf(archive, "%s\n", read.cnpj);
                fscanf(archive, "%s\n", read.founded);
                fscanf(archive, "%f", &read.potency);
                if (!compareStrings(read.name, "null"))
                    insertCompanyNode(list, read);
                strcpy(read.name, "null");
            }

        }
        else
        {
            fprintf(stderr, "Error: Something went wrong with the archive 'companies.txt'!\n");
            exit(EXIT_FAILURE);
        }
        fclose(archive);
        rename(ARCHIVE_COMPANIES, BACKUP_ARCHIVE_COMPANIES);
        remove(ARCHIVE_COMPANIES);
    }
    else
    {
        printf("answer comparison %d\n", compareStrings(type, CONSUMERS));
        fprintf(stderr, "Error: Archive not found.\n");
        exit(EXIT_FAILURE);
    }
}

void printList(const List *list, char *type)
{
    if (compareStrings(type, CONSUMERS))
    {

        if (isEmpty(list))
        {
            puts("0 consumers found at archive 'consumers.txt'");
        }
        else
        {
            Node *p = list->begin;
            while (p != NULL)
            {
                printf("Consumer: %s; CPF: %s\n", p->consumers->name, p->consumers->cpf);
                p = p->next;
            }
        }
    }
    else if (compareStrings(type, COMPANIES)) 
    {
        if (isEmpty(list))
        {
            puts("0 companies found at archive 'companies.txt'");
        }
        else
        {
            Node *p = list->begin;
            while (p != NULL)
            {
                printf("Company: %s; CNPJ: %s\n", p->companies->name, p->companies->cnpj);
                p = p->next;
            }
        }
    }
}

void destroyNode(Node **N)
{
    Node *node = *N;

    if (node->consumers != NULL)
        free(node->consumers);

    if (node->companies != NULL)
        free(node->companies);

    free(node);
    *N = NULL;
}

void destroyList(List **L)
{
    List *list = *L;
    Node *node = list->begin;
    if (!isEmpty(list))
    {
        Node *aux = node;
        while (aux != NULL)
        {
            node = node->next;
            destroyNode(&aux);
            aux = node;
        }
    }
    free(list);
    *L = NULL;
}

/***************** FUNCTIONALITIES *******************/

void printMenu()
{
    printf("==================================================\n");
    printf("       SGC - SISTEMA DE GERACAO COMPARTILHADA\n");
    printf("                MENU DE OPCOES \n");
    printf("                 APC - 2022.1\n");
    printf("==================================================\n");
    printf(" ________________________________________________\n");
    printf("|                                                |\n");
    printf("| > Selecione a tarefa desejada:                 |\n");
    printf("|                                                |\n");
    printf("| 1) Cadastrar usina                             |\n");
    printf("| 2) Cadastrar consumidor                        |\n");
    printf("| 3) Cadastrar contrato entre consumidor e usina |\n");
    printf("| 4) Consultar usina cadastrada                  |\n");
    printf("| 5) Consultar consumidor cadastrado             |\n");
    printf("| 6) Excluir usina                               |\n");
    printf("| 7) Excluir consumidor                          |\n");
    printf("| 8) Encerrar programa                           |\n");
    printf("|________________________________________________|\n");
}

void newConsumer(List *list)
{
    CONSUMER p;
    printf("================================\n");
    printf("     CADASTRAR CONSUMIDOR\n");
    printf("================================\n");
    printf("[*] Digite o nome do consumidor: \n");
    fflush(stdin);
    scanf("%[^\n]s", p.name);
    printf("[*] Digite o CPF do consumidor: \n");
    for (;;)
    {
        scanf("%s", p.cpf);
        if (!checkCPF(p.cpf))
            printf("[!] CPF invalido. Digite novamente no seguinte formato XXXYYYZZZLL:\n>> ");
        else
            break;
    }

    if (!checkConsumer(p.cpf, list))
    {
        insertConsumerNode(list, p);
        printf("[*] Consumidor cadastrado com sucesso.\n");
        system("pause");
    }
    else
    {
        printf("[!] ERRO: O CPF inserido ja esta cadastrado! \n");
        system("pause");
    }
}

void newCompany(List *list)
{
    COMPANY u;
    printf("================================\n");
    printf("        CADASTRAR USINA\n");
    printf("================================\n");
    printf("[*] Digite o nome da usina:\n>> ");
    fflush(stdin);
    scanf("%[^\n]s", u.name);
    printf("[*] Digite o CNPJ da usina no formato: XX.XXX.XXX/0001-XX, sem a pontuacao:\n>> ");
    fflush(stdin);
    scanf("%s", u.cnpj);

    while (!checkCNPJ(u.cnpj))
    {
        printf("[!] Digite um CNPJ valido, no formato: XX.XXX.XXX/0001-XX, sem a pontuacao:\n>> ");
        fflush(stdin);
        scanf("%s", u.cnpj);
    }

    printf("[!] Digite a data da fundacao da empresa, no formato dd/mm/aaaa:\n>> ");
    fflush(stdin);
    scanf("%s", u.founded);

    while (!checkDate(u.founded))
    {
        printf("[!] Digite uma data valida. dd/mm/aaaa\n>> ");
        scanf("%s", u.founded);
    }
    

    printf("[*] Digite a potencia estimada da usina, em MW: \n");
    scanf("%f", &u.potency);

    while (u.potency <= 0)
    {
        printf("[!] ERRO: A potencia deve ser maior que 0 (zero)!\n>> ");
        scanf("%f", &u.potency);
    }

    if (!checkCompany(u.cnpj, list))
    {
        insertCompanyNode(list, u);
        printf("[*] Usina cadastrada com sucesso!\n");
        system("pause");
    }
    else
    {
        printf("[!] ERRO: esse CNPJ ja esta cadastrado!\n");
        system("pause");
    }
}

bool checkConsumer(char *cpf, const List *list)
{

    Node *p = list->begin;

    while (p != NULL && !compareStrings(cpf, p->consumers->cpf))
    {
        p = p->next;
    }

    return (p != NULL) ? true : false;
}

bool checkCompany(char *cnpj, const List *list)
{

    Node *p = list->begin;

    while (p != NULL && !compareStrings(cnpj, p->companies->cnpj))
    {
        p = p->next;
    }

    return (p != NULL) ? true : false;
}

bool checkCPF(const char *cpf)
{

    if (strlen(cpf) != 11 || strcmp(cpf, "00000000000") == 0 || strcmp(cpf, "11111111111") == 0 || strcmp(cpf, "22222222222") == 0 ||
        strcmp(cpf, "33333333333") == 0 || strcmp(cpf, "44444444444") == 0 || strcmp(cpf, "55555555555") == 0 || strcmp(cpf, "66666666666") == 0 ||
        strcmp(cpf, "77777777777") == 0 || strcmp(cpf, "88888888888") == 0 || strcmp(cpf, "99999999999") == 0)
        return false;
    else
    {
        int v1, v2, soma = 0;

        for (int i = 0, j = 10; i < 9 || j >= 2; i++, j--)
            soma += ((cpf[i] - 48) * j);

        soma *= 10;

        v1 = (soma % 11 == 10) ? '0' : (soma % 11) + 48;

        soma = 0;

        for (int i = 0, j = 11; i < 10 || j >= 2; i++, j--)
            soma += (cpf[i] - 48) * j;

        soma *= 10;

        v2 = (soma % 11 >= 10) ? '0' : (soma % 11) + 48;

        return (v1 == cpf[9] && v2 == cpf[10]) ? true : false;
    }
}

bool checkCNPJ(const char *cnpj)
{
    if (strlen(cnpj) == 14)
    {
        int v1, v2, soma = 0;

        for (int i = 0, j = 5; i <= 3 || j >= 2; i++, j--)
            soma = soma + ((cnpj[i] - 48) * j);


        for (int i = 4, j = 9; i <= 11 || j >= 2; i++, j--)
            soma = soma + ((cnpj[i] - 48) * j); 

        v1 = (soma % 11 < 2) ? '0' : (11 - soma % 11) + 48;

        soma = 0;

        for (int i = 0, j = 6; i <= 4 || j >= 2; i++, j--)
            soma = soma + ((cnpj[i] - 48) * j);

        for (int i = 5, j = 9; i <= 12 || j >= 2; i++, j--)
            soma = soma + ((cnpj[i] - 48) * j);

        v2 = (soma % 11 < 2) ? '0' : (11 - soma % 11) + 48;

        return (v1 == cnpj[12] && v2 == cnpj[13]) ? true : false;

    }
    else
    {
        return false;
    }
}

// Consumer, Company, Contract
void updateArchive(const List *consumerList, const List *companyList)
{
    if (!isEmpty(consumerList) && !isEmpty(companyList))
    {
        FILE *archive1 = fopen(ARCHIVE_CONSUMERS, "at");
        Node *aux = consumerList->begin;

        while (aux != NULL)
        {
            fprintf(archive1, "%s\n", aux->consumers->name);
            fprintf(archive1, "%s\n", aux->consumers->cpf);
            aux = aux->next;
        }

        fclose(archive1);
        remove(BACKUP_ARCHIVE_CONSUMERS);

        FILE *archive2 = fopen(ARCHIVE_COMPANIES, "at");
        aux = companyList->begin;

        while (aux != NULL)
        {
            fprintf(archive2, "%s\n", aux->companies->name);
            fprintf(archive2, "%s\n", aux->companies->cnpj);
            fprintf(archive2, "%s\n", aux->companies->founded);
            fprintf(archive2, "%.2f\n", aux->companies->potency);
            aux = aux->next;
        }

        fclose(archive2);
        remove(BACKUP_ARCHIVE_COMPANIES);
    }
    else 
    {
        FILE *archive1 = fopen(ARCHIVE_CONSUMERS, "at");
        FILE *archive2 = fopen(ARCHIVE_COMPANIES, "at");
        fclose(archive1);
        fclose(archive2);
        remove(BACKUP_ARCHIVE_CONSUMERS);
        remove(BACKUP_ARCHIVE_COMPANIES);
    }
}

/*****************************************************/