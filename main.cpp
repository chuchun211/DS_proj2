#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <climits>
using namespace std;

class Point {
public:
	Point() : x(-1), y(-1), next(NULL) {}
	Point(int x, int y) : x(x), y(y), next(NULL) {}
	~Point() {}
	int x, y;
	Point* next;
};

template <class T>
class Queue {
public:
	Queue() { front = rear = NULL; }
	~Queue() {}
	bool IsEmpty() const { return front == NULL; }
	T* Front() const { return front; }
	T* Rear() const { return rear; }
	void Push(T* item) {
		if (front == NULL) front = rear = item;
		rear->next = item;
		rear = item;
	}
	void Pop() {
		T tmp = *front;
		front = front->next;
		tmp.~T();
	}
private:
	T *front, *rear;
};

class Floor {
public:
	Floor(int m, int n, int B)
		: row(m), col(n), capacity(B), steps(-1), uncleaned(0) {
		arr = new char*[m];
		visited = new int*[m];
		dist = new int*[m];
		far = new int*[m];
		for (int i = 0; i < m; i++) {
			arr[i] = new char[n];
			visited[i] = new int[n];
			fill(visited[i], visited[i] + n, 0);
			dist[i] = new int[n];
			fill(dist[i], dist[i] + n, -1);
			far[i] = new int[n];
			fill(far[i], far[i] + n, -1);
		}
	}
	~Floor() {
		for (int i = 0; i < row; i++) {
			delete[] arr[i];
			delete[] visited[i];
			delete[] dist[i];
			delete[] far[i];
		}
		delete[] arr;
		delete[] visited;
		delete[] dist;
		delete[] far;
	}
	void cleaning();
	void get_battery_station();
	void compute_dis();
	void compute_far();
	void forwarding();
	void backing();
	string encode_output() {
		stringstream ss;
		ss << steps;
		ss << command;
		return ss.str();
	}
	char** arr;
private:
	int row, col, capacity;
	int br, bc, r, c, fr, fc;
	int battery, uncleaned, steps, goback;
	int** visited;
	int** dist;
	int** far;
	string command;
};

void Floor::get_battery_station()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (arr[i][j] == '0') {
				uncleaned++;
			}
			else if (arr[i][j] == 'R') {
				r = br = i;
				c = bc = j;
			}
		}
	}
}

// using BFS to compute the shortest distance
void Floor::compute_dis()
{
	Queue<Point> que;
	Point* p = new Point(bc, br);
	que.Push(p);
	dist[br][bc] = 0;
	while (!que.IsEmpty()) {
		Point* p = que.Front();
		if (arr[p->y][p->x] == '0' || arr[p->y][p->x] == 'R') {
			if (p->x - 1 >= 0 && arr[p->y][p->x - 1] == '0' && dist[p->y][p->x - 1] == -1) {
				dist[p->y][p->x - 1] = dist[p->y][p->x] + 1;
				Point* tmp = new Point(p->x - 1, p->y);
				que.Push(tmp);
			}
			if (p->x + 1 < col && arr[p->y][p->x + 1] == '0' && dist[p->y][p->x + 1] == -1) {
				dist[p->y][p->x + 1] = dist[p->y][p->x] + 1;
				Point* tmp = new Point(p->x + 1, p->y);
				que.Push(tmp);
			}
			if (p->y - 1 >= 0 && arr[p->y - 1][p->x] == '0' && dist[p->y - 1][p->x] == -1) {
				dist[p->y - 1][p->x] = dist[p->y][p->x] + 1;
				Point* tmp = new Point(p->x, p->y - 1);
				que.Push(tmp);
			}
			if (p->y + 1 < row && arr[p->y + 1][p->x] == '0' && dist[p->y + 1][p->x] == -1) {
				dist[p->y + 1][p->x] = dist[p->y][p->x] + 1;
				Point* tmp = new Point(p->x, p->y + 1);
				que.Push(tmp);
			}
		}
		que.Pop();
	}
	compute_far();
	return;
}

