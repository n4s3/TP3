#include "functions.h"       // Procedures .h File ...
#include "AbstractMachine.h" // Abstract Machine Functions ...
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Used to call the function 'strcpy'
//
// STANDARD C COLOR REPRESENTATION FOR A BETTER MENU
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"

#define ADMIN_CODE 123 // Predefined admin code
#define transfer "transfer"
#define deposit "deposit"
#define withdrawal "withdrawal"

void clear_screen() {
  system("cls || clear");
} // A function that clears the screen cls for windows and clear for linux

void initialize_list(BankAccount **head) {
  *head = NULL;
} // We set the linked list to a null state so we can work with it brand new

/* This is a function that reads the accounts infos from the account.txt file
 and parses it as a bankaccount struct then add it to the end of the list,
 all of this while keeping the number of accounts added as an int
 'num_accounts_read'
 */
void read_accounts_from_file(BankAccount **head, const char *filename) {
  FILE *file = fopen(
      filename,
      "r"); // Opening the file in the read mode and returning file as a pointer
  // if this pointer is null that means that there's no such file named
  // 'file_name' or there happens to be a memory leak
  if (file == NULL) {
    printf("Failed to open %s for reading.\n", filename);
    return;
  }

  struct BankAccount new_account; // creating a new BankAccount
  int num_accounts_read =
      0; // This is going to be used just as clarified earlier
  while (fscanf(file, "%d %d %99s %lf", &new_account.account_number,
                &new_account.code, new_account.customer,
                &new_account.balance) ==
         4) { //  ❗❗ This is a very important condition
    // In order to read all the data in the file (with the right types) we used
    // the function fscanf from <stdio.h> so we can read and parse the data from
    // the .txt file to the created account and return in integer that
    // indictates that we read all the data types correctly ❗❗
    add_account(head, new_account); // Adding the created account to our list
    num_accounts_read++; // incrementing the number of accounts read from the
                         // file
  }

  fclose(file); // Closing the file (freeing the file pointer)
}

/* this function does just the same as the previous one you can refer to it */
void read_transactions_from_file(BankAccount **head, const char *filename) {
  FILE *file =
      fopen(filename, "r"); // refer to 'read_accounts_from_file' function
  if (file == NULL) {
    printf("Failed to open file.\n");
    return;
  }

  int account_number; // This is the account number that is going to be used as
                      // our reference to the transaction list
  char operation_code[20];
  char date[20];
  double amount;

  while (fscanf(file, "%d %s %s %lf", &account_number, operation_code, date,
                &amount) == 4) { // refer to 'read_accounts_from_file' function
    BankAccount *current = *head;
    while (current != NULL) {
      if (current->account_number == account_number) {
        Transaction *new_transaction; // creating a new transaction (a temp one)
        createNewTransaction(
            &new_transaction); // allocating memory for the transaction

        if (new_transaction == NULL) {
          printf("Memory allocation failed.\n");
          fclose(file);
          return;
        }

        strcpy(new_transaction->operation_code, operation_code);
        // the function strcpy from the lib <string.h> 'strcpy(char *a, char
        // *b)' copies content of the array b to
        strcpy(new_transaction->date, date); // assigning data
        new_transaction->amount = amount;    // assigning data

        assignTransactionToAccount(
            current,
            new_transaction); // assigning the transacition to its account
        // refer to 'AbstractMachine.c'
        new_transaction->next = NULL; // we set the tail of the list
        break;
      }
      current =
          nextBank(current); // we pass to the next bank account in the list
    }
  }

  fclose(file); // again we free the created file pointer
}

void add_account(BankAccount **head, struct BankAccount new_account) {
  // Usual stuff
  BankAccount *new_node;
  createAccount(&new_node);
  if (new_node == NULL) {
    printf("Memory allocation failed.\n"); // print this if the malloc failed
    return;
  }
  // We assign data to the newly created account
  new_node->account_number = new_account.account_number;
  new_node->code = new_account.code;
  strcpy(new_node->customer, new_account.customer);
  new_node->balance = new_account.balance;
  new_node->transactions = NULL; // when we add this new account the list of
                                 // transaction should be void
  new_node->next = *head;
  *head = new_node;
}

void delete_account(BankAccount **head, int account_number) {
  BankAccount *temp = *head, *prev = NULL;
  if (temp != NULL && temp->account_number == account_number) {
    *head = nextBank(temp);
    free(temp); // we free the allocated memory for this account
    return;
  }

  while (temp != NULL && temp->account_number != account_number) {
    prev = temp;
    temp = nextBank(temp);
  }

  if (temp == NULL)
    return;

  prev->next = nextBank(temp);
  free(temp);                              // deletion
  printf("Account Deleted Successfully."); // we print out this message in case
                                           // of a successfull deletion
}

