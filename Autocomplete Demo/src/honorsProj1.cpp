//*****************
//Program Name: Honors Project 1
//Author: Clarissa Cheung
//IDE Used: Eclipse
//Program description: demoing tries with an auto suggestion for prefix program
//*****************
#include <iostream>
#include <fstream>
using namespace std;
int length = 5;
struct Node{
	Node *child[26]; //to hold array of characters
	bool isWord; //to signify if the current character is the end of the word
};
int displayMenu();
bool isLastNode(Node* root);
void recommend(Node* root, string prefix);
void printAutoSuggestions(Node* root, string key);
bool search(Node *root, string str);
void insert(Node *root, string str);
char convertChar(char c);

int main(){
	//As I didn't think it was necessary to compile together my own list of words,
	//I directly downloaded wordList.txt from https://github.com/dwyl/english-words,
	//using the words_alpha.txt file
	Node* root = new Node;
	root->isWord = false;
	for (int i = 0; i < 26; i++)
		root->child[i] = nullptr;
	ifstream file("wordList.txt");
	string line;
	if(!file){
		cout << "wordList.txt could not be opened\n";
		return 1;
	}else{
		while(getline(file, line))
			insert(root, line);
	}
	file.close();

	/* Since I used such a big word file, I get more auto suggestions than needed. I also
	 * don't have a way to record the user's most used words over time in order to filter
	 * recommendations, so I'm just gonna output the words that are within a specific
	 * range in terms of the number of letters in the word. It's not perfect for a real
	 * world application, but it should be enough for a demo of the auto complete feature.
	 */
	int choice = 0, choice2 = 0;
	string pre;
	while(choice!=3){
		choice = displayMenu();
		switch(choice){
			case 1:
				cout << "\nEnter prefix: ";
				cin >> pre;
				if(length > pre.length()){
					cout << "Printing all words found for '" << pre << "' with length " << length << "...\n";
					printAutoSuggestions(root, pre);
				}else{
					length = 5;
					cout << "Error, recommended word length is smaller than or equal to the prefix's length\n";
					cout << "Setting it back to default\n";
				}
				cout << "\n";
				break;
			case 2:
				do{
					cout << "\nEnter a value to change the length of the words outputted: ";
					cin >> choice2;
				}while(choice2 < 0);
				length = choice2;
				cout << "Changing length of recommended words to " << choice2 <<"\n\n";
				break;
			case 3:
				cout << "Quitting menu...\n";
				choice = 3;
				break;
		}
	}
	cout << "\nThis is my original work; apart from standard tutoring or class collaboration, \nI neither received help nor copied this code from another source.";
	return 0;
}

//displayMenu(): displays the function menu to print word
// recommendations and change the recommended word length
//Arguments: N/A | Returns: choice (int)
int displayMenu(){
	int choice = 0;
	do{
		cout << "Enter: [1] to print out auto suggestions\n";
		cout << "       [2] to change the words printed according to their length\n";
		cout << "       [3] Quit\n";
		cin >> choice;
	}while(choice < 1 || choice > 3);
	return choice;
}

//convertChar(): Converts given letter to decimal form and subtracts 97(a) in order to
//				 get the index
//Arguments: c (char) | Returns: char
char convertChar(char c)
	{ return((int)c - (int)'a'); }

//insert(): if the prefix doesn't exist in the trie, inserts it, but if it does
//			exist, it marks the leaf node
//Arguments: str (string), *root (Node) | Returns: void
void insert(Node *root, string str){
	Node *newNode = root;
	//for each letter in string, goes on different level
	for (int i = 0; i < str.length(); i++){
		if (!newNode->child[convertChar(str[i])]){
			Node *ch = new Node;
			ch->isWord = false;
			for (int i = 0; i < 26; i++)
				ch->child[i] = nullptr;
			newNode->child[convertChar(str[i])] = ch;
		}

		newNode = newNode->child[convertChar(str[i])];
	}
	newNode->isWord = true; //shows which character is the end of the word
}

//search(): searches if the trie contains a certain prefix
//Arguments: str (string), *root (Node) | Returns: (bool)
bool search(Node *root, string str){
	Node *current = root;
	for (int i = 0; i < str.length(); i++){
		if (!current->child[convertChar(str[i])])
			return false;
		current = current->child[convertChar(str[i])];
	}
	return (current != nullptr && current->isWord);
}

//isLastNode(): checks if the current node doesn't have any more children
//Arguments: *Node (Node) | Returns: (bool)
bool isLastNode(Node* root){
	for (int i = 0; i < 26; i++)
	if (root->child[i])
		return 0;
	return 1;
}

//recommend(): uses recursion to cycle throughout the trie and print
//			   out the children of the given prefix
//Arguments: prefix (string), *root (Node) | Returns: void
void recommend(Node* root, string prefix){
	//if the end of the character is the end of a string(is a word)
	//for 2nd condition, see lines 43 to 48
	if (root->isWord && prefix.length() == length)
		cout << prefix << endl;
	if (isLastNode(root))
		return;
	for (int i = 0; i < 26; i++){
		if (root->child[i]){
			//according to the ascii table, 'a' starts at 97 in decimal form, so we are using this
			//to access each letter as a number. I am using recusion to loop throughout the trie
			//and continually add onto the prefix until I complete the word with the given prefix
			prefix.push_back(97 + i);
			recommend(root->child[i], prefix);
			//If I didn't delete whatever I added onto the prefix, the newly formed prefix
			//would be treated as the new prefix, resulting in the function not returning
			//the right amount of word recommendations for every prefix
			prefix.pop_back();
		}
	}
}

//printAutoSuggestions(): prints all suggestions for prefix given
//Arguments: prefix (string), *root (Node) | Returns: void
void printAutoSuggestions(Node* root, string prefix){
	Node* newNode = root;
	//for each letter in prefix, traverse the trie to find if the prefix
	//exists and get to the point where the prefix ends in order to find
	//the children
	for (int i = 0; i < prefix.length(); i++){
		int index = convertChar(prefix[i]);
		if (!newNode->child[index])
			cout << "\nNo string found with this prefix\n";
		newNode = newNode->child[index];
	}
	//if no children and prefix exists as word
	if ((newNode->isWord == true) && isLastNode(newNode)){
		cout << prefix << endl;
		cout << "\nNo other strings found with this prefix\n";
	}
	//if prefix has children, print out those that fit the requirements
	if (!isLastNode(newNode)){
		recommend(newNode, prefix);
	}
}