void Floor::compute_far()
{
	int d = INT_MIN;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (dist[i][j] >= d && visited[i][j] == 0) {
				fr = i;
				fc = j;
				d = dist[i][j];
			}
		}
	}
	Queue<Point> que;
	Point* p = new Point(fc, fr);
	que.Push(p);
	for (int i = 0; i < row; i++) {
		fill(far[i], far[i] + col, -1);
	}
	far[fr][fc] = 0;
	while (!que.IsEmpty()) {
		Point* p = que.Front();
		if (arr[p->y][p->x] == '0' || arr[p->y][p->x] == 'R') {
			if (p->x - 1 >= 0 && (arr[p->y][p->x - 1] == '0' || arr[p->y][p->x - 1] == 'R') && far[p->y][p->x - 1] == -1) {
				far[p->y][p->x - 1] = far[p->y][p->x] + 1;
				Point* tmp = new Point(p->x - 1, p->y);
				que.Push(tmp);
			}
			if (p->x + 1 < col && (arr[p->y][p->x + 1] == '0' || arr[p->y][p->x + 1] == 'R') && far[p->y][p->x + 1] == -1) {
				far[p->y][p->x + 1] = far[p->y][p->x] + 1;
				Point* tmp = new Point(p->x + 1, p->y);
				que.Push(tmp);
			}
			if (p->y - 1 >= 0 && (arr[p->y - 1][p->x] == '0' || arr[p->y - 1][p->x] == 'R') && far[p->y - 1][p->x] == -1) {
				far[p->y - 1][p->x] = far[p->y][p->x] + 1;
				Point* tmp = new Point(p->x, p->y - 1);
				que.Push(tmp);
			}
			if (p->y + 1 < row && (arr[p->y + 1][p->x] == '0' || arr[p->y + 1][p->x] == 'R') && far[p->y + 1][p->x] == -1) {
				far[p->y + 1][p->x] = far[p->y][p->x] + 1;
				Point* tmp = new Point(p->x, p->y + 1);
				que.Push(tmp);
			}
		}
		que.Pop();
	}
	return;
}

void Floor::forwarding()
{
	// visited the unvisited one first
	// if the distance of the four directions are the same
	// right > left > down > up
	int shortest = INT_MAX;
	int mov = 0;
	int pr, pc;
	if (c + 1 < col && far[r][c + 1] != -1) {
		if (!visited[r][c + 1]) {
			c = c + 1;
			visited[r][c] = 1;
			mov = 1;
			uncleaned--;
		}
		else {
			if (far[r][c + 1] < shortest) {
				shortest = far[r][c + 1];
				pr = r;
				pc = c + 1;
			}
		}
	}
	if (!mov && c - 1 >= 0 && far[r][c - 1] != -1) {
		if (!visited[r][c - 1]) {
			c = c - 1;
			visited[r][c] = 1;
			mov = 1;
			uncleaned--;
		}
		else {
			if (far[r][c - 1] < shortest) {
				shortest = far[r][c - 1];
				pr = r;
				pc = c - 1;
			}
		}
	}
	if (!mov && r + 1 < row && far[r + 1][c] != -1) {
		if (!visited[r + 1][c]) {
			r = r + 1;
			visited[r][c] = 1;
			mov = 1;
			uncleaned--;
		}
		else {
			if (far[r + 1][c] < shortest) {
				shortest = far[r + 1][c];
				pr = r + 1;
				pc = c;
			}
		}
	}
	if (!mov && r - 1 >= 0 && far[r - 1][c] != -1) {
		if (!visited[r - 1][c]) {
			r = r - 1;
			visited[r][c] = 1;
			mov = 1;
			uncleaned--;
		}
		else {
			if (far[r - 1][c] < shortest) {
				shortest = far[r - 1][c];
				pr = r - 1;
				pc = c;
			}
		}
	}
	if (!mov) {
		r = pr;
		c = pc;
	}
}

