#pragma once
#include "../Errors/CustomExceptions.h"
#include "Transaction.h"
#include <memory>

class TransactionService {
public:
    template <typename Source, typename Target>
    void perform_transaction(Source& source, Target& target, int amount){
        Transaction<Source, Target> transaction(source, target, amount);
        transaction.execute();
    }
};