// A function that reads from the standard input the account number and asks the
// 'Admin' for the data that he wants to change
void edit_account(BankAccount **head, int account_number) {
  BankAccount *current = *head;
  // We iterate through the list of bank accounts
  while (current != NULL) {
    // Until we find the right account with the right number
    if (current->account_number == account_number) {
      // Asking the 'Admin' for the data that is going to be edited
      printf("Enter new account number: ");
      scanf("%d", &current->account_number);
      printf("Enter new code: ");
      scanf("%d", &current->code);
      printf("Enter new customer name: ");
      scanf("%s", current->customer);
      printf("Enter new balance: ");
      scanf("%lf", &current->balance);
      printf("Account Edited Successfully");
      return;
    }
    current = nextBank(current); // we go through the next bank account in case
                                 // the numbers don't match
  }
  printf("Account not found.\n"); // we print this message in case of a failed
                                  // search
}

void display_accounts(BankAccount *head) { // A function that goes through the
                                           // list of bankaccounts and prints
  // each and every bank account using a temporairly created bankaccount
  // 'current'
  if (head == NULL) {
    printf("No accounts found.\n");
    return; // we get return nothing in the case of an empty list
  }

  printf("Accounts:\n");
  printf("Account Number\tCode\tCustomer\tBalance\n");
  BankAccount *current = head;
  while (current != NULL) {
    printf("%d\t\t%d\t%s\t%.2f\n", current->account_number, current->code,
           current->customer,
           current->balance);    // Printing the account's details
    current = nextBank(current); // Iterating to the next account
  }
}

/* A function that transfers an amount between to numbers
with two given Numbers 'BankAccounts' */
void transfer_amount(BankAccount **head, int sender_account,
                     int receiver_account, double amount) {
  BankAccount *sender = *head;   // a temp account
  BankAccount *receiver = *head; // a temp account
  // First we try to find the BankAccounts based on their numbers
  // So we go to each element of the created linked list and compare the numbers
  // of each account
  while (sender != NULL && sender->account_number != sender_account)
    sender = nextBank(sender);
  while (receiver != NULL && receiver->account_number != receiver_account)
    receiver = nextBank(receiver);

  if (sender == NULL || receiver == NULL) {
    printf("Invalid account numbers.\n");
    return; // We print out this message in the case that we didn't find any
            // account
  }
  if (sender->balance < amount) {
    printf("Insufficient balance in the sender's account.\n");
    return;
  }
  sender->balance -=
      amount; // subtracting the transfered amount from the sender's balance
  receiver->balance +=
      amount; // adding the transfered amount to the receiver's balance.
  Transaction *p;
  createNewTransaction(&p);
  // Creating a new transaction and allocating memory for it using malloc
  // 'AbstractMachine.c' And assigning data to it
  strcpy(p->operation_code, "transfer");
  printf("Enter the date in the form YYYY-MM-DD: "); // this format should be
                                                     // respected
  scanf("%s", p->date);
  p->amount = -amount;
  p->next = sender->transactions;
  assignTransactionToAccount(
      sender, p); // we assign the transaction to the respective bankaccount
  Transaction *q;
  createNewTransaction(&q); // Same goes here
  strcpy(q->date, p->date);
  strcpy(q->operation_code, transfer);
  q->amount = amount;
  q->next = receiver->transactions;
  assignTransactionToAccount(receiver, q);      // same thing
  printf("Amount transferred successfully.\n"); // We printout this message in
}

// A function used to deposit an amount (balance) to a given account
void deposit_fund(BankAccount **head, int account_number, double amount) {
  // First we search for the account with a given BankAccount Number by
  // itterating through the list of bankaccounts
  BankAccount *current = *head;
  while (current != NULL && current->account_number != account_number) {
    current = nextBank(current);
  }
  if (current == NULL) {
    printf("Account not found.\n");
    return;
  }
  current->balance += amount;
  // allocating mem for the newly created transaction
  Transaction *q;
  createNewTransaction(&q);
  printf("Enter the date in the form YYYY-MM-DD: "); // Same procedure as the
                                                     // last function
  scanf("%s", q->date);
  strcpy(q->operation_code, deposit);
  q->amount = amount;
  q->next = current->transactions;
  assignTransactionToAccount(current, q);
  printf("Amount deposited successfully.\n");
}

