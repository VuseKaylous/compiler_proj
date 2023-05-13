#include<iostream>
#include<regex>
#include<cstring>
using namespace std;

vector<string> keywords{"int", "float", "void", "boolean", "if", "else", "for", "while", "break", "continue", "return", "main"};
// string signs[] = {"?", "(", ")", "[", "]","{", "}", ",", ";", "!", "<", ">", "<=", ">=", "==", "!=", "=", "+", "-", "*", "/", "||", "&&"};
vector<string> operators{"<=", ">=", "<", ">", "==", "!=", "=", "+", "-", "*", "/", "!", "||", "&&"};
vector<string> separators{"(", ")", "[", "]","{", "}", ",", ";"};
// regex signs ("([\?:()[]{},;])");

vector<string> words;

bool checkPrefix(string prefix, string si) {
    if (prefix.size() > si.size()) return false;
    for (int i = 0; i < prefix.length(); i++) {
        if (prefix[i] != si[i]) return false;
    }
    return true;
}

string get_substr(string si, int pos, int len) {
    string rt = "";
    for (int i = pos; i < pos + len; i++) if (i < si.size()) rt = rt + si[i];
    return rt;
}

string get_substr(string si, int pos) {
    string rt = "";
    for (int i = pos; i < si.size(); i++) rt = rt + si[i];
    return rt;
}

void splitSeparators(string si) {
    if (si == "") return;
    if (si[0] == '"') {
        words.push_back(si);
        return;
    }
    int pos = -1;
    bool check = true;
    int len = 1;
    for (int i = 0; i < separators.size(); i++) {
        if (si == separators[i]) {
            words.push_back(si);
            return;
        }
        for (int j = 0; j < si.size(); j++) {
            if (get_substr(si, j, 1) == separators[i]) {
                pos = j;
                check = false;
                len = separators[i].size();
                break;
                // goto splitPoint;
            }
        }
        if (!check) break;
    }
    if (check) for (int i = 0; i < operators.size(); i++) {
        if (si == operators[i]) {
            words.push_back(si);
            return;
        }
        for (int j = 0; j < si.size(); j++) {
            if (j + operators[i].size() - 1 < si.size() && get_substr(si, j, operators[i].size()) == operators[i]) {
                pos = j;
                check = false;
                len = operators[i].size();
                break;
            }
        }
        if (!check) break;
    }
    // cout << si << " " << check << "\n";
    if (check) {
        words.push_back(si);
        return;
    }
    if (pos > 0) splitSeparators(get_substr(si, 0, pos));
    words.push_back(get_substr(si, pos, len));
    if (pos + 1 < si.size()) splitSeparators(si.substr(pos+len));
}

int main() {
    // string linkFile; cin >> linkFile ;
    freopen("test.vc", "r", stdin);
    freopen("test.vctok", "w", stdout);
    string s;
    bool skip = false;
    vector<string> temp_words;
    bool isString = false;
    string concatStr = "";
    while (cin >> s) {
        if (s.size() >= 2 && s[0] == '/' && s[1] == '*') skip = true;
        if (!skip) {
            // splitSeparators(s);
            temp_words.push_back(s);
        }
        if (s.size() >= 2 && s[s.size() - 1] == '/' && s[s.size() - 2] == '*') skip = false;
    }
    for (int i = 0; i < temp_words.size(); i++) {
        for (int j = 0; j < temp_words[i].size(); j++) {
            if (temp_words[i][j] == '"') {
                if (!isString) {
                    isString = true;
                    words.push_back(get_substr(temp_words[i], 0, j));
                    concatStr = "";
                } else {
                    isString = false;
                    words.push_back(concatStr + '"');
                    temp_words[i] = get_substr(temp_words[i], j + 1);
                    j = -1;
                }
            }
            if (isString) concatStr = concatStr + temp_words[i][j];
        }
        if (isString) concatStr = concatStr + " ";
        else words.push_back(temp_words[i]);
    }
    temp_words = words;
    words.clear();
    for (int i = 0; i < temp_words.size(); i++) {
        splitSeparators(temp_words[i]);
    }
    // for (int i = 0; i < words.size(); i++) {
    //     if (words[i][0] == '\'') {
    //         s = "";
    //         for (int j = i; j < words.size(); j++) {
    //             s = s + " " + words[j];
    //             if (words[j].back() == '\'') {
    //                 words.erase(words.begin() + i + 1, words.begin() + j + 1);
    //                 break;
    //             }
    //         }
    //         words[i] = s;
    //     }
    // }
    words.push_back("$");
    for (int i = 0; i < words.size(); i++) cout << words[i] << "\n" ;
}