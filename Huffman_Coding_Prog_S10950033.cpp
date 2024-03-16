#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class Node {
	friend class HuffmanTree;
private:
	int count = 1;							//此色碼出現次數
	int color = -1;							//色碼，若為內部節點令為-1
	vector<int> code;						//編碼紀錄
	bool Ccheak = false;					//編碼是否完成(編碼結束時code為編碼之倒序，需調整)
	Node* Rchild = NULL;		
	Node* Lchild = NULL;
	Node* parent = NULL;
public:
	Node(int rgb,int c) :count(c), color(rgb) {};	//色碼及其次數
	Node(int c) :count(c), color(-1) {};			//內部節點之count值
	int GetColor() {
		return color;
	}
	vector<int> Code() {							//反轉編碼次序
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
	vector<Node*> Index;							//處理HuffmanTree建立過程之陣列
	vector<Node*> Datas;							//存放排序資料節點
public:
	void Build(int array[]) {						//讀入資料並建立節點
		for (int i = 0; i < 256; i++) {
			Node* temp = new Node(i, array[i]);
			Datas.push_back(temp);
		}
		sort(Datas);								//sorting(小到大)
		coding();									//編碼
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
	//sorting(小到大)
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
	//編碼
	void coding(){
		int i = 0;
		Node* temp = NULL;
		//Data資料未讀完或Index中不只一個節點(若Data都讀完且Index剩一個即為root)
		while (i < 256||Index.size()>1) {						
			if (i < 256)								//Data資料未讀完，兩最小加入Index
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
			temp = new Node(Index[0]->count + Index[1]->count);	//建立新內部節點
			temp->Lchild = Index[0];							//使成為其左右子樹
			temp->Rchild = Index[1];
			this->pushCode(Index[0], 0);						//左子樹加入編碼0
			this->pushCode(Index[1], 1);						//左子樹加入編碼1
			Index[0]->parent = Index[1]->parent = temp;
			Index[0]=temp;										//新節點加入陣列
			for (int i = 1; i < Index.size()-1; i++) {			//調整陣列
				Index[i] = Index[i + 1];
			}
			Index.pop_back();
			sort(Index);
			temp = NULL;
		}
	}
	void pushCode(Node* n, int c) {								//將子樹中所有資料節點都加入編碼
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

	Node* search(int item) {				//用於編碼時轉換
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
	void construct(int c, string s) {				//建立解碼之對應資料
		DecodeNode* temp = new DecodeNode(c, s);
		Table.push_back(temp);
		temp = NULL;
		delete temp;
	}
	void Reverse() {
		if (!Re) {
			vector<DecodeNode*> temp;				//因原讀取排序為長到短，但短碼讀取頻率較高，因此將資料反序
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
	int width = 0, height = 0, RGB = 0;			//紀錄檔案中資料大小等資訊
	string title[2];							//存取檔案開頭檔案名以及軟體加註 
	string s ;									//檔案名以變數儲存，方便I/O
	int request = 0;
	do {
		cout << "Number of picture: " << endl;	//簡易選擇轉換檔案
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
			in.open(s + ".pgm");				//開啟檔案
			out.open(s + "encode.hc");			//編碼後輸出之檔案
			decode.open(s + "key.txt");			//CodeTable
			if (!in)
			{
				cout << "error(f)" << endl;
			}

			for (int i = 0; i < 2; i++)
				getline(in, title[i]);				 
			in >> width >> height >> RGB;			//讀取資訊
			int histogram[256] = { 0 };				//儲存各色碼出現次數


			vector<int> Dataseq;					//儲存圖片原始內容
			int t = -1;
			while (in >> t) {
				Dataseq.push_back(t);
				histogram[t] += 1;
			}
			for (int i = 0; i < 256; i++)
			{
				cout << "Number of Color " << i << " : " << histogram[i] << endl;
			}

			HTree.Build(histogram);							//建立HuffmanTree
			Node* tNode = NULL;
			for (int i = 0; i < 256; i++) {					//輸出CodeTable
				tNode = HTree.GetCode(i);
				decode << tNode->GetColor() << " ";
				for (auto& j : tNode->Code()) {
					decode << j;
				}
				decode << endl;
			}
			for (int i = 0; i < 2; i++) {					//輸出編碼後之資料
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
			for (int i = 0; i < 2; i++)				//讀取檔案資訊
				getline(in, title[i]);		 
			in >> width >> height >> RGB;
			key.open(s + "key.txt");				//開啟解碼之對應Table
			out.open("D" + s + ".pgm");				//輸出解碼後檔案
			if (!key || !out)
			{
				cout << "error(key)" << endl;
			}
			for (int i = 0; i < 2; i++) {
				out << title[i] << endl;
			}
			out << width << " " << height << endl;		//對輸出檔寫入檔案資訊
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