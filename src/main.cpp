// Lihat cara compile yang detail di README karena tidak bisa dicompile dengan biasa.
#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <iomanip>
#include <thread>

using std::vector, std::string, std::pair;
using std::cout, std::cerr, std::cin;
using std::ifstream, std::ofstream, std::stringstream;
using std::thread, std::ref;

struct Sequence {
    vector<int> elements;
    int value;
};

struct Configuration {
    int width, height;
    vector<vector<int>> matrix;
    int total_rewards;
    vector<Sequence> rewards;
    int maximum_buffer_size;
};

void search(const Configuration &config, const int &row, const int &col, const bool &vertical, vector<vector<bool>> &taken, vector<int> &buffer, vector<int> &optimal_token_path, vector<pair<int, int>> &path, vector<pair<int, int>> &optimal_path, int &max_reward) {
    int cur_prize = 0;
    for (const auto &reward : config.rewards) {
        for (int i = 0, buf_sz = (int)buffer.size(), rew_sz = (int)reward.elements.size(); i <= buf_sz - rew_sz; ++i) {
            bool valid = true;
            for (int j = i; valid && j < i + rew_sz; ++j) {
                if (buffer[j] != reward.elements[j-i]) {
                    valid = false;
                }
            }
            if (valid) {
                cur_prize += reward.value;
                break;
            }
        }
    }
    if (cur_prize > max_reward) {
        max_reward = cur_prize;
        optimal_token_path = buffer;
        optimal_path = path;
    }
    if (buffer.size() == config.maximum_buffer_size) {
        return;
    }
    if (vertical) {
        for (int i = 0; i < config.height; ++i) {
            if (!taken[i][col]) {
                buffer.push_back(config.matrix[i][col]);
                taken[i][col] = true;
                path.emplace_back(col, i);                
                search(config, i, col, false, taken, buffer, optimal_token_path, path, optimal_path, max_reward);
                path.pop_back();
                taken[i][col] = false;
                buffer.pop_back();
            }
        }
    } else {
        for (int i = 0; i < config.width; ++i) {
            if (!taken[row][i]) {
                buffer.push_back(config.matrix[row][i]);
                taken[row][i] = true;
                path.emplace_back(i, row);
                search(config, row, i, true, taken, buffer, optimal_token_path, path, optimal_path, max_reward);
                path.pop_back();
                taken[row][i] = false;
                buffer.pop_back();
            }
        }
    }
}

bool read_text_file(Configuration &config) {
    cout << 
        "Format text file:\n\n"
        
        "buffer_size\n"
        "matrix_width matrix_height\n"
        "matrix (jumlah row sebanyak matrix_height dan jumlah kolom sebanyak matrix_width)\n"
        "number_of_sequences\n"
        "sequences_1\n"
        "sequences_1_reward\n"
        "sequences_2\n"
        "sequences_2_reward\n"
        "...\n"
        "sequences_n\n"
        "sequences_n_reward\n\n"

        "Masukkan alamat lengkap file: ";
    cout.flush();

    string filename;
    cin >> filename;
    ifstream input_file(filename);

    if (!input_file.is_open()) {
        cerr << "File gagal dibuka.\n\n";
        return false;
    }
    
    input_file >> config.maximum_buffer_size;
    input_file >> config.width >> config.height;
    config.matrix.resize(config.height);
    for (int i = 0; i < config.height; ++i) {
        config.matrix[i].resize(config.width);
    }
    for (int i = 0; i < config.height; ++i) {
        for (int j = 0; j < config.width; ++j) {
            char a, b;
            input_file >> a >> b;
            config.matrix[i][j] = 256 * a + b;
        }
    }
    input_file >> config.total_rewards;
    input_file.ignore();
    for (int i = 0; i < config.total_rewards; ++i) {
        Sequence s;
        string elements_line;
        getline(input_file, elements_line);

        stringstream ss(elements_line);
        char a, b;
        while (ss >> a >> b) {
            s.elements.push_back(256 * a + b);
        }
        input_file >> s.value;
        input_file.ignore();
        config.rewards.push_back(s);
    }
    return true;
}

