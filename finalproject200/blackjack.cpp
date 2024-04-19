#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <map
class Card {
public:
    enum Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK = 10, QUEEN = 10, KING = 10, ACE = 11 };
    enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };

    Card(Rank r, Suit s) : rank(r), suit(s) {}

    int getValue() const  {
        return rank;
    } 