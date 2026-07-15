#include <string>
#include <array>
#include <limits>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <future>
#include <climits>
#include <stdexcept>

using namespace std;

int score_inner_board(const string& board) {
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
            return INT_MAX;
        }
        if (OCount == 3) {
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

    return score;
}

int score_meta_board(const array<string, 9>& meta_board) {
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
            score += (inner_score * weights[i]);
        }
    }
    int meta_score = score_inner_board(meta_string);
    if (meta_score == INT_MAX or meta_score == INT_MIN) {
        return meta_score;
    }

    score += (meta_score * 1000);
    return score;
}

bool valid_board(const string& board) {
    int valid_score = score_inner_board(board);
    if (valid_score == INT_MAX or valid_score == INT_MIN or (valid_score == 0 and ranges::count(board, '.') == 0)) return false;
    return true;
}

struct CacheEntry {
    int score;
    int depth;
    int bound;
};
thread_local unordered_map<string, CacheEntry> minimax_cache;
int minimax(const array<string, 9>& meta_board, int turn, int meta_pos, int depth = 10, int alpha = INT_MIN, int beta = INT_MAX) {
    string cache_key = to_string(meta_pos) + "|" + to_string(depth) + "|" + ((turn == 1) ? "1" : "-1");
    for (const string& board : meta_board) {
        cache_key += ("|" + board);
    }
    auto search = minimax_cache.find(cache_key);
    if (search != minimax_cache.end()) {
        CacheEntry e = search->second;

        if (e.bound == 0) return e.score;
        if (e.bound == 1) alpha = max(alpha, e.score);
        if (e.bound == -1) beta = min(beta, e.score);
        if (alpha >= beta) return e.score;
    }

    int term_score = score_meta_board(meta_board); 
    if (term_score == INT_MAX or term_score == INT_MIN or depth == 0) {
        minimax_cache[cache_key] = {term_score, depth, 0};
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
        minimax_cache[cache_key] = {0, depth, 0};
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
                        minimax_cache[cache_key] = {alpha, depth, 1};
                        return alpha;
                    }
                } else {
                    beta = min(score, beta);
                    if (beta <= alpha) {
                        minimax_cache[cache_key] = {beta, depth, -1};
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
    int score = 0;
};

BestMove find_best(const auto& board, int turn, int next_outer) {
    static bool first_turn = true;
    int max_depth = 7;
    
    vector<int> valid_metas;
    if (not first_turn) {
        if (valid_board(board[next_outer])) {
            valid_metas = {next_outer};
        } else {
            for (int i : {4,0,2,6,8,1,3,5,7}) {
                if (valid_board(board[i])) {
                    valid_metas.push_back(i);
                }
            }
        }
    } else {
        first_turn = false;
        valid_metas = {4,0,2,6,8,1,3,5,7};
    }

    BestMove best;
    best.played_outer = -1;
    best.played_inner = -1;

    best.score = ((turn == 1) ? INT_MIN : INT_MAX);
    vector<BestMove> possible_moves;
    vector<future<int>> current_threads;
    for (int valid_outer_pos : valid_metas) {
        for (int inner_pos : {4,0,2,6,8,1,3,5,7}) {
            if (board[valid_outer_pos][inner_pos] == '.') {
                array<string, 9> new_board = board;
                new_board[valid_outer_pos][inner_pos] = ((turn == 1) ? 'X' : 'O');
                BestMove temp_move = {new_board, valid_outer_pos, inner_pos, 0};
                possible_moves.push_back(move(temp_move));
                current_threads.push_back(async(launch::async, minimax, new_board, -turn, inner_pos, max_depth, INT_MIN, INT_MAX));
            }
        }
    }
    
    for (int i = 0; i < current_threads.size(); ++i) {
        int score = current_threads[i].get();
        if ((turn == 1 and score > best.score) or (turn == -1 and score < best.score)) {
            best.board = possible_moves[i].board;
            best.played_inner = possible_moves[i].played_inner;
            best.played_outer = possible_moves[i].played_outer;
            best.score = score;
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
        try {
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
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
        }
    }
}