// A function that substracts the Balance from the given bank account by
// searching for this BankAccount using his respective number
void withdraw_fund(BankAccount **head, int account_number, double amount) {
  BankAccount *current =
      *head; // Initializing a temp BankAccount to itterate through the list to
             // the head of the linked list
  while (current != NULL && current->account_number != account_number) {
    current = nextBank(current);
  }
  if (current == NULL) {
    printf(
        "Account not found.\n"); // we print this in case of an unfound account
    return;
  }
  if (current->balance < amount) {
    printf("Insufficient balance.\n");
    return;
  }
  current->balance -=
      amount; // we subract the given amount from the bankaccount's number
  Transaction *q;
  createNewTransaction(&q);
  printf("Enter the date in the form YYYY-MM-DD: "); // The date should be in
                                                     // this formatt or
                                                     // otherwise the date
                                                     // cannot be read properly
  scanf("%s", q->date);
  strcpy(q->operation_code, withdrawal);
  q->amount = -amount;
  q->next = current->transactions;
  assignTransactionToAccount(current, q);
  printf("Amount withdrawn successfully.\n"); // Printing this message in case
                                              // of a successfull withdrawal
}
/* // A function used to authenticate the User (Admin or not)
int authenticate_admin() {
  int code;
  printf("Enter admin code: ");
  scanf("%d", &code);
  if (code == ADMIN_CODE)
    return 1;
  else
    return 0;
}

*/
/* This is a pretty important function it's used to write the made changes from
 * the standard output to the .txt file in this case accounts.txt
 * It reads the data from the linked list of BankAccount and then parses it to
 * the .txt file
 */
void save_accounts_to_file(BankAccount *head, const char *filename) {
  FILE *file = fopen(filename, "w"); // we set a FILE pointer 'file' with the
                                     // function fopen with the mode 'write'
  if (file == NULL) {                // Same as shown before
    printf("Failed to open file.\n");
    return;
  }

  BankAccount *current = head; // We create a BankAccount initialized to the
                               // head of the linked list
  while (current != NULL) {
    fprintf(file, "%d %d %s %.2f\n", current->account_number, current->code,
            current->customer, current->balance);
    current = nextBank(current);
  }

  fclose(file); // We free the FILE pointer
}

/* Same As before 'save_accounts_to_file' */
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

/* A function to check the total Balance of a given bank account*/
void check_total_amount(BankAccount *head, int account_number) {
  // same principle as before we create a 'local' bankaccount that we use to
  // itterate through the list of bankaccounts from their head
  // We look for a match between the numbers
  BankAccount *current = head;
  while (current != NULL) {
    if (current->account_number == account_number) {
      printf("Total amount in account %d: %.2f\n", account_number,
             current->balance);
      return;
    }
    current = nextBank(current);
  }
  printf("Account not found.\n"); // We print this in case of an unfound account
}

/* A function that prints out the transaction history of a bank account in a
 * given date*/
