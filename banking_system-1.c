#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define FILENAME "accounts.dat"

// ANSI Color Codes
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

struct Account {
    int accountNumber;
    char holderName[50];
    char accountType[20];
    float balance;
};

struct Account accounts[MAX_ACCOUNTS];
int count = 0;

void loadFromFile() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) return;
    fread(&count, sizeof(int), 1, fp);
    fread(accounts, sizeof(struct Account), count, fp);
    fclose(fp);
}

void saveToFile() {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf(RED "Error saving data!\n" RESET);
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(accounts, sizeof(struct Account), count, fp);
    fclose(fp);
}

int findAccount(int accNum) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accNum)
            return i;
    }
    return -1;
}

void createAccount() {
    if (count >= MAX_ACCOUNTS) {
        printf(RED "Account limit reached!\n" RESET);
        return;
    }
    struct Account a;
    printf(CYAN "\n Enter Account Number: " RESET);
    scanf("%d", &a.accountNumber);

    if (findAccount(a.accountNumber) != -1) {
        printf(RED "Account already exists!\n" RESET);
        return;
    }

    printf(CYAN " Enter Account Holder Name: " RESET);
    scanf(" %[^\n]", a.holderName);
    printf(CYAN " Account Type (Savings/Current): " RESET);
    scanf(" %[^\n]", a.accountType);
    printf(CYAN " Enter Initial Deposit (min 0): " RESET);
    scanf("%f", &a.balance);

    accounts[count++] = a;
    saveToFile();
    printf(GREEN "\n Account created successfully!\n" RESET);
    printf(GREEN " Account Number: %d\n" RESET, a.accountNumber);
    printf(GREEN " Holder: %s\n" RESET, a.holderName);
    printf(GREEN " Balance: GHS %.2f\n" RESET, a.balance);
}

void deposit() {
    int accNum;
    float amount;
    printf(CYAN "\n Enter Account Number: " RESET);
    scanf("%d", &accNum);
    int idx = findAccount(accNum);
    if (idx == -1) {
        printf(RED " Account not found!\n" RESET);
        return;
    }
    printf(CYAN " Enter Deposit Amount: GHS " RESET);
    scanf("%f", &amount);
    if (amount <= 0) {
        printf(RED " Invalid amount!\n" RESET);
        return;
    }
    accounts[idx].balance += amount;
    saveToFile();
    printf(GREEN "\n Deposit Successful!\n" RESET);
    printf(GREEN " New Balance: GHS %.2f\n" RESET, accounts[idx].balance);
}

void withdraw() {
    int accNum;
    float amount;
    printf(CYAN "\n Enter Account Number: " RESET);
    scanf("%d", &accNum);
    int idx = findAccount(accNum);
    if (idx == -1) {
        printf(RED " Account not found!\n" RESET);
        return;
    }
    printf(CYAN " Enter Withdrawal Amount: GHS " RESET);
    scanf("%f", &amount);
    if (amount <= 0) {
        printf(RED " Invalid amount!\n" RESET);
        return;
    }
    if (amount > accounts[idx].balance) {
        printf(RED " Insufficient funds! Balance: GHS %.2f\n" RESET, accounts[idx].balance);
        return;
    }
    accounts[idx].balance -= amount;
    saveToFile();
    printf(GREEN "\n Withdrawal Successful!\n" RESET);
    printf(GREEN " Remaining Balance: GHS %.2f\n" RESET, accounts[idx].balance);
}

void balanceEnquiry() {
    int accNum;
    printf(CYAN "\n Enter Account Number: " RESET);
    scanf("%d", &accNum);
    int idx = findAccount(accNum);
    if (idx == -1) {
        printf(RED " Account not found!\n" RESET);
        return;
    }
    printf(BLUE "\n ---- Account Details ----\n" RESET);
    printf(" Account No : %d\n", accounts[idx].accountNumber);
    printf(" Holder     : %s\n", accounts[idx].holderName);
    printf(" Type       : %s\n", accounts[idx].accountType);
    printf(GREEN " Balance    : GHS %.2f\n" RESET, accounts[idx].balance);
}

void displayAll() {
    if (count == 0) {
        printf(YELLOW "\n No accounts found!\n" RESET);
        return;
    }
    printf(BLUE "\n %-10s %-20s %-10s %-10s\n" RESET,
        "Acc No", "Holder Name", "Type", "Balance");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf(" %-10d %-20s %-10s GHS %.2f\n",
            accounts[i].accountNumber,
            accounts[i].holderName,
            accounts[i].accountType,
            accounts[i].balance);
    }
}

int main() {
    loadFromFile();
    int choice;

    do {
        printf(BLUE "\n\n ==========================================\n" RESET);
        printf(YELLOW "    CODEALPHA - BANKING MANAGEMENT SYSTEM \n" RESET);
        printf(BLUE " ==========================================\n" RESET);
        printf(GREEN " 1. Create Account\n" RESET);
        printf(GREEN " 2. Deposit\n" RESET);
        printf(GREEN " 3. Withdraw\n" RESET);
        printf(GREEN " 4. Balance Enquiry\n" RESET);
        printf(GREEN " 5. Display All Accounts\n" RESET);
        printf(RED " 6. Exit\n" RESET);
        printf(CYAN "\n Enter your choice: " RESET);
        scanf("%d", &choice);

        switch(choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: balanceEnquiry(); break;
            case 5: displayAll(); break;
            case 6: printf(YELLOW "\n Thank you for using CodeAlpha Bank!\n\n" RESET); break;
            default: printf(RED "\n Invalid choice! Try again.\n" RESET);
        }
    } while (choice != 6);

    return 0;
}
