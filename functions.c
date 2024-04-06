#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADMIN_CODE 123 // Predefined admin code

void clear_screen() { system("cls || clear"); }

void initialize_list(BankAccount **head) { *head = NULL; }

void read_accounts_from_file(BankAccount **head, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open %s for reading.\n", filename);
    return;
  }

  struct BankAccount new_account;
  int num_accounts_read = 0;
  while (fscanf(file, "%d %d %99s %lf", &new_account.account_number,
                &new_account.code, new_account.customer,
                &new_account.balance) == 4) {
    add_account(head, new_account);
    num_accounts_read++;
  }

  fclose(file);

  if (num_accounts_read == 0) {
    printf("No accounts found in %s.\n", filename);
  } else {
    printf("Successfully read %d accounts from %s.\n", num_accounts_read,
           filename);
  }
}

void read_transactions_from_file(BankAccount **head, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open file.\n");
    return;
  }

  int account_number;
  char operation_code[20];
  char date[20];
  double amount;

  while (fscanf(file, "%d %s %s %lf", &account_number, operation_code, date,
                &amount) == 4) {
    BankAccount *current = *head;
    while (current != NULL) {
      if (current->account_number == account_number) {
        Transaction *new_transaction;
        createNewTransaction(&new_transaction);

        if (new_transaction == NULL) {
          printf("Memory allocation failed.\n");
          fclose(file);
          return;
        }

        strcpy(new_transaction->operation_code, operation_code);
        strcpy(new_transaction->date, date);
        new_transaction->amount = amount;

        assignTransactionToAccount(current, new_transaction);
        new_transaction->next = NULL;

        break;
      }
      current = nextBank(current);
    }
  }

  fclose(file);
}

void add_account(BankAccount **head, struct BankAccount new_account) {
  BankAccount *new_node;
  createAccount(&new_node);
  if (new_node == NULL) {
    printf("Memory allocation failed.\n");
    return;
  }

  new_node->account_number = new_account.account_number;
  new_node->code = new_account.code;
  strcpy(new_node->customer, new_account.customer);
  new_node->balance = new_account.balance;
  new_node->transactions = NULL;

  new_node->next = *head;
  *head = new_node;
}

void delete_account(BankAccount **head, int account_number) {
  BankAccount *temp = *head, *prev = NULL;
  if (temp != NULL && temp->account_number == account_number) {
    *head = nextBank(temp);
    free(temp);
    return;
  }

  while (temp != NULL && temp->account_number != account_number) {
    prev = temp;
    temp = nextBank(temp);
  }

  if (temp == NULL)
    return;

  prev->next = nextBank(temp);
  free(temp);
}

void edit_account(BankAccount **head, int account_number) {
  BankAccount *current = *head;
  while (current != NULL) {
    if (current->account_number == account_number) {
      printf("Enter new account number: ");
      scanf("%d", &current->account_number);
      printf("Enter new code: ");
      scanf("%d", &current->code);
      printf("Enter new customer name: ");
      scanf("%s", current->customer);
      printf("Enter new balance: ");
      scanf("%lf", &current->balance);
      return;
    }
    current = nextBank(current);
  }
  printf("Account not found.\n");
}

void display_accounts(BankAccount *head) {
  if (head == NULL) {
    printf("No accounts found.\n");
    return;
  }

  printf("Accounts:\n");
  printf("Account Number\tCode\tCustomer\tBalance\n");
  BankAccount *current = head;
  while (current != NULL) {
    printf("%d\t\t%d\t%s\t%.2f\n", current->account_number, current->code,
           current->customer, current->balance);
    current = nextBank(current);
  }
}

