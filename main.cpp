#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <climits>
using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

class Floor{
public:
    Floor(int m, int n, int B)
    : row(m), col(n), capacity(B), steps(-1), uncleaned(0) {
        visited = new int*[m];
        dist = new int*[m];
        far = new int*[m];
        for(int i=0; i<m; i++) {
            visited[i] = new int[n];
            fill(visited[i], visited[i]+n, 0);
            dist[i] = new int[n];
            fill(dist[i], dist[i]+n, -1);
            far[i] = new int[n];
            fill(far[i], far[i]+n, -1);
        }
    }
    ~Floor() {
        for(int i=0; i<row; i++) {
            delete [] arr[i];
            delete [] visited[i];
            delete [] dist[i];
            delete [] far[i];
        }
        delete [] arr;
        delete [] visited;
        delete [] dist;
        delete [] far;
    }
    void draw_the_map(char** a) {arr = a;}
    void cleaning();
    void get_battery_station();
    void compute_dis();
    void compute_far();
    string encode_output() {
        stringstream ss;
        ss << steps;
        ss << command;
        return ss.str();
    }
    friend ostream& operator<<(ostream &output, const Floor &f);
private:
    int row, col, capacity;
    int br, bc, battery, uncleaned, steps, fr, fc;
    char** arr;
    int** visited;
    int** dist;
    int** far;
    string command;
};

void Floor::get_battery_station()
{
    for(int i=0; i<row; i++) {
        for(int j=0; j<col; j++) {
            if(arr[i][j] == '0') {
                uncleaned++;
            }
            else if(arr[i][j] == 'R') {
                br = i;
                bc = j;
                //cout << i << " " << j << endl;
            }
        }
    }
}

// using BFS to compute the shortest distance
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
    //for(int i=0; i<row; i++) {
    //    for(int j=0; j<col; j++) {
    //        printf("%2d ", dist[i][j]);
    //    }
    //    cout << endl;
    //}
    //cout << endl;
    compute_far();
    return;
}

void Floor::compute_far()
{
    int d = INT_MIN;
    for(int i=0; i<row; i++) {
        for(int j=0; j<col; j++) {
            if( dist[i][j] >= d && visited[i][j] == 0 ) {
                fr = i;
                fc = j;
                d = dist[i][j];
            }
        }
    }
    //cout << fr << " " << fc << endl;
	queue<Point> que;
    que.push(Point(fc, fr));
	//cout << "here\n";
	for (int i = 0; i < row; i++) {
		fill(far[i], far[i] + col, -1);
	}
    far[fr][fc] = 0;
    while(!que.empty()) {
        Point p = que.front();
        que.pop();
        if(arr[p.y][p.x] == '0' || arr[p.y][p.x] == 'R') {
            if( p.x - 1 >= 0 && (arr[p.y][p.x-1] == '0' || arr[p.y][p.x-1] == 'R') && far[p.y][p.x - 1] == -1 ) {
                far[p.y][p.x-1] = far[p.y][p.x] + 1;
                que.push(Point(p.x-1, p.y));
            }
            if( p.x + 1 < col && (arr[p.y][p.x+1] == '0' || arr[p.y][p.x+1] == 'R') && far[p.y][p.x + 1] == -1 ) {
                far[p.y][p.x+1] = far[p.y][p.x] + 1;
                que.push(Point(p.x+1, p.y));
            }
            if( p.y - 1 >= 0 && (arr[p.y-1][p.x] == '0' || arr[p.y-1][p.x] == 'R') && far[p.y - 1][p.x] == -1 ) {
                far[p.y-1][p.x] = far[p.y][p.x] + 1;
                que.push(Point(p.x, p.y-1));
            }
            if( p.y + 1 < row && (arr[p.y+1][p.x] == '0' || arr[p.y+1][p.x] == 'R') && far[p.y + 1][p.x] == -1 ) {
                far[p.y+1][p.x] = far[p.y][p.x] + 1;
                que.push(Point(p.x, p.y+1));
            }
        }
    }
    //for(int i=0; i<row; i++) {
    //    for(int j=0; j<col; j++) {
    //        printf("%2d ", far[i][j]);
    //    }
    //    cout << endl;
    //}
    return;
}

