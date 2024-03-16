#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class Node {
	friend class HuffmanTree;
private:
	int count = 1;							//����X�X�{����
	int color = -1;							//��X�A�Y�������`�I�O��-1
	vector<int> code;						//�s�X����
	bool Ccheak = false;					//�s�X�O�_����(�s�X������code���s�X���˧ǡA�ݽվ�)
	Node* Rchild = NULL;		
	Node* Lchild = NULL;
	Node* parent = NULL;
public:
	Node(int rgb,int c) :count(c), color(rgb) {};	//��X�Ψ䦸��
	Node(int c) :count(c), color(-1) {};			//�����`�I��count��
	int GetColor() {
		return color;
	}
	vector<int> Code() {							//����s�X����
		if (!Ccheak) {
			vector<int> temp;
			int n = code.size();
			for (int i = 0; i < n; i++) {
				temp.push_back(code[code.size() - 1]);
				code.pop_back();
			}
			code = temp;
			Ccheak = true;
		}
		return code;
	}
};
class HuffmanTree {
private:
	Node* root = NULL;
	vector<Node*> Index;							//�B�zHuffmanTree�إ߹L�{���}�C
	vector<Node*> Datas;							//�s��ƧǸ�Ƹ`�I
public:
	void Build(int array[]) {						//Ū�J��ƨëإ߸`�I
		for (int i = 0; i < 256; i++) {
			Node* temp = new Node(i, array[i]);
			Datas.push_back(temp);
		}
		sort(Datas);								//sorting(�p��j)
		coding();									//�s�X
		/*for (int i = 0; i < Datas.size(); i++) {
			cout << "< " << Datas[i]->color << ". " << Datas[i]->count <<", ";
			for (auto& j : Datas[i]->Code()) {
				cout << j ;
			}
			cout<< " > ";
		}
		
		
		cout << endl << "Index :" << endl;
		for (auto& i : Index) {
			cout << "< " << i->color << ". " << i->count << "> ";
		}*/
	}
	//sorting(�p��j)
	void sort(vector<Node*> &Data) {				
		int min = Data[0]->count;
		int temp = 0;
		Node* t = NULL;
		for (int i = 0; i < Data.size();i++) {
			min = Data[i]->count;
			temp = i;
			for (int j = i; j < Data.size(); j++) {
				if (Data[j]->count < min) {
					min = Data[j]->count;
					temp = j;
				}
			}
			if (temp != i) {
				t = Data[i];
				Data[i]= Data[temp];
				Data[temp]=t;
				t = NULL;
			}
		}
	}
	//�s�X
	void coding(){
		int i = 0;
		Node* temp = NULL;
		//Data��ƥ�Ū����Index�����u�@�Ӹ`�I(�YData��Ū���BIndex�Ѥ@�ӧY��root)
		while (i < 256||Index.size()>1) {						
			if (i < 256)								//Data��ƥ�Ū���A��̤p�[�JIndex
			{	
				Index.push_back(Datas[i]);
				i++;
				if (i < 256)
				{
					Index.push_back(Datas[i]);
					i++;
				}
				sort(Index);						
			}
			temp = new Node(Index[0]->count + Index[1]->count);	//�إ߷s�����`�I
			temp->Lchild = Index[0];							//�Ϧ����䥪�k�l��
			temp->Rchild = Index[1];
			this->pushCode(Index[0], 0);						//���l��[�J�s�X0
			this->pushCode(Index[1], 1);						//���l��[�J�s�X1
			Index[0]->parent = Index[1]->parent = temp;
			Index[0]=temp;										//�s�`�I�[�J�}�C
			for (int i = 1; i < Index.size()-1; i++) {			//�վ�}�C
				Index[i] = Index[i + 1];
			}
			Index.pop_back();
			sort(Index);
			temp = NULL;
		}
	}
	void pushCode(Node* n, int c) {								//�N�l�𤤩Ҧ���Ƹ`�I���[�J�s�X
		if (n->Lchild != NULL) {
			pushCode(n->Lchild, c);
		}
		if (n->Rchild!=NULL) {
			pushCode(n->Rchild, c);
		}
		if (n->color>=0) {
			n->code.push_back(c);
		}
	}

