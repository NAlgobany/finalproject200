#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <map>
#include <cstdlib>

class Card {
public:
    enum Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK = 10, QUEEN = 10, KING = 10, ACE = 11 };
    enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };

    Card(Rank r, Suit s) : rank(r), suit(s) {}

    int getValue() const {
        return rank;
    }

private:
    Rank rank;
    Suit suit;
};

class Deck {
public:
    std::vector<Card> cards;

    Deck() {
        cards.reserve(52);
        for (int s = Card::HEARTS; s <= Card::SPADES; ++s) {
            for (int r = Card::TWO; r <= Card::ACE; ++r) {
                cards.emplace_back(static_cast<Card::Rank>(r), static_cast<Card::Suit>(s));
            }
        }
    }

    void shuffle() {
        static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
        std::shuffle(cards.begin(), cards.end(), rng);
    }
};

class Shoe {
public:
    std::vector<Card> cards;
    static std::mt19937 rng;

    Shoe(int numDecks = 6) {
        rng.seed(static_cast<unsigned int>(std::time(nullptr)));
        for (int i = 0; i < numDecks; ++i) {
            Deck deck;
            deck.shuffle();
            cards.insert(cards.end(), deck.cards.begin(), deck.cards.end());
        }
        shuffle();
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    Card dealCard() {
        if (cards.empty() || cards.size() < 100) {
            std::cout << "Reshuffling the shoe..." << std::endl;
            shuffle();
        }
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};

class Hand {
public:
    std::vector<Card> cards;

    void addCard(const Card& card) {
        cards.push_back(card);
    }

    int getValue() const {
        int totalValue = 0;
        int aceCount = 0;
        for (const Card& card : cards) {
            totalValue += card.getValue();
            if (card.getValue() == Card::ACE) {
                aceCount++;
            }
        }
        while (totalValue > 21 && aceCount > 0) {
            totalValue -= 10; // Convert Ace from 11 to 1
            aceCount--;
        }
        return totalValue;
    }
};

std::mt19937 Shoe::rng;

class BlackjackGame {
public:
    Shoe shoe;
    int wallet = 0;

    void playHand(bool playerHits) {
        Hand playerHand, dealerHand;
        playerHand.addCard(shoe.dealCard());
        playerHand.addCard(shoe.dealCard());
        dealerHand.addCard(shoe.dealCard());
        dealerHand.addCard(shoe.dealCard());

        if (playerHits) {
            playerHand.addCard(shoe.dealCard());
        }

        while (dealerHand.getValue() < 17) {
            dealerHand.addCard(shoe.dealCard());
        }

        int playerScore = playerHand.getValue();
        int dealerScore = dealerHand.getValue();

        if ((playerScore > 21) || (playerScore < dealerScore && dealerScore <= 21)) {
            wallet--;
        }
        else if (playerScore == dealerScore) {
            // draw, no change to wallet
        }
        else {
            wallet++;
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));  // Seed random number generator
    BlackjackGame game;
    std::map<std::pair<int, bool>, std::pair<int, int>> results;

    for (int i = 0; i < 100000; ++i) {
        bool hit = rand() % 2 == 0;
        int startingWallet = game.wallet;
        game.playHand(hit);
        int result = game.wallet - startingWallet;
        int playerStartingValue = 10; // Simplified assumption for simulation

        if (result > 0) {
            results[{playerStartingValue, hit}].first++;
        }
        else {
            results[{playerStartingValue, hit}].second++;
        }
    }

    // Output results
    for (auto& res : results) {
        std::cout << "Starting Value: " << res.first.first << ", Hit: " << res.first.second
            << " -> Wins: " << res.second.first << ", Losses: " << res.second.second << std::endl;
    }

    return 0;
}
