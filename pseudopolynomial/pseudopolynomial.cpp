#include <bits/stdc++.h>
#include <fstream>
#include <chrono>
#include <numeric>
using namespace std;

struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t h = 0;
        for (int x : v) {
            h ^= hash<int>()(x) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

using StateMap = unordered_map<vector<int>, unsigned long long, VectorHash>;

unsigned long long count_solutions(const vector<vector<int>>& A, const vector<int>& b) {
    int m = A.size();
    int n = A[0].size();

    vector<StateMap> dp(n + 1);

    vector<int> zero(m, 0);
    dp[0][zero] = 1;

    for (int k = 1; k <= n; k++) {
        for (auto& [f, cnt] : dp[k - 1]) {
            dp[k][f] += cnt;


            vector<int> new_f = f;
            bool ok = true;

            for (int i = 0; i < m; i++) {
                new_f[i] += A[i][k - 1];
                if (new_f[i] > b[i]) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                dp[k][new_f] += cnt;
            }
        }
    }

    unsigned long long total = 0;
    for (auto& [f, cnt] : dp[n]) {
        total += cnt;
    }

    return total;
}
unsigned long long exact_count(int n,
                      const vector<vector<int>>& A,
                      const vector<int>& b) {

    unsigned long long total = 0;
    int m = A.size();

    for (int mask = 0; mask < (1 << n); mask++) {  
        vector<int> x(n, 0);
        for (int j = 0; j < n; j++) {
            if (mask & (1 << j)) {
                x[j] = 1;
            }
        }

        bool ok = true;
        for (int i = 0; i < m && ok; i++) {
            int sum = 0;
            for (int j = 0; j < n; j++) {
                sum += A[i][j] * x[j];
            }
            if (sum > b[i]) {
                ok = false;
            }
        }

        if (ok) total++;
    }

    return total;
}


