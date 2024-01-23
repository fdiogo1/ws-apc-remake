#include "C:\Users\Admin\Desktop\Developer\ws-apc-remake\after\include\worklib.h"
#include <stdio.h>

int main(void)
{
    int option;

    List *consumersList = createList(CONSUMERS);
    List *companyList = createList(COMPANIES);
    List *contractsList = createList(CONTRACTS);
    loadFromArchive(consumersList, CONSUMERS);
    loadFromArchive(companyList, COMPANIES);
    loadContracts(contractsList, consumersList, companyList);

    do
    {
        puts("--------------");
        printList(consumersList, CONSUMERS);
        puts("--------------");
        printList(companyList, COMPANIES);
        puts("--------------");
        printList(contractsList, CONTRACTS);
        puts("--------------");

        // printMenu();
        fflush(stdin);
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                newCompany(companyList);
                break; 
            case 2:
                newConsumer(consumersList);
                break;
            case 8:
                break;
            default:
                puts("Functionality not found. Try again.");
                break;
        }

    } 
    while (option != 8);

    updateArchive(consumersList, companyList, contractsList);

    destroyList(&consumersList);
    destroyList(&companyList);
    destroyList(&contractsList);
}