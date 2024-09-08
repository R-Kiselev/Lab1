void handle_add_card(std::unique_ptr<Account, std::default_delete<Account>>& account);

void handle_delete_card(std::unique_ptr<Account, std::default_delete<Account>>& account);

void handle_set_name(std::unique_ptr<Account, std::default_delete<Account>>& account);

void handle_set_balance(std::unique_ptr<Account, std::default_delete<Account>>& account);