void Floor::cleaning()
{
    int r = br; // the position where the robot is
    int c = bc;
    int goback;
    stringstream ss;
    visited[r][c] = 1;
    while(uncleaned > 0) {
        if(r == br && c == bc) {// charge!
            goback = 0;
            battery = capacity;
        }
        steps++;
        ss << endl << r << " " << c;
        //cout << uncleaned << endl;
        //cout << r << " " << c << endl;
        if( battery > dist[r][c] ) {
            // visited the unvisited one first
            // if the distance of the four directions are the same
            // right > left > down > up
            int shortest = INT_MAX;
            int mov = 0;
            int pr, pc;
            if( c + 1 < col && far[r][c+1] != -1 ) {
                //cout << "right\n";
                if(!visited[r][c+1]) {
                    c = c + 1;
                    visited[r][c] = 1;
                    mov = 1;
                    uncleaned--;
                }
                else { // maybe stuck in the corner or somewhat, should go back
                    if(far[r][c+1] < shortest) {
                        shortest = far[r][c+1];
                        pr = r;
                        pc = c + 1;
                    }
                }
            }
            if( !mov && c - 1 >= 0 && far[r][c-1] != -1 ) {
                //cout << "left\n";
                if(!visited[r][c-1]) {
                    c = c - 1;
                    visited[r][c] = 1;
                    mov = 1;
                    uncleaned--;
                }
                else { // maybe stuck in the corner or somewhat, should go back
                    if(far[r][c-1] < shortest) {
                        shortest = far[r][c-1];
                        pr = r;
                        pc = c - 1;
                    }
                }
            }
            if( !mov && r + 1 < row && far[r+1][c] != -1 ) {
                //cout << "down\n";
                if(!visited[r+1][c]) {
                    r = r + 1;
                    visited[r][c] = 1;
                    mov = 1;
                    uncleaned--;
                }
                else { // maybe stuck in the corner or somewhat, should go back
                    if(far[r+1][c] < shortest) {
                        shortest = far[r+1][c];
                        pr = r + 1;
                        pc = c;
                    }
                }
            }
            if( !mov && r - 1 >= 0 && far[r-1][c] != -1 ) {
                //cout << "up\n";
                if(!visited[r-1][c]) {
                    r = r - 1;
                    visited[r][c] = 1;
                    mov = 1;
                    uncleaned--;
                }
                else { // maybe stuck in the corner or somewhat, should go back
                    if(far[r-1][c] < shortest) {
                        shortest = far[r-1][c];
                        pr = r - 1;
                        pc = c;
                    }
                }
            }
            if(!mov) {
                r = pr;
                c = pc;
            }
        }
        else { // runs out of the battery, need to go back to 'R'
            // find the shortest path back to 'R'
            // it should prefer the one which has not been visited yet
            if(!goback) {
                goback = 1;
                compute_far();
            }
            //cout << "here\n";
            int shortest = INT_MAX;
            int mov = 0;
            int pr, pc;
            if( c + 1 < col && dist[r][c+1] == battery-1 ) {
                //cout << "right\n";
                if(!visited[r][c+1]) {
                    c = c + 1;
                    visited[r][c] = 1;
                    mov = 1;
                    uncleaned--;
                }
                else { // maybe stuck in the corner or somewhat, should go back
                    if(dist[r][c+1] < shortest) {
                        shortest = dist[r][c+1];
                        pr = r;
                        pc = c + 1;
                    }
                }
            }
            if( !mov && r + 1 < row && dist[r+1][c] == battery-1 ) {
                //cout << "down\n";
                if(!visited[r+1][c]) {
                    r = r + 1;
                    visited[r][c] = 1;
                    mov = 1;
                    uncleaned--;
                }
                else { // maybe stuck in the corner or somewhat, should go back
                    if(dist[r+1][c] < shortest) {
                        shortest = dist[r+1][c];
                        pr = r + 1;
                        pc = c;
                    }
                }
            }
            if( !mov && c - 1 >= 0 && dist[r][c-1] == battery-1 ) {
                //cout << "left\n";
                if(!visited[r][c-1]) {
                    c = c - 1;
                    visited[r][c] = 1;
                    mov = 1;
                    uncleaned--;
                }
                else { // maybe stuck in the corner or somewhat, should go back
                    if(dist[r][c-1] < shortest) {
                        shortest = dist[r][c-1];
                        pr = r;
                        pc = c - 1;
                    }
                }
            }
            if( !mov && r - 1 >= 0 && dist[r-1][c] == battery-1 ) {
                //cout << "up\n";
                if(!visited[r-1][c]) {
                    r = r - 1;
                    visited[r][c] = 1;
                    mov = 1;
                    uncleaned--;
                }
                else { // maybe stuck in the corner or somewhat, should go back
                    if(dist[r-1][c] < shortest) {
                        shortest = dist[r-1][c];
                        pr = r - 1;
                        pc = c;
                    }
                }
            }
            if(!mov) {
                r = pr;
                c = pc;
            }
        }
        battery--;
        //for(int i=0; i<row; i++) {
        //    for(int j=0; j<col; j++) {
        //        cout << visited[i][j] << " ";
        //    }
        //    cout << endl;
        //}
    }
    while(r != br || c != bc) {
        steps++;
        ss << endl << r << " " << c;
        //cout << r << " " << c << endl;
        int shortest = INT_MAX;
        int pr, pc;
        if( c + 1 < col && dist[r][c+1] != -1 ) {
           // cout << "right\n";
            if(dist[r][c+1] < shortest) {
                shortest = dist[r][c+1];
                pr = r;
                pc = c + 1;
            }
        }
        if( r + 1 < row && dist[r+1][c] != -1 ) {
            //cout << "down\n";
            if(dist[r+1][c] < shortest) {
                shortest = dist[r+1][c];
                pr = r + 1;
                pc = c;
            }
        }
        if( c - 1 >= 0 && dist[r][c-1] != -1 ) {
           // cout << "left\n";
            if(dist[r][c-1] < shortest) {
                shortest = dist[r][c-1];
                pr = r;
                pc = c - 1;
            }
        }
        if( r - 1 >= 0 && dist[r-1][c] != -1 ) {
           // cout << "up\n";
            if(dist[r-1][c] < shortest) {
                shortest = dist[r-1][c];
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
   // cout << r << " " << c << endl;
	ss << endl;
	for(int i=0; i<row; i++) {
		for(int j=0; j<col; j++) {
		    ss << visited[i][j];
		}
		ss << endl;
	}
    command = ss.str();
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
