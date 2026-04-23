#include <bits/stdc++.h>
#include <fstream>
#include <chrono>


using namespace std;


bool feasible(const vector<int>& x, const vector<vector<int>>& A, const vector<int>& b) {
    int m = A.size();
    int n = x.size();

    for (int i = 0; i < m; i++) {
        int sum = 0;
        for (int j = 0; j < n; j++) {
            sum += A[i][j] * x[j];
        }
        if (sum > b[i]) return false;
    }
    return true;
}

bool in_Omega_i(const vector<int>& x, int limit) {
    int sum = 0;
    for (int v : x) sum += v;
    return sum <= limit;
}

void mcmc_step(vector<int>& x,
               const vector<vector<int>>& A,
               const vector<int>& b,
               int limit,
               mt19937& rng) {

    int n = x.size();
    uniform_int_distribution<int> dist(0, n - 1);

    int j = dist(rng);

    vector<int> y = x;
    y[j] = 1 - y[j];

    if (in_Omega_i(y, limit) && feasible(y, A, b)) {
        x = y;
    }
}
vector<int> sample_Omega_i(int n,
                  const vector<vector<int>>& A,
                  const vector<int>& b,
                  int limit,
                  mt19937& rng,
                  int steps,
                  vector<int> start) {

    vector<int> x = start;

    for (int step = 0; step < steps; step++) {
        mcmc_step(x, A, b, limit, rng);
    }

    return x;
}
double estimate_ratio(int n,  
                      const vector<vector<int>>& A,
                      const vector<int>& b,
                      int i,
                      long long M,
                      mt19937& rng,
                      long long mcmc_steps,
                      vector<int>& state) {

    int X = 0;

    for (int t = 0; t < M; t++) {
        state = sample_Omega_i(n, A, b, i, rng, mcmc_steps, state);

        if (in_Omega_i(state, i - 1)) {
            X++;
        }
    }

    return (double)X / M;
}

double approximate_count(int n,
                         const vector<vector<int>>& A,
                         const vector<int>& b,
                         double eps,
                         double delta) {

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    double log_term = log(2.0 * n / delta);
    long long M = (long long)(n * log_term / (eps * eps));

    long long mcmc_steps = (long long)(4 * pow(n, (A.size()/4)) / eps);
    std::cout<<"M=" << M << " mcmc_steps=" << mcmc_steps << endl;
    double result = 1.0;

    vector<int> state(n, 0);

    for (int i = 1; i <= n; i++) {
        double r = estimate_ratio(n, A, b, i, M, rng, mcmc_steps, state);

        if (r == 0) {
            cerr << "Warning: ratio is zero at i=" << i << endl;
            return -1;
        }

        result *= (1.0 / r);

        cout << "i=" << i << " r=" << r << " partial=" << result << endl;
    }

    return result;
}

long long exact_count(int n,
                      const vector<vector<int>>& A,
                      const vector<int>& b) {

    long long total = 0;
    int m = A.size();

    for (int mask = 0; mask < (1 << n); mask++){
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
