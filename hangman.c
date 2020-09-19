#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

char randomletter(){
	return (rand() % 26) + 65;
}

int alreadyGuessed(char letter, char* guessed){
	if(guessed[toupper(letter) - 65]) return 1;
	return 0;
}

int solved(char* word, char* guessed){
	//iterate through the secret word until a letter that has not been guessed is found
	int i;
	char curr;
	for(i = 0; i < strlen(word) - 1; i++){
		if(!alreadyGuessed(word[i], guessed)){
			//if the current letter has NOT been guessed
			return 0; //not solved
		}
	}
	//made it to the end, so all letters guessed
	return 1; //solved
}

int guessRandom(char* secret){
	char guessed[26] = "";
	int guesses = 0;
	while(!solved(secret, guessed)){
		char newguess = randomletter();
		while(alreadyGuessed(newguess, guessed)){
			newguess = randomletter();
		}
		guessed[newguess - 65] = 1;
		printf("guessing %c\n", newguess);
		guesses++;
	}
	return guesses;
}

int guessMostCommon(char* secret){
	char* most_common = "EARIOTNSLCUDPMHGBFYWKVXZJQ";
	char guessed[26] = "";
	int guesses = 0;
	int i;
	for(i = 0; i<26; i++){
		//iterate through the most common letters
		guesses++;
		printf("guessing %c\n", most_common[i]);
		guessed[most_common[i] - 65] = 1;
		printf("%s\n", guessed);
		if(solved(secret, guessed)){
			//solved
			return guesses;
		}
	}
	return guesses;
}

int guessIndexFreqs(char* secret){
	//read in the most frequent letters for each index from a text file
	FILE* f;
	f = fopen("freqindex.txt", "r");
	int guesses = 0;
	char guessed[26] = "";
	int i;
	for(i = 0; i < strlen(secret); i++){
		printf("index %d\n", i);
		char buff[32];
		fgets(buff, 32, f);
		printf("%s\n", buff);

		int j;
		for(j = 0; j < 26; j++){
			if(alreadyGuessed(secret[i], guessed)){
				break;
			}
			//guess down the line of common letters per index
			char letter_to_guess = buff[j];
			if(!alreadyGuessed(letter_to_guess, guessed)){
				printf("guessing %c\n", letter_to_guess);
				guessed[letter_to_guess - 65] = 1;
				guesses++;
				if(solved(secret, guessed)){
					return guesses;
				}
			}
		}
	}
	return guesses;
}

int guessDictionary(char* secret){
	//make guesses based on searching through a large dictionary
	//first, narrows the dictionary down to words of the same length
	//then, for each of those, tallies up the most frequent letter for words that share the known letters
	FILE *f;
	f = fopen("words.txt", "r");
	int guesses = 0;
	char guessed[26] = "";
	char wrong[26] = "";
	int len_secret = strlen(secret) - 1;
	char *current_word = (char*)calloc(strlen(secret), sizeof(char));

	while(strlen(current_word) != len_secret){
		int freq[26] = {0};
		char buff[200] = "";
		while(fgets(buff, 200, f)){
			//go through all words	
			if(strlen(buff) - 2 == len_secret){
				//printf("%s %d\n", buff, strlen(buff));
				int i;
				int valid = 1;
				int word_freq[26] = {0};
				for(i = 0; i < strlen(buff) - 2; i++){
					word_freq[buff[i] - 97] += 1;
					if(alreadyGuessed(buff[i], wrong)){
						//printf("invalid %c %c\n", current_word[i], buff[i]);
						valid = 0;
						break;
					}
					if(current_word[i] && current_word[i] != buff[i]){
						valid = 0;
						break;
					}
					int j;
					for(j = 0; j < len_secret; j++){
						if(current_word[j] 
								&& current_word[j] == buff[i] 
								&& current_word[i] != buff[i]){
							//we already found this letter, but not in this index
							valid = 0;
							break;
						}
					}
				}
				if(valid){
					//add frequencies to the list
					int j;
					for(j = 0; j < 26; j++){
						freq[j] += word_freq[j];

					}
				}
			}

			memset(buff, 0, 200);
		}

		//get most frequent letter
		int i;
		int max = -1;
		int max_index = 0;
		for(i = 0; i < 26; i++){
			//printf("%c: %d\n", i+65, freq[i]);
			if(!guessed[i]){
				if(freq[i] > max){
					max = freq[i];
					max_index = i;
				}	
			}
		}

		//update current known word
		char letter_to_guess = max_index + 97;
		printf("guessing: %c\n", letter_to_guess);
		int correct = 0;
		for(i = 0; i < strlen(secret); i++){
			if(secret[i] == letter_to_guess){
				current_word[i] = letter_to_guess;
				correct = 1;
			}
		}
		if(!correct){
			wrong[max_index] = 1;
		}

		for(i = 0; i < strlen(secret); i++){
			printf("%c", current_word[i]);
		}
		printf("\n");
		guessed[max_index] = 1;
		guesses++;
		rewind(f);
		memset(freq, 0, 26);
	}
	free(current_word);
	return guesses;
}

int main(){
	char secret[200] = "";

	srand(time(0));

	while(1){
		int valid = 0;
		while(!valid){
			valid = 1;

			printf("enter word (/q to quit): ");
			fgets(secret, 200, stdin);
			if(!strcmp(secret, "/q\n")){
				return 0;
			}

			int i;
			for(i = 0; i < strlen(secret) - 1; i++){
				if(!isalpha(secret[i])){
					valid = 0;
					break;
				}
				else{
					secret[i] = tolower(secret[i]);
				}
			}
			if(!valid){
				printf("invalid word\n");
				memset(secret, 0, 200);
			}
		}

		//printf("guessing most common solved in %d guesses\n", guessMostCommon(secret));
		//printf("guessing random solved in %d guesses\n", guessRandom(secret));
		//printf("guessing mose common letter by index solved in %d guesses\n", guessIndexFreqs(secret));
		printf("guessing with dictionary solved in %d guesses\n", guessDictionary(secret));

		memset(secret, 0, 200);
	}

	return 0;
}
