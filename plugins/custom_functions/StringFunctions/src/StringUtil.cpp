/*******************************************************************************

 Copyright (c) 2017-present

 *******************************************************************************/
#include <string>
#include "StringUtil.h"
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
#include <numeric>

struct trieNode {
	bool isWord;
	char key;
	std::string text;
	size_t frequency;
	std::map<char, trieNode*> children;
};

/******************************************************************************/

std::string StringUtil::stringReverse(const std::string &text) {
    //create a copy
    std::string copyText(text);
    std::reverse(copyText.begin(), copyText.end());
    return copyText;
}

/* 
 * create a new node
 */
trieNode* createNode(trieNode* parent, char key, bool isWord) {
    trieNode *child = new trieNode;
	child->isWord = false;
	child->key = '\0';
	child->frequency = 0;

	if (parent != NULL) {
		parent->isWord = isWord;
		parent->key = key;
		parent->children[key] = child; 
		parent->frequency = 0;
	} else {
		parent = child;
	}

	return parent;
}

/**
 * returns NULL if character is not found in the map
 */
trieNode * checkIfKeyExist(trieNode* curr, char c) {
	if (curr->children.find(c) == curr->children.end()) {
		return NULL;
	} else {
		return curr;
	}
}

/**
 * insert a character in a Trie
 */
void insertTrie(trieNode*& curr, std::string word) {
	trieNode * currentNode = curr; 

	for (size_t i = 0; i < word.length(); i++) {
		char key = word[i]; 
		if (checkIfKeyExist(currentNode, key) == NULL) {
			currentNode = createNode(currentNode, key, false);
		} 
		currentNode = currentNode->children[key];
	}
    //for last node
	currentNode->isWord = true;
	currentNode->frequency = currentNode->frequency + 1; 
	currentNode->text = word; 
}
/**
 * preorder traversal of a Trie 
*/
void traverseTrie(trieNode* root, unsigned& maxCount, std::map<std::string, unsigned>& mFrequent) {
    trieNode* currentNode = root;

	std::map<char, trieNode*>::iterator it;

	for (it = currentNode->children.begin(); it != currentNode->children.end(); it++) {
		//if greater frequency word is found
		if (maxCount < it->second->frequency) {	
			mFrequent.clear();
			maxCount = it->second->frequency;
			mFrequent[it->second->text] = it->second->frequency;
		}
		//collect all words if same frequency
		if (maxCount == it->second->frequency) {
			//check if duplicates is not there
			if ( mFrequent.find(it->second->text) == mFrequent.end() ) {
				mFrequent[it->second->text] = it->second->frequency;
			}
		}
		traverseTrie(it->second, maxCount, mFrequent);
	}
}

/**
 * return the most frequent word along with its frequency
*/
std::string StringUtil::frequentWord(const std::string &text) {

	std::vector<std::string> sText;
	std::stringstream streamText(text);
	std::string currText;
	std::string resultText = "";

    /* extract all the words in a string */
	for (size_t i = 0;  streamText >> currText; i++) {
	    sText.push_back(currText);
	}

	//create an empty node
    trieNode * root = createNode(NULL, '\0', false);

	/* for every words in a string, add to a Trie */
	for (std::vector<std::string>::const_iterator iterString = sText.begin(); iterString != sText.end(); iterString++) {
		insertTrie(root, *iterString);
	}

	/* get most frequent word */
	std::map<std::string, unsigned> mFrequent;
	unsigned maxCount = 0;

	/* do a preorder traversal */
    traverseTrie(root, maxCount, mFrequent);

	std::map<std::string, unsigned>::iterator it;
	std::stringstream ss; 

	/* getting result in format */
	resultText += "[";
	for (it = mFrequent.begin(); it != mFrequent.end(); it++) {
		ss.str(std::string());
		ss << it->second;
		resultText += "{" + it->first + ":" +  ss.str() + "}";
	}
	resultText += "]";

	//cleaning
	sText.clear();
	mFrequent.clear();
    return resultText;
}