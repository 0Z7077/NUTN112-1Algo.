#include <iostream>
#include <fstream>
#include<cmath>
#include<vector>
using namespace std;

class Point {
private:
    int x = 0;
    int y = 0;
    bool mark = false;                          //標記這個點有沒有在Convex-Hull邊界上
    int num = -1;                               //點編號
    int in1 = 0, in2 = 0;                       //若未在邊界，要加入哪兩點之間(#3)
    bool covery = false;                        //是否已在範圍內(#3)
public:
    void SetIn(int v1, int v2) { in1 = v1; in2 = v2; }
    void SetPoint(int a, int b) { x = a; y = b; }
    int GetX() { return x; }
    int GetY() { return y; }
    int Num() { return num; }
    bool C() { return covery; }
    bool M() { return mark; }
    void Mark(bool m) { mark = m; }
    void N(int n) { num = n; }
    void Covery(bool c) { covery = c; }
};
class Edges {
private:
    int end1 = -1;                                  //端點1
    int end2 = -1;                                  //端點2
    float dist = 0;                                 //length
    float a = 0, b = 0, c = 0;                      //方程式的變數
    bool cut = 0;                                   //是否保留在最短路徑中
    vector<Point> IN;                               //若否，插入的點集
public:
    void setdel() { cut = 1; }
    bool Del() { return cut; }
    void edge(int a, int b) {
        end1 = a;
        end2 = b;
    }
    void addin(Point* a) {
        IN.push_back(*a);
    }
    vector<Point> getin() {
        return IN;
    }
    void setLine(float A, float B, float C, float D) { a = A; b = B; c = C; dist = D; }
    float disttoV(Point& x) {
        return fabs(a * x.GetX() + b + c * x.GetY()) / pow(pow(a, 2) + pow(c, 2), 0.5);
    }

    int v1() { return end1; }
    int v2() { return end2; }

};


float** D(vector<Point>& v) {                                           //Adj. metrix 計算
    float** adjM = new float* [v.size()];
    for (int i = 0; i < v.size(); i++) {
        adjM[i] = new float[v.size()];
        for (int j = 0; j < v.size(); j++) {
            adjM[i][j] = pow((v[i].GetX() - v[j].GetX()), 2) + pow((v[i].GetY() - v[j].GetY()), 2);
        }
    }
    return adjM;
}

int lineFunc(Point endA, Point endB, Point x) {                         //計算直線方程、點x與該直線的關係
    float a = 0, b = 0, c = 0;
    int k = 0;
    if ((endA.GetX() - endB.GetX()) == 0) {
        a = 1;
        b = -endA.GetX();
        c = 0;
    }
    else {
        a = endB.GetY() - endA.GetY();
        c = (endA.GetX() - endB.GetX());
        b = -(a * endA.GetX()) - c * endA.GetY();
    }
    k = (x.GetX() * a + c * x.GetY() + b);
    return k;
}
vector<Point> ConvexHull(vector<Point>& array, vector<Edges>& CH_E) {
    vector<Point> CH;
    int size = array.size();
    int lt = 0, gt = 0, temp = 0;
    for (int endA = 0; endA < size; endA++) {
        for (int i = endA + 1; i < size; i++) {
            lt = 0;
            gt = 0;
            for (int j = 0; j < size; j++) {
                {
                    temp = 0;
                    if (j != i && j != endA)
                    {
                        temp = lineFunc(array[endA], array[i], array[j]);           //計算該點在線段的哪側
                        if (temp > 0) gt++;
                        else if (temp < 0) lt++;
                    }
                }

            }
            if (gt == 0 && lt == 0) {
                cout << "False!";
            }
            else if (gt == 0 || lt == 0) {                                              //所有點都在同側
                if (gt == size - 2 || lt == size - 2)
                {
                    if (!array[endA].M()) {
                        CH.push_back(array[endA]);
                        array[endA].Mark(1);
                    }
                    if (!array[i].M())
                    {
                        CH.push_back(array[i]);
                        array[i].Mark(1);
                    }
                    Edges e;
                    e.edge(endA, i);
                    CH_E.push_back(e);
                }
            }
        }
    }
    return CH;
}

