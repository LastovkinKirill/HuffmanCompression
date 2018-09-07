// HuffmanCompression.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stack> 
#include <vector>
#include <map>
#include <list>
#include <fstream>

using namespace std;

struct Link
{
	char data;
	Link* next;
	Link* previous;
};



Link* createLink(char aData)
{
	Link* aLink = new Link;
	aLink->data = aData;
	aLink->next = NULL;
	aLink->previous = NULL;
	return aLink;
}

void displayLink(Link* aLink)
{
	cout << aLink->data << " ";
}

struct doublyLinkedList
{
	Link* first;
	Link* last;
	int size;
};

doublyLinkedList* createDLList()
{
	doublyLinkedList* aList = new doublyLinkedList;
	aList->first = NULL;
	aList->last = NULL;
	aList->size = 0;
	return aList;
}

bool isEmpty(doublyLinkedList* aList)
{
	return (aList->first == NULL);
}

void insertLast(doublyLinkedList* aList, char aData)
{
	Link* newLink = createLink(aData);
	if (isEmpty(aList))
		aList->first = newLink;
	else
	{
		aList->last->next = newLink;
		newLink->previous = aList->last;
	}
	aList->last = newLink;
	aList->size++;

}


Link* deleteLast(doublyLinkedList* aList) // we can use bool func
{									    //but in stack and Queue 
	if (isEmpty(aList))
	{
		return NULL;
	}
	else
	{
		Link* temp = aList->last;
		if (aList->first->next == NULL)
			aList->first = NULL;
		else
			aList->last->previous->next = NULL;
		aList->last = aList->last->previous;
		aList->size--;
		return temp;
	}
}

void displayForward(doublyLinkedList* aList)
{
	Link* current = aList->first;
	while (current != NULL)
	{
		displayLink(current);
		current = current->next;
	}
	cout << endl;
}


class Node
{
public:
	char theChar = 'O' ;
	int frequency;
	Node* leftChild = NULL;
	Node* rightChild = NULL;

	void displayNode()
	{
		cout << theChar << endl;
	}
};

class Tree
{
public:
	Node* root;

	Tree()
	{
		Node* newNode = new Node;
		root = newNode;
	}

	doublyLinkedList* codeList = createDLList();
	vector <list <char>> vecList;
	
	void BuildTable(Node* root)
	{
		if (root->leftChild != NULL)
		{
			insertLast(codeList, '0');
			BuildTable(root->leftChild);
		}

		if (root->rightChild != NULL)
		{
			insertLast(codeList, '1');
			BuildTable(root->rightChild);
		}

		if (root->leftChild == NULL && root->rightChild == NULL)
		{
			int i = 0;
			Link* current = codeList->first;
			list <char> listCode ;
			listCode.push_back(root->theChar);
			while (current != NULL)
			{
				listCode.push_back(current->data);
				current = current->next;
			}
			vecList.push_back(listCode);
		}
		//displayForward(codeList);
		deleteLast(codeList);
	}

	void displayTree()
	{
		stack <Node*> globalStack;

		globalStack.push(root);
		int nBlanks = 32;
		bool isRowEmpty = false;
		cout << "......................................................" << endl;
		while (isRowEmpty == false)
		{
			stack <Node*> localStack;
			isRowEmpty = true;
			for (int j = 0; j<nBlanks; j++)
				cout << " ";
			while (globalStack.empty() == false)
			{
				Node* temp = globalStack.top();
				globalStack.pop();
				if (temp != NULL)
				{
					//if(temp->theChar == NULL)

					cout << temp->theChar;
					localStack.push(temp->leftChild);
					localStack.push(temp->rightChild);
					if (temp->leftChild != NULL || temp->rightChild != NULL)
						isRowEmpty = false;
				}
				else
				{
					cout << "--";
					localStack.push(NULL);
					localStack.push(NULL);
				}
				for (int j = 0; j < nBlanks * 2 - 2; j++)
					cout << ' ';
			}
			cout << endl;
			nBlanks /= 2;
			while (localStack.empty() == false)
			{
				globalStack.push(localStack.top());
				localStack.pop();
			}
		}
		cout << "......................................................" << endl;
	}
};

class priorityQ
{
private:
	int maxSize;
	Tree** queMass;
	int nItems;

public:
	
	priorityQ(int s)
	{
		maxSize = s;
		queMass = new Tree*[maxSize];
		nItems = 0;
	}

	void insert(Tree* tNode)
	{
		int j;

		if (nItems == 0)
			queMass[nItems++] = tNode;
		else
		{
			for (j = nItems - 1; j >= 0; j--)
			{
				if (tNode->root->frequency > queMass[j]->root->frequency)
					queMass[j + 1] = queMass[j];
				else
					break;
			}
			queMass[j + 1] = tNode;
			nItems++;
		}
	}