void view_transaction_history(BankAccount *head, int account_number) {
  BankAccount *current = head;
  // Same principle
  while (current != NULL) {
    if (current->account_number == account_number) {
      printf("Transaction history for account %d:\n", account_number);
      Transaction *t =
          current->transactions; // we initialize a 'local' pointer to the head
                                 // of the bankaccount's transaction list
      // then we use it to see the transactions in the given date
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
// A Function to display admin menu
// We print the menu using Simple colors (defined ontop)
void admin_menu(struct BankAccount **head) {
  int code; // local code used to compare the input code and the admin code

  printf(GREEN "+---------------------------------------------+\n" RESET);
  printf(GREEN "|          Welcome to ESIBANK Admin Menu           |\n" RESET);
  printf(GREEN "+---------------------------------------------+\n" RESET);
  printf(GREEN "|                                             |\n" RESET);
  printf(GREEN "|  Please enter your code to proceed:        |\n" RESET);
  printf(GREEN "|                                             |\n" RESET);
  printf(GREEN "+---------------------------------------------+\n" RESET);
  printf("|  Enter Code: ");
  scanf("%d", &code);
  // Authentication
  if (code == ADMIN_CODE) {
    // Authentication successful
    int choice;
    do {
      clear_screen();
      printf(RED "+---------------------------------------------+\n" RESET);
      printf(RED "|                  " GREEN "Admin Menu" RED
                 "                    |\n" RESET);
      printf(RED "+---------------------------------------------+\n" RESET);
      printf(RED "|                                             |\n" RESET);
      printf(RED "|  " WHITE "Please select an option:" RED
                 "                  |\n" RESET);
      printf(RED "|                                             |\n" RESET);
      printf(RED "|  " YELLOW "1. " WHITE "Add Account" RED
                 "                             |\n" RESET);
      printf(RED "|  " YELLOW "2. " WHITE "Delete Account" RED
                 "                          |\n" RESET);
      printf(RED "|  " YELLOW "3. " WHITE "Edit Account" RED
                 "                            |\n" RESET);
      printf(RED "|  " YELLOW "4. " WHITE "Display Accounts" RED
                 "                        |\n" RESET);
      printf(RED "|  " YELLOW "6. " WHITE "Back to Main Menu" RED
                 "                        |\n" RESET);
      printf(RED "|                                             |\n" RESET);
      printf(RED "+---------------------------------------------+\n" RESET);
      printf(RED "Enter your choice: " RESET);
      scanf("%d", &choice);

      switch (choice) {
      case 1:
        // Add Account functionality
        {
          struct BankAccount new_account;
          printf("Enter Account Number: ");
          scanf("%d", &new_account.account_number);
          printf("Enter Code: ");
          scanf("%d", &new_account.code);
          printf("Enter Customer Name: ");
          scanf("%s", new_account.customer);
          printf("Enter Initial Balance: ");
          scanf("%lf", &new_account.balance);
          add_account(head, new_account);
          printf("Account added successfully!\n");
        }
        break;
      case 2:
        // Delete Account
        {
          int account_number;
          printf("Enter Account Number to delete: ");
          scanf("%d", &account_number);
          delete_account(head, account_number);
        }
        break;
      case 3:
        // Edit Account
        {
          int account_number;
          printf("Enter Account Number to edit: ");
          scanf("%d", &account_number);
          edit_account(head, account_number);
        }
        break;
      case 4:
        // Display Accounts
        display_accounts(*head);
        break;
      case 5:
        printf("Exiting admin menu...\n");
        break;
      default:
        printf("Invalid choice! Please try again.\n");
      }
      printf("Press Enter to continue...");
      getchar(); // Used to clear the input and output buffer please refer to
                 // the following documentation >>
                 // https://www.geeksforgeeks.org/getchar-function-in-c/
      getchar();
    } while (choice != 5);
  } else {
    printf("Authentication failed! Please check your code.\n");
  }
}
// Function to display customer menu
void customer_menu(BankAccount **head) {
  int account_number;
  int code; // AccNum And code used to authenticate

  printf(YELLOW "+---------------------------------------------+\n" RESET);
  printf(YELLOW
         "|          Welcome to ESIBANK Customer Menu         |\n" RESET);
  printf("+---------------------------------------------+\n" RESET);
  printf(YELLOW "|                                             |\n" RESET);
  printf(YELLOW
         "|  Please enter your account number and code to proceed: |\n" RESET);
  printf("|                                             |\n");
  printf(YELLOW "+---------------------------------------------+\n" RESET);
  printf("|  Enter Account Number: ");
  scanf("%d", &account_number);
  printf("|  Enter Code: ");
  scanf("%d", &code);

  BankAccount *current = *head;
  while (current != NULL) {
    if (current->account_number == account_number && current->code == code) {
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
          // Check Amount
          check_total_amount(*head, account_number);
          break;
        case 2: {
          // Deposit
          double amount;
          printf("Enter amount to deposit: ");
          scanf("%lf", &amount);
          deposit_fund(head, account_number, amount);
          break;
        }
        case 3: {
          // Withdraw
          double amount;
          printf("Enter amount to withdraw: ");
          scanf("%lf", &amount);
          withdraw_fund(head, account_number, amount);
          break;
        }
        case 4: {
          // Transfer
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
          // Check Transfer history
          char start_date[20], end_date[20];
          printf("Enter the date (YYYY-MM-DD): ");
          scanf("%s", start_date);
          check_transaction_history_by_date(*head, account_number, start_date);
          break;
        }
        case 6:
          // Exit
          printf("Exiting customer menu.\n");
          break;
        default:
          printf("Invalid choice. Please try again.\n");
        }
        // refer to main.c
        getchar();
      } while (choice != 6);
      return;
    }
    current = nextBank(current);
  }
  printf("Account not found.\n");
}

// A function used to get the data from the 'admin' to add new bankacc
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

// Function to get numerical representation of date (YYYYMMDD) // used in the
// next function
int get_numeric_date(const char *date) {
  int year, month, day;
  sscanf(date, "%d-%d-%d", &year, &month, &day);
  return year * 10000 + month * 100 + day;
}

// Function to check transaction history within a given date
void check_transaction_history_by_date(BankAccount *head, int account_number,
                                       const char *date) {
  BankAccount *current = head;
  while (current != NULL) {
    if (current->account_number == account_number &&
        current->transactions != NULL) {
      printf("Transaction history for account %d on %s:\n", account_number,
             date);
      Transaction *t = current->transactions;
      int found = 0; // Flag to check if any transactions found for the date
      while (t != NULL) {
        if (strcmp(t->date, date) == 0) {
          printf("Operation: %20s, Amount: %.2f\n", t->operation_code,
                 t->amount);
        }
        t = nextTransaction(t);
      }
      return;
    }
    current = nextBank(current);
  }
  printf("Account not found.\n");
}