float ComputeArea(vector<Point>& points)
{
    if (points.size() < 3)return 0;
    float area = 0;
    for (int i = 0; i < points.size(); i++)
        area += points[i].GetX() * points[(i + 1) % points.size()].GetY() - points[i].GetY() * points[(i + 1) % points.size()].GetX();          //外積面積公式
    return fabs(area / 2.0);
}
void BruteForce(vector<Point>& Path, vector<Point>& shortestPath, float& min, int start) {
    for (int i = start; i < Path.size(); i++) {                             //用遞迴達成排列
        swap(Path[start], Path[i]);
        BruteForce(Path, shortestPath, min, start + 1);
        swap(Path[start], Path[i]);
    }
    if (start == Path.size()) {                                             //完成一組排列
        float temp = 0;
        Path.push_back(Path.front());                                       //要回到起點，將起點加入末端
        for (int i = 0; i < Path.size() - 1; i++) {
            temp += pow((pow((Path[i + 1].GetX() - Path[i].GetX()), 2) + pow((Path[i + 1].GetY() - Path[i].GetY()), 2)), 0.5);
        }
        if (temp < min) {
            min = temp;
            shortestPath = Path;
        }
        Path.pop_back();                                                    //還原vector
    }
}
float Bshorteastpath(vector<Point>& points, vector<Point>& ShortestPath) {
    float min = 100000;
    BruteForce(points, ShortestPath, min, 0);
    return min;
}

float ConvexHull_TSP(vector<Edges>& CH_E, vector<Point>& points, vector<Edges>& Path) {
    for (auto& i : CH_E) {                                                          //設定目前Convex-Hull的邊距離和直線方程
        float a = 0, b = 0, c = 0, k = 0;
        if ((points[i.v1()].GetX() - points[i.v2()].GetX()) == 0) {
            a = 1;
            b = -points[i.v1()].GetX();
            c = 0;
        }
        else {
            a = points[i.v2()].GetY() - points[i.v1()].GetY();
            c = (points[i.v1()].GetX() - points[i.v2()].GetX());
            b = -(a * points[i.v1()].GetX()) - c * points[i.v1()].GetY();
        }
        k = pow(pow((points[i.v1()].GetX() - points[i.v2()].GetX()), 2) + pow((points[i.v1()].GetY() - points[i.v2()].GetY()), 2), 0.5);
        i.setLine(a, b, c, k);
    }
    for (auto& v : points) {
        float min = 100000, temp = 0;
        Edges* tempe = NULL;
        if (!v.M()) {
            for (auto& e : CH_E) {
                temp = e.disttoV(v);
                if (temp < min) {                   //尋找最近的邊
                    min = temp;
                    v.SetIn(e.v1(), e.v2());        //v在哪兩點之間
                    tempe = &e;
                }
            }
            if (min != 100000) {
                tempe->setdel();                     //此邊會被取代
                tempe->addin(&v);
            }
        }
    }

    for (auto& e : CH_E) {
        if (e.Del()) {                                      //若有點要投影到此邊
            vector<Point> temp;
            temp = e.getin();                               //temp為要投影的點所形成的向量
            int ne = -1;
            Point* T = NULL;
            int Size = temp.size();
            for (int i = 0; i < Size + 1; i++)                //找出投影點順序，每次找與起點最近的點，再將起點設為該點
            {
                cout << endl << "( " << e.v1() + 1 << ", " << e.v2() + 1 << " )" << endl;
                float min = 10000, var = 0;
                Edges newE;
                T = NULL;
                for (auto& t : temp) {
                    if (!t.C() && ne != t.Num()) {
                        var = pow(pow((points[e.v1()].GetX() - t.GetX()), 2) + pow((points[e.v1()].GetY() - t.GetY()), 2), 0.5);    //距離
                        if (var < min) {
                            min = var;
                            ne = t.Num();
                            if (T != NULL) T->Covery(0);                            //有更小的值，將上次找到的取消
                            t.Covery(true);                                         //標記為已處理該點，避免重複執行同一點
                            T = &t;
                        }
                    }
                }

                if (ne != -1)
                {
                    if (!points[ne].C())
                    {
                        points[ne].Covery(true);
                        newE.edge(e.v1(), ne);                          //將起點與最近點 形成的邊加入最短路徑中
                        Path.push_back(newE);
                        e.edge(ne, e.v2());                             //起點設為該點
                    }
                }

            }
            if (ne != -1)
            {
                Edges newE;
                newE.edge(ne, e.v2());
                Path.push_back(newE);
            }
        }
        else {
            Path.push_back(e);
        }

    }
    float lenofP = 0;
    for (auto& i : Path) {                                          //計算新的路徑長
        float a = 0, b = 0, c = 0, k = 0;
        if ((points[i.v1()].GetX() - points[i.v2()].GetX()) == 0) {
            a = 1;
            b = -points[i.v1()].GetX();
            c = 0;
        }
        else {
            a = points[i.v2()].GetY() - points[i.v1()].GetY();
            c = (points[i.v1()].GetX() - points[i.v2()].GetX());
            b = -(a * points[i.v1()].GetX()) - c * points[i.v1()].GetY();
        }
        lenofP += pow(pow((points[i.v1()].GetX() - points[i.v2()].GetX()), 2) + pow((points[i.v1()].GetY() - points[i.v2()].GetY()), 2), 0.5);
    }
    return lenofP;
}


