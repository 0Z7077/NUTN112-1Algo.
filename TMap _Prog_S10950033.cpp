#include <iostream>
#include <fstream>
#include<cmath>
#include<vector>
using namespace std;

class Point {
private:
    int x = 0;
    int y = 0;
    bool mark = false;                          //�аO�o���I���S���bConvex-Hull��ɤW
    int num = -1;                               //�I�s��
    int in1 = 0, in2 = 0;                       //�Y���b��ɡA�n�[�J�����I����(#3)
    bool covery = false;                        //�O�_�w�b�d��(#3)
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
    int end1 = -1;                                  //���I1
    int end2 = -1;                                  //���I2
    float dist = 0;                                 //length
    float a = 0, b = 0, c = 0;                      //��{�����ܼ�
    bool cut = 0;                                   //�O�_�O�d�b�̵u���|��
    vector<Point> IN;                               //�Y�_�A���J���I��
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


float** D(vector<Point>& v) {                                           //Adj. metrix �p��
    float** adjM = new float* [v.size()];
    for (int i = 0; i < v.size(); i++) {
        adjM[i] = new float[v.size()];
        for (int j = 0; j < v.size(); j++) {
            adjM[i][j] = pow((v[i].GetX() - v[j].GetX()), 2) + pow((v[i].GetY() - v[j].GetY()), 2);
        }
    }
    return adjM;
}

int lineFunc(Point endA, Point endB, Point x) {                         //�p�⪽�u��{�B�Ix�P�Ӫ��u�����Y
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
                        temp = lineFunc(array[endA], array[i], array[j]);           //�p����I�b�u�q������
                        if (temp > 0) gt++;
                        else if (temp < 0) lt++;
                    }
                }

            }
            if (gt == 0 && lt == 0) {
                cout << "False!";
            }
            else if (gt == 0 || lt == 0) {                                              //�Ҧ��I���b�P��
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
        area += points[i].GetX() * points[(i + 1) % points.size()].GetY() - points[i].GetY() * points[(i + 1) % points.size()].GetX();          //�~�n���n����
    return fabs(area / 2.0);
}
void BruteForce(vector<Point>& Path, vector<Point>& shortestPath, float& min, int start) {
    for (int i = start; i < Path.size(); i++) {                             //�λ��j�F���ƦC
        swap(Path[start], Path[i]);
        BruteForce(Path, shortestPath, min, start + 1);
        swap(Path[start], Path[i]);
    }
    if (start == Path.size()) {                                             //�����@�ձƦC
        float temp = 0;
        Path.push_back(Path.front());                                       //�n�^��_�I�A�N�_�I�[�J����
        for (int i = 0; i < Path.size() - 1; i++) {
            temp += pow((pow((Path[i + 1].GetX() - Path[i].GetX()), 2) + pow((Path[i + 1].GetY() - Path[i].GetY()), 2)), 0.5);
        }
        if (temp < min) {
            min = temp;
            shortestPath = Path;
        }
        Path.pop_back();                                                    //�٭�vector
    }
}
float Bshorteastpath(vector<Point>& points, vector<Point>& ShortestPath) {
    float min = 100000;
    BruteForce(points, ShortestPath, min, 0);
    return min;
}

