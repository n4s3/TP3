
#include "AbstractMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to return the next bank account in the list
BankAccount *nextBank(BankAccount *p) { return p->next; }

// Function to create a new bank account
void createAccount(BankAccount **p) {
  *p = (BankAccount *)malloc(sizeof(BankAccount));
  if (*p == NULL) {
    printf("Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
}

// Function to assign a transaction to an account
void assignTransactionToAccount(BankAccount *p, Transaction *q) {
  if (p->transactions == NULL) {
    p->transactions = q;
  } else {
    q->next = p->transactions;
    p->transactions = q;
  }
}

// Function to assign a bank account to another bank account
void assignBankToBank(BankAccount *p, BankAccount *q) { p->next = q; }

// Function to delete a transaction
void deleteTransaction(Transaction *p) { free(p); }

// Function to create a new transaction
void createNewTransaction(Transaction **p) {
  *p = (Transaction *)malloc(sizeof(Transaction));
  if (*p == NULL) {
    printf("Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
}

// Function to return the next transaction in the list
Transaction *nextTransaction(Transaction *p) { return p->next; }
