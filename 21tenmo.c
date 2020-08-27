/**
 * @file 21tenmo.c
 * @author YOUR NAME HERE
 */

// DO NOT MODIFY THE INCLUDE(s) LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "21tenmo.h"
#include "errcodes.h"

/**
 * @brief method for executing the command selected from the menu.
 *
 * @param choice a single number character indicating the command to be executed
 * @param accountNumber for retrieving info from the DB
 * @return State the state of the program (USER_EXIT is the 'finishing' state)
 */
State executeCommand(char choice, unsigned int accountNumber) {
    switch (choice) {
        case '1':
            // TODO: implement
            printf("Current Balance: %.2f", arr[accountNumber - FIRST_ACCOUNT_NUMBER].balance);
            return SHOW_BALANCE;
        case '2':
            modifyBalance(accountNumber);
            return MODIFY_BALANCE;
        case '3':
            sendMoney(accountNumber);
            return SEND_MONEY;
        case '4':
            requestMoney(accountNumber);
            return RECEIVE_MONEY;
        case '5':
            checkRequests(accountNumber);
            return REQUESTS;
        case '6':
            // TODO: implement
            writeCSV();
            return USER_EXIT;
        default:
            printf("ERROR: Invalid choice entered (%c)\n", choice);
            return MENU;
    }
}

/**
 * @brief helper method for depositing/withdrawing fund to the active account.
 *
 * @param accountNumber for retrieving info from the DB
 */
void modifyBalance(unsigned int accountNumber) {
    UNUSED_PARAM(accountNumber);

    // TODO: implement -- Uncomment the printf below when not testing
    myErrNo = NO_ERROR;


    printf("Enter an amount to deposit/withdraw (for withdraw add a '-' before the number): $");
    char buffer[50];
    getLine(buffer, 50);
    float amount = strtof(buffer, NULL);

    if ((arr[accountNumber - FIRST_ACCOUNT_NUMBER].balance + amount) >= 0) {
        arr[accountNumber - FIRST_ACCOUNT_NUMBER].balance += amount;
    } else {
        myErrNo = ERR_NOT_ENOUGH_FUNDS;
        return;
    }
}

/**
 * @brief helper method for sending money to another account from current active account.
 *
 * @param senderNumber for retrieving info from the DB
 */
void sendMoney(unsigned int senderNumber) {
    UNUSED_PARAM(senderNumber);

    // TODO: implement  -- Uncomment the printf calls below when not testing
    printf("Enter the accountNumber of the receiver: ");
    char buffer[50];
    getLine(buffer, 50);
    unsigned int recieveNum = strtoul(buffer, NULL, 10);

    if (formatAndCheckCurrency(buffer) == -1) {
        myErrNo = ERR_INVALID_AMOUNT;
        return;
    }

    if(recieveNum < FIRST_ACCOUNT_NUMBER || recieveNum > (FIRST_ACCOUNT_NUMBER + MAX_CSV_SIZE - 1)) {
        myErrNo = ERR_INVALID_ACC_NUM;
        return;
    } else if (arr[recieveNum - FIRST_ACCOUNT_NUMBER].accountNumber == 0) {
        myErrNo = ERR_INACTIVE_ACCOUNT;
        return;
    } else if (senderNumber < FIRST_ACCOUNT_NUMBER || senderNumber > (FIRST_ACCOUNT_NUMBER + MAX_CSV_SIZE - 1)) {
        myErrNo = ERR_INVALID_ACC_NUM;
        return;
    } else if (arr[senderNumber - FIRST_ACCOUNT_NUMBER].accountNumber == 0) {
        myErrNo = ERR_INACTIVE_ACCOUNT;
        return;
    } else if (senderNumber == recieveNum) {
        myErrNo = ERR_SELF_REQUEST;
        return;
    }

    printf("Enter the amount you want to send: $");
    getLine(buffer, 50);
    float send = strtof(buffer, NULL);
    if (send <= 0) {
        myErrNo = ERR_INVALID_AMOUNT;
        return;
    } else if ((arr[senderNumber - FIRST_ACCOUNT_NUMBER].balance - send) < 0) {
        myErrNo = ERR_NOT_ENOUGH_FUNDS;
        return;
    } else {
        arr[senderNumber - FIRST_ACCOUNT_NUMBER].balance -= send;
        arr[recieveNum - FIRST_ACCOUNT_NUMBER].balance += send;
    }
}

