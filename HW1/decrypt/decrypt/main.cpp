#include<iostream>
#include<string>
#include<vector>
//#define arg
using namespace std;

void Caesar(string ciphertext, string key);
void Playfair(string ciphertext, string key);
void Vernam(string ciphertext, string key);
void RailFence(string ciphertext, string key);
void RowTransition(string ciphertext, string key);

#ifdef arg
int main(int argc, char* argv[]) {
#else
int main() {
    int argc = 7;
    const char* argv[7];
    argv[0] = "";
    argv[1] = "-m"; argv[2] = "caesar";
    argv[3] = "-i"; argv[4] = "GDKKNEQ`JV";
    argv[5] = "-k"; argv[6] = "-27";
#endif // arg
    string method;
    string ciphertext;
    string key;

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-m") == 0) {
            method = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i], "-i") == 0) {
            ciphertext = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i], "-k") == 0) {
            key = argv[i + 1];
            i++;
        }
    }
    if (method == "caesar") {
        Caesar(ciphertext, key);
    }
    else if (method == "playfair") {
        Playfair(ciphertext, key);
    }
    else if (method == "vernam") {
        Vernam(ciphertext, key);
    }
    else if (method == "railfence") {
        RailFence(ciphertext, key);
    }
    else if (method == "row") {
        RowTransition(ciphertext, key);
    }
    return 0;
}

void Caesar(string ciphertext, string key) {
    int shift = stoi(key);
    shift %= 26;
    string plaintext;
    for (int i = 0; i < ciphertext.length(); i++) {
        int alpha_pos = ciphertext[i] - 'A';
        int minus_pos = alpha_pos - shift;
        if (minus_pos < 0) {
            plaintext.push_back(tolower(minus_pos + 26 + 'A'));
        }
        else if (minus_pos >= 26) {
            plaintext.push_back(tolower(minus_pos - 26 + 'A'));
        }
        else { //0~25
            plaintext.push_back(tolower(minus_pos + 'A'));
        }
    }
    cout << plaintext << endl;
}

void Playfair(string ciphertext, string key) {

}

void Vernam(string ciphertext, string key) {

}

void RailFence(string ciphertext, string key) {

}

void RowTransition(string ciphertext, string key) {

}