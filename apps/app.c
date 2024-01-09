#include <stdio.h>
#include "C:\Users\Admin\Desktop\Developer\ws-apc-remake\after\include\worklib.h"

int main(void)
{
    int option;

    List *consumersList = createList(CONSUMERS);
    List *companyList = createList(COMPANIES);
    loadFromArchive(consumersList, CONSUMERS);
    loadFromArchive(companyList, COMPANIES);

    do
    {
        puts("--------------");
        printList(consumersList, CONSUMERS);
        puts("--------------");
        printList(companyList, COMPANIES);
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

    updateArchive(consumersList, companyList);

    destroyList(&consumersList);
    destroyList(&companyList);
}