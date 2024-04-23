#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>

class Card {
public:
    char rank;
    char suit;

    Card(char r, char s) : rank(r), suit(s) {}

    int value() const {
        if (rank >= '2' && rank <= '9') return rank - '0';
        if (rank == 'T' || rank == 'J' || rank == 'Q' || rank == 'K') return 10;
        if (rank == 'A') return 11;
        return 0;
    }

    void display() const {
        std::cout << rank << suit << " ";
    }
};

class Deck {
private:
    std::vector<Card> cards;
    std::mt19937 rng;

public:
    Deck() : rng(std::random_device()()) {
        const std::string ranks = "23456789TJQKA";
        const std::string suits = "SDCH";  // Spades, Diamonds, Clubs, Hearts
        for (char suit : suits) {
            for (char rank : ranks) {
                cards.emplace_back(rank, suit);
            }
        }
        shuffle();
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    Card deal() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    bool empty() const {
        return cards.empty();
    }
};

class Shoe {
private:
    std::vector<Card> cards;
    std::mt19937 rng;
    int num_decks;

    void refill() {
        std::cout << "Refilling and shuffling shoe...\n";
        cards.clear();
        for (int i = 0; i < num_decks; ++i) {
            Deck deck;
            while (!deck.empty()) {
                cards.push_back(deck.deal());
            }
        }
        shuffle();
    }

public:
    explicit Shoe(int num_decks = 6) : num_decks(num_decks), rng(std::random_device()()) {
        refill();
    }

    void shuffle() {
        if (cards.size() < 100) {
            refill();
        }
        else {
            std::shuffle(cards.begin(), cards.end(), rng);
        }
    }

    Card deal() {
        if (cards.size() < 100) {
            shuffle();
        }
        Card card = cards.back();
        cards.pop_back();
        return card;
    }

    size_t size() const {
        return cards.size();
    }
};

class Hand {
private:
    std::vector<Card> cards;

public:
    void add_card(Card card) {
        cards.push_back(card);
    }

    int compute_value() const {
        int value = 0;
        int aces = 0;
        for (const Card& card : cards) {
            int card_value = card.value();
            value += card_value;
            if (card.rank == 'A') aces++;
        }
        while (value > 21 && aces > 0) {
            value -= 10;
            aces--;
        }
        return value;
    }

    void display() const {
        for (const Card& card : cards) {
            card.display();
        }
        std::cout << " (" << compute_value() << ")" << std::endl;
    }

    bool is_bust() const {
        return compute_value() > 21;
    }

    void clear() {
        cards.clear();
    }
};

class BlackjackGame {
private:
    Shoe shoe;

public:
    BlackjackGame() : shoe(6) {}

    void play() {
        Hand player_hand, dealer_hand;
        player_hand.add_card(shoe.deal());
        dealer_hand.add_card(shoe.deal());
        player_hand.add_card(shoe.deal());
        dealer_hand.add_card(shoe.deal());

        std::cout << "Dealer's first card: ";
        dealer_hand.cards[0].display();
        std::cout << std::endl;

        bool player_done = false;
        while (!player_done) {
            std::cout << "Your hand: ";
            player_hand.display();
            if (player_hand.is_bust()) {
                std::cout << "Bust! You lose." << std::endl;
                return;
            }

            std::cout << "Hit (h) or Stand (s)? ";
            char choice;
            std::cin >> choice;
            if (choice == 'h') {
                player_hand.add_card(shoe.deal());
            }
            else {
                player_done = true;
            }
        }

        while (dealer_hand.compute_value() < 17) {
            dealer_hand.add_card(shoe.deal());
        }

        std::cout << "Dealer's hand: ";
        dealer_hand.display();

        if (dealer_hand.is_bust() or player_hand.compute_value() > dealer_hand.compute_value()) {
            std::cout << "You win!" << std::endl;
        }
        else if (player_hand.compute_value() < dealer_hand.compute_value()) {
            std::cout << "You lose." << std::endl;
        }
        else {
            std::cout << "Draw." << std::endl;
        }
    }
};

int main() {
    srand(time(NULL));
    BlackjackGame game;
    char playAgain = 'y';
    while (playAgain == 'y') {
        game.play();
        std::cout << "Play another hand? (y/n): ";
        std::cin >> playAgain;
    }

    return 0;
}
