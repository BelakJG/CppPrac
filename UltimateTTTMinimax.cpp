#include <string>
#include <array>
#include <limits>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>

using namespace std;

const int INT_MIN = numeric_limits<int>::min();
const int INT_MAX = numeric_limits<int>::max();

unordered_map<string, int> inner_board_cache;
int score_inner_board(const string& board) {
    if (auto search = inner_board_cache.find(board); search != inner_board_cache.end()) {
        return search->second;
    }
    int winning_positions[8][3] = {
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
    int score = 0;
    for (auto position : winning_positions) {
        int XCount = 0;
        int OCount = 0;
        int empty = 0;
        for (int i = 0; i <= 2; ++i) {
            char move = board[position[i]];
            if (move == 'X') {
                ++XCount;
            } else if (move == 'O') {
                ++OCount;
            } else if (move == '.') {
                ++empty;
            }
        }

        //check for winners
        if (XCount == 3) {
            inner_board_cache[board] = INT_MAX;
            return INT_MAX;
        }
        if (OCount == 3) {
            inner_board_cache[board] = INT_MIN;
            return INT_MIN;
        }

        //score if one player has a possible win
        if (empty == 1) {
            if (XCount == 2) {
                score += 50;
            } else if (OCount == 2) {
                score -= 50;
            }
        }
    }

    //check if draw
    if (ranges::count(board, '.') == 0) {
        inner_board_cache[board] = 0;
        return 0;
    }

    //add score to single position with weight based on position
    int weights[9] = {2,1,2,1,8,1,2,1,2};
    for (int i = 0; i < 9; ++i) {
        if (board[i] == 'X') {
            score += weights[i];
        } else if (board[i] == 'O') {
            score -= weights[i];
        }
    }

    inner_board_cache[board] = score;
    return score;
}

unordered_map<string, int> meta_board_cache;
int score_meta_board(const auto& meta_board) {
    string cache_key = "";
    for (const string& board : meta_board) {
        cache_key += board;
    }
    if (auto search = meta_board_cache.find(cache_key); search != meta_board_cache.end()) {
        return search->second;
    }
    string meta_string = "";
    int score = 0;
    for (int i = 0; i < 9; ++i) {
        int weights[9] = {2,1,2,1,8,1,2,1,2};

        string inner_board = meta_board[i];
        int inner_score = score_inner_board(inner_board);
        if (inner_score == INT_MAX) {
            meta_string += 'X';
            score += (100 * weights[i]);
        } else if (inner_score == INT_MIN) {
            meta_string += 'O';
            score -= (100 * weights[i]);
        } else if (inner_score == 0 and ranges::count(inner_board, '.') == 0) {
            meta_string += 'D';
        } else {
            meta_string += '.';
            score += (inner_score * 50 * weights[i]);
        }
    }
    int meta_score = score_inner_board(meta_string);
    if (meta_score == INT_MAX or meta_score == INT_MIN) {
        meta_board_cache[cache_key] = meta_score;
        return meta_score;
    }

    score += (meta_score * 1000);
    meta_board_cache[cache_key] = score;
    return score;
}

bool valid_board(const string& board) {
    int valid_score = score_inner_board(board);
    if (valid_score == INT_MAX or valid_score == INT_MIN or (valid_score == 0 and ranges::count(board, '.') == 0)) return false;
    return true;
}

int minimax(const array<string, 9>& meta_board, int turn, int meta_pos, int depth = 10, int alpha = INT_MIN, int beta = INT_MAX) {
    int term_score = score_meta_board(meta_board); 
    if (term_score == INT_MAX or term_score == INT_MIN or depth == 0) {
        return term_score;
    }

    int move_order[9] = {4,0,2,6,8,1,3,5,7};
    vector<int> valid_metas;
    if (valid_board(meta_board[meta_pos])) {
        valid_metas.push_back(meta_pos);
    } else {
        for (int i : move_order) {
            if (valid_board(meta_board[i])) {
                valid_metas.push_back(i);
            }
        }
    }
    if (valid_metas.empty()) {
        return 0;
    }

    for (int valid_meta_pos : valid_metas) {
        for (int inner_pos : move_order) {
            if (meta_board[valid_meta_pos][inner_pos] == '.') {
                array<string, 9> new_board = meta_board;
                new_board[valid_meta_pos][inner_pos] = ((turn == 1) ? 'X' : 'O');
                int score = minimax(new_board, -turn, inner_pos, depth - 1, alpha, beta);
                if (turn == 1) {
                    alpha = max(score, alpha);
                    if (alpha >= beta) {
                        return alpha;
                    }
                } else {
                    beta = min(score, beta);
                    if (beta <= alpha) {
                        return beta;
                    }
                }
            }
        }
    }

    return ((turn == 1) ? alpha : beta);
}

struct BestMove {
    array<string, 9> board;
    int played_outer;
    int played_inner;
    int score;
};

BestMove find_best(const auto& board, int turn, int next_outer) {
    static bool first_turn = true;
    int max_depth = 8;
    
    vector<int> valid_metas;
    if (not first_turn) {
        if (valid_board(board[next_outer])) {
            valid_metas = {next_outer};
        } else {
            for (int i = 0; i < 9; ++i) {
                if (valid_board(board[i])) {
                    valid_metas.push_back(i);
                }
            }
        }
    } else {
        first_turn = false;
        max_depth = 4;
        valid_metas = {0,1,2,3,4,5,6,7,8};
    }

    BestMove best;
    best.played_outer = -1;
    best.played_inner = -1;

    best.score = INT_MIN;
    for (int valid_outer_pos : valid_metas) {
        for (int inner_pos = 0; inner_pos < 9; ++inner_pos) {
            if (board[valid_outer_pos][inner_pos] == '.') {
                array<string, 9> new_board = board;
                new_board[valid_outer_pos][inner_pos] = ((turn == 1) ? 'X' : 'O');
                int score = minimax(new_board, -turn, inner_pos, max_depth);
                if ((score > best.score and turn == 1) or (score < best.score and turn == -1)) {
                    best.board = new_board;
                    best.played_outer = valid_outer_pos;
                    best.played_inner = inner_pos;
                    best.score = score;
                }
            }
        }
    }

    cout << "Best Move | Outer: " << best.played_outer + 1 << " Inner: " << best.played_inner + 1 << " Score: " << best.score << endl;
    return best;
}

int main() {
    array<string, 9> board = {".........",".........",".........",".........",".........",".........",".........",".........","........."};

    int next_outer = 1;
    int turn = 1;
    while (true) {
        BestMove best = find_best(board, turn, next_outer);
        board = best.board;
        turn = -turn;

        if (valid_board(board[best.played_inner])) {
            cout << "Opponent turn | Outer: " << best.played_inner + 1 << " Inner: ";
            cin >> next_outer;
            --next_outer;
            board[best.played_inner][next_outer] = ((turn == 1) ? 'X' : 'O');
        } else {
            cout << "Opponent turn | Outer: ";
            int actual_outer;
            cin >> actual_outer;
            --actual_outer;
            cout << "Inner: ";
            cin >> next_outer;
            --next_outer;
            board[actual_outer][next_outer] = ((turn == 1) ? 'X' : 'O');
        }
        turn = -turn;
        cout << endl;
    }
}
