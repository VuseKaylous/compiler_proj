#include<iostream>
#include<cstring>
using namespace std;

const int N = 50;

struct Node {
    string id;
    int next[N];
    Node(string id) {
        this->id = id;
        memset(next, 0, sizeof(next));
    }
    Node() {
        this->id = "";
        memset(next, 0, sizeof(next));
    }
};

int main() {
    freopen("rules.txt", "r", stdin);
    freopen("table.txt", "r", stdout);
    
}