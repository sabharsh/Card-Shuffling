#include <stdlib.h>
#include "shuffle.h"

// You can add more functions to this file.

void _shuffle(CardDeck orig_deck, int k);
void _interleave(CardDeck upper_deck, CardDeck lower_deck, int k);
void _interleaveRecursive(CardDeck upper, CardDeck lower, CardDeck shuffle, int k);
void _removeCard(char * deck);
void _addCard(char * deck, char card);

void repeat_shuffle(CardDeck orig_deck, int k) 
{
	if(k <= 0)
	{
		printDeck(orig_deck);
	}
	else
	{
		_shuffle(orig_deck, k);
	}
}

void divide(CardDeck orig_deck, CardDeck * upper_deck, CardDeck * lower_deck) 
{
	for(int i = 0; i < orig_deck.size-1; i++)//for traversing through orig_deck
	{
		int counter_orig_deck = 0;
		int counter_deck_part = 0;
		upper_deck[i].size    = 0;
		while(counter_orig_deck<=i)//extracting upper deck
		{
			upper_deck[i].cards[counter_deck_part] = orig_deck.cards[counter_orig_deck];
			upper_deck[i].size++;
			counter_deck_part++;
			counter_orig_deck++;
		}
		upper_deck[i].cards[counter_deck_part] = 0; //adding delimiter
		
		counter_deck_part     = 0;
		lower_deck[i].size    = 0;
		while(counter_orig_deck<orig_deck.size) //extracting lower deck
		{
			lower_deck[i].cards[counter_deck_part] = orig_deck.cards[counter_orig_deck];
			lower_deck[i].size++;
			counter_deck_part++;
			counter_orig_deck++;
		}
		lower_deck[i].cards[counter_deck_part] = 0; //adding delimiter
	}
}


void interleave(CardDeck upper_deck, CardDeck lower_deck)
{
	CardDeck shuffled_deck={
		.size = 0,
		.cards = {0}
	};

	_interleaveRecursive(upper_deck, lower_deck, shuffled_deck,0); //the k passed is 0 which will allows the deck to 
																   //get printed when repeat_shuffle is called from _interleave.
}

void shuffle(CardDeck orig_deck) 
{
	_shuffle(orig_deck, 0);//the k passed is 0 which will allows the deck to 
						   //get printed when repeat_shuffle is called from _interleave.
}

void _shuffle(CardDeck orig_deck, int k)
{
	int numpairs = orig_deck.size - 1;

	CardDeck * upper_deck = malloc( numpairs * sizeof(*upper_deck) );
	CardDeck * lower_deck = malloc( numpairs * sizeof(*lower_deck) );

	// freeing allocated memory if out of memory
	if (upper_deck == NULL || lower_deck == NULL)
	{
		free(upper_deck);
		free(lower_deck);
		return;
	}

	// call divideDeck to fill upper_deck and lower_deck 
	divide(orig_deck, upper_deck, lower_deck);

	while (numpairs > 0) 
	{
		// for each pairs call interleave
		_interleave(upper_deck[numpairs - 1], lower_deck[numpairs-1], k);
		numpairs--;
	}
	free(upper_deck);
	free(lower_deck);
}

void _interleave(CardDeck upper_deck, CardDeck lower_deck, int k)
{
	CardDeck shuffled_deck={
		.size = 0,
		.cards = {0}
	};

	_interleaveRecursive(upper_deck, lower_deck, shuffled_deck, k);
}

void _interleaveRecursive(CardDeck upper, CardDeck lower, CardDeck shuffle, int k)
{
	if(lower.size > 0) //adding card from lower deck
	{
		shuffle.cards[shuffle.size] = lower.cards[0];
		shuffle.size++;
		char card_lower = lower.cards[0];
		_removeCard(lower.cards); //removing top-most card of lower deck for recursive call of function
		lower.size--;
		
		_interleaveRecursive(upper, lower, shuffle, k);
		
		_addCard(lower.cards,card_lower); //restoring lower deck to original condition (before removing top-most card)
		lower.size++;
		shuffle.size--;
	}
	if(upper.size > 0) //adding card from upper deck
	{
		shuffle.cards[shuffle.size] = upper.cards[0];
		shuffle.size++;
		char card_upper = upper.cards[0];
		_removeCard(upper.cards); //removing top-most card of upper deck for recursive call of function
		upper.size--;
		
		_interleaveRecursive(upper, lower, shuffle, k); //restoring upper deck to original condition (before removing top-most card)
		
		_addCard(upper.cards,card_upper);
		upper.size++;
		shuffle.size--;
	}
	if(lower.size == 0 && upper.size == 0) //base case
	{
		repeat_shuffle(shuffle, k-1);
		//print_deck(shuffle);
		//printf("\n");
	}
}

void _removeCard(char * deck)
{
	int counter=1;
	while(deck[counter] != 0)
	{
		deck[counter-1]=deck[counter];
		counter++;
	}
	deck[counter-1]=deck[counter];
}

void _addCard(char * deck, char card)
{
	int counter=0;
	while(deck[counter] != 0)
	{
		counter++;
	}
	while(counter >= 0)
	{
		deck[counter+1]=deck[counter];
		counter--;
	}
	deck[0]=card;
}
