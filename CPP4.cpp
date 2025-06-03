#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>

using namespace std;

bool isValidCode(const string& code) {
    if (code.length() < 1 || code.length() > 5) return false;
    for (char c : code) {
        if (c < '1' || c > '9') return false;
    }
    return true;
}

int main() {
    int N, K;
    cin >> N >> K;

    if (N < 2 || N > 10 || K < 2 || K > 6) {
        cerr << "Число букв должно быть от 2 до 10, а длина слов — от 2 до 6." << endl;
        return 1;
    }

    vector<string> codes(N);
    for (int i = 0; i < N; ++i) {
        cin >> codes[i];
        if (!isValidCode(codes[i])) {
            cerr << "Шифр должен содержать от 1 до 5 цифр от 1 до 9." << endl;
            return 1;
        }
    }

    unordered_map<string, int> encryptedWords;

    vector<int> word(K, 0);
    while (true) {
        string encrypted = "";
        for (int i = 0; i < K; ++i) {
            encrypted += codes[word[i]];
        }
        encryptedWords[encrypted]++;

        int pos = K - 1;
        while (pos >= 0) {
            if (word[pos] < N - 1) {
                word[pos]++;
                break;
            } else {
                word[pos] = 0;
                pos--;
            }
        }
        if (pos < 0) break;
    }

    int count = 0;
    for (const auto& pair : encryptedWords) {
        if (pair.second == 1) count++;
    }

    cout << count << endl;
    return 0;
}

