/**
 * @file main.c
 * @author YOUR NAME HERE
 */

// DO NOT MODIFY THE INCLUDE(s) LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "21tenmo.h"
#include "errcodes.h"

// Initializing the global err number variable to default
unsigned int myErrNo = NO_ERROR;

/**
 * @brief helper function for printing out the usage of the program.
 */
void printUsage(void) {
    printf("Usage for adding a new account:      ./21tenmo -a \"personName\"\n");
    printf("Usage for logging in a current user: ./21tenmo -l accountNumber\n");
}

/**
 * @brief MAIN Method, responsible for parsing & validating arguments and
 * handling events (e.g. modifying the database, logging in user).
 */
int main(int argc, char const *argv[])
{
    if (argc < 3) {
       myErrNo = ERR_INVALID_ARGS;
       myExit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-a") == 0) {
       DBsize = DBsize + 1;
       readCSV();
       Account acc;
       if (strlen(argv[2]) <= MAX_NAME_SIZE && strlen(argv[2]) >= MIN_NAME_SIZE) {
          strcpy(acc.personName, argv[2]);
       } else {
          myErrNo = ERR_INVALID_NAME;
          myExit(EXIT_FAILURE);
       }
       int num1 = FIRST_ACCOUNT_NUMBER + (DBsize - 1);
       if (num1 > (FIRST_ACCOUNT_NUMBER + MAX_CSV_SIZE - 1)) {
           myErrNo = ERR_DB_FULL;
           myExit(EXIT_FAILURE);
       }
       acc.accountNumber = num1;
       arr[DBsize - 1] = acc;
       writeCSV();

    } else if (strcmp(argv[1],"-l") == 0) {
       int num2 = strtoul(argv[2], NULL, 10);
       if (num2 < FIRST_ACCOUNT_NUMBER && (num2 > (FIRST_ACCOUNT_NUMBER + MAX_CSV_SIZE - 1))) {
          readCSV();
          userMenu(num2);
       } else {
         myErrNo = ERR_INVALID_ACC_NUM;
         myExit(EXIT_FAILURE);
       }
    } else {
         printUsage();
         myErrNo = ERR_INVALID_ARGS;
         myExit(EXIT_FAILURE);
    }

    UNUSED_PARAM(argc);
    UNUSED_PARAM(argv);
    //*** Do NOT remove this ***
    return EXIT_SUCCESS;
}
