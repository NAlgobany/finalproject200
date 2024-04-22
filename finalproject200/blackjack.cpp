#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <map>

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
public
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

    Shoe(int numDecks = 6) { 
        for (int i = 0; i < numDecks; ++i) {
            Deck deck;
            deck.shuffle();
            cards.insert(cards.end(), deck.cards.begin(), deck.cards.end());
        }
        shuffle();
    }

    void shuffle() {
        static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    Card dealCard() {
        if (cards.size() < 100) {
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