void transfer_amount(BankAccount **head, int sender_account,
                     int receiver_account, double amount) {
  BankAccount *sender = *head;
  BankAccount *receiver = *head;
  while (sender != NULL && sender->account_number != sender_account)
    sender = nextBank(sender);
  while (receiver != NULL && receiver->account_number != receiver_account)
    receiver = nextBank(receiver);
  if (sender == NULL || receiver == NULL) {
    printf("Invalid account numbers.\n");
    return;
  }
  if (sender->balance < amount) {
    printf("Insufficient balance in the sender's account.\n");
    return;
  }
  sender->balance -= amount;
  receiver->balance += amount;
  Transaction *p;
  createNewTransaction(&p);
  strcpy(p->operation_code, "transfer");
  printf("Enter the date in the form YYYY-MM-DD: ");
  scanf("%s", p->date);
  p->amount = -amount;
  p->next = sender->transactions;
  assignTransactionToAccount(sender, p);
  Transaction *q;
  createNewTransaction(&q);
  strcpy(q->date, p->date);
  strcpy(q->operation_code, "transfer");
  q->amount = amount;
  q->next = receiver->transactions;
  assignTransactionToAccount(receiver, q);
  deleteTransaction(p);
  deleteTransaction(q);
  printf("Amount transferred successfully.\n");
}

void deposit_fund(BankAccount **head, int account_number, double amount) {
  BankAccount *current = *head;
  while (current != NULL && current->account_number != account_number) {
    current = nextBank(current);
  }
  if (current == NULL) {
    printf("Account not found.\n");
    return;
  }
  current->balance += amount;
  Transaction *q;
  createNewTransaction(&q);
  printf("Enter the date in the form YYYY-MM-DD: ");
  scanf("%s", q->date);
  strcpy(q->operation_code, "deposit");
  q->amount = amount;
  q->next = current->transactions;
  assignTransactionToAccount(current, q);
  deleteTransaction(q);
  printf("Amount deposited successfully.\n");
}

void withdraw_fund(BankAccount **head, int account_number, double amount) {
  BankAccount *current = *head;
  while (current != NULL && current->account_number != account_number) {
    current = nextBank(current);
  }
  if (current == NULL) {
    printf("Account not found.\n");
    return;
  }
  if (current->balance < amount) {
    printf("Insufficient balance.\n");
    return;
  }
  current->balance -= amount;
  Transaction *q;
  createNewTransaction(&q);
  printf("Enter the date in the form YYYY-MM-DD: ");
  scanf("%s", q->date);
  strcpy(q->operation_code, "withdraw");
  q->amount = -amount;
  q->next = current->transactions;
  assignTransactionToAccount(current, q);
  deleteTransaction(q);
  printf("Amount withdrawn successfully.\n");
}

int authenticate_admin() {
  int code;
  printf("Enter admin code: ");
  scanf("%d", &code);
  if (code == ADMIN_CODE)
    return 1;
  else
    return 0;
}

void save_accounts_to_file(BankAccount *head, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Failed to open file.\n");
    return;
  }

  BankAccount *current = head;
  while (current != NULL) {
    fprintf(file, "%d %d %s %.2f\n", current->account_number, current->code,
            current->customer, current->balance);
    current = nextBank(current);
  }

  fclose(file);
}

void save_transactions_to_file(BankAccount *head, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Failed to open file.\n");
    return;
  }

  BankAccount *current = head;
  while (current != NULL) {
    Transaction *t = current->transactions;
    while (t != NULL) {
      fprintf(file, "%d %s %s %.2f\n", current->account_number,
              t->operation_code, t->date, t->amount);
      t = nextTransaction(t);
    }
    current = nextBank(current);
  }

  fclose(file);
}

void check_total_amount(BankAccount *head, int account_number) {
  BankAccount *current = head;
  while (current != NULL) {
    if (current->account_number == account_number) {
      printf("Total amount in account %d: %.2f\n", account_number,
             current->balance);
      return;
    }
    current = nextBank(current);
  }
  printf("Account not found.\n");
}

