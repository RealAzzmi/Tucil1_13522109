#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Sequence {
    vector<int> elements;
    int reward;
};

int buffer_size;
int width, height;
int matrix[1000][1000];
int total_sequences;
vector<Sequence> rewards;

vector<int> buffer;
bool taken[1000][1000];
int max_prize;
vector<int> answer;

void search(int row, int col, bool vertical) {
    if ((int)buffer.size() > buffer_size) return;

    int cur_prize = 0;
    for (auto &reward : rewards) {
        for (int i = 0; i <= (int)buffer.size() - (int)reward.elements.size(); ++i) {
            bool valid = true;
            for (int j = i; valid && j < i + (int)reward.elements.size(); ++j) {
                if (buffer[j] != reward.elements[j-i]) valid = false;
            }
            if (valid) cur_prize += reward.reward;
        }
    }
    if (cur_prize > max_prize) {
        max_prize = cur_prize;
        answer = buffer;
    }
    if (vertical) {
        for (int i = 0; i < height; ++i) {
            if (taken[i][col]) continue;
            buffer.push_back(matrix[i][col]);
            taken[i][col] = true;
            search(i, col, false);
            buffer.pop_back();
            taken[i][col] = false;
        }
    } else {
        for (int i = 0; i < width; ++i) {
            if (taken[row][i]) continue;
            buffer.push_back(matrix[row][i]);
            taken[row][i] = true;
            search(row, i, true);
            buffer.pop_back();
            taken[row][i] = false;
        }
    }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> buffer_size;
    cin >> width >> height;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            cin >> hex >> matrix[i][j];
        }
    }
    cin >> dec >> total_sequences;
    for (int i = 0; i < total_sequences; ++i) {
        int frequency;
        cin >> frequency;
        Sequence s;
        s.elements = vector<int>(frequency);
        for (int j = 0; j < frequency; ++j) {
            cin >> hex >> s.elements[j];
        }
        cin >> dec >> s.reward;
        rewards.push_back(s);
    }
    
    for (int col = 0; col < width; ++col ) {
        taken[0][col] = true;
        buffer.push_back(matrix[0][col]);
        search(0, col, true);
        taken[0][col] = false;
        buffer.pop_back();
    }

    cout << max_prize << '\n';
    for (auto i : answer) {
        cout << hex << i << ' ';
    }
    cout << '\n';
}