#include <bits/stdc++.h>
using namespace std;
using P = pair<int, int>;
using Output = vector<vector<P>>;
#define rep(i,n) for (int i = 0; i < (n); ++i)

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
    string check_move(vector<P> rect) {
        return "";
    }
    void apply_move(vector<P> rect) {
    }
};

Input parse_input() {
    int N, M;
    cin >> N >> M;
    vector<P> ps(M);
    rep(i,M) cin >> ps[i].first >> ps[i].second;
    return Input{N, ps};
}

void solve() {
    Input input = parse_input();
    Status status = Status(input);
}

int main(){
    solve();
} 