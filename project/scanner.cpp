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

void splitSeparators(string si) {
    int pos = -1;
    for (int i = 0; i < separators.size(); i++) {
        for (int j = 0; j < si.size(); j++) {
            if (si.substr(j, 1) == separators[i]) {
                // splitSeparators(si.substr(0, i));
                // words.push_back(separators[i]);
                // splitSeparators(si.substr(i+1));
                pos = i;
                // return;
                goto splitPoint;
            }
        }
    }
    for (int i = 0; i < operators.size(); i++) {
        for (int j = 0; j < si.size(); j++) {
            if (si.substr(j, 1) == separators[i]) {
                // splitSeparators(si.substr(0, i));
                // words.push_back(separators[i]);
                // splitSeparators(si.substr(i+1));
                pos = i;
                // return;
                goto splitPoint;
            }
        }
    }
    words.push_back(si);
    return;

splitPoint:
    splitSeparators(si.substr(0, pos));
    words.push_back(si.substr(pos, 1));
    splitSeparators(si.substr(pos+1));
}

int main() {
    // string linkFile; cin >> linkFile ;
    freopen("test.vc", "r", stdin);
    freopen("test.vctok", "w", stdout);
    string s;
    
    while (cin >> s) {
        splitSeparators(s);
    }
    for (int i = 0; i < words.size(); i++) {
        if (words[i][0] == '"') {
            s = "";
            for (int j = i; j < words.size(); j++) {
                s = s + words[j];
                if (words[j].back() == '"') {
                    words.erase(words.begin() + i + 1, words.begin() + j + 1);
                    break;
                }
            }
            words[i] = s;
        }
        if (words[i][0] == '\'') {
            s = "";
            for (int j = i; j < words.size(); j++) {
                s = s + words[j];
                if (words[j].back() == '\'') {
                    words.erase(words.begin() + i + 1, words.begin() + j + 1);
                    break;
                }
            }
            words[i] = s;
        }
    }
    words.push_back("$");
    for (int i = 0; i < words.size(); i++) cout << words[i] << "\n" ;
}