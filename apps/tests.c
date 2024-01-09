#include <stdlib.h>
#include <stdio.h>
#include "C:\Users\Admin\Desktop\Developer\ws-apc-remake\after\include\worklib.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Correct usage: ./test <function> <input>\n");
        return 1;
    }
    else if (compareStrings(argv[1], "cpf"))
    {
        if (checkCPF(argv[2]))
        {
            printf("The CPF %s is valid!\n", argv[2]);
            return 0;
        }
        else
        {
            printf("The CPF %s isn't valid!\n", argv[2]);
            return 1;
        }
    }
    else if (compareStrings(argv[1], "date"))
    {
        if (checkDate(argv[2]))
        {
            printf("The date '%s' is valid!\n", argv[2]);
            return 0;
        }
        else 
        {
            printf("Date invalid.\n");
            printf("Format: dd/mm/aaaa\n");
            return 1;
        }
    }
    else if (compareStrings(argv[1], "cnpj"))
    {
        if (checkCNPJ(argv[2]))
        {
            printf("The CNPJ %s is valid!\n", argv[2]);
            return 0;
        }
        else
        {
            printf("The CNPJ %s isn't valid!\n", argv[2]);
            return 1;
        }
            
    }
    else 
    {
        puts("Something went wrong.");
        return 1;
    }
}
