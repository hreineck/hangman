# Hangman

A bot to play hangman against.

Hangman is a pretty boring game, but it's kind of interesting from a math perspective.

Given a secret word, the player guesses a letter. If the secret word contains the letter, all of the instances of that letter in the word are revealed.

Because of this, the longer the word is, the easier it is to solve.

It also gets easier to solve the word the more correct guesses you make, as it reduces the set of possible words.

It also means the worst possible score is 26 guesses.

It's like password cracking, but incredibly easy.

The bot can solve using four strategies, but only the last one is optimal.

## 1. Random Guessing
This isn't very effective.

## 2. Guessing the Most Frequent Letters
I used a quick script to find the most common letter out of all the words in words.txt (https://github.com/dwyl/english-words)

The bot simply guesses down the line.

It's slightly more effective than random guessing.

## 3. Guessing the Most Frequent Letters for each Index
Freq.c gives the most common letter at each index in the words in words.txt

The bot then guesses down the line until it gets the first index right, then moves on to the next index, and so on.

This strategy still isn't very good, but it's the most interesting to me because it's the most similar to password cracking, as it deals with guessing each index of the secret word correctly and is agnostic of the length of the word.

If you're brute forcing a password, you have a 1/(# of possible characters) chance of guessing the letter at any given index.

A brute force algorithm that simply guesses all the letters in order will then have a worst case of n^n guesses, where n is the number of possible characters.

However, if you happen to guess the first letter correct, you reduce the number of guesses you need to (n-1)^(n-1)

If you had a list of many possible words used in passwords, and given that most (non-random) passwords start with letters, you could potentially reduce the time by guessing the 
most common letters first at each index.

Anyway, this strategy is still pretty garbage for the actual game.

## 4. Iterating through a Dictionary
The bot loops through all the words in dictionary, ruling out any words that:
- Don't match the secret word's length
- Contain any letters that have been guessed, but were wrong
- Don't contain the letters in the correct indexes that are known to be in the word

Then it guesses the most frequent letter out of all of those words.

It does really well for words that are in the dictionary. For longer words, it often gets a perfect score (# of guesses = # of unique letters in the secret word).

Some limitations of this strategy:
- It's a bit slow, because it runs through the entire list every loop. This could be remedied by having separate dictionaries for every word length, or by storing the indexes of all the valid words as you go through the dictionary.
- The dictionary contains a LOT of words, and most of them aren't very common. Because of this, the last couple guesses end up being nearly random, since it ususally finds five or six words with all but one of their letters exactly the same. This could be fixed by building up data for the usage of all the words in the dictionary and using that to break ties (this would also be useful for strategy 3.)
- It does really poorly against words that aren't in the dictionary, because once it runs out of possible words, it starts guessing randomly from A-Z. You could supplement this with strategy 3 to make it a bit better.
- It's still useless against random strings, but that can't be helped.
