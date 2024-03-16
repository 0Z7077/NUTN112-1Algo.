#include<iostream>
#include <fstream>		//�ɮ׶}������
#include <sstream>		//�B�z�r����
#include<string>		
#include <cctype>		//�B�z�r����
#include <algorithm>	//�B�z�r����
#include <iomanip>		//�B�z�r����
#include <ctime>		
#include <random>		//��HTreapNode ��priority ��
using namespace std;

/*vvv--------Binary Search Tree----------vvv*/
class BSTNode {
	friend class BST;
private:
	BSTNode* parent = NULL;
	BSTNode* Lchild = NULL;
	BSTNode* Rchild = NULL;
	string word = 0;
public:
	BSTNode(string w) : parent(0), Lchild(0), Rchild(0), word(w) {};
};
class BST {
private:
	BSTNode* root = NULL;
	int N = 0;
public:
	BSTNode* search(string s) {
		BSTNode* temp = root;
		if (temp == NULL) {
			cout << "The tree is empty." << endl;
		}else{
			while (temp->word != s && temp != NULL) {

				if (s < temp->word) {
					if (temp->Lchild != NULL)	temp = temp->Lchild;
					else break;
				}
				else if(s > temp->word) {
					if (temp->Rchild != NULL) temp = temp->Rchild;
					else break;
				}
			}
			if (temp->word == s) {
//				cout << "The word, " << s << ", is in the dictionary." << endl;
			}
			else {
//				cout << "Do not found the word." << endl;
				temp = NULL;
			}
		}
		return temp;
	}
	void insert(string s) {
		BSTNode* temp = new BSTNode(s);
		BSTNode* base = root;
		BSTNode* t = NULL;
		if (root == NULL) {
			root = temp;
			this->N = 1;
		}
		else {
			while (base != NULL) {
				if (s == base->word) {
//					cout << "Alredy exists." << endl;
					break;
				}
				else if (s < base->word) {
					t = base;
					base = base->Lchild;
				}
				else {
					t = base;
					base = base->Rchild;
				}
			}
			if (base == NULL) {
				temp->parent = t;
				if (t->word > s) {
					temp->parent->Lchild = temp;
				}else{
					temp->parent->Rchild = temp;
				}
				this->N += 1;
			}
		}
		temp = base = t = NULL;
		delete temp;
		delete base;
		delete t;
	}
	void Del(string s) {
		BSTNode* temp = this->search(s);
		BSTNode* LsubMAX = NULL;
		if (temp == NULL ) {											//Do not found the target.
//			cout << "c.1";
		}
		else if (temp->Lchild == NULL && temp->Rchild == NULL) {		//The target is a leaf
//			cout << "c.2";
			if (temp!=root&&temp->parent->Lchild == temp) {
				temp->parent->Lchild = NULL;
			}
			else if (temp!=root&&temp->parent->Rchild == temp) {
				temp->parent->Rchild = NULL;
			}
			else if (temp == root) {
				root = NULL;
			}
			else cout << "ERROR";
			this->N -= 1;
		}
		else {
//			cout << "c.3";
			if (temp->Lchild == NULL) {									//The target do not have Left-subtree
				if (temp != root) {
					if (temp->parent->Rchild == temp) {
						temp->parent->Rchild = temp->Rchild;

					}
					else {
						temp->parent->Lchild = temp->Rchild;
					}
					temp->Rchild->parent = temp->parent;
				}
				else {
					temp->Rchild->parent = NULL;
					root = temp->Rchild;
				}
			}
			else if (temp->Rchild == NULL) {							//The target do not have Right-subtree
				if (temp != root) {
					if (temp->parent->Rchild == temp) {
						temp->parent->Rchild = temp->Lchild;

					}
					else {
						temp->parent->Lchild = temp->Lchild;
					}
					temp->Lchild->parent = temp->parent;
				}
				else {
					temp->Lchild->parent = NULL;
					root = temp->Lchild;
				}
			}
			else {
				while (LsubMAX->Rchild != NULL) {
					LsubMAX = LsubMAX->Rchild;
				}
				cout << LsubMAX->word;
				if (temp == root) {
					if (LsubMAX->parent->Lchild == LsubMAX) {
						LsubMAX->parent->Lchild = LsubMAX->Lchild;
					}else{
						LsubMAX->parent->Rchild = LsubMAX->Lchild;
					}
					LsubMAX->Lchild = temp->Lchild;
					LsubMAX->Rchild = temp->Rchild;
					root = LsubMAX;
				}
				else {
					if (temp->parent->Lchild == temp) {
						temp->parent->Lchild = LsubMAX;
					}
					else {
						temp->parent->Rchild = LsubMAX;
					}
					if (LsubMAX->parent->Lchild == LsubMAX) {
						LsubMAX->parent->Lchild = LsubMAX->Lchild;
					}
					else {
						LsubMAX->parent->Rchild = LsubMAX->Lchild;
					}
					LsubMAX->Lchild = temp->Lchild;
					LsubMAX->Rchild = temp->Rchild;
				}

			}
			this->N -= 1;
			
		}
		temp = LsubMAX = NULL;
		delete temp;
		delete LsubMAX;
	}
	void recurPrint(BSTNode* t) {
		if (t->Lchild != NULL) {
			recurPrint(t->Lchild);
		}
		cout << " < " << t->word << "> , ";
		if (t->Rchild != NULL) {
			recurPrint(t->Rchild);
		}
	}
	void printBST() {
		this->recurPrint(root);

		cout << "N = " << this->N << ", root is : " << root->word << endl;
	}
};
/*^^^--------Binary Search Tree----------^^^*/

