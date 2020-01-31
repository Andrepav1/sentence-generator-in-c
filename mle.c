#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 		1
#define FALSE 		0

#define MAXSIZE 	64

#define START 		"<s>"
#define END 		"</s>"
#define NEWLINE		'\n'

struct stringNode {
	char * string;
	int stringCount;
	struct stringNode * nextString;
	struct stringNode * prevString;
};

struct stringNode * head_string;
struct stringNode * head_string_sentence;

int wordCount = 0;

int appendChar(char*s, char c) {
     int len = strlen(s);
     s[len] = c;
     s[len+1] = '\0';
     return 0;
}

int appendString(char * s) {

		wordCount++;

		// printf("%s added.\n",s);

		if(head_string == NULL) {
			head_string = (struct stringNode *) malloc(sizeof(struct stringNode));
			head_string->string = s;
			head_string->nextString = NULL;
			head_string->prevString = NULL;
			return;
		}

		struct stringNode * curr_string = head_string;

	  // Go though each struct until we get to the end
	  while (curr_string->nextString != NULL)
	    curr_string = curr_string->nextString;

		struct stringNode * temp_string = curr_string;

		// Allocating space for next string
	  curr_string->nextString = (struct stringNode *) malloc(sizeof(struct stringNode));
	  curr_string = curr_string->nextString;

		// Setting new currString parameters
	  curr_string->string = s;
	  curr_string->nextString = NULL;
		curr_string->prevString = temp_string;
}

int appendSentenceString(char * s) {

		// printf("%s added.\n",s);

		if(head_string_sentence == NULL) {
			head_string_sentence = (struct stringNode *) malloc(sizeof(struct stringNode));
			head_string_sentence->string = s;
			head_string_sentence->nextString = NULL;
			head_string_sentence->prevString = NULL;
			return;
		}

		struct stringNode * curr_string = head_string_sentence;

	  // Go though each struct until we get to the end
	  while (curr_string->nextString != NULL)
	    curr_string = curr_string->nextString;

		struct stringNode * temp_string = curr_string;

		// Allocating space for next string
	  curr_string->nextString = (struct stringNode *) malloc(sizeof(struct stringNode));
	  curr_string = curr_string->nextString;

		// Setting new currString parameters
	  curr_string->string = s;
	  curr_string->nextString = NULL;
		curr_string->prevString = temp_string;
}

showCorpus() {

  struct stringNode * curr = head_string;
  while (curr != NULL) {
		fprintf(stderr, "%s ", curr->string);
		if(curr->string == END) { // New line
			fprintf(stderr, "\n");
		}
		else if(curr->string == START) {} // Do nothing
		else {} // Do nothing
		curr = curr->nextString;
  }
}

showAllSentences() {

  // fprintf(stderr, "Sentence:\n");
  struct stringNode * curr = head_string_sentence;
  while (curr != NULL) {
		fprintf(stderr, "%s ", curr->string);
		if(curr->string == END) { // New line
			fprintf(stderr, "\n");
		}
		else if(curr->string == START) {} // Do nothing
		else {} // Do nothing

		curr = curr->nextString;
  }
}

showSentence(sentenceIndex) {
	int curr_index = 1;
  struct stringNode * curr = head_string_sentence;
  while (curr != NULL) {
		if(curr_index == sentenceIndex) {
			fprintf(stderr, "%s ", curr->string);
		}

		if(curr->string == END) { // Increase index
			curr_index++;
		}
		else if(curr->string == START) {} // Do nothing
		else {} // Do nothing

		curr = curr->nextString;
  }
}

int getMaxCommonDivisor(int a, int b) {
	int min = a <= b ? a : b;
	while(a%min != 0 && b%min != 0) min--;
	return min;
}

int countPairOcc(char * w1, char * w2) {

	int count = 0;
	struct stringNode * curr = head_string;

	while(curr->nextString != NULL) {
		//fprintf(stderr, "%s - %s\n",curr->string,w1);
		if(strcmp(curr->string,w1) == 0 && strcmp(curr->nextString->string,w2) == 0) {
			count++;
		}

		curr = curr->nextString;
	}

	// fprintf(stderr, "%s - %s count: %d\n",w1,w2, count);
	return count;
}

int countOcc(char * w1) {

	int count = 0;
	struct stringNode * curr = head_string;

	while(curr != NULL) {
		// fprintf(stderr, "%s - %s\n",curr->string,w1);
		if(strcmp(curr->string,w1) == 0)
			count++;

		curr = curr->nextString;
	}
	return count;
}

