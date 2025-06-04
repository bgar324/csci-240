#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

int main() {
    const int N = 98304;
    vector<int> data(N);
    ifstream infile("large100k.txt");
    for (int i = 0; i < N; ++i) {
        infile >> data[i];
    }
    infile.close();

    auto start1 = chrono::high_resolution_clock::now();
    ofstream out1("output_text.txt");
    for (int i = 0; i < N; ++i) {
        out1 << data[i] << "\n";
    }
    out1.close();
    auto end1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1).count();
    cout << "Text output time: " << duration1 << " ms\n";

    auto start2 = chrono::high_resolution_clock::now();
    ofstream out2("output_bin_one.bin", ios::binary);
    for (int i = 0; i < N; ++i) {
        out2.write(reinterpret_cast<char*>(&data[i]), sizeof(int));
    }
    out2.close();
    auto end2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end2 - start2).count();
    cout << "Binary output (one value at a time) time: " << duration2 << " ms\n";

    auto start3 = chrono::high_resolution_clock::now();
    ofstream out3("output_bin_buffer.bin", ios::binary);
    for (int i = 0; i < N; i += 256) {
        out3.write(reinterpret_cast<char*>(&data[i]), 256 * sizeof(int));
    }
    out3.close();
    auto end3 = chrono::high_resolution_clock::now();
    auto duration3 = chrono::duration_cast<chrono::milliseconds>(end3 - start3).count();
    cout << "Binary output (256 values at a time) time: " << duration3 << " ms\n";

    return 0;
}