float ConvexHull_TSP(vector<Edges>& CH_E, vector<Point>& points, vector<Edges>& Path) {
    for (auto& i : CH_E) {                                                          //�]�w�ثeConvex-Hull����Z���M���u��{
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
                if (temp < min) {                   //�M��̪���
                    min = temp;
                    v.SetIn(e.v1(), e.v2());        //v�b�����I����
                    tempe = &e;
                }
            }
            if (min != 100000) {
                tempe->setdel();                     //����|�Q���N
                tempe->addin(&v);
            }
        }
    }

    for (auto& e : CH_E) {
        if (e.Del()) {                                      //�Y���I�n��v�즹��
            vector<Point> temp;
            temp = e.getin();                               //temp���n��v���I�ҧΦ����V�q
            int ne = -1;
            Point* T = NULL;
            int Size = temp.size();
            for (int i = 0; i < Size + 1; i++)                //��X��v�I���ǡA�C����P�_�I�̪��I�A�A�N�_�I�]�����I
            {
                cout << endl << "( " << e.v1() + 1 << ", " << e.v2() + 1 << " )" << endl;
                float min = 10000, var = 0;
                Edges newE;
                T = NULL;
                for (auto& t : temp) {
                    if (!t.C() && ne != t.Num()) {
                        var = pow(pow((points[e.v1()].GetX() - t.GetX()), 2) + pow((points[e.v1()].GetY() - t.GetY()), 2), 0.5);    //�Z��
                        if (var < min) {
                            min = var;
                            ne = t.Num();
                            if (T != NULL) T->Covery(0);                            //����p���ȡA�N�W����쪺����
                            t.Covery(true);                                         //�аO���w�B�z���I�A�קK���ư���P�@�I
                            T = &t;
                        }
                    }
                }

                if (ne != -1)
                {
                    if (!points[ne].C())
                    {
                        points[ne].Covery(true);
                        newE.edge(e.v1(), ne);                          //�N�_�I�P�̪��I �Φ�����[�J�̵u���|��
                        Path.push_back(newE);
                        e.edge(ne, e.v2());                             //�_�I�]�����I
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
    for (auto& i : Path) {                                          //�p��s�����|��
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



    /*vvvvvvvvvv�ɮ�I/Ovvvvvvvvvvv*/
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
    /*^^^^^^^^^^�ɮ�I/O^^^^^^^^^^*/
    int x = 0, y = 0;
    //Ū��
    vector<Point> points;
    int n = 0;
    while (in >> x >> y) {
        Point t;
        t.SetPoint(x, y);
        t.N(n);
        n++;
        points.push_back(t);

    }
    float** dists = D(points);                 //adjacency matrix �x�s�I���B�Z��

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

    //�Ĥ@�D
    //�ɤO�k��̤p��
    float min = 10000000;
    int end1 = 0, end2 = 0;                         //�����̵u�Z�������I
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

    //�ĤG�D
    out << " # 2" << endl;
    out << "The Covex-Hull is consists of following points " << endl;
    vector<Edges> CH_E;                                     //�Φ�Convex-Hull����
    vector<Point> CH = ConvexHull(points, CH_E);            //Convex-Hull��ɤW���I
    for (auto& i : CH) {
        out << "( " << i.GetX() << " , " << i.GetY() << " )" << endl;
    }
    out << "Edges : " << endl;
    for (auto& i : CH_E) {
        out << " ( Point " << i.v1() + 1 << " , Point " << i.v2() + 1 << " ) " << endl;
    }
    float** CH_adjM = D(CH);                                //�H�Z�����v����Adj. Metrix
    //�ɤO�k��̤j��
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
    // ��3�D
    out << " #3 " << endl;
    out << " (1) " << endl;
    float sp = 0;
    vector<Point> ShortestPath;                                     //����Shortest Path�g�L���I����
    sp = Bshorteastpath(points, ShortestPath);                      //Shortest Path����
    out << " Length of the shortest path : " << sp << endl;
    for (auto& i : ShortestPath) {
        out << " Point " << i.Num() + 1 << " ->";
    }
    out << "done" << endl;

    out << " (2) " << endl;
    vector<Edges> Path;
    float TSP = ConvexHull_TSP(CH_E, points, Path);                        //�H�s�t��k�DShortestpath
    out << "Length of Shortest Path by Convex-Hull-TSP Algorithm is : " << TSP << endl;
    for (auto& i : Path) {
        out << " ( Point " << i.v1() + 1 << " , Point " << i.v2() + 1 << " ) " << endl;
    }

    /*vvvvvvvvvv�ɮ�I/Ovvvvvvvvvvv*/
    in.close();
    out.close();
    /*^^^^^^^^^^�ɮ�I/O^^^^^^^^^^*/
    return 0;
}