void Floor::backing()
{
	// runs out of the battery, need to go back to 'R'
	// find the shortest path back to 'R'
	// it should prefer the one which has not been visited yet
	if (!goback) {
		goback = 1;
		if (visited[fr][fc])
			compute_far();
	}
	int shortest = INT_MAX;
	int mov = 0;
	int pr, pc;
	if (c + 1 < col && dist[r][c + 1] < battery && dist[r][c + 1] != -1) {
		if (!visited[r][c + 1]) {
			c = c + 1;
			visited[r][c] = 1;
			mov = 1;
			uncleaned--;
		}
		else {
			if (dist[r][c + 1] < shortest) {
				shortest = dist[r][c + 1];
				pr = r;
				pc = c + 1;
			}
		}
	}
	if (!mov && r + 1 < row && dist[r + 1][c] < battery && dist[r + 1][c] != -1) {
		if (!visited[r + 1][c]) {
			r = r + 1;
			visited[r][c] = 1;
			mov = 1;
			uncleaned--;
		}
		else {
			if (dist[r + 1][c] < shortest) {
				shortest = dist[r + 1][c];
				pr = r + 1;
				pc = c;
			}
		}
	}
	if (!mov && c - 1 >= 0 && dist[r][c - 1] < battery && dist[r][c - 1] != -1) {
		if (!visited[r][c - 1]) {
			c = c - 1;
			visited[r][c] = 1;
			mov = 1;
			uncleaned--;
		}
		else {
			if (dist[r][c - 1] < shortest) {
				shortest = dist[r][c - 1];
				pr = r;
				pc = c - 1;
			}
		}
	}
	if (!mov && r - 1 >= 0 && dist[r - 1][c] < battery && dist[r - 1][c] != -1) {
		if (!visited[r - 1][c]) {
			r = r - 1;
			visited[r][c] = 1;
			mov = 1;
			uncleaned--;
		}
		else {
			if (dist[r - 1][c] < shortest) {
				shortest = dist[r - 1][c];
				pr = r - 1;
				pc = c;
			}
		}
	}
	if (!mov) {
		r = pr;
		c = pc;
	}
}

void Floor::cleaning()
{
	stringstream ss;
	visited[r][c] = 1;
	while (uncleaned > 0) {
		if (r == br && c == bc) {// charge!
			goback = 0;
			battery = capacity;
		}
		steps++;
		ss << endl << r << " " << c;
		if (visited[fr][fc]) compute_far();
		if (battery > dist[r][c] + 3) forwarding();
		else backing();
		battery--;
	}
	while (r != br || c != bc) {
		steps++;
		ss << endl << r << " " << c;
		int shortest = INT_MAX;
		int pr, pc;
		if (c + 1 < col && dist[r][c + 1] != -1) {
			if (dist[r][c + 1] < shortest) {
				shortest = dist[r][c + 1];
				pr = r;
				pc = c + 1;
			}
		}
		if (r + 1 < row && dist[r + 1][c] != -1) {
			if (dist[r + 1][c] < shortest) {
				shortest = dist[r + 1][c];
				pr = r + 1;
				pc = c;
			}
		}
		if (c - 1 >= 0 && dist[r][c - 1] != -1) {
			if (dist[r][c - 1] < shortest) {
				shortest = dist[r][c - 1];
				pr = r;
				pc = c - 1;
			}
		}
		if (r - 1 >= 0 && dist[r - 1][c] != -1) {
			if (dist[r - 1][c] < shortest) {
				shortest = dist[r - 1][c];
				pr = r - 1;
				pc = c;
			}
		}
		r = pr;
		c = pc;
		battery--;
	}
	steps++;
	ss << endl << r << " " << c;
	command = ss.str();
}

int main(int, char** argv)
{
	ifstream fin;
	fin.open(argv[1], ifstream::in);
	ofstream log("final.path");
	string data;
	if (!fin)
		cout << "Fail to open " << argv[1] << endl;
	else {
		int m, n, B;
		fin >> m >> n >> B;
		Floor floor(m, n, B);
		for (int i = 0; i < m; i++) {
			fin >> floor.arr[i];
		}
		floor.get_battery_station();
		floor.compute_dis();
		floor.cleaning();
		data = floor.encode_output();
	}
	log << data;
	log.close();

	return 0;
}