/**
 * @brief helper method for formatting and checking a currencyString
 *
 * In this program, a currencyString can have at most 1 dot and/or
 * at most one '-' sign, and many commas. Comma separtion
 * validation is not a requirement.
 *
 * Note: In case of error, the passed in string should NOT be modified
 *
 * @param currencyString the string to validate/format
 * @return int 0 on successful formatting, nonzero upon error
 */
int formatAndCheckCurrency(char *currencyString) {
    UNUSED_PARAM(currencyString);

    // TODO: implement
    int pCount = 0;
    char str[strlen(currencyString)];
    int dCount = 0;
    int length = strlen(currencyString);
    for (int i = 0; i < length; i++) {
        if (!(i != 0 || currencyString[i] != '-' || strlen(currencyString) <= 1)) {
            str[dCount] = currencyString[i];
            dCount++;
        } else if (pCount > 1) {
            myErrNo = ERR_INVALID_AMOUNT;
            return -1;
        } else if (currencyString[i] == ',') {
            continue;
        } else if (currencyString[i] == '.') {
            pCount++;
            str[dCount] = currencyString[i];
            dCount++;
        } else if (currencyString[i] < '0' || currencyString[i] > '9') {
            myErrNo = ERR_INVALID_AMOUNT;
            return -1;
        } else {
            str[dCount] = currencyString[i];
            dCount++;
        }
    }
    str[dCount] = 0;
    strcpy(currencyString, str);
    return 0;
}

/**
 * @brief helper method for requesting money from a different account
 *
 * @param accountNumber for retrieving info from the DB
 */
void requestMoney(unsigned int accountNumber) {
    UNUSED_PARAM(accountNumber);
    // TODO: implement -- Uncomment the printf below when not testing
    printf("Enter an accountNumber to request from: ");
    char buffer[50];
    getLine(buffer, 50);
    unsigned int reqNumAcc = strtoul(buffer, NULL, 10);
    if(reqNumAcc < FIRST_ACCOUNT_NUMBER || reqNumAcc > (FIRST_ACCOUNT_NUMBER + MAX_CSV_SIZE - 1)) {
        myErrNo = ERR_INVALID_ACC_NUM;
        return;
    } else if (arr[reqNumAcc - FIRST_ACCOUNT_NUMBER].accountNumber == 0) {
        myErrNo = ERR_INACTIVE_ACCOUNT;
        return;
    } else if (arr[reqNumAcc - FIRST_ACCOUNT_NUMBER].requesterNumber != 0) {
        myErrNo = ERR_EXISTING_REQUEST;
        return;
    } else if (accountNumber == reqNumAcc) {
        myErrNo = ERR_SELF_REQUEST;
        return;
    }

    printf("Enter the amount you want to ask this sender: $");
    getLine(buffer, 50);
    float request = strtof(buffer, NULL);
    if (request > 0) {
        arr[reqNumAcc - FIRST_ACCOUNT_NUMBER].requesterNumber = accountNumber;
        arr[reqNumAcc - FIRST_ACCOUNT_NUMBER].requestAmount = request;
    } else {
        myErrNo = ERR_INVALID_AMOUNT;
        return;
    }

}

/**
 * @brief helper method for checking/approving/denying pending requests.
 *
 * @param accountNumber for retrieving info from the DB
 */
