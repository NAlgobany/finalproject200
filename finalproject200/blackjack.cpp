#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <map>
#include <cctype>

class Card {
public:
    int value;
    char suit;
    Card(char s, int v) : suit(s), value(v) {}
    int getValue() const {
        if (value == 1) return 11;
        if (value >= 10) return 10;
        return value;
    }
};

class Deck {
public:
    std::vector<Card> cards;
    Deck() {
        const char suits[] = { 'H', 'D', 'C', 'S' };
        for (char s : suits) {
            for (int v = 1; v <= 13; v++) {
                cards.emplace_back(s, v);
            }
        }
    }
};

class Shoe {
private:
    std::vector<Card> cards;
    int deckCount;

public:
    explicit Shoe(int count) : deckCount(count) {
        for (int i = 0; i < deckCount; ++i) {
            Deck deck;
            cards.insert(cards.end(), deck.cards.begin(), deck.cards.end());
        }
        shuffle();
    }

    void shuffle() {
        cards.clear();

        for (int i = 0; i < deckCount; ++i) {
            Deck deck;
            cards.insert(cards.end(), deck.cards.begin(), deck.cards.end());
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cards.begin(), cards.end(), g);

        std::cout << "Shoe reshuffled with new cards. Total cards: " << cards.size() << std::endl;
    }

    Card draw() {
        if (cards.empty()) {
            std::cerr << "Cannot draw from an empty shoe." << std::endl;

            return Card('H', 1);
        }

        Card card = cards.back();
        cards.pop_back();

        if (cards.size() <= 100) {
            std::cout << "Card count after draw is " << cards.size() << ". Reshuffling..." << std::endl;
            shuffle();
        }

        return card;
    }



    ~Shoe() {
        std::cout << "Shoe destroyed. Remaining cards: " << cards.size() << std::endl;
    }
};

int calculateTotal(const std::vector<Card>& cards) {
    int total = 0;
    int aceCount = 0;
    for (const Card& card : cards) {
        int cardValue = card.getValue();
        total += cardValue;
        if (cardValue == 11) aceCount++;
    }
    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }
    return total;
}

class BlackjackGame {
private:
    Shoe shoe;
    std::map<std::pair<int, int>, std::map<char, std::vector<int>>> results;

public:
    explicit BlackjackGame() : shoe(6) {}

    void playHand() {
        std::vector<Card> playerCards = { shoe.draw(), shoe.draw() };
        std::vector<Card> dealerCards = { shoe.draw(), shoe.draw() };

        std::cout << "Your cards: " << playerCards[0].getValue() << " and " << playerCards[1].getValue() << std::endl;
        std::cout << "Dealer's face-up card: " << dealerCards[0].getValue() << std::endl;

        while (true) {
            std::cout << "Do you want to hit or stand? (h/s): ";
            char decision;
            std::cin >> decision;
            decision = std::tolower(decision);
            if (decision == 'h') {
                playerCards.push_back(shoe.draw());
                std::cout << "You drew a " << playerCards.back().getValue() << std::endl;
                if (calculateTotal(playerCards) > 21) {
                    std::cout << "Bust! Your total is over 21.\n";
                    return;
                }
            }
            else if (decision == 's') {
                break;
            }
            else {
                std::cout << "Invalid input, please enter 'h' or 's'.\n";
                continue;
            }
        }

        int playerTotal = calculateTotal(playerCards);
        int dealerTotal = calculateTotal(dealerCards);

        while (dealerTotal < 17) {
            dealerCards.push_back(shoe.draw());
            dealerTotal = calculateTotal(dealerCards);
        }

        std::cout << "Dealer's total is " << dealerTotal << std::endl;

        if (playerTotal > 21 || (dealerTotal <= 21 && dealerTotal > playerTotal)) {
            std::cout << "You lose.\n";
        }
        else if (playerTotal == dealerTotal) {
            std::cout << "It's a draw.\n";
        }
        else {
            std::cout << "You win!\n";
        }
    }

    void simulateStrategy() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        for (int i = 0; i < 100000; i++) {
            std::vector<Card> playerCards = { shoe.draw(), shoe.draw() };
            std::vector<Card> dealerCards = { shoe.draw() };

            int initialPlayerTotal = calculateTotal(playerCards);
            int initialDealerValue = dealerCards.front().getValue();

            char decision = dis(gen) ? 'h' : 's';
            if (decision == 'h') {
                playerCards.push_back(shoe.draw());
            }
            int finalPlayerTotal = calculateTotal(playerCards);
            dealerCards.push_back(shoe.draw());
            while (calculateTotal(dealerCards) < 17) {
                dealerCards.push_back(shoe.draw());
            }
            int finalDealerTotal = calculateTotal(dealerCards);

            int outcome = 0;
            if ((finalPlayerTotal > 21) || (finalPlayerTotal < finalDealerTotal && finalDealerTotal <= 21)) {
                outcome = -1;
            }
            else if (finalPlayerTotal == finalDealerTotal) {
                outcome = 0;
            }
            else if (finalPlayerTotal <= 21 && (finalPlayerTotal > finalDealerTotal || finalDealerTotal > 21)) {
                outcome = 1;
            }

            results[{initialPlayerTotal, initialDealerValue}][decision].push_back(outcome);
        }
    }

    void printResults() {
        for (auto& kv : results) {
            for (auto& inner_kv : kv.second) {
                int wins = std::count(inner_kv.second.begin(), inner_kv.second.end(), 1);
                int losses = std::count(inner_kv.second.begin(), inner_kv.second.end(), -1);
                int draws = std::count(inner_kv.second.begin(), inner_kv.second.end(), 0);
                std::cout << "Player: " << kv.first.first << ", Dealer: " << kv.first.second
                    << ", Decision: " << inner_kv.first << " -> Wins: " << wins
                    << ", Losses: " << losses << ", Draws: " << draws << std::endl;
            }
        }
    }

    void menu() {
        while (true) {
            std::cout << "Do you want to (1) play a hand, (2) simulate strategy, or (3) exit? ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                playHand();
            }
            else if (choice == 2) {
                simulateStrategy();
                printResults();
                break;
            }
            else if (choice == 3) {
                break;
            }
            else {
                std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
            }
        }
    }
};

int main() {
    BlackjackGame game;
    game.menu();

    return 0;
}