float calculateMLE(char * w2, char * w1) {
	fprintf(stderr,"P(%s|%s) = ",w2,w1);

  	// Avoids division with zero
	if(countOcc(w1) == 0) {
		fprintf(stderr,"0\n");
		return 0;
	}

	//fprintf(stderr,"%s not zero\n",w1);

	//fprintf(stderr,"Calculating pair count: %s - %s\n",w1,w2);
	int pairCount = countPairOcc(w1,w2);

	//fprintf(stderr,"Calculating word count: %s\n",w1);
	int w1count = countOcc(w1);

	float result = (float) pairCount / w1count;

	if(strcmp(w1,END) == 0)
		fprintf(stderr,"%d/%d (~ %8.7f) =\n",pairCount, w1count,result);
	else
		fprintf(stderr,"%d/%d (~ %8.7f) *\n",pairCount, w1count,result);

	return result;
}

calculateSentencesProb() {

	struct stringNode * curr_string = head_string;
	struct stringNode * curr_string_sentence = head_string_sentence;
	int sentenceCount = 1;

	while(curr_string_sentence->nextString != NULL) {
		fprintf(stderr, "==============================================================\n");
		fprintf(stderr, "Calculating Maximum Likelihood Estimation (MLE) of sentence %d:\n", sentenceCount);
		showSentence(sentenceCount++);
		fprintf(stderr, "\n==============================================================\n");
		float mleProb = 1;
		int perc1 = 1;
		int perc2 = 1;
		while(strcmp(curr_string_sentence->nextString->string,START) != 0) {

			char * w1 = curr_string_sentence->string;
			char * w2 = curr_string_sentence->nextString->string;

			mleProb *= calculateMLE(w2,w1);
			perc1 *= countPairOcc(w1,w2);
			perc2 *= countOcc(w1);

			// fprintf(stderr, "curr mleProb: %6.5f\n", mleProb);

			curr_string_sentence = curr_string_sentence->nextString;
			if(curr_string_sentence->nextString == NULL) break;

		}
		if(mleProb == 0) {
			fprintf(stderr, "= 0.\n");

		} else {
			int mcd = getMaxCommonDivisor(perc1,perc2);
			fprintf(stderr, "= %d/%d (~ %6.5f).\n", perc1 / mcd, perc2 / mcd, mleProb);
		}

		if(curr_string_sentence->nextString == NULL) break;

		curr_string_sentence = curr_string_sentence->nextString;

	}

}

char * generateWord(char * currWord) {

	// fprintf(stderr, "==========================\n");
	if(strcmp(currWord, START) == 0) {
		fprintf(stderr, "Generated:");
	}
	else if(strcmp(currWord, "s") == 0 || strcmp(currWord, "t") == 0 ||
					strcmp(currWord, "d") == 0 || strcmp(currWord, "ve") == 0) {
		fprintf(stderr, "'%s", currWord);
	}
	else if(strcmp(currWord, END) == 0) {
		return END;
	}
	else {
		fprintf(stderr, " %s", currWord);
	}
	// fprintf(stderr, "\n==========================\n");
	struct stringNode * head_word_prob;

	struct stringNode * curr_string = head_string;

	int currWordCount = 0;

	while(curr_string != NULL) {

		// Add next word to probable next word, and increase counter
		if(strcmp(curr_string->string, currWord) == 0) {
			currWordCount++;

			if(head_word_prob == NULL) {
				// fprintf(stderr, "First word added %s...\n", curr_string->nextString->string);
				head_word_prob = (struct stringNode *) malloc(sizeof(struct stringNode));
				head_word_prob->string = curr_string->nextString->string;
				head_word_prob->stringCount = 1;
				head_word_prob->nextString = NULL;
				head_word_prob->prevString = NULL;

				curr_string = curr_string->nextString;
				continue;
			}

			struct stringNode * curr_word_prob = head_word_prob;

			while (curr_word_prob != NULL) {

				if(strcmp(curr_word_prob->string, curr_string->nextString->string) == 0) {
					curr_word_prob->stringCount++;
					// fprintf(stderr, "Increase count for %s...\n", curr_string->nextString->string);

					break;
				}

				if(curr_word_prob->nextString == NULL) break;
				curr_word_prob = curr_word_prob->nextString;

			}

			if(curr_word_prob->nextString == NULL) {
				// fprintf(stderr, "Adding new word %s...\n", curr_string->nextString->string);

				// Word not added to word probabilities, adding now
				curr_word_prob->nextString = (struct stringNode *) malloc(sizeof(struct stringNode));
				curr_word_prob = curr_word_prob->nextString;
				curr_word_prob->string = curr_string->nextString->string;
				curr_word_prob->stringCount = 1;
				curr_word_prob->nextString = NULL;
				curr_word_prob->prevString = NULL;
			}

		}
		curr_string = curr_string->nextString;
	}

	int randWord = rand() % currWordCount;

	struct stringNode * curr_word_prob = head_word_prob;

	int wordIndex = 0;
	// fprintf(stderr, "Random word at index %d\n", randWord);
	while (curr_word_prob != NULL) {
		// fprintf(stderr, "possible next word: %s (%d/%d)\n", curr_word_prob->string, curr_word_prob->stringCount, currWordCount);

		wordIndex += curr_word_prob->stringCount;

		if(randWord < wordIndex) break;

		curr_word_prob = curr_word_prob->nextString;
	}

	head_word_prob = NULL;

	return curr_word_prob->string;
}

