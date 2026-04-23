#include "mcmc_knapscak.h"
using namespace std;


int main(int argc, char* argv[])
{
    if (argc != 6)
    {
        cerr << "Usage: " << argv[0] << " input.txt eps delta n_iterations output.txt" << endl;
        return 1;
    }
    
    fstream input_file;
    fstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[5], ios::out);
    long long n_iterations = stoll(argv[4]);
    double eps = stod(argv[2]);
    double delta = stod(argv[3]);

    int n, m;
    input_file >> n >> m;
    vector<vector<int>> A(m, vector<int>(n));
    vector<int> b(m);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            input_file >> A[i][j];
        }
    }
    for(int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < m; i++) {
        input_file >> b[i];
    }
        auto start = chrono::steady_clock::now();
        long long exact_result = exact_count(n, A, b);
        auto end = chrono::steady_clock::now();
        long long exact_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Exact count = " << (long long)exact_result << ", Time = " << exact_time << " ms" << "\n";
        output_file <<"Iteration,Approximate result,Exact result,Time approx,Time exact\n";
    for (int i = 0; i < n_iterations; i++) {
        output_file << i+1 << ",";
        start = chrono::steady_clock::now();
        double approx_result = approximate_count(n, A, b, eps, delta);
        end = chrono::steady_clock::now();
        long long approx_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Iteration " << i+1 << ": Approximate count = " << (long long)approx_result << ", Time = " << approx_time << " ms" << endl;
        
        output_file << (long long)approx_result << "," << (long long)exact_result << "," << approx_time << "," << exact_time << "\n";
    }
}