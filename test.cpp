#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define ll long long
#define db long double
#define x first
#define y second
#define mp make_pair
#define pb push_back
#define all(a) a.begin(), a.end()

using namespace std;

const int N = 6;

int g[N][N];

const int INF = 1e9;

int gen_edge(int i, int j) {
	return rand() % 100000;
}

template <int n>
int hamilton_cycle(int gr[n][n]) {

	vector<int> v(n);
	for (int i = 0; i < n; ++i) {
		v[i] = i;
	}

	int ans = INF;
	int cnt = 0;
	do {
		int tut = 0;
		for (int i = 0; i < v.size(); ++i) {
			int w = v[i], to = v[(i+1) % v.size()];
			tut += gr[w][to];
			tut = min(tut, INF);
		}

		ans = min(ans, tut);

	}
	while (next_permutation(all(v)));
	return ans;

}

bool good(char c) {
	if (c >= 'a' && c <= 'z') return true;
	if (c >= '0' && c <= '9') return true;
	if (c >= 'A' && c <= 'Z') return true;
	if (c >= '_' && c <= '_') return true;
	return false;
}

string get_string(int x) {
	if (x==0) return "0";
	string t = "";
	while (x) {
		t += (char) ((x%10) + '0');
		x /= 10;
	}

	reverse(all(t));
	return t;

}

void upd(string &s) {
	if (s.size() <= 6) return;

	int len = s.size();
	if (s[len - 6] == 'x') {
		int A = s[len - 5] - '0', B = s[len - 3] - '0';
		string W = "";
		W += get_string(g[A][B]);
		for (int e = len - 6; e < len; ++e) {
			W += s[e];
		}
		s = W;
	}

	else return;

}

template <int n>
int lp_solution(int gr[n][n]) {

	int pid = fork();

	if (pid == 0) {

		freopen("input.txt", "w", stdout);
		int fd = open("base.txt", NULL);

		string t;
		char ch;

		while (read(fd, &ch, 1)) {
			t += ch;
		}

		string q = "";
		int start = 0;

		while (start < t.size()) {
			if (!good(t[start])) {
				q += t[start++];
				continue;
			}

			string spec = "";

			while (good(t[start])) {
				spec += t[start++];
			}

			upd(spec);
			q += spec;

		}

		close(fd);

		cout << q << endl;

		freopen("output.txt", "w", stdout);

		execl("qsopt", "qsopt", "-L", "input.txt", NULL);
		exit(0);

	}

	else {
		waitpid(pid, NULL, 0);

		int fd = open("output.txt", NULL);

		string t;
		char ch;

		while (read(fd, &ch, 1)) {
			t += ch;
		}

		close(fd);

		for (int i = 0; i + 8 < t.size(); ++i) {
			string sbs = "";
			for (int j = 0; j < 8; ++j) sbs += t[i+j];
			if (sbs == "LP Value") {
				int ptr = i + 8;
				while (!good(t[ptr])) ptr++;
				string res = "";
				while (t[ptr] != '.') res += t[ptr++];

				int ans = 0;
				int deg = 1;
				reverse(all(res));

				for (auto ch : res) {
					ans += deg * (ch - '0');
					deg *= 10;
				}

				return ans;

			}
		}
	}

}


main(){
#ifdef LOCAL
	freopen("A_input.txt", "r", stdin);
	//freopen("A_output.txt", "w", stdout);
#endif
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	srand(time(NULL));

	int t = 0;

	while (true) {
		for (int i = 0; i < N; ++i) {
			g[i][i] = INF;
			for (int j = i+1; j < N; ++j) {
				g[i][j] = gen_edge(i, j);
				g[j][i] = g[i][j];
			}
		}

		int A = hamilton_cycle<N>(g);
		int B = lp_solution<N>(g);

		if (A != B) {
			cout << "GRAPH:\n";
			cout << N << endl;
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) {
					if (g[i][j] == INF) cout << "X ";
					else cout << g[i][j] << " ";
				}
				cout << '\n';
			}
			cout << "HAMILTONIAN CYCLE SIZE IS " << A << endl;
			cout << "BUT IN THE LP ALGORTIHM " << B << endl;
			exit(0);
		}

		else {
			cout << "OK " << (t++) << "-th LAUNCH " << A << endl;
		}

	}
}