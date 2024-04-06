/*

Resources Used in this projects  👉  :
   🔺 Youtube Channels : GeeksForGeeks LowLevelLearning
   🔺 WebArticles + Documentation : https://www.geeksforgeeks.org/c/
https://gnu.org
   🔺 The code is more readable if you visit our github
https://github.com/n4s3/TP3.git
 */

#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

// STANDARD C COLORS
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
int main() {
  BankAccount *head; // Creating a local head pointer to BankAccount
  initialize_list(&head);
  read_accounts_from_file(
      &head, "accounts.txt"); // Using these two functions to read the data
  read_transactions_from_file(&head, "transactions.txt");
  int choice;
  do {
    clear_screen(); // we clear the screen for the next submenu
    printf(
        BLUE
        "███████ ███████ ██       ██████   █████  ███    ██ ██   ██\n" RESET);
    printf(BLUE
           "██      ██      ██       ██   ██ ██   ██ ████   ██ ██  ██\n" RESET);
    printf(BLUE
           "█████   ███████ ██ █████ ██████  ███████ ██ ██  ██ █████\n" RESET);
    printf(BLUE
           "██           ██ ██       ██   ██ ██   ██ ██  ██ ██ ██  ██\n" RESET);
    printf(
        BLUE
        "███████ ███████ ██       ██████  ██   ██ ██   ████ ██   ██\n" RESET);
    printf(MAGENTA "BY "
                   "BELARBI ANES ABDELMALEK " CYAN " & " RED
                   " NEKHOUL MOHAMMED "
                   "ASSIL\n" RESET);
    printf(GREEN "source code : https://github.com/n4s3/TP3.git" RESET);
    printf(RED "+---------------------------------------------+\n" RESET);
    printf(RED "|                  " GREEN "Main Menu" RED
               "                    |\n" RESET);
    printf(RED "+---------------------------------------------+\n" RESET);
    printf(RED "|                                             |\n" RESET);
    printf(RED "|  " WHITE "Please select an option:" RED
               "                  |\n" RESET);
    printf(RED "|                                             |\n" RESET);
    printf(RED "|  " YELLOW "1. " WHITE "Admin Menu" RED
               "                            |\n" RESET);
    printf(RED "|  " YELLOW "2. " WHITE "Customer Menu" RED
               "                         |\n" RESET);
    printf(RED "|  " YELLOW "3. " WHITE "Exit" RED
               "                                   |\n" RESET);
    printf(RED "|                                             |\n" RESET);
    printf(RED "+---------------------------------------------+\n" RESET);
    printf(RED "Enter your choice: " RESET);
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      admin_menu(&head);
      break;
    case 2:
      customer_menu(&head);
      break;
    case 3:
      save_accounts_to_file(head, "accounts.txt");
      save_transactions_to_file(head, "transactions.txt");
      printf("Exiting program. Goodbye!\n");
      exit(0);
      break;
    default:
      printf("Invalid choice! Please try again.\n");
    }
    printf("Press Enter to continue...");
    getchar(); // Wait for Enter key
    getchar();
  } while (choice != 3); // This ensures that we enter the right set of choices

  return 0;
}
