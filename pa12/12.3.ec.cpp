#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

int main() {
    const int N = 98304;
    vector<int> dataText(N);
    auto startText = chrono::high_resolution_clock::now();
    ifstream inText("output_text.txt");
    for (int i = 0; i < N; ++i) {
        inText >> dataText[i];
    }
    inText.close();
    auto endText = chrono::high_resolution_clock::now();
    auto durText = chrono::duration_cast<chrono::milliseconds>(endText - startText).count();
    cout << "Text input time: " << durText << " ms\n";
    cout << "Text first 5: ";
    for (int i = 0; i < 5; ++i) {
        cout << dataText[i] << (i < 4 ? " " : "\n");
    }
    cout << "Text last 5: ";
    for (int i = N - 5; i < N; ++i) {
        cout << dataText[i] << (i < N - 1 ? " " : "\n");
    }

    vector<int> dataBinOne(N);
    auto startBinOne = chrono::high_resolution_clock::now();
    ifstream inBinOne("output_bin_one.bin", ios::binary);
    for (int i = 0; i < N; ++i) {
        inBinOne.read(reinterpret_cast<char*>(&dataBinOne[i]), sizeof(int));
    }
    inBinOne.close();
    auto endBinOne = chrono::high_resolution_clock::now();
    auto durBinOne = chrono::duration_cast<chrono::milliseconds>(endBinOne - startBinOne).count();
    cout << "Binary input (one at a time) time: " << durBinOne << " ms\n";
    cout << "BinOne first 5: ";
    for (int i = 0; i < 5; ++i) {
        cout << dataBinOne[i] << (i < 4 ? " " : "\n");
    }
    cout << "BinOne last 5: ";
    for (int i = N - 5; i < N; ++i) {
        cout << dataBinOne[i] << (i < N - 1 ? " " : "\n");
    }

    vector<int> dataBinBuf(N);
    auto startBinBuf = chrono::high_resolution_clock::now();
    ifstream inBinBuf("output_bin_buffer.bin", ios::binary);
    const int B = 256;
    int buffer[B];
    for (int i = 0; i < N; i += B) {
        inBinBuf.read(reinterpret_cast<char*>(buffer), B * sizeof(int));
        for (int j = 0; j < B; ++j) {
            dataBinBuf[i + j] = buffer[j];
        }
    }
    inBinBuf.close();
    auto endBinBuf = chrono::high_resolution_clock::now();
    auto durBinBuf = chrono::duration_cast<chrono::milliseconds>(endBinBuf - startBinBuf).count();
    cout << "Binary input (256 at a time) time: " << durBinBuf << " ms\n";
    cout << "BinBuf first 5: ";
    for (int i = 0; i < 5; ++i) {
        cout << dataBinBuf[i] << (i < 4 ? " " : "\n");
    }
    cout << "BinBuf last 5: ";
    for (int i = N - 5; i < N; ++i) {
        cout << dataBinBuf[i] << (i < N - 1 ? " " : "\n");
    }

    return 0;
}