/*vvv-------------Splay Tree-------------vvv*/
class SPTNode {
	friend class SplayTree;
private:
	SPTNode* parent = NULL;
	SPTNode* Lchild = NULL;
	SPTNode* Rchild = NULL;
	string word = 0;
public:
	SPTNode(string w) : parent(0), Lchild(0), Rchild(0), word(w) {};
};
class SplayTree {
private:
	SPTNode* root = NULL;
	int N = 0;
public:
	void setroot(SPTNode* t) { root = t; }
	SPTNode* getroot() { return root; }
	void rotateR(SPTNode* t) {									//x is x->parent->Lchild
		SPTNode* temp = t->parent;
		if (temp->parent != NULL) {
			if (temp->parent->Rchild == temp) {
				temp->parent->Rchild = t;
			}
			else {
				temp->parent->Lchild = t;
			}
			t->parent = temp->parent;
			temp->parent = t;
			temp->Lchild = t->Rchild;
			if (t->Rchild != NULL)t->Rchild->parent = temp;
			t->Rchild = temp;
		}
		else {
			temp->Lchild = t->Rchild;
			if (t->Rchild != NULL)t->Rchild->parent = temp;
			t->Rchild = temp;
			temp->parent = t;
			t->parent = NULL;
			root = t;
		}
		temp = NULL;
		delete temp;
	}
	void rotateL(SPTNode* t) {									//x is x->parent->Rchild
		SPTNode* temp = t->parent;
		if (temp->parent != NULL) {
			if (temp->parent->Rchild == temp) {
				temp->parent->Rchild = t;
			}
			else {
				temp->parent->Lchild = t;
			}
			t->parent = temp->parent;
			temp->parent = t;
			temp->Rchild = t->Lchild;
			if (t->Lchild != NULL)	t->Lchild->parent = temp;
			t->Lchild = temp;
		}
		else {
			temp->Rchild = t->Lchild;
			if (t->Lchild != NULL)	t->Lchild->parent = temp;
			t->Lchild = temp;
			temp->parent = t;
			t->parent = NULL;
			root = t;
		}
		temp = NULL;
		delete temp;		
	}
	void adjust(SPTNode* t) {
		while (t != root&&t->parent!=NULL) {
			if (t->parent->Lchild == t) {
				this->rotateR(t);
			}
			else if (t->parent->Rchild == t) {
				this->rotateL(t);
			}
			else {
				cout << "ERROR";
				break;
			}
		}
	}
	void insert(string s){
		SPTNode* temp = new SPTNode(s);
		SPTNode* base = root;
		SPTNode* t = NULL;
		if (root == NULL) {
			root = temp;
			this->N = 1;
		}
		else if (temp->word == root->word) {
			temp = root;
		}
		else {
			while (base != NULL) {
				if (s == base->word) {
//					cout << "Alredy exists." << endl;
					break;
				}
				else if (s < base->word) {
					t = base;
					base = base->Lchild;
				}
				else {
					t = base;
					base = base->Rchild;
				}
			}
			if (base == NULL) {
				temp->parent = t;
				if (t->word > s) {
					temp->parent->Lchild = temp;
				}
				else {
					temp->parent->Rchild = temp;
				}
				this->N += 1;
			}
		}
		this->adjust(temp);
		temp = base = t = NULL;
		delete temp;
		delete base;
		delete t;
	}
	SPTNode* search(string s){
		SPTNode* temp = root;
		if (temp == NULL) {
			cout << "The tree is empty." << endl;
		}
		else {
			while (temp->word != s && temp != NULL) {

				if (s < temp->word) {
					if (temp->Lchild != NULL)	temp = temp->Lchild;
					else break;
				}
				else if (s > temp->word) {
					if (temp->Rchild != NULL) temp = temp->Rchild;
					else break;
				}
			}
			if (temp->word == s) {
//				cout << "The word, " << s << ", is in the dictionary." << endl;
				this->adjust(temp);
			}
			else {
//				cout << "Do not found the word." << endl;
				temp = NULL;
			}
		}
		return temp;
	}
	void Del(string s){
		SPTNode* temp = this->search(s);
		SPTNode* LsubMAX = NULL;
		if (temp == NULL) {											//Do not found the target.
//			cout << "c.1";
		}
		else if (temp->Lchild == NULL && temp->Rchild == NULL) {		//trivial tree.
//			cout << "c.2";
			if (temp == root) {
				root = NULL;
			}
			else cout << "ERROR";
			this->N -= 1;
		}
		else {
//			cout << "c.3";
			if (temp->Lchild == NULL) {									//The target has no Left-subtree
				temp->Rchild->parent = NULL;
				root = temp->Rchild;
			}
			else if (temp->Rchild == NULL) {							//The target has no Right-subtree
					temp->Lchild->parent = NULL;
					root = temp->Lchild;
			}
			else {
				LsubMAX = temp->Lchild;
				while (LsubMAX->Rchild != NULL) {
					LsubMAX = LsubMAX->Rchild;
				}
				cout << LsubMAX->word;
				if (temp == root) {
					SplayTree Lsubtree;
					Lsubtree.setroot(root->Lchild);
					Lsubtree.adjust(LsubMAX);
					this->root = LsubMAX;
					LsubMAX->Rchild = temp->Rchild;
					temp->Rchild->parent = root;
				}

			}
			this->N -= 1;
		}
		temp = LsubMAX = NULL;
		delete temp;
		delete LsubMAX;
	}
	void recurPrint(SPTNode* t) {
		if (t->Lchild != NULL) {
			recurPrint(t->Lchild);
		}
		cout << " < " << t->word << "> , ";
		if (t->Rchild != NULL) {
			recurPrint(t->Rchild);
		}
	}
	void printSPT() {
		this->recurPrint(root);
		cout << "N = " << this->N <<", root is : "<<root->word << endl;
	}
};
/*^^^-------------Splay Tree-------------^^^*/

