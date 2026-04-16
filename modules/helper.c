#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/helper.h"

int askTryAgainOrGoBack(void)
{
    char input[20];
    int choice;
    char extra;

    while (1)
    {
        printf("\n1. Try again\n");
        printf("0. Go back\n");
        printf("Enter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("Invalid input. Please enter 1 or 0.\n");
            continue;
        }

        if (sscanf(input, " %d %c", &choice, &extra) != 1)
        {
            printf("Invalid input. Please enter 1 or 0.\n");
            continue;
        }

        if (choice == 1 || choice == 0)
        {
            return choice;
        }

        printf("Invalid choice. Please enter 1 or 0.\n");
    }
}

void toLowerCase(char str[])
{
    int i;

    for (i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

int generateNextNumber(char fileName[], char prefix[])
{
    FILE *file;
    char line[200];
    char id[20];
    int max;
    int num;
    int prefixLength;

    file = fopen(fileName, "r");

    if (file == NULL)
    {
        return 1;
    }

    max = 0;
    prefixLength = strlen(prefix);

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%19[^|]", id);

        if (strncmp(id, prefix, prefixLength) == 0)
        {
            num = atoi(id + prefixLength);

            if (num > max)
            {
                max = num;
            }
        }
    }

    fclose(file);
    return max + 1;
}