	Tree* remove()
	{	return queMass[--nItems];}

	Tree* peekMin()
	{	return queMass[nItems - 1];}

	int size()
	{ return nItems; }
};

priorityQ* setTrees(char* line)
{
	priorityQ* thePriQue = new priorityQ(127);

	unsigned int lineLenght = strlen(line);

	char* newAlf = new char[127];
	int nItems = 0;

	newAlf[0] = line[0];
	nItems++;

	for (int i = 1; i < lineLenght; i++)
	{
		for (int j = 0; j < nItems; j++)
		{
			if (line[i] == newAlf[j])
				break;
			if (j == nItems - 1)
			{
				newAlf[nItems] = line[i];
				nItems++;
			}
		}
	}

	int freq = 0;

	for (int i = 0; i < nItems; i++)
	{
		for (int j = 0; j < lineLenght; j++)
		{
			if (newAlf[i] == line[j])
				freq++;
		}
		Tree* newTree = new Tree();
		newTree->root->theChar = newAlf[i];
		newTree->root->frequency = freq;
		cout << newAlf[i] << freq << endl;
		thePriQue->insert(newTree);
		freq = 0;
	}
	return thePriQue;
}

Tree* HuffmanTree(char* line)
{
	unsigned int lineLenght = strlen(line);
	priorityQ* QueueTrees = setTrees(line);
	int sizeOfQueue = QueueTrees->size();

	Tree* newTree = NULL;
	for (int i = 0; i < sizeOfQueue - 1; i++)
	{
		newTree = new Tree();

		int newFrequency = QueueTrees->peekMin()->root->frequency;
		newTree->root->leftChild = QueueTrees->remove()->root;

		newFrequency += QueueTrees->peekMin()->root->frequency;
		newTree->root->rightChild = QueueTrees->remove()->root;

		newTree->root->frequency = newFrequency;

		QueueTrees->insert(newTree);

	}
	newTree->displayTree();
	//cout << endl;
	return newTree;
}

void fillTable(Tree* HuffTree)
{
	HuffTree->BuildTable(HuffTree->root);
}

void displayTable(Tree* HuffTree)
{
	int count = 0;
	for (auto itV = begin(HuffTree->vecList); itV != end(HuffTree->vecList); itV++)
	{
		for (auto itL = begin(*itV); itL != end(*itV); itL++)
		{
			if (count == 1)
				cout << " : ";
			cout << *itL;
			count++;
		}
		cout << endl;
		count = 0;
	}
}

int sizeOfencode(Tree* HuffTree, char* line)
{
	int length = 0;
	for (int i = 0; i < strlen(line); i++)
	{
		for (auto itV = begin(HuffTree->vecList); itV != end(HuffTree->vecList); itV++)
		{
			for (auto itL = begin(*itV); itL != end(*itV); itL++)
			{
				if (line[i] != *begin(*itV))
					break;
				length += itV->size() - 1;
				break;
			}
		}
	}
	return length;
}

void encode(Tree* HuffTree, char* line, char* encodeLine)
{
	int j = 0;

	for (int i = 0; i < strlen(line); i++)
	{
		for (auto itV = begin(HuffTree->vecList); itV != end(HuffTree->vecList); itV++)
		{
			for (auto itL = begin(*itV); itL != end(*itV); itL++)
			{
				if (line[i] != *begin(*itV))
					break;
				if (line[i] == *itL)
					continue;
				encodeLine[j++] = *itL;
			}
		}
	}
}

void decode(Tree* HuffTree, char* encodeLine)
{
	Node* currentNode = HuffTree->root;
	for (int i = 0; i < strlen(encodeLine); i++)
	{
		if (encodeLine[i] == '0')
		{ 
			currentNode = currentNode->leftChild;
			if (currentNode->leftChild != NULL || currentNode->rightChild != NULL)
				continue;
			else
			{
				cout<<currentNode->theChar;
				currentNode = HuffTree->root;
			}
		}
		else if (encodeLine[i] == '1')
		{
			currentNode = currentNode->rightChild;
			if (currentNode->leftChild != NULL || currentNode->rightChild != NULL)
				continue;
			else
			{
				cout << currentNode->theChar;
				currentNode = HuffTree->root;
			}
		}
	}
}

int main()
{
	char* line = "SUSIE SAYS IT IS EASY";

	Tree* HuffTree = HuffmanTree(line);

	fillTable(HuffTree);
	displayTable(HuffTree);

	char* encodeLine = new char[sizeOfencode(HuffTree, line)];

	encode(HuffTree,line,encodeLine);
	
	for (int i = 0; i < sizeOfencode(HuffTree, line); i++)
	{
		cout << encodeLine[i];
	}
	cout << endl;
	decode(HuffTree, encodeLine);
	cout << endl;

    return 0;
}


