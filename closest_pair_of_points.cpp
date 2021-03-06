#include<iostream>
#include<cstdio>
#include<vector>
#include<cmath>
#include<algorithm>
#include<fstream>
using namespace std;
typedef pair<double,double> Point;
typedef vector<Point> Points;
#define x first
#define y second
bool cmpx(const Point& p1, const Point& p2) {return (p1.x < p2.x);}
bool cmpy(const Point& p1, const Point& p2){return (p1.y < p2.y);}

double enumerate(Points P){
	int n=P.size();
	double dmin;
	for(int i=0; i<n; i++)
		for (int j = i+1; j<n; j++){
            dmin = min(dmin, Distance(P[i], P[j]));
		}
	return dmin;
}

double Distance(const Point &p1, const Point &p2) {
    double dx = p1.x - p2.x, dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

double closest(Points p){
	double dmin;
    int s = p.size();
    for(int i=0;i<s;i++)

        for(int j=1; (p[i+j].y-p[i].y<dmin)&& i+j<s; j++)
            dmin = min(dmin, Distance(p[i], p[i+j]));
	return dmin;
}

double DnC(Points px, Points py) {
	int n=px.size();

	if(n <= 3) return enumerate(px);

	int mid = n / 2;

	Points pyl, pyr;
	for(int i=0;i<n;i++){
		if(px[i].x <= px[mid].x) pyl.push_back(px[i]);
		else pyr.push_back(px[i]);
	}

	Points pxl = Points(px.begin(), px.begin()+mid);
	Points pxr = Points(px.begin()+mid + 1, px.end());

	double dl = DnC(pxl, pyl);
	double dr = DnC(pxr, pyr);
	double dmin = min(dl, dr);

    dmin = min(dmin, closest(pyl));
    dmin = min(dmin, closest(pyr));

	return dmin;
}

double closest(Points p) {
	Points px = p, py = p;
	sort(px.begin(), px.end(), cmpx);
	sort(py.begin(), py.end(), cmpy);
	return DnC(px, py);
}

int main(int argc, char* argv[]){
    int N;
    ifstream fin(argv[1]);
    fin >> N;
    Points points;
    double tmpx, tmpy;
    for(int i=0; i<N; i++){
        fin >> tmpx >> tmpy;
        points.push_back( make_pair(tmpx, tmpy) );
    }
    fin.close();

    printf("Shortest distance: %f\n", closest(points));

    return 0;
}