void view_transaction_history(BankAccount *head, int account_number) {
  BankAccount *current = head;
  while (current != NULL) {
    if (current->account_number == account_number) {
      printf("Transaction history for account %d:\n", account_number);
      Transaction *t = current->transactions;
      while (t != NULL) {
        printf("Date: %s, Operation: %s, Amount: %.2f\n", t->date,
               t->operation_code, t->amount);
        t = nextTransaction(t);
      }
      return;
    }
    current = nextBank(current);
  }
  printf("Account not found.\n");
}
// Function to display admin menu
void admin_menu(BankAccount **head) {
  int choice;
  do {
    printf("\nAdmin Menu:\n");
    printf("1. Add Account\n");
    printf("2. Delete Account\n");
    printf("3. Edit Account\n");
    printf("4. Display Accounts\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1: {
      struct BankAccount new_account;
      printf("Enter account number: ");
      scanf("%d", &new_account.account_number);
      printf("Enter code: ");
      scanf("%d", &new_account.code);
      printf("Enter customer name: ");
      scanf("%s", new_account.customer);
      printf("Enter balance: ");
      scanf("%lf", &new_account.balance);
      add_account(head, new_account);
      break;
    }
    case 2: {
      int account_number;
      printf("Enter account number to delete: ");
      scanf("%d", &account_number);
      delete_account(head, account_number);
      break;
    }
    case 3: {
      int account_number;
      printf("Enter account number to edit: ");
      scanf("%d", &account_number);
      edit_account(head, account_number);
      break;
    }
    case 4:
      display_accounts(*head);
      break;
    case 5:
      printf("Exiting admin menu.\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 6);
}

// Function to display customer menu
void customer_menu(BankAccount **head) {
  int account_number;
  printf("Enter account number: ");
  scanf("%d", &account_number);

  BankAccount *current = *head;
  while (current != NULL) {
    if (current->account_number == account_number) {
      int choice;
      do {
        printf("\nCustomer Menu:\n");
        printf("1. Check Total Amount\n");
        printf("2. Deposit Fund\n");
        printf("3. Withdraw Fund\n");
        printf("4. Transfer Amount\n");
        printf("5. Check Transaction History\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
          check_total_amount(*head, account_number);
          break;
        case 2: {
          double amount;
          printf("Enter amount to deposit: ");
          scanf("%lf", &amount);
          deposit_fund(head, account_number, amount);
          break;
        }
        case 3: {
          double amount;
          printf("Enter amount to withdraw: ");
          scanf("%lf", &amount);
          withdraw_fund(head, account_number, amount);
          break;
        }
        case 4: {
          int receiver_account;
          double amount;
          printf("Enter receiver's account number: ");
          scanf("%d", &receiver_account);
          printf("Enter amount to transfer: ");
          scanf("%lf", &amount);
          transfer_amount(head, account_number, receiver_account, amount);
          break;
        }
        case 5: {
          char start_date[20], end_date[20];
          printf("Enter start date (YYYY-MM-DD): ");
          scanf("%s", start_date);
          printf("Enter end date (YYYY-MM-DD): ");
          scanf("%s", end_date);
          check_transaction_history(*head, account_number, start_date,
                                    end_date);
          break;
        }
        case 6:
          printf("Exiting customer menu.\n");
          break;
        default:
          printf("Invalid choice. Please try again.\n");
        }
      } while (choice != 6);
      return;
    }
    current = nextBank(current);
  }
  printf("Account not found.\n");
}

void add_account_menu(BankAccount **head) {
  struct BankAccount new_account;
  printf("Enter account number: ");
  scanf("%d", &new_account.account_number);
  printf("Enter code: ");
  scanf("%d", &new_account.code);
  printf("Enter customer name: ");
  scanf("%s", new_account.customer);
  printf("Enter balance: ");
  scanf("%lf", &new_account.balance);
  add_account(head, new_account);
  printf("Account added successfully.\n");
}

// Function to get numerical representation of date (YYYYMMDD)
int get_numeric_date(const char *date) {
  int year, month, day;
  sscanf(date, "%d-%d-%d", &year, &month, &day);
  return year * 10000 + month * 100 + day;
}

// Function to check transaction history within a date range
void check_transaction_history_by_date(BankAccount *head, int account_number,
                                       const char *date) {
  BankAccount *current = head;
  while (current != NULL) {
    if (current->account_number == account_number) {
      printf("Transaction history for account %d on %s:\n", account_number,
             date);
      Transaction *t = current->transactions;
      int found = 0; // Flag to check if any transactions found for the date
      while (t != NULL) {
        if (strcmp(t->date, date) == 0) {
          printf("Operation: %s, Amount: %.2f\n", t->operation_code, t->amount);
          found = 1; // Set the flag to indicate at least one transaction found
        }
        t = nextTransaction(t);
      }
      if (!found) {
        printf("No transactions found for the specified date.\n");
      }
      return;
    }
    current = nextBank(current);
  }
  printf("Account not found.\n");
}
