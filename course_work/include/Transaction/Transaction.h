// Transaction.h
#pragma once
#include "../Errors/CustomExceptions.h"
#include "../Card/Card.h"
#include "../Account/Account.h"

template <typename Source, typename Target>
class Transaction {
public:
    Transaction(Source& source, Target& target, int amount)
            : source_(source), target_(target), amount_(amount) {}

    void execute(){
        if (amount_ <= 0) {
            throw ValidationException("Transfer amount must be positive");
        }
        if (source_->get_balance() < amount_) {
            throw TransactionException("Insufficient funds in the source account or card");
        }

        source_->set_balance(source_->get_balance() - amount_);
        target_->set_balance(target_->get_balance() + amount_);
    }

private:
    Source& source_;
    Target& target_;
    int amount_;
};