void generate_game(Configuration &config) {
    cout << 
        "\nFormat input:\n\n"
        "Jumlah token unik\n"
        "Token-token\n"
        "Ukuran buffer\n"
        "Ukuran matriks\n"
        "Jumlah sekuens\n"
        "Ukuran maksimal sekuens\n\n";
    cout.flush();
    
    int total_tokens;
    cin >> total_tokens;
    vector<int> tokens(total_tokens);
    for (int i = 0; i < total_tokens; ++i) {
        char a, b;
        cin >> a >> b;
        tokens[i] = 256 * a + b;
    }
    cin >> config.maximum_buffer_size;
    cin >> config.width >> config.height;
    cin >> config.total_rewards;
    int max_sequence_size;
    cin >> max_sequence_size;

    config.matrix.resize(config.height);
    for (int j = 0; j < config.height; ++j) {
        config.matrix[j].resize(config.width);
    }
    std::random_device dev;
    std::mt19937 generator(dev());
    std::uniform_int_distribution<int> distribution_length(2, max_sequence_size);
    std::uniform_int_distribution<int> distribution_token(0, total_tokens - 1);
    std::uniform_int_distribution<int> distribution_reward(-13, 10);
    for (int j = 0; j < config.height; ++j) {
        config.matrix[j].resize(config.width);
        for (int i = 0; i < config.width; ++i) {
            config.matrix[j][i] = tokens[distribution_token(generator)];
        }
    }
    for (int i = 0; i < config.total_rewards; ++i) {
        int length = distribution_length(generator);
        Sequence s;
        for (int j = 0; j < length; ++j) {
            s.elements.push_back(tokens[distribution_token(generator)]);
            s.value = distribution_reward(generator);
        }
        config.rewards.push_back(s);
    }

    cout << "Berikut adalah konfigurasi permainan yang dihasilkan secara acak.\n\n";

    for (int i = 0; i < config.height; ++i) {
        for (int j = 0; j < config.width; ++j) {
            int b = (config.matrix[i][j] % 256);
            int a = ((config.matrix[i][j] - b) / 256);
            cout << (char)a << (char)b << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
    for (const auto &reward : config.rewards) {
        for (const auto &i : reward.elements) {
            int b = (i % 256);
            int a = ((i - b) / 256);
            cout << (char)a << (char)b << ' ';
        }
        cout << '\n';
        cout << reward.value << '\n';
    }
    cout << '\n';
}

void search_first_row(const Configuration &config, int col, int &max_reward, vector<pair<int, int>> &optimal_path, vector<int> &optimal_token_path) {
    vector<vector<bool>> taken(config.height, vector<bool>(config.width));
    vector<int> buffer;
    vector<pair<int, int>> path;

    taken[0][col] = true;
    buffer.push_back(config.matrix[0][col]);
    path.emplace_back(col, 0);
    search(config, 0, col, true, taken, buffer, optimal_token_path, path, optimal_path, max_reward);
    path.pop_back();
    taken[0][col] = false;
    buffer.pop_back();
}

string get_current_datetime() {
    auto now = std::chrono::system_clock::now();
    auto local_time = std::chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << std::put_time(std::localtime(&local_time), "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

bool save_text_to_file(const string& content, string filename) {
    ofstream file(filename);

    if (file.is_open()) {
        file << content;
        file.close();
        return true;
    } else {
        return false;
    }
}

int main() {
    cout << "Penyelesaian Cyberpunk 2077 Breach Protocol dengan Algoritma Bruce Force.\n\n";

    while (true) {
        cout <<
            "Pilih metode input:\n"
            "1. Input dengan text file.\n"
            "2. Input dihasilkan secara automatis.\n\n";
        cout.flush();

        int input_type;
        cout << ":> ";
        cout.flush();
        cin >> input_type;

        Configuration config;

        if (input_type == 1) {
            if (!read_text_file(config)) {
                continue;
            }
        } else if (input_type == 2) {
            generate_game(config);
        } else {
            cout << "Input tidak valid.\n\n";
            continue;
        }

        int max_reward = 0;
        vector<pair<int, int>> optimal_path;
        vector<int> optimal_token_path;

        auto start = std::chrono::steady_clock::now();

        vector<int> max_rewards(config.width);
        vector<vector<pair<int, int>>> optimal_paths(config.width);
        vector<vector<int>> optimal_token_paths(config.width);

        vector<thread> threads;
        for (int col = 0; col < config.width; ++col) {
            threads.push_back(thread(search_first_row, ref(config), col, ref(max_rewards[col]), ref(optimal_paths[col]), ref(optimal_token_paths[col])));
        }
        for (auto &thread : threads) {
            thread.join();
        }
        for (int col = 0; col < config.width; ++col) {
            if (max_reward < max_rewards[col]) {
                max_reward = max_rewards[col];
            }
        }
        for (int col = 0; col < config.width; ++col) {
            if (max_rewards[col] == max_reward) {
                optimal_path = optimal_paths[col];
                optimal_token_path = optimal_token_paths[col];
                break;
            }
        }
        auto end = std::chrono::steady_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        stringstream response_stream;

        if (max_reward == 0) {
            response_stream << "Tidak ditemukan solusi yang menghasilkan total hadiah yang positif.\n";
        } else {
            response_stream << max_reward << '\n';
            for (auto &i : optimal_token_path) {
                int b = (i % 256);
                int a = ((i - b) / 256);
                response_stream << (char)a << (char)b << ' ';
            }
            response_stream << '\n';

            for (const auto &pos : optimal_path) {
                response_stream << pos.first + 1;
                response_stream << ", ";
                response_stream << pos.second + 1;
                response_stream << '\n';
            }
            response_stream << '\n';
        }
        response_stream << (double)duration / 1e6 << " ms";
        
        string response = response_stream.str();
        cout << response;
        cout.flush();
        cout << "\n\nApakah ingin menyimpan solusi? (y/n) ";
        cout.flush();
        char confirmation;
        cin >> confirmation;
        if (confirmation == 'y' || confirmation == 'Y') {
            string filename = get_current_datetime();
            if (save_text_to_file(response, filename)) {
                cout << "Solusi telah disimpan di " << filename << "\n\n";
            } else {
                cout << "Solusi gagal disimpan dalam sebuah file karena error.\n\n";
            }
        } else {
            cout << "Solusi tidak disimpan dalam sebuah file.\n\n";
        }
    }
}