/*vvv---------------Treap----------------vvv*/
class TreapNode {
	friend class Treap;
private:
	TreapNode* parent = NULL;
	TreapNode* Lchild = NULL;
	TreapNode* Rchild = NULL;
	int priority = 0;
	string word = 0;
public:
	TreapNode(string w) : priority(), parent(0), Lchild(0), Rchild(0), word(w) {};
};
class Treap {
private:
	TreapNode* root = NULL;
	int N = 0;
public:
	TreapNode* search(string s){
		TreapNode* temp = root;
		if (temp == NULL) {
			cout << "The tree is empty." << endl;
		}
		else {
			while (temp->word != s && temp != NULL) {

				if (s < temp->word) {
					if (temp->Lchild != NULL)	temp = temp->Lchild;
					else break;
				}
				else if (s > temp->word) {
					if (temp->Rchild != NULL) temp = temp->Rchild;
					else break;
				}
			}
			if (temp->word == s) {
//				cout << "The word, " << s << ", is in the dictionary." << endl;
			}
			else {
//				cout << "Do not found the word." << endl;
				temp = NULL;
			}
		}
		return temp;
	}
	void rotateR(TreapNode* t) {									//x is x->parent->Lchild
		TreapNode* temp = t->parent;
		if (temp->parent != NULL) {
			if (temp->parent->Rchild == temp) {
				temp->parent->Rchild = t;
			}
			else {
				temp->parent->Lchild = t;
			}
			t->parent = temp->parent;
			temp->parent = t;
			temp->Lchild = t->Rchild;
			if (t->Rchild != NULL)t->Rchild->parent = temp;
			t->Rchild = temp;
		}
		else {
			temp->Lchild = t->Rchild;
			if (t->Rchild != NULL)t->Rchild->parent = temp;
			t->Rchild = temp;
			temp->parent = t;
			t->parent = NULL;
			root = t;
		}
		temp = NULL;
		delete temp;
		}
	void rotateL(TreapNode* t) {									//x is x->parent->Rchild
		TreapNode* temp = t->parent;
		if (temp->parent != NULL) {
			if (temp->parent->Rchild == temp) {
				temp->parent->Rchild = t;
			}
			else {
				temp->parent->Lchild = t;
			}
			t->parent = temp->parent;
			temp->parent = t;
			temp->Rchild = t->Lchild;
			if (t->Lchild != NULL)	t->Lchild->parent = temp;
			t->Lchild = temp;
		}
		else {
			temp->Rchild = t->Lchild;
			if (t->Lchild != NULL)	t->Lchild->parent = temp;
			t->Lchild = temp;
			temp->parent = t;
			t->parent = NULL;
			root = t;
		}
		temp = NULL;
		delete temp;
	}
	void adjust(TreapNode* t){
		if (t->parent != NULL) {
			while (t->priority <= t->parent->priority) {
				if (t->priority == t->parent->priority) {
					t->priority -= 1;					//�Ypriority�ȭ��ơA�ק惡NODE��priority��
				}
				if(t->parent->Lchild==t){
					this->rotateR(t);
				}
				else if (t->parent->Rchild == t) {
					this->rotateL(t);
				}
				else {
					cout << "ERROR";
					break;
				}
				if (t->parent == NULL) break;
			}
		}
	}
	void insert(int p, string s){
		TreapNode* temp = new TreapNode(s);
		TreapNode* base = root;
		TreapNode* t = NULL;
		temp->priority = p;
		if (root == NULL) {
			root = temp;
			this->N = 1;
		}
		else {
			while (base != NULL) {
				if (s == base->word) {
//					cout << "Alredy exists." << endl;
					break;
				}
				else if (s < base->word) {
					t = base;
					base = base->Lchild;
				}
				else {
					t = base;
					base = base->Rchild;
				}
			}
			if (base == NULL) {
				temp->parent = t;
				if (t->word > s) {
					temp->parent->Lchild = temp;
				}
				else {
					temp->parent->Rchild = temp;
				}
				this->N += 1;
				this->adjust(temp);
			}
		}
		temp = base = t = NULL;
		delete temp;
		delete base;
		delete t;
	}
	void Del(string s){
		TreapNode* temp = this->search(s); 
		if (temp != NULL) {
			temp->priority = 5005;
			while (temp->Lchild != NULL && temp->Rchild != NULL) {
				if (temp->Lchild->priority < temp->Rchild->priority) {
					this->rotateR(temp->Lchild);
				}
				else {
					this->rotateL(temp->Rchild);
				}
			}
			if (temp->Lchild == NULL && temp->Rchild != NULL) {
				temp->Rchild->parent = temp->parent;
				if (temp->parent != NULL) {
					if (temp->parent->Lchild == temp) {
						temp->parent->Lchild = temp->Rchild;
					}
					else {
						temp->parent->Rchild = temp->Rchild;
					}
				}
				else {
					root = temp->Rchild;
				}
			}
			else if (temp->Rchild == NULL && temp->Lchild != NULL) {
				temp->Lchild->parent = temp->parent;
				if (temp->parent != NULL) {
					if (temp->parent->Lchild == temp) {
						temp->parent->Lchild = temp->Lchild;
					}
					else {
						temp->parent->Rchild = temp->Lchild;
					}
				}
				else {
					root = temp->Lchild;
				}
			}
			else {
				if (temp == root) {
					root = NULL;
				}
				else {
					if (temp->parent->Lchild == temp) {
						temp->parent->Lchild = NULL;
					}
					else {
						temp->parent->Rchild = NULL;
					}
				}
			}
			N -= 1;
		}
		temp = NULL;
		delete temp;
	}
	void recurPrint(TreapNode* t) {
		if (t->Lchild != NULL) {
			recurPrint(t->Lchild);
		}
		cout << " < " << t->word << "> , ";
		if (t->Rchild != NULL) {
			recurPrint(t->Rchild);
		}
	}
	void printTreap() {
		this->recurPrint(root);
		cout << "N = " << this->N << ", root is : " << root->word << endl;
	}
};
/*^^^---------------Treap----------------^^^*/