void checkRequests(unsigned int accountNumber) {
    UNUSED_PARAM(accountNumber);
    // TODO: implement
    char buffer[50];
    getLine(buffer, 50);

    //int y = strcmp(buffer, "Y");
    //int n = strcmp(buffer, "N");

    if (strcmp(buffer, "Y") == 0) {
        if (arr[accountNumber - FIRST_ACCOUNT_NUMBER].balance < arr[accountNumber - FIRST_ACCOUNT_NUMBER].requestAmount) {
            arr[accountNumber - FIRST_ACCOUNT_NUMBER].requestAmount = 0;
            arr[accountNumber - FIRST_ACCOUNT_NUMBER].requesterNumber = 0;
            myErrNo = ERR_NOT_ENOUGH_FUNDS;
            return;
        }
    }
    if (strcmp(buffer, "N") == 0) {
        arr[accountNumber - FIRST_ACCOUNT_NUMBER].requestAmount = 0;
        arr[accountNumber - FIRST_ACCOUNT_NUMBER].requesterNumber = 0;
        return;
    }
    if (!(strcmp(buffer, "Y") == 0 || strcmp(buffer, "N") == 0)) {
        myErrNo = ERR_INVALID_ARGS;
        return;
    }
    if (arr[accountNumber - FIRST_ACCOUNT_NUMBER].requesterNumber == 0) {
        printf("no funds pending in account");
        return;
    }

    int numRequired = arr[accountNumber - FIRST_ACCOUNT_NUMBER].requesterNumber;
    arr[accountNumber - FIRST_ACCOUNT_NUMBER].balance -= arr[accountNumber - FIRST_ACCOUNT_NUMBER].requestAmount;
    arr[numRequired - FIRST_ACCOUNT_NUMBER].balance += arr[accountNumber - FIRST_ACCOUNT_NUMBER].requestAmount;
    arr[accountNumber - FIRST_ACCOUNT_NUMBER].requestAmount = 0;
    arr[accountNumber - FIRST_ACCOUNT_NUMBER].requesterNumber = 0;
}

// ----------------------- DO NOT MODIFY CODE BELOW -----------------------
/**
 * @brief entry point of program for current users; called from main to display menu and process commands.
 *
 * @param accountNumber for retrieving info from the DB
 */
void userMenu(unsigned int accountNumber) {
    Account acc = getAccountForTAUseOnly(accountNumber); // HEADER FILE NEEDS TO BE IMPLEMENTED FIRST

    // User operations --> IMPORTANT NOTE: User has to exit to write the DB @see executeCommand
    State currState = MENU;

    // TA USE ONLY: For reading input --> this has to be at least bigger than 1 char + NULL terminator + 1 (extra character)
    // to determine invalid inputs such as "12" --> this is due to the way getLine() is setup
    char choiceBuffer[3];
    while (currState != USER_EXIT) {
        // Resetting err number
        myErrNo = NO_ERROR;

        showMenu(acc.accountNumber);

        // Reading input
        getLine(choiceBuffer, sizeof(choiceBuffer));

        if (strlen(choiceBuffer) == 1) {
            // Processing input
            currState = executeCommand(choiceBuffer[0], accountNumber);
        } else {
            myErrNo = ERR_INVALID_ARGS;
            printf("ERROR: Invalid input\n");
        }
    }
}

/**
 * @brief Helper function for displaying the user options menu.
 *
 * @param accountNumber for retrieving info from the DB
 */
void showMenu(unsigned int accountNumber) {
    (void) (accountNumber);
    // Feel free to add more details to the menu title such as the personName!

    printf("\n$$$$$$$$$$$$$$$$$$$$$ Welcome to 21tenmo ! $$$$$$$$$$$$$$$$$$$$$\n");
    printf("\nSelect one of the options below (just the number):\n");
    printf("1. Show Balance\n");
    printf("2. Modify Balance\n");
    printf("3. Send Money\n");
    printf("4. Request Money\n");
    printf("5. Check Requests\n");
    printf("6. Save & Exit\n");
    printf("\n> ");
}