	Node* search(int item) {				//�Ω�s�X���ഫ
		Node* temp = NULL;
		for (auto& i : Datas) {
			if (item == i->color) {
				temp = i;
				break;
			}
		}
		return temp;
	}
	Node* GetCode(int i) {
		return Datas[i];
	}
};
class DecodeNode {
	friend class DecodeTable;
private:
	int color;
	string code;
public:
	DecodeNode(int c, string C) :color(c), code(C) {};
};
class DecodeTable {
private:
	vector<DecodeNode*> Table;
	bool Re = false;
public:
	void construct(int c, string s) {				//�إ߸ѽX���������
		DecodeNode* temp = new DecodeNode(c, s);
		Table.push_back(temp);
		temp = NULL;
		delete temp;
	}
	void Reverse() {
		if (!Re) {
			vector<DecodeNode*> temp;				//�]��Ū���ƧǬ�����u�A���u�XŪ���W�v�����A�]���N��Ƥϧ�
			int t = Table.size();
			for (int i = 0; i < t; i++) {
				temp.push_back(Table[Table.size() - 1]);
				Table.pop_back();
			}
			Table = temp;
			Re = true;
		}
	}
	int decoding(string s) {
		int temp = -1;
		for (auto& i : Table) {
			if (i->code == s) {
				temp = i->color;
				break;
			}
		}
		return temp;
	}
};
int main() {

	ifstream in,key;	
	ofstream out, decode;	
	HuffmanTree HTree;
	int width = 0, height = 0, RGB = 0;			//�����ɮפ���Ƥj�p����T
	string title[2];							//�s���ɮ׶}�Y�ɮצW�H�γn��[�� 
	string s ;									//�ɮצW�H�ܼ��x�s�A��KI/O
	int request = 0;
	do {
		cout << "Number of picture: " << endl;	//²������ഫ�ɮ�
		cout << "1. p1" << endl;
		cout << "2. p2" << endl;
		cout << "3. p3" << endl;
		cout << "4. terminate." << endl;
		cin >> request;
		switch (request)
		{
		case 1: {
			s = "p1";
			break;
		}
		case 2: {
			s = "p2";
			break;
		}
		case 3: {
			s = "p3";
			break;
		}
		case 4: {
			s = "#";
			break;
		}
		default:
			break;
		}
		if (request < 4 && request > 0) {
			in.open(s + ".pgm");				//�}���ɮ�
			out.open(s + "encode.hc");			//�s�X���X���ɮ�
			decode.open(s + "key.txt");			//CodeTable
			if (!in)
			{
				cout << "error(f)" << endl;
			}

			for (int i = 0; i < 2; i++)
				getline(in, title[i]);				 
			in >> width >> height >> RGB;			//Ū����T
			int histogram[256] = { 0 };				//�x�s�U��X�X�{����


			vector<int> Dataseq;					//�x�s�Ϥ���l���e
			int t = -1;
			while (in >> t) {
				Dataseq.push_back(t);
				histogram[t] += 1;
			}
			for (int i = 0; i < 256; i++)
			{
				cout << "Number of Color " << i << " : " << histogram[i] << endl;
			}

			HTree.Build(histogram);							//�إ�HuffmanTree
			Node* tNode = NULL;
			for (int i = 0; i < 256; i++) {					//��XCodeTable
				tNode = HTree.GetCode(i);
				decode << tNode->GetColor() << " ";
				for (auto& j : tNode->Code()) {
					decode << j;
				}
				decode << endl;
			}
			for (int i = 0; i < 2; i++) {					//��X�s�X�ᤧ���
				out << title[i] << endl;
			}
			out << width << " " << height << endl;
			out << RGB << endl;
			vector<int> c;
			for (auto& i : Dataseq) {
				c = HTree.search(i)->Code();
				for (auto& C : c) {
					out << C;
				}
				out << " ";
			}
			in.close();
			out.close();
			decode.close();

			//Decode

			in.open(s + "encode.hc");
			if (!in)
			{
				cout << "error(f)" << endl;
			}
			for (int i = 0; i < 2; i++)				//Ū���ɮ׸�T
				getline(in, title[i]);		 
			in >> width >> height >> RGB;
			key.open(s + "key.txt");				//�}�ҸѽX������Table
			out.open("D" + s + ".pgm");				//��X�ѽX���ɮ�
			if (!key || !out)
			{
				cout << "error(key)" << endl;
			}
			for (int i = 0; i < 2; i++) {
				out << title[i] << endl;
			}
			out << width << " " << height << endl;		//���X�ɼg�J�ɮ׸�T
			out << RGB << endl;
			vector<string> CodeDataseq;
			DecodeTable KEYdict;
			string tempS, k;
			int Keyindex = 0, Keytemp;
			while (in >> tempS) {
				CodeDataseq.push_back(tempS);
			}
			while (key >> Keyindex >> k) {
				KEYdict.construct(Keyindex, k);
			}
			KEYdict.Reverse();
			for (auto& i : CodeDataseq) {
				Keytemp = KEYdict.decoding(i);
				if (Keytemp < 0) {
					cout << "ERROR";
					break;
				}
				else {
					out << Keytemp << " ";
				}
			}
			in.close();
			out.close();
			key.close();
		}
		else
		{
			continue;
		}
	} while (s != "#");
	
	
	return 0;
}