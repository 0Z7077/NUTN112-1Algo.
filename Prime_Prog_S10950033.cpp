#include <iostream>
#include <fstream>  //檔案開啟關閉
#include <cstdlib>
#include <cmath>
#include <random> /* 亂數相關函數 */
#include <ctime> /* 時間相關函數 */
using namespace std;
typedef long long ll;
bool Basic_Prime_Testing(ll n);
bool Sieve_of_Eratosthenes(ll n);
bool Fermat_Primality_Testing(ll n,int k);
int main()
{
    clock_t start, end;							//建立clock_t變數(長整數形態)以記錄時間，以毫秒為單位
    ifstream in;
    ofstream out;
    ll n = 0;
    int count = 0, k = 100;
    float p = 0, correct = 0, t1 = 0, t2 = 0, t3 = 0;
    bool BPT, SoE, FPT;
    out.open("output.txt");
    if (out.fail()) {
        cout << "output file opening failed";
        exit(1);
    }
    in.open("input.txt");
    if (in.fail()) {
        cout << "input file opening failed";
        exit(1);                                    // 程式錯誤終止
    }
    while (in >> n) {
        start = clock();	                        //紀錄1.開始時間
        BPT = Basic_Prime_Testing(n);
        end = clock();                              //紀錄1. 結束時間
        t1 += (end - start);
        start = clock();	                        //紀錄2.開始時間
        SoE = Sieve_of_Eratosthenes(n);
        end = clock();                              //紀錄2. 結束時間
        t2 += (end - start);
        start = clock();	                        //紀錄3.開始時間
        FPT = Fermat_Primality_Testing(n, k);
        end = clock();                              //紀錄3. 結束時間
        t3 += (end - start);
        if (SoE==FPT) correct++;
        count++;
        out << "n = " << n << endl;
        out << "1. Basic Prime Testing : ";
        if (BPT) {
            out << "n is prime." << endl;
            out << "2. Sieve of Eratosthenes : ";
            out << "n is prime." << endl;
        }
        else {
            out << "n is not prime." << endl;
            out << "2. Sieve of Eratosthenes : ";
            out << "n is not prime." << endl;
        }
        out << "3. Fermat Primality Testing : ";
        if (FPT) {
            out << "n might be prime." << endl;
        }
        else {
            out << "n is not prime." << endl;
        }
    }
    p = correct / count;
    in.close();
    out.close();
    return 0;
}
bool Basic_Prime_Testing(ll n) {
    if (n < 4) return true;
    for (ll i = 2; i < (sqrt(n)+1); i++) {
        if ((n % i) == 0) {
            return false;
        }
    }
    return true;
}

bool Sieve_of_Eratosthenes(ll n) {
    if (n < 4) return true;
    ll* NumList = new ll[ll(sqrt(n))+1]();
    ll multi=0;
    for (ll i = 2; i <= sqrt(n); i++) {
        if (NumList[i] == 0) {                          //未被標記的數
            if ((n % i) == 0) {                         //找到因數
                delete [] NumList;
                return false;
            }
            multi = i * 2;                              //標記i的所有倍數
            while (multi < sqrt(n)) {
                NumList[multi] = 1;
                multi += i;
            }
        }
    }
    delete[] NumList;
    return true;
}

bool Fermat_Primality_Testing(ll n, int k) {
    ll x = 0, r = 1, e = n-1;
    default_random_engine generator;
    uniform_int_distribution<int> distribution(2, n);
    while (k > 0) {
        x = distribution(generator);                                //隨機取數x
        while (e > 0) {
            if (e % 2 == 1) {                                       //轉換成2進制加速運算
                r = (r * x) % n;                                    //前一個指數次(b)*之前的餘數項
            }
            x = (x * x) % n;
            e /= 2;                                                 //2的指數次執行，降低時間
        }
        if (r!=1) {                                                 //費馬小定理不成立，n為合成數
            return false;
        }
        k--;
    }    
    return true;
}