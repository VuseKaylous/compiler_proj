#include<iostream>
#include<regex>
#include<cstring>
using namespace std;

vector<string> keywords{"int", "float", "void", "boolean", "if", "else", "for", "while", "break", "continue", "return", "main"};
// string signs[] = {"?", "(", ")", "[", "]","{", "}", ",", ";", "!", "<", ">", "<=", ">=", "==", "!=", "=", "+", "-", "*", "/", "||", "&&"};
vector<string> operators{"<", ">", "<=", ">=", "==", "!=", "=", "+", "-", "*", "/", "!", "||", "&&"};
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
    for (int i = pos; i < pos + len; i++) if (pos + len < si.size()) rt = rt + si[i];
    return rt;
}

string get_substr(string si, int pos) {
    string rt = "";
    for (int i = pos; i < si.size(); i++) rt = rt + si[i];
    return rt;
}

void splitSeparators(string si) {
    if (si == "") return;
    int pos = -1;
    bool check = true;
    for (int i = 0; i < separators.size(); i++) {
        for (int j = 0; j < si.size(); j++) {
            if (get_substr(si, j, 1) == separators[i]) {
                pos = i;
                check = false;
                break;
                // goto splitPoint;
            }
        }
        if (!check) break;
    }
    if (check) for (int i = 0; i < operators.size(); i++) {
        for (int j = 0; j < si.size(); j++) {
            if (j + operators[i].size() - 1 < si.size() && get_substr(si, j, operators[i].size()) == operators[i]) {
                pos = i;
                check = false;
                break;
            }
        }
        if (!check) break;
    }
    if (pos == -1) {
        words.push_back(si);
        return;
    }
    if (pos > 0) splitSeparators(get_substr(si, 0, pos));
    words.push_back(get_substr(si, pos, 1));
    if (pos + 1 < si.size()) splitSeparators(si.substr(pos+1));
}

int main() {
    // string linkFile; cin >> linkFile ;
    freopen("test.vc", "r", stdin);
    freopen("test.vctok", "w", stdout);
    string s;
    bool skip = false;
    while (cin >> s) {
        if (s.size() >= 2 && s[0] == '/' && s[1] == '*') skip = true;
        if (!skip) splitSeparators(s);
        if (s.size() >= 2 && s[s.size() - 1] == '/' && s[s.size() - 2] == '*') skip = false;
        // if (s.size() >= 2) cout << s.substr(0, 2) << "\n";
        // words.push_back(s);
    }
    // for (int i = 0; i < words.size(); i++) {
    //     if (words[i][0] == '"') {
    //         s = "";
    //         for (int j = i; j < words.size(); j++) {
    //             s = s + " " + words[j];
    //             if (words[j].back() == '"') {
    //                 words.erase(words.begin() + i + 1, words.begin() + j + 1);
    //                 break;
    //             }
    //         }
    //         words[i] = s;
    //     }
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
    //     if (words[i].size() >= 2 && words[i][0] == '/' && words[i][1] == '*') {
    //         s = "";
    //         for (int j = i; j < words.size(); j++) {
    //             s = s + " " + words[j];
    //             if (words[j].size() >= 2 &&
    //                 words[j][words[j].size() - 1] == '/' &&
    //                 words[j][words[j].size() - 2] == '*') {
    //                 words.erase(words.begin() + i, words.begin() + j + 1);
    //                 break;
    //             }
    //         }
    //     }
    // }
    words.push_back("$");
    for (int i = 0; i < words.size(); i++) cout << words[i] << "\n" ;
}