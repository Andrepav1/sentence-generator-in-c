# sentence-generator-in-c 

This is a Sentence Generator, that uses bigrams and the maximum likelihood estimation function to generate a sequence of words.
Once compiled, the program needs two params: 
The first one is the corpus file, which is a list of sentences.
The second file in another list of sentences, which will be used by the program to calculate the probability of that sentence to appear.
The program can also generate a sentence depending of the corpus file, by randomly selecting the next work using bigrams.
