#include "AbstractMachine.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  BankAccount *head;
  initialize_list(&head);
  read_accounts_from_file(&head, "accounts.txt");
  read_transactions_from_file(&head, "transactions.txt");

  int choice;
  while (1) {
    clear_screen();
    printf("Welcome to the Bank Management System\n");
    printf("=====================================\n");
    printf("1. Admin\n");
    printf("2. Customer\n");
    printf("3. Exit\n");
    printf("Choose an option: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      if (authenticate_admin()) {
        admin_menu(&head);
      } else {
        printf("Authentication failed. Access denied.\n");
      }
      break;
    case 2:
      customer_menu(&head);
      break;
    case 3:
      save_accounts_to_file(head, "accounts.txt");
      save_transactions_to_file(head, "transactions.txt");
      printf("Exiting program. Goodbye!\n");
      exit(0);
    default:
      printf("Invalid choice. Please try again.\n");
    }
  }

  return 0;
}
