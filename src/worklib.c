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
    CONSUMER *consumer;
    COMPANY *company;
    float power;
    char date[DATE_SIZE];
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
    CONTRACT *contracts;
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
    if (strlen(date) != 10)
        return false;

    char dayString[3] = {date[0], date[1]};
    char monthString[3] = {date[3], date[4]};
    char yearString[5] = {date[6], date[7], date[8], date[9]};

    int day = atoi(dayString);
    int month = atoi(monthString);
    int year = atoi(yearString);

    if (day < 1 || day > 31)
        return false;

    if (month < 1 || month > 12)
        return false;

    if (year < 1900 || year > 2100)
        return false;

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
    Node *new = (Node *)calloc(1, sizeof(Node));
    new->companies = (COMPANY *)calloc(1, sizeof(COMPANY));
    strcpy(new->companies->name, p.name);
    strcpy(new->companies->cnpj, p.cnpj);
    strcpy(new->companies->founded, p.founded);
    new->companies->potency = p.potency;
    return new;
}

Node *createContractNode(CONTRACT *p)
{
    Node *new = (Node *)calloc(1, sizeof(Node));
    new->contracts = (CONTRACT *)calloc(1, sizeof(CONTRACT));

    strcpy(new->contracts->date, p->date);
    new->contracts->company = p->company;
    new->contracts->consumer = p->consumer;
    new->contracts->power = p->power;

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

void insertContractNode(List *list, CONTRACT *p)
{
    Node *newNode = createContractNode(p);

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
        strcpy(read.name, "");

        if (archive != NULL)
        {
            while (!feof(archive))
            {
                fscanf(archive, " %[^\n]s", read.name);
                fscanf(archive, "%s", read.cpf);
                if (!compareStrings(read.name, ""))
                    insertConsumerNode(list, read);
                strcpy(read.name, "");
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
                fscanf(archive, "%s", read.founded);
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
        fprintf(stderr, "Error: Archive '%s' not found.\n", type);
        exit(EXIT_FAILURE);
    }
}

void loadContracts(List *contracts, const List *consumers, const List *companies)
{
    FILE *archive = fopen(ARCHIVE_CONTRACTS, "rt");

    char cnpj[CNPJ_SIZE];
    char cpf[CPF_SIZE];
    float power;
    char date[DATE_SIZE];

    CONTRACT *insert = (CONTRACT*) calloc(1, sizeof(CONTRACT));

    COMPANY *p2 = NULL;
    CONSUMER *p3 = NULL;

    if (archive != NULL)
    {
        strcpy(cnpj, "");
        while (!feof(archive))
        {
            fscanf(archive, "%s", cnpj);
            fscanf(archive, "%f", &power);
            fscanf(archive, "%s", cpf);
            fscanf(archive, "%s", date);

            p2 = getCompany(cnpj, companies);
            p3 = getConsumer(cpf, consumers);

            if (!compareStrings(cnpj, "") && p2 != NULL && p3 != NULL)
            {
                insert->company = p2;
                insert->consumer = p3;
                insert->power = power;
                strcpy(insert->date, date);
                insertContractNode(contracts, insert);
            }
            
            strcpy(cnpj, "");
        }
    }

    free(insert);

    fclose(archive);
    rename(ARCHIVE_CONTRACTS, BACKUP_ARCHIVE_CONTRACTS);
    remove(ARCHIVE_CONTRACTS);
}

void updateArchive(const List *consumerList, const List *companyList, const List *contractList)
{
    FILE *archive1 = fopen(ARCHIVE_CONSUMERS, "at");
    FILE *archive2 = fopen(ARCHIVE_COMPANIES, "at");
    FILE *archive3 = fopen(ARCHIVE_CONTRACTS, "at");

    Node *aux1 = consumerList->begin;
    Node *aux2 = companyList->begin;
    Node *aux3 = contractList->begin;

    if (!isEmpty(consumerList))
    {
        while (aux1 != NULL)
        {
            fprintf(archive1, "%s\n", aux1->consumers->name);
            fprintf(archive1, "%s\n", aux1->consumers->cpf);
            aux1 = aux1->next;
        }
    }

    if (!isEmpty(companyList))
    {
        while (aux2 != NULL)
        {
            fprintf(archive2, "%s\n", aux2->companies->name);
            fprintf(archive2, "%s\n", aux2->companies->cnpj);
            fprintf(archive2, "%s\n", aux2->companies->founded);
            fprintf(archive2, "%.2f\n", aux2->companies->potency);
            aux2 = aux2->next;
        }
    }

    if (!isEmpty(contractList))
    {
        while (aux3 != NULL)
        {
            fprintf(archive3, "%s\n", aux3->contracts->company->cnpj);
            fprintf(archive3, "%.2f\n", aux3->contracts->power);
            fprintf(archive3, "%s\n", aux3->contracts->consumer->cpf);
            fprintf(archive3, "%s\n", aux3->contracts->date);
            aux3 = aux3->next;
        }
    }

    fclose(archive1);
    remove(BACKUP_ARCHIVE_CONSUMERS);

    fclose(archive2);
    remove(BACKUP_ARCHIVE_COMPANIES);

    fclose(archive3);
    remove(BACKUP_ARCHIVE_CONTRACTS);
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
    else if (compareStrings(type, CONTRACTS))
    {
        if (isEmpty(list))
        {
            puts("0 contracts found at archive 'contracts.txt'");
        }
        else
        {
            Node *p = list->begin;

            while (p != NULL)
            {
                printf("Contracted company: %s; Potency: %.2f; Consumer: %s; Date: %s\n", p->contracts->company->name, p->contracts->power, p->contracts->consumer->name, p->contracts->date);
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

    if (node->contracts != NULL)
        free(node->contracts);

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

void newContract(List *contracts, const List *consumers, const List *companies)
{
    printf("Soon.\n");
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

CONSUMER *getConsumer(char *cpf, const List *list)
{

    Node *p = list->begin;

    while (p != NULL && !compareStrings(cpf, p->consumers->cpf))
    {
        p = p->next;
    }

    return (p != NULL) ? p->consumers : NULL;
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

COMPANY *getCompany(char *cnpj, const List *list)
{

    Node *p = list->begin;

    while (p != NULL && !compareStrings(cnpj, p->companies->cnpj))
    {
        p = p->next;
    }

    return (p != NULL) ? p->companies : NULL;
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

/*****************************************************/