#include <bits/stdc++.h>
using namespace std;
using P = pair<int, int>;
using Output = vector<vector<P>>;
#define rep(i,n) for (int i = 0; i < (n); ++i)
#define DEBUG 0

const double TIME_LIMIT = 4.5;
struct Timer {
    clock_t start;
    Timer() { reset(); }
    void reset() { start = clock(); }
    double get() { return (double)(clock() - start) / CLOCKS_PER_SEC; }
};
Timer timer;

int signum(int n) {
    if (n > 0) return 1;
    if (n < 0) return -1;
    return 0;
}

const P DXY[] = {
	{1, 0},
	{1, 1},
	{0, 1},
	{-1, 1},
	{-1, 0},
	{-1, -1},
	{0, -1},
	{1, -1}
};

struct Input {
    int N;
    vector<P> ps;
};

struct Status {
    vector<vector<bool>> has_point;
    vector<vector<vector<bool>>> used;
    Status (Input input) {
        has_point.resize(input.N, vector<bool>(input.N, false));
        used.resize(input.N, vector(input.N, vector<bool>(8, false)));
        rep(i,input.ps.size()) {
            has_point[input.ps[i].first][input.ps[i].second] = true;
        }
    }
    // O(N)
    string check_move(vector<P> rect) {
        for (int i = 1; i < 4; i++) {
            if (!has_point[rect[i].first][rect[i].second]) {
                return "(" + to_string(rect[i].first) + ", " + to_string(rect[i].second) + ") does not contain a dot";
            }
        }
        if (DEBUG) cerr << "here" << endl;
        if (DEBUG) cerr << rect[0].first << " : " << rect[0].second << endl;
        if (has_point[rect[0].first][rect[0].second]) {
            return "(" + to_string(rect[0].first) + ", " + to_string(rect[0].second) + ") already contains a dot";
        }

        if (DEBUG) cerr << "here" << endl;
        int dx01 = rect[1].first - rect[0].first;
        int dy01 = rect[1].second - rect[0].second;
        int dx03 = rect[3].first - rect[0].first;
        int dy03 = rect[3].second - rect[0].second;
        if (dx01 * dx03 + dy01 * dy03 != 0) {
            return "Illegal rectangle";
        }
        if (dx01 != 0 && dy01 != 0 && abs(dx01) != abs(dy01)) {
            return "Illegal rectangle";
        }
        if (rect[1].first + dx03 != rect[2].second || rect[1].second + dy03 != rect[2].second) {
            return "Illegal rectangle";
        }
        if (DEBUG) cerr << "here" << endl;

        rep(i,4) {
            int x = rect[i].first;
            int y = rect[i].second;
            int tx = rect[(i + 1) % 4].first;
            int ty = rect[(i + 1) % 4].second;
            int dx = signum(tx - x);
            int dy = signum(ty - y);
            int dir;
            rep(k,8) if (DXY[k].first == dx && DXY[k].second == dy) {
                dir = k;
            }
            if (DEBUG) cerr << dx << " : " << dy << endl;
            if (DEBUG) cerr << x << " : " << y << endl;
            if (DEBUG) cerr << "before while" << endl;
            while (x != tx || y != ty) {
                if (x != rect[i].first || y != rect[i].second) {
                    if (has_point[x][y]) {
                        return "There is an obstacle at (" + to_string(x) + ", " + to_string(y) + ")";
                    }
                }
                if (used[x][y][dir]) {
                    return "Overlapped rectangles";
                }
                x += dx;
                y += dy;
                if (used[x][y][dir ^ 4]) {// 反対方向
                    return "Overlapped rectangles";
                }
            }
            if (DEBUG) cerr << "after while" << endl;
        }
        return "";
    }
    // O(N)
    void apply_move(vector<P> rect) {
        has_point[rect[0].first][rect[0].second] = true;
        rep(i,4) {
            int x = rect[i].first;
            int y = rect[i].second;
            int tx = rect[(i + 1) % 4].first;
            int ty = rect[(i + 1) % 4].second;
            int dx = signum(tx - x);
            int dy = signum(ty - y);
            int dir;
            rep(k,8) if (DXY[k].first == dx && DXY[k].second == dy) {
                dir = k;
            }
            while (x != tx || y != ty) {
                used[x][y][dir] = true;
                x += dx;
                y += dy;
                used[x][y][dir ^ 4] = true;
            }
        }
    }
};

Input parse_input() {
    int N, M;
    cin >> N >> M;
    vector<P> ps(M);
    rep(i,M) cin >> ps[i].first >> ps[i].second;
    return Input{N, ps};
}

void parse_output(Output output) {
    cout << output.size() << endl;
    rep(i,output.size()) {
        rep(j,4) {
            cout << output[i][j].first << " " << output[i][j].second;
            if (j == 3) cout << endl;
            else cout << " ";
        }
    }
}

void solve() {
    Input input = parse_input();
    Status status = Status(input);

    Output output;
    int found = 0;

    timer.reset();
    while (timer.get() < TIME_LIMIT) {
    rep(i,input.ps.size()) {
        vector<P> rect(4, {0, 0});
        rect[2] = input.ps[i];
        rep(dir,8) {
            int x = rect[2].first;
            int y = rect[2].second;
            int dx = DXY[dir].first;
            int dy = DXY[dir].second;
            while (0 <= x && x < input.N && 0 <= y && y < input.N) {
                if (x != rect[2].first && y != rect[2].second && status.has_point[x][y]) {
                    rect[1] = {x, y};
                    // if (DEBUG) cerr << "found" << endl;
                    break;
                }
                x += dx;
                y += dy;
            }
            x = rect[2].first;
            y = rect[2].second;
            dx = DXY[(dir + 2) % 8].first;
            dy = DXY[(dir + 2) % 8].second;
            while (0 <= x && x < input.N && 0 <= y && y < input.N) {
                if (x != rect[2].first && y != rect[2].second && status.has_point[x][y]) {
                    rect[3] = {x, y};
                    // if (DEBUG) cerr << "found" << endl;
                    break;
                }
                x += dx;
                y += dy;
            }
            int dx21 = rect[1].first - rect[2].first;
            int dy21 = rect[1].second - rect[2].second;
            rect[0].first = rect[3].first + dx21;
            rect[0].second = rect[3].second + dy21;
            if (rect[0].first < 0 || input.N <= rect[0].first || rect[0].second < 0 || input.N <= rect[0].second) {
                rect[1] = rect[3] = {0, 0};
                continue;
            }
            if (DEBUG) cerr << "check" << endl;
            string err = status.check_move(rect);
            if (DEBUG) cerr << "end check" << endl;
            if (err.length() == 0) {
                if (DEBUG) cerr << "apply" << endl;
                status.apply_move(rect);
                output.push_back(rect);
                input.ps.push_back(rect[0]);
                break;
            }
            if (DEBUG) cerr << err << endl;
        }
    }
    }

    parse_output(output);
}

int main(){
    solve();
} 