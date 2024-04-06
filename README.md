# TP-ALSDD Ms BOUZIANE G08 
***BELARBI ANES ABDELMALEK, NEKHOUL MOHAMED ACIL G08***


## Project Nature
*Developing an application for managing bank accounts process and allowing 
customers to access their account and perform various banking transactions.*

#### Customer's Transactions
-  Transferring amount from one account to another. 
-  Deposit fund to an account. 
-  Make a withdrawal of funds.  
-  Check the total amount.  
-  Check the history of bank transactions within a given date. 
#### Admin's procedures 🖥️
-  Add customer accounts.  
-  Delete customer accounts. 
-  Edit Code/Customer. 


## Program Structure 💻



`functions.c` all the functionalities of the program (transactions, reading input from file...etc).
``` bash 

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "AbstractMachine.h"

void clear_screen();
void initialize_list(BankAccount **head);
void read_accounts_from_file(BankAccount **head, const char *filename);
void read_transactions_from_file(BankAccount **head, const char *filename);
void add_account(BankAccount **head, struct BankAccount new_account);
void delete_account(BankAccount **head, int account_number);
void edit_account(BankAccount **head, int account_number);
void display_accounts(BankAccount *head);
void transfer_amount(BankAccount **head, int sender_account, int receiver_account, double amount);
void deposit_fund(BankAccount **head, int account_number, double amount);
void withdraw_fund(BankAccount **head, int account_number, double amount);
int authenticate_admin();
void save_accounts_to_file(BankAccount *head, const char *filename);
void save_transactions_to_file(BankAccount *head, const char *filename);
void check_total_amount(BankAccount *head, int account_number);
void view_transaction_history(BankAccount *head, int account_number);
void admin_menu(BankAccount **head);
void customer_menu(BankAccount **head);
void add_account_menu(BankAccount **head);
void delete_account_menu(BankAccount **head);
void edit_account_menu(BankAccount *head);
void transfer_amount_menu(BankAccount *head);
void deposit_fund_menu(BankAccount *head);
void withdraw_fund_menu(BankAccount *head);
void check_total_amount_menu(BankAccount *head);
void view_transaction_history_menu(BankAccount *head);
void check_transaction_history(BankAccount *head, int account_number, const char *start_date, const char *end_date);

#endif /* FUNCTIONS_H */


```

`AbstractMachine.c` standard abstract machine as you know it.
``` bash 
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

```

### How Do We read from the .txt files?
``` bash
659036 2154 AnesBelarbi 1456.25
```
- **Accounts.txt** In this sample from accounts.txt the data is represented as such : (AccNum, Code, Firstname_LastName, Balance) **Just a quick side note ❗: The Account Name Is in the form FirstNameLastName just because we ran into some issues when we tried to make it seprate (when reading the file)**
``` bash
214587 deposit 2023-01-05 54.00
```
- **Transaction.txt**  In this sample the data is represented as such (AccNum, OperactionCode, Date(should be YYYY-MM-DD), Amount)
- for more about that check (fucntions.c **read_accounts_from_file** and **read_transactions_from_file**)
---


### Installation

Clone this project!

```bash
git clone https://github.com/n4s3/TP3.git
```
