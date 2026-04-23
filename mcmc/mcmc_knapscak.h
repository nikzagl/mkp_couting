#include <bits/stdc++.h>
#include <fstream>

using namespace std;


double approximate_count(int n,
                         const vector<vector<int>>& A,
                         const vector<int>& b,
                         double eps,
                         double delta);
long long exact_count(int n,
                      const vector<vector<int>>& A,
                        const vector<int>& b);