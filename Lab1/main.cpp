#include <iostream>
#include <vector>
#include <string>

class BankCard
{
private:
	std::string CardNumber_;
	std::string ExpireDate_;
	int CardBalance_;
public:
	BankCard(std::string CardNumber, std::string ExpireDate, int CardBalance = 0)
		: CardNumber_(CardNumber), ExpireDate_(ExpireDate), CardBalance_(CardBalance)
	{
	}

	void SetCardNumber(std::string CardNumber)
	{
		CardNumber_ = CardNumber;
	}

	void SetExpireDate(std::string ExpireDate)
	{
		ExpireDate_ = ExpireDate;
	}

	void SetCardBalance(int CardBalance)
	{
		CardBalance_ = CardBalance;
	}

	std::string GetCardNumber()
	{
		return CardNumber_;
	}

	std::string GetExpireDate()
	{
		return ExpireDate_;
	}

	int GetCardBalance()
	{
		return CardBalance_;
	}

	void DisplayCardInfo()
	{
		std::cout << "Card number:" << CardNumber_ << std::endl;
		std::cout << "Expire date:" << ExpireDate_ << std::endl;
		std::cout << "Card balance:" << CardBalance_ << std::endl;
	}
};


int main()
{
	
}