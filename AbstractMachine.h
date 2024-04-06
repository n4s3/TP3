#ifndef ABSTRACTMACHINE_H_INCLUDED
#define ABSTRACTMACHINE_H_INCLUDED
struct Transaction {
    char operation_code[20];
    char date[20];
    double amount;
    struct Transaction *next;
};
struct BankAccount {
    int account_number;
    int code;
    char customer[100];
    double balance;
    struct Transaction *transactions;
    struct BankAccount *next;
};
typedef struct BankAccount BankAccount;
typedef struct Transaction Transaction;
BankAccount *nextBank(BankAccount *p);
void createAccount(BankAccount **p);
void assignTransactionToAccount(BankAccount *p, Transaction *q);
void assignBankToBank(BankAccount *p, BankAccount *q);
void deleteTransaction(Transaction *p);
void createNewTransaction(Transaction **p);
Transaction *nextTransaction(Transaction *p);
#endif