generateSentence() {

	char * currWord = (char *) malloc(sizeof(char)*MAXSIZE);
	currWord = START;
	int i;
	for(i = 0; i<100; i++) {

		currWord = generateWord(currWord);
		if(strcmp(currWord,END) == 0) break;

	}
	fprintf(stderr, ".\n");
}

void main (int argc, char** argv) {

	FILE * fd;
	FILE * fs;
	char ch;

	//Take first argument and open it using fd
	fd = fopen(argv[1], "r");

	//Take first argument and open it using fs
	fs = fopen(argv[2], "r");

	// Checking that argumant is a valid file
	if(fd == NULL || fs == NULL) {
		fprintf(stderr, "Bad arguments: \n");
		fprintf(stderr, "Correct usage is ./mle [corpus-file] [sentence-file].\n");
		exit(EXIT_FAILURE);
	}

// ==============================================================
// Uploading all words in stringNode LList
// ==============================================================

	ch = fgetc(fd);
	ch = tolower(ch);

	appendString(START);
	while (ch != EOF) {

		char * str = (char *) malloc(sizeof(char)*MAXSIZE);

		// while(ch != ' ' && ch != '\n' && ch != EOF && ch != ',' && ch != '.' && ch != '?')
		while(isalpha(ch)) {

			appendChar(str,ch);
			ch = fgetc(fd);
			ch = tolower(ch);
		}

		appendString(str);

		while(!isalpha(ch)) {
			char next_ch = fgetc(fd);

			if(ch == EOF || next_ch == EOF) { // End Of File
				ch = next_ch;
				break;
			}
			else if(ch == NEWLINE) { // Start new sentence
				appendString(END);
				appendString(START);
			}
			else {} // Do nothing

			ch = tolower(next_ch);
		}

		if (ch == EOF) break;
	}
	appendString(END);

	//showCorpus();

// ==============================================================
// Inserting the sentence to be read
// ==============================================================

	ch = fgetc(fs);
	ch = tolower(ch);

	// Should the program insert START and END word
	int autoStrtnd = TRUE;	

	if(autoStrtnd)
		appendSentenceString(START);
	
	while (ch != EOF) {

		char * str = (char *) malloc(sizeof(char)*MAXSIZE);
		
		//while(ch != ' ' && ch != '\n' && ch != EOF && ch != ',' && ch != '.' && ch != '?') {
		while(isalpha(ch)) {

			appendChar(str,ch);
			ch = fgetc(fs);
			ch = tolower(ch);
		}

		appendSentenceString(str);

		while(!isalpha(ch)) {
			char next_ch = fgetc(fs);

			if(ch == EOF || next_ch == EOF) { // End Of File
				ch = next_ch;
				break;
			}
			else if(ch == NEWLINE) { // Start new sentence
				
				if(autoStrtnd) {
					appendSentenceString(END);
					appendSentenceString(START);
				}
			}
			else {} // Do nothing

			ch = tolower(next_ch);
		}

		if (ch == EOF) break;
	}
	if(autoStrtnd)
		appendSentenceString(END);

// =======================================================

	//showAllSentences();

	// fprintf(stderr,"Word Count: %d\n", wordCount);

	int mode = 2;
	fprintf(stderr,"=====================================================\n");
	fprintf(stderr,"Choose a functionality:\n");
	fprintf(stderr,"Calculate probability of sentences file -> Press [1]\n");
	fprintf(stderr,"Generate sentences from the corpus file -> Press [2]\n");

	scanf("%d", &mode);

	if (mode != 1 && mode != 2) {
		fprintf(stderr,"Invalid choice\n");
		return;
	}

	srand((unsigned) time(NULL));

	switch(mode) {
		case 1: calculateSentencesProb();
						break;
		case 2:	generateSentence(START);
						break;
		default:
		break;
	}

	fclose(fd);

}