int main() {



    /*vvvvvvvvvv檔案I/Ovvvvvvvvvvv*/
    ifstream in;
    ofstream out;
    out.open("output.txt");
    if (out.fail()) {
        cout << "output file opening failed";
        exit(1);
    }
    in.open("Map1.txt");
    if (in.fail()) {
        cout << "input file opening failed";
        exit(1);
    }
    /*^^^^^^^^^^檔案I/O^^^^^^^^^^*/
    int x = 0, y = 0;
    //讀檔
    vector<Point> points;
    int n = 0;
    while (in >> x >> y) {
        Point t;
        t.SetPoint(x, y);
        t.N(n);
        n++;
        points.push_back(t);

    }
    float** dists = D(points);                 //adjacency matrix 儲存點集、距離

    //Showing the graph of points
    int cou = 0;
    for (int Y = 20; Y >= 0; Y--) {
        cou = 0;
        for (int X = 0; X <= 20; X++) {
            for (int i = 0; i < points.size(); i++) {
                if (points[i].GetX() == X && points[i].GetY() == Y) {
                    out << " " << i + 1 << " ";
                    cou++;
                }
            }
            if (!cou) {
                out << " - ";
            }
            cou = 0;
        }
        out << endl;
    }

    //第一題
    //暴力法找最小值
    float min = 10000000;
    int end1 = 0, end2 = 0;                         //紀錄最短距離的兩點
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            if (dists[i][j] < min) {
                min = dists[i][j];
                end1 = i + 1;
                end2 = j + 1;
            }
        }
    }
    out << " # 1" << endl;
    out << "Shortest distance ,which length is " << min << ",is between Point " << end1 << " and  Point " << end2 << endl;

    //第二題
    out << " # 2" << endl;
    out << "The Covex-Hull is consists of following points " << endl;
    vector<Edges> CH_E;                                     //形成Convex-Hull的邊
    vector<Point> CH = ConvexHull(points, CH_E);            //Convex-Hull邊界上的點
    for (auto& i : CH) {
        out << "( " << i.GetX() << " , " << i.GetY() << " )" << endl;
    }
    out << "Edges : " << endl;
    for (auto& i : CH_E) {
        out << " ( Point " << i.v1() + 1 << " , Point " << i.v2() + 1 << " ) " << endl;
    }
    float** CH_adjM = D(CH);                                //以距離為權重的Adj. Metrix
    //暴力法找最大值
    float max = 0;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            if (dists[i][j] > max) {
                max = dists[i][j];
                end1 = i + 1;
                end2 = j + 1;
            }
        }
    }
    max = pow(max, 0.5);
    out << "Longest distance ,which length is " << max << ",is between Point ( " << CH[end1].GetX() << " , " << CH[end1].GetY() << " ) and  Point ( " << CH[end2].GetX() << " , " << CH[end2].GetY() << " ) " << endl;
    out << "Area of the Convex-Hull is " << ComputeArea(CH) << endl;
    // 第3題
    out << " #3 " << endl;
    out << " (1) " << endl;
    float sp = 0;
    vector<Point> ShortestPath;                                     //紀錄Shortest Path經過的點順序
    sp = Bshorteastpath(points, ShortestPath);                      //Shortest Path長度
    out << " Length of the shortest path : " << sp << endl;
    for (auto& i : ShortestPath) {
        out << " Point " << i.Num() + 1 << " ->";
    }
    out << "done" << endl;

    out << " (2) " << endl;
    vector<Edges> Path;
    float TSP = ConvexHull_TSP(CH_E, points, Path);                        //以新演算法求Shortestpath
    out << "Length of Shortest Path by Convex-Hull-TSP Algorithm is : " << TSP << endl;
    for (auto& i : Path) {
        out << " ( Point " << i.v1() + 1 << " , Point " << i.v2() + 1 << " ) " << endl;
    }

    /*vvvvvvvvvv檔案I/Ovvvvvvvvvvv*/
    in.close();
    out.close();
    /*^^^^^^^^^^檔案I/O^^^^^^^^^^*/
    return 0;
}