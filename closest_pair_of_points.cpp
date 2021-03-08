#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<vector>
#include<math.h>
using namespace std;

struct Point
{
	double x, y;
};

int pointpair;
vector<Point> p;

double randomnumber(double min, double max)
{
	return ((double)rand() / (double)RAND_MAX) * (max - min) + min;
}

//Generate random points and store in a file

void generaterandomnumbers()
{
    srand(time(0));// For random points each time
    ofstream randompoints;
    randompoints.open("points.txt");
    if(randompoints.is_open())
    {
        int no_of_pairs = 100;
        pointpair = no_of_pairs;
		double xmin = -10, xmax = 10, ymin = -10, ymax = 10;// range of points between +10 and -10
		for (int i = 0; i < no_of_pairs; i++)
        {
            randompoints << randomnumber(xmin, xmax) << " " << randomnumber(ymin, ymax) << endl;
        }
		randompoints.close();
    }
}

//Calculate the distance between the points

double calculatedistance(Point p1, Point p2)
{
	return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

//Merge function

vector<Point> merge(vector<Point> left, vector<Point> right, string coord) {
	int i = 0, j = 0, k = 0; //i:return vect j:left k:right
	int lsize = left.size();
	int rsize = right.size();
	int size = lsize + rsize;
	vector<Point> sortedpt(size);
	for (i = 0; i < size; i++) {

		if (j < lsize && k < rsize) {

		//Sort x-coordinate
			if (coord == "x") {
				if (left[j].x <= right[k].x) {
					sortedpt[i] = left[j];
					j++;
				}
				else {
					sortedpt[i] = right[k];
					k++;
				}
			}
			else { //Sort y-coordinate
				if (left[j].y <= right[k].y) {
					sortedpt[i] = left[j];
					j++;
				}
				else {
					sortedpt[i] = right[k];
					k++;
				}
			}

		}
		else if (j < lsize) {
			sortedpt[i] = left[j];
			j++;
		}
		else if (k < rsize) {
			sortedpt[i] = right[k];
			k++;
		}
	}
	return sortedpt;
}

vector<Point> mergesort(vector<Point> vect, string coord) {
	int size = vect.size();
	int mid = size / 2;
	vector<Point> left;
	vector<Point> right;
	if (size <= 1)
		return vect;
	for (int i = 0; i < mid; i++)
	{
		left.push_back(vect[i]);
		right.push_back(vect[i + mid]);
	}
	if (size % 2 != 0)
		right.push_back(vect.back());

	left = mergesort(left, coord);
	right = mergesort(right, coord);

	return merge(left, right, coord);
}

vector<Point> finddist(vector<Point> p) {
	int psize = p.size();
	if (psize < 2)
		return p;
	else
	{
		double min = calculatedistance(p[0], p[1]);
		Point p1 = p[0], p2 = p[1];
		for (int i = 0; i < psize; i++)
			for (int j = i + 1; j < psize; j++)
			{
				double distpp = calculatedistance(p[i], p[j]);
				if (distpp < min) {
					min = distpp;
					p1 = p[i];
					p2 = p[j];
				}
			}
		vector<Point> minPoints;
		minPoints.push_back(p1);
		minPoints.push_back(p2);
		return minPoints;
	}
}

// Find the closest pair of points

vector<Point> closestpair(vector<Point> points) {
	if (points.size() <= 3) {
		return finddist(points);
	}
	int pointsSize = points.size();
	int half = pointsSize / 2;
	vector<Point> leftPairs;
	vector<Point> rightPairs;
	vector<Point> leftResult, rightResult;

	if (pointsSize % 2 == 0) {
		for (int i = 0; i < half; i++)
		{
			leftPairs.push_back(points[i]);
			rightPairs.push_back(points[i + half]);
		}
	}
	else
	{
		for (int i = 0; i <= half; i++)
		{
			leftPairs.push_back(points[i]);
		}
		for (int i = half + 1; i < pointsSize; i++)
		{
			rightPairs.push_back(points[i]);
		}
	}

	leftResult = closestpair(leftPairs);
	rightResult = closestpair(rightPairs);
	double minLeft = calculatedistance(leftResult[0], leftResult[1]);
	double minRight = calculatedistance(rightResult[0], rightResult[1]);
	double minLR = min(minLeft, minRight);
	vector<Point> stripPoints;
	for (int i = half-1; i > 0; i--)
	{
		if (points[half].x - points[i].x < minLR)
			stripPoints.push_back(points[i]);
		else
			break;
	}
	for (int i = half + 1; i < pointsSize; i++)
	{
		if (points[i].x- points[half].x < minLR)
			stripPoints.push_back(points[i]);
		else
			break;
	}
	stripPoints.push_back(points[half]);
	vector<Point> sortedStripPointsy;
	vector<Point> stripClosestPair;
	if (stripPoints.size() > 1)
	{
		double min = minLR;
		sortedStripPointsy = mergesort(stripPoints, "y");
		int sizePointStrip = sortedStripPointsy.size();
		for (int i = 0; i < sizePointStrip - 1; i++)
			for (int j = i + 1; j < sizePointStrip && (sortedStripPointsy[j].y - sortedStripPointsy[i].y < min); j++)
			{
				double pairDistance = calculatedistance(sortedStripPointsy[i], sortedStripPointsy[j]);
				if (pairDistance < min)
				{
					min = pairDistance;
					stripClosestPair.clear();
					stripClosestPair.push_back(sortedStripPointsy[i]);
					stripClosestPair.push_back(sortedStripPointsy[j]);
				}
			}

		if (stripClosestPair.size()>0)
			return stripClosestPair;
		else
			return (minLeft < minRight) ? leftResult : rightResult;
	}
	else
		return (minLeft < minRight) ? leftResult : rightResult;
}


int main()
{
    cout<<"\t\t***********************************"<<endl;
    cout<<"\t\t   Finding Closest Pair of Points"<<endl;
    cout<<"\t\t***********************************"<<endl;
    generaterandomnumbers();

    string line;
	ifstream pointsfile("points.txt");
	if (pointsfile.is_open())
	{
		getline(pointsfile, line);
		pointpair = stoi(line);
		int i = 0;
		while (getline(pointsfile, line))
		{
			double x = stod(line.substr(0, line.find(" ")));
			double y = stod(line.substr(line.find(" ")));
			Point pointtemp;
			pointtemp.x = x;
			pointtemp.y = y;
			p.push_back(pointtemp);
			i++;
		}
		pointsfile.close();
	}

	if (p.size() < 2) //if only two points
    {
		cout << "p1: " << p[0].x << " " << p[0].y << " , p2: " << p[1].x << " " << p[1].y << calculatedistance(p[0], p[1]) << endl;
	}
	else
    {
		vector<Point> sortedx;
		sortedx = mergesort(p, "x");
		vector<Point> closestpt = closestpair(sortedx);
		cout<<endl;
		cout << "Minimum Distance: " << calculatedistance(closestpt[0], closestpt[1]) << endl;
        cout << "Closest Pair: (" << closestpt[0].x << "," << closestpt[0].y << ") (" << closestpt[1].x << "," << closestpt[1].y << ")" << endl;
	}

    return 0;
}
