#include <bits/stdc++.h>
using namespace std;
using P = pair<int, int>;
using Output = vector<vector<P>>;
#define rep(i,n) for (int i = 0; i < (n); ++i)
#define DEBUG 0


random_device seed_gen;
mt19937 engine{seed_gen()};
uniform_real_distribution<> dist{0.0, 1.0};

int board_size;

const double TIME_LIMIT = 4.75;
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

bool inside(P p, int N) {
    return 0 <= p.first && p.first < N && 0 <= p.second && p.second < N;
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

bool comp(vector<P> &a, vector<P> &b, double p) {

    int a_len = max(abs(a[1].first - a[0].first), abs(a[1].second - a[0].second)) + max(abs(a[3].first - a[0].first), abs(a[3].second - a[0].second));
    int b_len = max(abs(b[1].first - b[0].first), abs(b[1].second - b[0].second)) + max(abs(b[3].first - b[0].first), abs(b[3].second - b[0].second));
    if (a_len != b_len) {
        if (dist(engine) > p) return a_len > b_len;
        else return a_len < b_len;
    }

    int c = board_size / 2;
    int a_w = abs(a[0].first - c) + abs(a[0].second - c);
    int b_w = abs(b[0].first - c) + abs(b[0].second - c);
    // if (a_w != b_w) return a_w > b_w;
    if (dist(engine) > p) return a_w > b_w;
    else return a_w < b_w;

    return a_len > b_len;
}

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
            return "Illegal rectangle 1";
        }
        if (dx01 != 0 && dy01 != 0 && abs(dx01) != abs(dy01)) {
            return "Illegal rectangle 2";
        }
        if (rect[1].first + dx03 != rect[2].first || rect[1].second + dy03 != rect[2].second) {
            return "Illegal rectangle 3";
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
    int calc_score() {
        int res = 0;
        int c = board_size / 2;
        rep(x,board_size) {
            rep(y,board_size) {
                if (has_point[x][y]) {
                    res += abs(x - c) * abs(x - c) + abs(y - c) * abs(y - c) + 1;
                }
            }
        }
        return res;
    }
};

Input parse_input() {
    int N, M;
    cin >> N >> M;
    board_size = N;
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

vector<vector<P>> search_rectangle(P point, Status status, int N) {
    vector<vector<P>> res;

    vector<P> rect(4);
    rect[2] = point;
    vector<P> each_dir(8);
    vector<bool> found(8, false);
    rep(dir,8) {
        int dx = DXY[dir].first;
        int dy = DXY[dir].second;
        int x = rect[2].first + dx;
        int y = rect[2].second + dy;
        while (inside({x, y}, N)) {
            if (status.has_point[x][y]) {
                each_dir[dir] = {x, y};
                found[dir] = true;
                break;
            }
            x += dx;
            y += dy;
        }
    }
    rep(dir,8) {
        if (found[dir] && found[(dir + 2) % 8]) {
            rect[1] = each_dir[dir];
            rect[3] = each_dir[(dir + 2) % 8];
            rect[0].first = rect[3].first + rect[1].first - rect[2].first;
            rect[0].second = rect[3].second + rect[1].second - rect[2].second;
            if (inside(rect[0], N)) {
                string err = status.check_move(rect);
                if (err.length() == 0) {
                    res.push_back(rect);
                }
            }
        }
    }
    return res;
}

vector<vector<P>> get_can_moves(Input input, Status status) {
    vector<vector<P>> res;
    rep(i,input.ps.size()) {
        vector<vector<P>> rects = search_rectangle(input.ps[i], status, input.N);
        for (vector<P> rect : rects) {
            res.push_back(rect);
        }
    }
    return res;
}

void add_can_moves(vector<vector<P>> &st, P point, Status status, int N) {
    vector<vector<P>> rects = search_rectangle(point, status, N);
    for (vector<P> rect : rects) {
        st.push_back(rect);
    }

    vector<P> each_dir(8);
    vector<bool> found(8, false);
    rep(dir,8) {
        int dx = DXY[dir].first;
        int dy = DXY[dir].second;
        int x = point.first + dx;
        int y = point.second + dy;
        while (inside({x, y}, N)) {
            if (status.has_point[x][y]) {
                each_dir[dir] = {x, y};
                found[dir] = true;
                break;
            }
            x += dx;
            y += dy;
        }
    }
    rep(dir,8) {
        if (found[dir]) {
            rects = search_rectangle(each_dir[dir], status, N);
            for (vector<P> rect : rects) {
                st.push_back(rect);
            }
        }
    }
}


pair<int, Output> func(Input input, double p) {
    Status status = Status(input);

    Output output;
    vector<vector<P>> can_move = get_can_moves(input, status);

    while (timer.get() < TIME_LIMIT && !can_move.empty()) {
        // sort(can_move.begin(), can_move.end(), comp);

        // vector<P> rect = can_move.back(); can_move.pop_back();
        vector<P> rect = can_move[0];
        for (vector<P> &r : can_move) {
            if (comp(rect, r, p)) {
                rect = r;
            }
        }

        
        string err = status.check_move(rect);
        if (err.length() == 0) {
            if (DEBUG) cerr << "apply" << endl;
            status.apply_move(rect);
            output.push_back(rect);

            vector<vector<P>> next_can_move;
            for (vector<P> r : can_move) {
                if (status.check_move(r).length() == 0) {
                    next_can_move.push_back(r);
                }
            }
            can_move = next_can_move;

            // ここで、 can_move に、新たに発生した有効な長方形を入れる。
            add_can_moves(can_move, rect[0], status, input.N);
            input.ps.push_back(rect[0]);
        }

        if (DEBUG) cerr << err << endl;
    }

    int score = status.calc_score();
    return {score, output};
}
void solve() {
    Input input = parse_input();
    timer.reset();
    int cnt = 0;

    pair<int, Output> best_result = func(input, -1);

    int loop_count = 0;
    int updt_count = 0;
    while (timer.get() < TIME_LIMIT) {
        loop_count++;
        pair<int, Output> result = func(input, dist(engine) / 20);
        if (result.first > best_result.first) {
            best_result = result;
            updt_count++;
        }
    }
    cerr << "loop: " << loop_count << endl;
    cerr << "time: " << timer.get() << endl;
    cerr << "updt: " << updt_count << endl;
    // cerr << "rectangle count: " << cnt << endl;
    parse_output(best_result.second);
}

void test();

int main(){
    solve();
    // test();
} 

void test () {
    Input input = Input{7, {{2,1},{3,1},{3,2},{1,3},{2,4}}};
    Status status = Status(input);
    assert("" == status.check_move({{2,2}, {2,1}, {3,1}, {3,2}}));
    assert("" == status.check_move({{4,2}, {3,1}, {1,3}, {2,4}}));
    auto st = get_can_moves(input, status);
    assert(st.size() == 2);
    while (!st.empty()) {
        cerr << "{";
        rep(i,st.back().size()) {
            cerr << "{" << st.back()[i].first << ", " << st.back()[i].second << "}";
            if (i < st.back().size() - 1) cerr << ", ";
        }
        cerr << "}" << endl;
        st.pop_back();
    }
}