int main() {
	BST BSTDict;
	SplayTree SPTDict;
	Treap TreapDict;
	ifstream Dict, Q1, Q2, Q3;
	ofstream out;
	float Timesection = 0;
	clock_t starttime, endtime;							//�إ�clock_t�ܼ�(����ƧκA)�H�O���ɶ��A�H�@�����
	default_random_engine generator;
	uniform_int_distribution<int> distribution(1, 5000);
	string token[5000];											//Dictionary �إ߫eŪ���ɮצr��Ȧs�󦹰}�C
	string file1token[5000];									//Ū���n�d�ߪ��ɮ� 1�r��Ȧs�󦹰}�C
	string file2token[5000];									//Ū���n�d�ߪ��ɮ� 2�r��Ȧs�󦹰}�C
	string file3token[5000];									//Ū���n�d�ߪ��ɮ� 3�r��Ȧs�󦹰}�C
	int nfile = 0, n1 = 0, n2 = 0, n3 = 0;										//Ū���r��ƶq
	string str;

/*vvvvvvvvvvvvvvv-------------TestFile-------------vvvvvvvvvvvvvvv*/
	/*vvvvvvvvvv�ɮ�I/Ovvvvvvvvvvv*/
	Dict.open("TestFile1.txt");							//�r���ɮ�
	Q1.open("TestFile11.txt");							//�d���ɮ�1
	Q2.open("TestFile12.txt");							//�d���ɮ�2
	Q3.open("TestFile13.txt");							//�d���ɮ�3
	if (Dict.fail() || Q1.fail() || Q2.fail() || Q3.fail()) {
		cout << "input file opening failed";
		exit(1);
	}
	out.open("Result.txt");
	if (out.fail()) {
		cout << "output file opening failed";
		exit(1);
	}


	nfile = 0;
	while (getline(Dict, str))
	{
		stringstream temp(str);						//�NŪ�J������Ʃ�btemp
		while (getline(temp, token[nfile], ' '))	//�N�Ӧ��Ƥ��Ψ�token��
		{
			transform(token[nfile].begin(), token[nfile].end(), token[nfile].begin(), tolower);
			//vvv�R��token�������I�Ÿ�
			token[nfile].erase(remove_if(token[nfile].begin(), token[nfile].end(), static_cast<int(*)(int)>(&ispunct)), token[nfile].end());
			nfile++;
		}
	}
	n1 = 0;
	while (getline(Q1, str))
	{
		stringstream temp(str);						//�NŪ�J������Ʃ�btemp
		while (getline(temp, file1token[n1], ' '))	//�N�Ӧ��Ƥ��Ψ�token��
		{
			transform(file1token[n1].begin(), file1token[n1].end(), file1token[n1].begin(), tolower);
			//vvv�R��token�������I�Ÿ�
			file1token[n1].erase(remove_if(file1token[n1].begin(), file1token[n1].end(), static_cast<int(*)(int)>(&ispunct)), file1token[n1].end());
			n1++;
		}
	}
	n2 = 0;
	while (getline(Q2, str))
	{
		stringstream temp(str);						//�NŪ�J������Ʃ�btemp
		while (getline(temp, file2token[n2], ' '))	//�N�Ӧ��Ƥ��Ψ�token��
		{
			transform(file2token[n2].begin(), file2token[n2].end(), file2token[n2].begin(), tolower);
			//vvv�R��token�������I�Ÿ�
			file2token[n2].erase(remove_if(file2token[n2].begin(), file2token[n2].end(), static_cast<int(*)(int)>(&ispunct)), file2token[n2].end());
			n2++;
		}
	}
	n3 = 0;
	while (getline(Q3, str))
	{
		stringstream temp(str);						//�NŪ�J������Ʃ�btemp
		while (getline(temp, file3token[n3], ' '))	//�N�Ӧ��Ƥ��Ψ�token��
		{
			transform(file3token[n3].begin(), file3token[n3].end(), file3token[n3].begin(), tolower);
			//vvv�R��token�������I�Ÿ�
			file3token[n3].erase(remove_if(file3token[n3].begin(), file3token[n3].end(), static_cast<int(*)(int)>(&ispunct)), file3token[n3].end());
			n3++;
		}
	}
	/*^^^^^^^^^^�ɮ�I/O^^^^^^^^^^*/
	
	//Build Binary Search Tree
	starttime = clock();
	for (int i = 0; i < nfile; i++)
	{	
		BSTDict.insert(token[i]);						
	}
	endtime = clock();
	Timesection = endtime - starttime;
	//	BSTDict.printBST();
	out << "1. Binary Search Tree �إߩҪ�ɶ��G" << Timesection << endl;

	//Build Splay Tree
	starttime = clock(); 
	for (int i = 0; i < nfile; i++) {
		SPTDict.insert(token[i]);
	}
	endtime = clock();
	Timesection = endtime - starttime;
	//	SPTDict.printSPT();
	out << "2. Splay Tree �إߩҪ�ɶ��G" << Timesection << endl;

	//Build Treap
	int x = 0;
	int* x1 = new int[nfile];
	for (int i = 0; i < nfile; i++) {
		x1[i] = distribution(generator);
	}
	starttime = clock();
	for (int i = 0; i < nfile; i++) {
		TreapDict.insert(x1[i], token[i]);
	}
	endtime = clock();
	Timesection = endtime - starttime;
	//TreapDict.printTreap();
	out << "3. Treap �إߩҪ�ɶ��G" << Timesection << endl;


	
	int sucess = 0;
	
	out << endl << "���浲�G" << endl;

	out << "1. Binary Search Tree " << endl;
	starttime = clock();
	for (int i = 0; i < n1; i++) {
		if (BSTDict.search(file1token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;
	out << "    a. ���\�v�G" << sucess << " / " << n1 << endl;
	out << "    b. �`��O�ɶ��G" << Timesection << endl;

	out << "2. Splay Tree " << endl;
	sucess = 0;
	starttime = clock();
	for (int i = 0; i < n1; i++) {
		if (SPTDict.search(file1token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;

	out << "    a. ���\�v�G" << sucess << " / " << n1 << endl;
	out << "    b. �`��O�ɶ��G" << Timesection << endl;

	out << "3. Treap " << endl;
	sucess = 0;
	starttime = clock();
	for (int i = 0; i < n1; i++) {
		if (TreapDict.search(file1token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;

	out << "    a. ���\�v�G" << sucess << " / " << n1 << endl;
	out << "    b. �`��O�ɶ��G" << Timesection << endl;

	out << endl << "File 2 ���G" << endl;
	sucess = 0;
	out << "1. Binary Search Tree " << endl;
	starttime = clock();
	for (int i = 0; i < n2; i++) {
		if (BSTDict.search(file2token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;
	out << "    a. ���\�v�G" << sucess << " / " << n2 << endl;
	out << "    b. �`��O�ɶ��G" << Timesection << endl;

	out << "2. Splay Tree " << endl;
	sucess = 0;
	starttime = clock();
	for (int i = 0; i < n2; i++) {
		if (SPTDict.search(file2token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;

	out << "    a. ���\�v�G" << sucess << " / " << n2 << endl;
	out << "    b. �`��O�ɶ��G" << Timesection << endl;

	out << "3. Treap " << endl;
	sucess = 0;
	starttime = clock();
	for (int i = 0; i < n2; i++) {
		if (TreapDict.search(file2token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;

	out << "    a. ���\�v�G" << sucess << " / " << n2 << endl;	
	out << "    b. �`��O�ɶ��G" << Timesection << endl;

	out << endl << "File 3 ���G" << endl;
	sucess = 0;
	out << "1. Binary Search Tree " << endl;
	starttime = clock();
	for (int i = 0; i < n3; i++) {
		if (BSTDict.search(file3token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;
	out << "    a. ���\�v�G" << sucess << " / " << n3 << endl;
	out << "    b. �`��O�ɶ��G" << Timesection << endl;

	out << "2. Splay Tree " << endl;
	sucess = 0;
	starttime = clock();
	for (int i = 0; i < n3; i++) {
		if (SPTDict.search(file3token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;

	out << "    a. ���\�v�G" << sucess << " / " << n3 << endl;
	out << "    b. �`��O�ɶ��G" << Timesection << endl;

	out << "3. Treap " << endl;
	sucess = 0;
	starttime = clock();
	for (int i = 0; i < n3; i++) {
		if (TreapDict.search(file3token[i]) != NULL) {
			sucess++;
		}
	}
	endtime = clock();
	Timesection = endtime - starttime;

	out << "    a. ���\�v�G" << sucess << " / " << n3 << endl;	
	out << "    b. �`��O�ɶ��G" << Timesection << endl;
	
	Dict.close();
	Q1.close();
	Q2.close();
	Q3.close();
	out.close();
/*^^^^^^^^^^^^^^^-------------TestFile-------------^^^^^^^^^^^^^^^*/
	int f = 0, f1 = 0;
	string w;
	cout << "(TestFile�����G�b����'Result.txt'��)" << endl;
	do {
		cout << "1. Binary Search Tree" << endl;
		cout << "2. Splay Tree" << endl;
		cout << "3. Treap" << endl;
		cout << "4. ����" << endl;
		cin >> f;
		switch (f)
		{
		case 1: {
			do {
				cout << "1. Searching a word" << endl;
				cout << "2. Adding a word" << endl;
				cout << "3. Deleting a word" << endl;
				cout << "4. �L�X�r��(Inorder)" << endl;
				cout << "5. ��^(��ܸ���x�s���c)" << endl;
				cin >> f1;
				switch (f1)
				{
				case 1: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (BSTDict.search(w) == NULL) {
						cout << endl << "�d�ߦr�G" << w << " ���b�r�夤�C" << endl;
					}
					else {
						cout << endl << "�d�ߦr�G" << w << "�b�r�夤�C" << endl;
					}
					continue;
				}
				case 2: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (BSTDict.search(w) == NULL) {
						BSTDict.insert(w);
						cout << endl << "�N�s�W�r�G" << w << "��r�夤�C" << endl;
					}
					else {
						cout << endl << "�s�W�r�G" << w << "�w�g�b�r�夤�C" << endl;
					}
					continue;
				}case 3: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (BSTDict.search(w) == NULL) {
						cout << endl << "���R���r�G" << w << "���b�r�夤�C" << endl;
					}
					else {
						BSTDict.Del(w);
						cout << endl << "���R���r�G" << w << "�q�r�夤�R���C" << endl;
					}
					continue;
				}
				case 4: {
					cout << "�L�X�r��(BST)" << endl;
					BSTDict.printBST();
				}case 5: {
					break;
				}
				default:
					cout << "��J���~�A�Э��s��J�C" << endl;
					continue;
				}
			}while (f1 != 5); continue;
		}
		case 2: {
			
			do {
				cout << "1. Searching a word" << endl;
				cout << "2. Adding a word" << endl;
				cout << "3. Deleting a word" << endl;
				cout << "4. �L�X�r��(Inorder)" << endl;
				cout << "5. ��^(��ܸ���x�s���c)" << endl;
				cin >> f1;
				switch (f1)
				{
				case 1: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (SPTDict.search(w) == NULL) {
						cout << endl << "�d�ߦr�G" << w << " ���b�r�夤�C" << endl;
					}
					else {
						cout << endl << "�d�ߦr�G" << w << "�b�r�夤�C" << endl;
					}continue;
				}
				case 2: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (SPTDict.search(w) == NULL) {
						SPTDict.insert(w);
						cout << endl << "�N�s�W�r�G" << w << "��r�夤�C" << endl;
					}
					else {
						cout << endl << "�s�W�r�G" << w << "�w�g�b�r�夤�C" << endl;
					}
					continue;
				}case 3: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (SPTDict.search(w) == NULL) {
						cout << endl << "���R���r�G" << w << "���b�r�夤�C" << endl;
					}
					else {
						SPTDict.Del(w);
						cout << endl << "���R���r�G" << w << "�q�r�夤�R���C" << endl;
					}
					continue;
				}
				case 4: {
					cout << "�L�X�r��(SPT)" << endl;
					SPTDict.printSPT();
				}case 5: {
					break;
				}
				default:
					cout << "��J���~�A�Э��s��J�C" << endl; continue;
				}
			} while (f1 != 5);
				continue;
		}case 3: {
			
			do {
				cout << "1. Searching a word" << endl;
				cout << "2. Adding a word" << endl;
				cout << "3. Deleting a word" << endl;
				cout << "4. �L�X�r��(Inorder)" << endl;
				cout << "5. ��^(��ܸ���x�s���c)" << endl;
				cin >> f1;
				switch (f1)
				{
				case 1: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (TreapDict.search(w) == NULL) {
						cout << endl << "�d�ߦr�G" << w << " ���b�r�夤�C" << endl;
					}
					else {
						cout << endl << "�d�ߦr�G" << w << "�b�r�夤�C" << endl;
					}
					continue;
				}
				case 2: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (TreapDict.search(w) == NULL) {
						x = distribution(generator);
						TreapDict.insert(x, w);
						cout << endl << "�N�s�W�r�G" << w << "��r�夤�C" << endl;
					}
					else {
						cout << endl << "�s�W�r�G" << w << "�w�g�b�r�夤�C" << endl;
					}
					continue;
				}case 3: {
					cout << "��J�r��(�^��)" << endl;
					cin >> w;
					if (TreapDict.search(w) == NULL) {
						cout << endl << "���R���r�G" << w << "���b�r�夤�C" << endl;
					}
					else {
						TreapDict.Del(w);
						cout << endl << "���R���r�G" << w << "�q�r�夤�R���C" << endl;
					}
					continue;
				}
				case 4: {
					cout << "�L�X�r��(Treap)" << endl;
					TreapDict.printTreap();
				}case 5: {
					break;
				}
				default:
					cout << "��J���~�A�Э��s��J�C" << endl; continue;
				}
			} while (f1!=5);
			continue;
		}case 4: {
			cout << "�����C" << endl;
			break;
		}

		default:
			cout << "��J���c�s���G";
			continue;
		}
	}while (f!=4);

	return 0;
}