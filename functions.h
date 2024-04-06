
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

