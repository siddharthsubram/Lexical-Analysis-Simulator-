// suppressing compiler warning for strtok
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>  // for sort and unique
#include <sstream>  // for string stream, string to int, string to float

using namespace std;

int readTokens(ifstream &inFile, char *tokens[]) {
	// reads tokens from inFile using the string tokenizer placing them in the token array and returning the number of tokens read 
	char line[80];      // char array used to hold lines of text input
	char *tokenPtr;     // pointer used to tokenize string
	int tokenCount = 0;
	char delimeters[4] = { ' ', '\t', '\n', '\r' };
	while (inFile.getline(line, 80)) {   	// reads upto 80 chars into line array
		cout << "line = " << line << endl;
		tokenPtr = strtok(line, delimeters);   	// assigns tokenPtr to  first token in line
		while (tokenPtr != NULL) {
			cout << tokenPtr << endl;
			tokens[tokenCount] = new char[strlen(tokenPtr + 1)];  	// allocate space for token
			strcpy(tokens[tokenCount++], tokenPtr);			// copy token to tokens array
			tokenPtr = strtok(NULL, delimeters);	// gets next token from line 
		} // while
	} // end while
	return tokenCount;
}  // end readTokens

void printTokens(char *tokens[], int tokenCount) {
	// displays the contents of the token array

	for (int i = 0; i < tokenCount; i++)
		cout << "Token " << i << ": " << tokens[i] << endl;
}

int searchTokens(char *searchString, char *tokens[], int tokenCount) {
	// *** searches the tokens array for searchString    *** returning its position, or -1 if not found
	for (int i = 0; i < tokenCount; i++)
	if (strcmp(searchString, tokens[i]) == 0)
		return i;
	return -1;

}

void printTokenList(vector<string> tokens)
{
	for (unsigned int i = 0; i< size(tokens); i++) cout << i << "\t" << tokens[i] << endl;
}

//bool functions for identifying tokens 
bool IsKeyword(string input)
{
	string Keywords[6] = { "if", "then", "else", "begin", "end", "char" };
	return (find(Keywords, Keywords + 6, input) != Keywords + 6);
}
bool IsInteger(string input)
{
	bool result = true;
	try
	{
		int i = stoi(input);
	}
	catch (...)
	{
		result = false;
	}
	return result;
}
bool IsReal(string input)
{
	bool result = true;
	try
	{
		float i = stof(input);
	}
	catch (...)
	{
		result = false;
	}
	return result;
}
bool IsDigit(string input)
{
	// ascii 0 = 48			9 = 57
	return (input.length() == 1 && (int)input[0] >= 48 && (int)input[0] <= 57);
}
bool IsAlpha(string input)
{
	// ascii A = 65	Z = 90	a=97 z=122
	bool result = (input.length() == 1);
	if (result)
	{
		result = ((int)input[0] >= 65 && (int)input[0] <= 90);
		if (!result) result = ((int)input[0] >= 97 && (int)input[0] <= 122);
	}
	return result;
}
bool IsSpecial(string input)
{
	bool result = (input.length() == 1);
	if (!result) return false;
	switch (input[0])
	{
	case ')':
	case '(':
	case '[':
	case ']':
	case '+':
	case '-':
	case '{':
	case '}':
	case '=':
	case ',':
	case ';': result = true; break;
	default: result = false; break;
	}
	return result;
}

void printTokenReport(vector<string> tokens)
{
	// Printing Original List
	cout << "Original List" << endl;
	printTokenList(tokens);

	// Find Unique & Sorted list
	vector<string> uniquetokens = tokens;
	sort(uniquetokens.begin(), uniquetokens.end());
	auto last = unique(uniquetokens.begin(), uniquetokens.end());
	uniquetokens.erase(last, uniquetokens.end());

	// Printing Original List
	cout << "Sorted & Unique List" << endl;
	printTokenList(uniquetokens);

	// Printing Attributes
	/*
	keyword -> if | then | else | begin | end
	integer->digit | digit integer
	real->integer.integer
	special -> (| ) | [| ] | +| -| = | , | ;
	digit -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
	character->a | b | c ... | z | A | B | C ... | Z
	*/
	int countKeyword = 0, countInteger = 0, countReal = 0, countSpecial = 0, countDigit = 0, countCharacter = 0, countIdentifier = 0;


	int occurances;
	for (unsigned int i = 0; i < size(uniquetokens); i++)
	{
		ostringstream message;
		string currentToken = uniquetokens[i];
		message << currentToken;
		occurances = count(tokens.begin(), tokens.end(), currentToken);
		message << " occured " << occurances << " times.";
		if (IsKeyword(currentToken))
		{
			countKeyword++;
			message << " This is a keyword. ";
		}
		else if (IsInteger(currentToken))
		{
			countInteger++;
			message << " This is an integer. ";
		}
		else if (IsReal(currentToken))
		{
			countReal++;
			message << " This is a real. ";
		}
		else if (IsDigit(currentToken))
		{
			countDigit++;
			message << " This is a digit. ";
		}
		else if (IsSpecial(currentToken))
		{
			countSpecial++;
			message << " This is a special character. ";
		}
		else if (IsAlpha(currentToken))
		{
			countCharacter++;
			message << " This is an alphabet. ";
		}
		else
		{
			countIdentifier++;
			message << " This is an identifier. ";
		}
		cout << message.str() << endl;
	}

	// Print summary of token categories
	cout << "Keywords " << countKeyword << endl;
	cout << "Integers " << countInteger << endl;
	cout << "Real " << countReal << endl;
	cout << "Special Chars " << countSpecial << endl;
	cout << "Digits " << countDigit << endl;
	cout << "Alpha " << countCharacter << endl;
	cout << "Identifiers " << countIdentifier << endl;


}

int main(int argc, char *argv[])
{


	char *words[50];			// array of pointers to point to up to 50 tokens
	int tokenCount = 0;			// used to count tokens as they are read 
	int position = -1;			// used to hold the position of search token 
	char searchString[50];      // used to hold string to search for

	string FilePath = argv[1];

	if (argc < 2) {
		cerr << "Error: file name argument not given" << endl;
		exit(1);
	}

	ifstream inFile(FilePath, ios::in);  // open file for input
	// checks for errors in opening the file
	if (!inFile) {
		cerr << "File " << argv[1] << " could not be opened" << endl;
		exit(1);
	}

	tokenCount = readTokens(inFile, words);

	inFile.close();

	// convert to vector for processing
	vector<string> tokens(begin(words), begin(words) + tokenCount);
	printTokenReport(tokens);


	cout << "Press any key to continue ...";
	cin.get();
	return 0;
}

