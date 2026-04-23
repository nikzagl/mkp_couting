#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include "pseudopolynomial.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    fstream input_fs(argv[1], ios::in);
    fstream output_fs(argv[2], ios::out);
    int n, m;
    input_fs >> n >> m;
    
    vector<vector<int>> A(m, vector<int>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            input_fs >> A[i][j];
        }
    }
    for(int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    vector<int> b(m);
    for (int i = 0; i < m; i++) {
        input_fs >> b[i];
    }
    output_fs <<"Iteration, Time dynamic programming (ms), Time brute force (ms)\n";
    int n_iterations = 15;
    int time_dp, time_bf;
    for (int i = 0; i < n_iterations; i++) {
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    cout << "Iteration " << i + 1 << ": " << count_solutions(A, b) << endl;
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    time_dp = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    start = chrono::steady_clock::now();  
    cout << exact_count(n, A, b) << endl;
    end = chrono::steady_clock::now();
    time_bf = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout<<"time_dp: "<<time_dp<<" ms, time_bf: "<<time_bf<<" ms"<<endl;
   
    output_fs << i + 1 << ", " << time_dp << ", " << time_bf << "\n";
    }
    output_fs.close();
}
