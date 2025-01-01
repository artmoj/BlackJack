#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

// Значения карт
enum Cards
{
	TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
	JACK = 10, QUEEN = 10, KING = 10, ACE = 11
};

// Заполнение колоды
void fillDeck(int deck[], int size)
{
	int index = 0;
	for (int value = TWO; value <= TEN; value++)
	{ // Добавляем карты с 2 до 10
		for (int suit = 0; suit < 4; suit++)
		{
			deck[index++] = value;
		}
	}
	// Добавляем картинки
	for (int suit = 0; suit < 4; suit++)
	{
		deck[index++] = JACK;
		deck[index++] = QUEEN;
		deck[index++] = KING;
	}
	// Добавляем тузы
	for (int suit = 0; suit < 4; suit++)
	{
		deck[index++] = ACE;
	}
}


// Перемешивание колоды
void shuffleDeck(int deck[], int size)
{
	srand(static_cast<unsigned int>(time(0)));
	for (int i = size - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		swap(deck[i], deck[j]);
	}
}

// Вывод колоды (для отладки)
void outputDeck(const int deck[], int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << deck[i] << " ";
	}
	cout << endl;
}

// Подсчет суммы карт с учетом тузов (1 или 11)
int calculateSum(const int cards[], int count)
{
	int sum = 0;
	int aceCount = 0;

	for (int i = 0; i < count; i++)
	{
		sum += cards[i];
		if (cards[i] == ACE)
		{
			aceCount++;
		}
	}

	// Если сумма больше 21, уменьшаем значение тузов
	while (sum > 21 && aceCount > 0)
	{
		sum -= 10;
		aceCount--;
	}

	return sum;
}

// Начальные карты игрока или дилера
void dealStartingCards(const int deck[], int& deckIndex, int hand[], int& handCount)
{
	for (int i = 0; i < 2; i++)
	{
		hand[handCount++] = deck[deckIndex++];
	}
}

// Добавление карты в руку
void drawCard(const int deck[], int& deckIndex, int hand[], int& handCount)
{
	if (deckIndex < 52)
	{
		hand[handCount++] = deck[deckIndex++];
	}
	else
	{
		cout << "Колода закончилась!" << endl;
	}
}

// Печать руки
void printHand(const char* name, const int hand[], int count)
{
	cout << name << ": ";
	for (int i = 0; i < count; i++)
	{
		cout << hand[i] << " ";
	}
	cout << "(сумма: " << calculateSum(hand, count) << ")\n";
}

int main()
{
	setlocale(LC_CTYPE, "ru");
	const int DECK_SIZE = 52;
	int deck[DECK_SIZE];

	int points = 1000;

	while (points > 0)
	{
		fillDeck(deck, DECK_SIZE);
		shuffleDeck(deck, DECK_SIZE);

		int deckIndex = 0;
		int playerHand[20] = {};
		int dealerHand[20] = {};
		int playerCount = 0, dealerCount = 0;

		// Раздача начальных карт
		dealStartingCards(deck, deckIndex, playerHand, playerCount);
		dealStartingCards(deck, deckIndex, dealerHand, dealerCount);

		int bet = 0;

		// Игрок делает ставку
		cout << "Ваш баланс: " << points << "\nВведите ставку: ";
		cin >> bet;
		if (bet > points || bet <= 0)
		{
			cout << "Неверная ставка. Игра окончена." << endl;
			break;
		}
		points -= bet;

		// Игровой цикл
		bool playerTurn = true;
		while (playerTurn)
		{
			printHand("Ваши карты", playerHand, playerCount);
			printHand("Карты дилера (скрыто одна карта)", dealerHand, 1);

			cout << "1: Взять карту\n2: Хватит\nВыбор: ";
			int choice;
			cin >> choice;

			if (choice == 1)
			{
				drawCard(deck, deckIndex, playerHand, playerCount);
				if (calculateSum(playerHand, playerCount) > 21)
				{
					cout << "Перебор! Вы проиграли.\n";
					break;
				}
			}
			else if (choice == 2)
			{
				playerTurn = false;
			}
			else
			{
				cout << "Неверный выбор.\n";
			}
		}

		if (calculateSum(playerHand, playerCount) <= 21)
		{
			// Ход дилера
			while (calculateSum(dealerHand, dealerCount) < 17)
			{
				drawCard(deck, deckIndex, dealerHand, dealerCount);
			}

			// Вывод итогов
			printHand("Ваши карты", playerHand, playerCount);
			printHand("Карты дилера", dealerHand, dealerCount);

			int playerSum = calculateSum(playerHand, playerCount);
			int dealerSum = calculateSum(dealerHand, dealerCount);

			if (dealerSum > 21 || playerSum > dealerSum)
			{
				cout << "Вы выиграли! Ваш выигрыш: " << bet * 2 << "\n";
				points += bet * 2;
			}
			else if (playerSum == dealerSum)
			{
				cout << "Ничья. Ваша ставка возвращена.\n";
				points += bet;
			}
			else
			{
				cout << "Вы проиграли.\n";
			}
		}

		cout << "Ваш баланс: " << points << "\n";

		if (points > 0)
		{
			cout << "Хотите сыграть еще раз? (1 - Да, 0 - Нет): ";
			int playAgain;
			cin >> playAgain;
			if (playAgain == 0)
			{
				break;
			}
		}
		else
		{
			cout << "У вас закончились деньги. Игра окончена.\n";
		}
	}
}