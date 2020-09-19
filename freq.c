#include <stdio.h>
#include <string.h>

void sort(int* freq, char* letters){
	//sorts the frequencies in descending order via insertion sort
	//and sorts its accompanying array of all letters
	int i;
	for(i = 1; i < 26; i++){
		int j = i;
		while(j > 0 && freq[j] > freq[j - 1]){
			//swap curr elem with elem to the left
			//and also do the same for the letters array
			int temp_freq = freq[j];
			char temp_letter = letters[j];
			freq[j] = freq[j - 1];
			letters[j] = letters[j - 1];
			freq[j - 1] = temp_freq;
			letters[j - 1] = temp_letter;
			j--;
		}
	}
}

int main(){
	FILE *f;
	char buff[50] = "";
	int frequencies[32][26] = {{0}}; //holds the frequency of each letter at each index
	int i;

	f = fopen("words.txt", "r");
	while(fgets(buff, 50, f)){
		//read in each word from the dictionary
		int i;
		
		//printf("%s", buff);
		for(i = 0; i < strlen(buff) - 1; i++){
			//iterate through letters in the word
			//add to the frequency
			frequencies[i][buff[i] - 97]++;
		}		
	}

	/*nicer output	
	printf("\n");
	for(i = 0; i < 32; i++){
		int j;
		printf("index %d:\n", i + 1);
		for(j = 0; j < 26; j++){
			printf("%c found %d times\n", j+97, frequencies[i][j]);
		}
	}
	*/

	//sort and output string for each index
	//the string will be the letters in order of descending frequency
	char letters[27];
	for(i = 0; i < 32; i++){
		strcpy(letters, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		sort(frequencies[i], letters);
		printf("%s\n", letters);
	}
}

