#include <string>
#include <iostream>
#include <algorithm>
#include <limits>
#include <ranges>
#include <unordered_map>

using namespace std;
const int INT_MIN = numeric_limits<int>::min();
const int INT_MAX = numeric_limits<int>::max();

void print_board(const string& board) {
    for (int i = 0; i < 9; i += 3) {
        char first = (board[0 + i] == '.') ? (i + 1 + '0') : board[0 + i];
        char second = (board[1 + i] == '.') ? (i + 2 + '0') : board[1 + i];
        char third = (board[2 + i] == '.') ? (i + 3 + '0') : board[2 + i];
        cout << first << " | " << second << " | " << third << endl;
    }
}

unordered_map<string, int> board_score_cache;
int score_board(const string& board) {
    if (auto search = board_score_cache.find(board); search != board_score_cache.end()) {
        return search->second;
    }

    int positions[8][3] = {
        //Horizontals
        {0,1,2},
        {3,4,5},
        {6,7,8},
        //Verticals
        {0,3,6},
        {1,4,7},
        {2,5,8},
        //Diags
        {0,4,8},
        {6,4,2}
    };
    //calculating score
    int score = 0;
    for (const auto& position : positions) {
        string line = {board[position[0]], board[position[1]], board[position[2]]};
        int XCount = 0;
        int OCount = 0;
        int empty = 0;
        for (const char& letter : line) {
            if (letter == 'X') {
                ++XCount;
            } else if (letter == 'O') {
                ++OCount;
            } else if (letter == '.') {
                ++empty;
            }
        }
        //X or O wins
        if (XCount == 3) {
            board_score_cache[board] = INT_MAX;
            return INT_MAX;
        } else if (OCount == 3) {
            board_score_cache[board] = INT_MIN;
            return INT_MIN;
        }
        //two in a line with empty spot
        if (empty == 1) {
            if (XCount == 2) {
                score += 20;
            } else if (OCount == 2) {
                score -= 20;
            }
        }
    }

    if (ranges::count(board, '.') == 0) {
        board_score_cache[board] = 0;
        return 0;
    }

    int weights[9] = {2,1,2,1,8,1,2,1,2};
    for (int i = 0; i < 9; ++i) {
        if (board[i] == 'X') {
            score += weights[i];
        } else if (board[i] == 'O') {
            score -= weights[i];
        }
    }

    board_score_cache[board] = score;
    return score;
}

unordered_map<string, int> minimax_cache;
int minimax(const string& board, int turn, int alpha = INT_MIN, int beta = INT_MAX) {
    string cache_key = board + ((turn == 1) ? "1" : "-1");
    if (auto search = minimax_cache.find(cache_key); search != minimax_cache.end()) {
        return search->second;
    }
    const int score = score_board(board);
    if (ranges::count(board, '.') == 0 or score == numeric_limits<int>::max() or score == numeric_limits<int>::min()) {
        minimax_cache[cache_key] = score;
        return score;
    }

    if (turn == 1) {
        //turn = 1 / X or maximizer
        for (int i = 0; i < 9; ++i) {
            if (board[i] == '.') {
                string new_board = board;
                new_board[i] = 'X';
                int value = minimax(new_board, -1, alpha, beta);
                if (value > alpha) alpha = value;
                if (alpha >= beta) {
                    minimax_cache[cache_key] = alpha;
                    return alpha;
                }
            }
        }
    } else {
        // turn = 0 / O or minimizer
        for (int i = 0; i < 9; ++i) {
            if (board[i] == '.') {
                string new_board = board;
                new_board[i] = 'O';
                int value = minimax(new_board, 1, alpha, beta);
                if (value < beta) beta = value;
                if (beta <= alpha) {
                    minimax_cache[cache_key] = beta;
                    return beta;
                }
            }
        }
    }

    if (turn == 1) {
        minimax_cache[cache_key] = alpha;
        return alpha;
    } else {
        minimax_cache[cache_key] = beta;
        return beta;
    }
}

string find_best(const string& board, int turn) {
    string best_board = board;
    if (turn == 1) {
        int best_score = INT_MIN;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == '.') {
                string new_board = board;
                new_board[i] = 'X';
                int score = minimax(new_board, -1);
                if (score > best_score) {
                    best_score = score;
                    best_board = new_board;
                }
            }
        }
    } else {
        int best_score = INT_MAX;
        for (int i = 0; i < 9; ++i) {
            if (board[i] == '.') {
                string new_board = board;
                new_board[i] = 'O';
                int score = minimax(new_board, 1);
                if (score < best_score) {
                    best_score = score;
                    best_board = new_board;
                }
            }
        }
    }
    for (int i = 0; i < 9; ++i) {
        if (board[i] != best_board[i]) {
            cout << ((turn == 1) ? 'X' : 'O') << "'s best move position is: " << (i + 1) << endl;
            break;
        }
    }
    return best_board;
}



int main() {
    string board = ".........";
    
    int turn = 0;
    int response = 0;
    cout << "Are you player 1 or 2?: ";
    cin >> response;
    if (response == 1) {
        turn = 1;
    } else {
        turn = -1;
        int index = -1;
        cout << "What position did \"X\" play? (1-9): ";
        cin >> index;
        --index;
        board[index] = 'X';
    }
    
    int score = -1;
    while (true) {
        cout << "\n\n\n\n\nCurrent Board:" << endl;
        print_board(board);
        board = find_best(board, turn);
        score = score_board(board);
        if (ranges::count(board, '.') == 0 or score == INT_MAX or score == INT_MIN) {
            cout << "Game Over!" << endl;
            break;
        }
        turn = -turn;
        int index = -1;
        cout << "What position did \"" << ((turn == 1) ? 'X' : 'O') << "\" play? (1-9): ";
        cin >> index;
        --index;
        board[index] = (turn == 1) ? 'X' : 'O';
        score = score_board(board);
        if (ranges::count(board, '.') == 0 or score == INT_MAX or score == INT_MIN) {
            cout << "Game Over!" << endl;
            break;
        }
        turn = -turn;
        cout << endl;
    }
    if (score == INT_MAX) {
        cout << "Player \"X\" wins!" << endl;
    } else if (score == INT_MIN) {
        cout << "Player \"O\" wins!" << endl;
    } else {
        cout << "Draw! Neither player wins!" << endl;
    }
}