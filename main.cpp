#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

class Floor{
public:
    Floor(int m, int n, int B)
    : row(m), col(n), battery(B), steps(-1) {
        visited = new int*[m];
        dist = new int*[m];
        for(int i=0; i<m; i++) {
            visited[i] = new int[n];
            fill(visited[i], visited[i]+n, 0);
            dist[i] = new int[n];
            fill(dist[i], dist[i]+n, -1);
        }
    }
    ~Floor() {
        for(int i=0; i<row; i++)
            delete [] arr[i];
        delete [] arr;
    }
    void draw_the_map(char** a) {arr = a;}
    void cleaning();
    void get_battery_station();
    void compute_dis();
    string encode_output() {
        stringstream ss;
        ss << steps << endl;
        ss << command;
        return ss.str();
    }
    friend ostream& operator<<(ostream &output, const Floor &f);
private:
    int row, col, battery;
    int br, bc;
    char** arr;
    int** visited;
    int** dist;
    int steps;
    string command;
};

void Floor::get_battery_station()
{
    for(int i=0; i<row; i++) {
        for(int j=0; j<col; j++) {
            if(arr[i][j] == 'R') {
                br = i;
                bc = j;
                //cout << i << " " << j << endl;
                return;
            }
        }
    }
}

void Floor::compute_dis()
{
    queue<Point> que;
    que.push(Point(bc, br));
    dist[br][bc] = 0;
    while(!que.empty()) {
        Point p = que.front();
        que.pop();
        if(arr[p.y][p.x] == '0' || arr[p.y][p.x] == 'R') {
            if( p.x - 1 >= 0 && arr[p.y][p.x-1] == '0' && dist[p.y][p.x-1] == -1 ) {
                dist[p.y][p.x-1] = dist[p.y][p.x] + 1;
                que.push(Point(p.x-1, p.y));
            }
            if( p.x + 1 < col && arr[p.y][p.x+1] == '0' && dist[p.y][p.x+1] == -1 ) {
                dist[p.y][p.x+1] = dist[p.y][p.x] + 1;
                que.push(Point(p.x+1, p.y));
            }
            if( p.y - 1 >= 0 && arr[p.y-1][p.x] == '0' && dist[p.y-1][p.x] == -1 ) {
                dist[p.y-1][p.x] = dist[p.y][p.x] + 1;
                que.push(Point(p.x, p.y-1));
            }
            if( p.y + 1 < row && arr[p.y+1][p.x] == '0' && dist[p.y+1][p.x] == -1 ) {
                dist[p.y+1][p.x] = dist[p.y][p.x] + 1;
                que.push(Point(p.x, p.y+1));
            }
        }
    }
//    for(int i=0; i<row; i++) {
//        for(int j=0; j<col; j++) {
//            printf("%2d ", dist[i][j]);
//        }
//        cout << endl;
//    }
    return;
}

void Floor::cleaning()
{

}

ostream& operator<<(ostream &output, const Floor &f)
{
    for(int i=0; i<f.row; i++)
        cout << f.arr[i] << endl;
    return output;
}

int main(int, char** argv)
{
    ifstream fin;
    fin.open(argv[1], ifstream::in);
    ofstream log("final.path");
    string data;
    if(!fin)
        cout << "Fail to open " << argv[1] << endl;
    else {
        int m, n, B;
        fin >> m >> n >> B;
        Floor floor(m, n, B);
        char** arr = new char*[m];
        for(int i=0; i<m; i++) {
            arr[i] = new char[n];
            fin >> arr[i];
        }
        floor.draw_the_map(arr);
        floor.get_battery_station();
        floor.compute_dis();
        floor.cleaning();
        data = floor.encode_output();
    }
    log << data;
    log.close();

    return 0;
}
