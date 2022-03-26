#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <utility>
#define arg
using namespace std;

void Caesar(string ciphertext, string key);
//decrypt.exe -m caesar -i LIPPSJVEOA -k 4
void Playfair(string ciphertext, string key);
//decrypt.exe -m playfair -i SMODSKXDVEHLONDMZBXMMONZFI -k plairfairexample
void Vernam(string ciphertext, string key);
//decrypt.exe -m vernam -i QIMK -k xm
void RailFence(string ciphertext, string key);
//decrypt.exe -m railfence -i WECRLTEERDSOEEFEAOCAIVDEN -k 3
void RowTransition(string ciphertext, string key);
//decrypt.exe -m row -i TTNAAPTMTSUOAODWCOIXKNLYPETZ -k 4312567

#ifdef arg
int main(int argc, char* argv[]) {
#else
int main() {
    int argc = 7;
    const char* argv[7];
    argv[0] = "";
    argv[1] = "-m"; argv[2] = "row";
    argv[3] = "-i"; argv[4] = "TTNAAPTMTSUOAODWCOIXKNLYPETZ"; //WECRLTEERDSOEEFEAOCAIVDEN
    argv[5] = "-k"; argv[6] = "4312567";
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
    cout << plaintext;
}

void Playfair(string ciphertext, string key) {
    char table[5][5];
    bool encounter[26] = {0};
    string no_repeat_key;
    string remain_char;
    int not_in_char = 0;
    int J_index = 'j' - 'a';
    for (int i = 0; i < key.length(); i++) {
        if (key[i] == 'j') key[i] = 'i';

        if (encounter[key[i]-'a']  == false) {
            encounter[key[i] - 'a'] = true;
            no_repeat_key.push_back(key[i]);
        }
    }
    for (int i = 0; i < 26; i++) {
        if (encounter[i] == false) {
            if (i != J_index) remain_char.push_back(i + 'a');
        }
    }
    key = no_repeat_key;
    int key_index = 0;
    int remain_index = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (key_index < key.length()) {
                table[i][j] = key[key_index];
                key_index++;
            }
            else {
                table[i][j] = remain_char[remain_index];
                remain_index++;
            }
        }
    }
    if ((ciphertext.length()) % 2 != 0) {
        cout << "ciphertext error";
        return;
    }
    vector<pair<char,char>> pairs;
    for (int i = 0; i < ciphertext.length(); i += 2) {
        pairs.push_back(make_pair(ciphertext[i], ciphertext[i + 1]));
    }
    auto find_pos = [&](char in) -> pair<int, int> {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (tolower(in) == table[i][j]) {
                    return make_pair(i,j);
                }
            }
        }
    };
    string plaintext;
    for (int i = 0; i < pairs.size(); i++) {
        pair<int, int> pos1 = find_pos(pairs[i].first);
        pair<int, int> pos2 = find_pos(pairs[i].second);
        if (pos1.first == pos2.first) {
            plaintext.push_back(tolower(table[pos1.first][pos1.second != 0 ? pos1.second - 1 : 4]));
            plaintext.push_back(tolower(table[pos2.first][pos2.second != 0 ? pos2.second - 1 : 4]));
        }
        else if (pos1.second == pos2.second) {
            plaintext.push_back(tolower(table[pos1.first != 0 ? pos1.first - 1 : 4][pos1.second]));
            plaintext.push_back(tolower(table[pos2.first != 0 ? pos2.first - 1 : 4][pos2.second]));
        }
        else {
            plaintext.push_back(tolower(table[pos1.first][pos2.second]));
            plaintext.push_back(tolower(table[pos2.first][pos1.second]));
        }
    }
    cout << plaintext;
}

//vernam(autokey)
void Vernam(string ciphertext, string key) {
    string plaintext;
    for (int i = 0; i < key.length(); i++) {
        plaintext.push_back(((ciphertext[i]-'A') ^ (key[i]-'a')) + 'a');
        if (ciphertext.length() > key.length()) {
            key.push_back(plaintext.back());
        }
    }
    cout << plaintext;
}

void RailFence(string ciphertext, string key) {
    int rail_count = stoi(key);
    int fence_len = 2 + (rail_count - 2) * 2; //V
    int index = 0;
    string plaintext;
    plaintext.resize(ciphertext.length());
    for (int i = 0; i < rail_count; i++) {
        if (i == 0) {
            for (int target_index = 0; true;  target_index+=fence_len) {
                if (target_index >= plaintext.length()) break;
                plaintext[target_index] = tolower(ciphertext[index++]);
            }
        }
        else if (i == rail_count - 1) {
            for (int target_index = i; true; target_index += fence_len) {
                if (target_index >= plaintext.length()) break;
                plaintext[target_index] = tolower(ciphertext[index++]);
            }
        }
        else {
            for (int target_index1 = i, target_index2 = fence_len - i; true;target_index1 += fence_len, target_index2 += fence_len) {
                if (target_index1 >= plaintext.length()) break;
                plaintext[target_index1] = tolower(ciphertext[index++]);
                if (target_index2 >= plaintext.length()) break;
                plaintext[target_index2] = tolower(ciphertext[index++]);
            }
        }
    }
    cout << plaintext;
}

void RowTransition(string ciphertext, string key) {
    int cols = key.length();
    int max_row = ciphertext.length() / cols;
    int remain_count = ciphertext.length() % cols;
    vector<int> col_to_rows;
    vector<int> order_to_col_index;
    string plaintext;
    plaintext.resize(ciphertext.size());
    for (int i = 0; i < cols; i++) {
        if (remain_count != 0) {
            if (i >= remain_count) {
                col_to_rows.push_back(max_row);
            }
            else {
                col_to_rows.push_back(max_row + 1);
            }
        }
        else {
            col_to_rows.push_back(max_row);
        }        
    }
    for (int i = '1'; i <= (cols + '0'); i++) {
        for (int j = 0; j < key.length(); j++) {
            if (i == key[j]) {
                order_to_col_index.push_back(j);
                break;
            }
        }
    }
    int index = 0;
    for (int i = 0; i < key.length(); i++) { //loop '1' to end
        int col = order_to_col_index[i];
        int rows = col_to_rows[col];
        for (int j = 0; j < rows;j++) {
            plaintext[j * cols + col] = tolower(ciphertext[index++]);
        }
    }
    cout << plaintext;
}
