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
#include "TTTzobristhash.cpp"

using namespace std;

int score_inner_board(const string& board) {
    static unordered_map<string, int> inner_cache;
    if (auto search = inner_cache.find(board); search != inner_cache.end()) {
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
            inner_cache[board] = INT_MAX;
            return INT_MAX;
        }
        if (OCount == 3) {
            inner_cache[board] = INT_MIN;
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
        inner_cache[board] = 0;
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

    inner_cache[board] = score;
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

    score += (meta_score * 2000);
    return score;
}

bool valid_board(const string& board) {
    int valid_score = score_inner_board(board);
    if (valid_score == INT_MAX or valid_score == INT_MIN or (valid_score == 0 and ranges::count(board, '.') == 0)) return false;
    return true;
}

struct CacheEntry {
    int TTmeta;
    int TTinner;
    int score;
    int depth;
    int bound;
};

ZobristHash zobrist;
int minimax(uint64_t& cache_key, array<string, 9>& meta_board, int turn, int meta_pos, int depth = 10, int alpha = INT_MIN, int beta = INT_MAX) {
    static unordered_map<uint64_t, CacheEntry> minimax_cache;
    int orig_alpha = alpha;
    int orig_beta = beta;
    int TTinner = -1;
    int TTmeta = -1;

    auto search = minimax_cache.find(cache_key);
    if (search != minimax_cache.end()) {
        const CacheEntry& e = search->second;
        TTinner = e.TTinner;
        TTmeta = e.TTmeta;

        if (search->second.depth >= depth) {
            if (e.bound == 0) return e.score;
            if (e.bound == 1) beta = min(beta, e.score);
            if (e.bound == -1) alpha = max(alpha, e.score);
            if (alpha >= beta) return e.score;
        }
    }

    int term_score = score_meta_board(meta_board); 
    if (term_score == INT_MAX or term_score == INT_MIN or depth == 0) {
        minimax_cache[cache_key] = {-1, -1, term_score, depth, 0};
        return term_score;
    }

    int move_order[9] = {4,0,2,6,8,1,3,5,7};
    vector<int> valid_metas;
    if (meta_pos != 9) {
        valid_metas.push_back(meta_pos);
    } else {
        for (int i : move_order) {
            if (valid_board(meta_board[i])) {
                valid_metas.push_back(i);
            }
        }
    }
    if (valid_metas.empty()) {
        minimax_cache[cache_key] = {-1, -1, 0, depth, 0};
        return 0;
    }

    int old_forced = valid_metas.size() == 1 ? meta_pos : 9;
    int best_meta = -1;
    int best_inner = -1;
    bool pruned = false;
    if (search != minimax_cache.end()) {
        CacheEntry e = search->second;
        if (TTinner != -1 and TTmeta != -1 and e.depth >= depth) {
            int next_forced = valid_board(meta_board[TTinner]) ? TTinner : 9;
            char next_move = turn == 1 ? 'X' : 'O';
            meta_board[TTmeta][TTinner] = next_move;
            zobrist.update(cache_key, TTmeta, TTinner, next_move, old_forced, next_forced);
            int score = minimax(cache_key, meta_board, -turn, e.TTinner, depth - 1, alpha, beta);
            zobrist.update(cache_key, TTmeta, TTinner, next_move, next_forced, old_forced);
            meta_board[TTmeta][TTinner] = '.';

            if (turn == 1 and score > alpha) {
                alpha = score;
                best_meta = TTmeta;
                best_inner = TTinner;
            } if (turn == -1 and score < beta) {
                beta = score;
                best_meta = TTmeta;
                best_inner = TTinner;
            }
            if (alpha >= beta) pruned = true;
        }
    }
    for (int valid_meta_pos : valid_metas) {
        if (pruned) break;
        for (int inner_pos : move_order) {
            if (valid_meta_pos == TTmeta and inner_pos == TTinner) continue;
            if (meta_board[valid_meta_pos][inner_pos] == '.') {
                char new_move = ((turn == 1) ? 'X' : 'O');
                int new_forced = (valid_board(meta_board[inner_pos]) ? inner_pos : 9);
                meta_board[valid_meta_pos][inner_pos] = new_move;
                zobrist.update(cache_key, valid_meta_pos, inner_pos, new_move, old_forced, new_forced);
                int score = minimax(cache_key, meta_board, -turn, new_forced, depth - 1, alpha, beta);
                zobrist.update(cache_key, valid_meta_pos, inner_pos, new_move, new_forced, old_forced);
                meta_board[valid_meta_pos][inner_pos] = '.';
                if (turn == 1) {
                    if (score > alpha) {
                        best_meta = valid_meta_pos;
                        best_inner = inner_pos;
                        alpha = score;
                    }
                    if (alpha >= beta) {
                        pruned = true;
                        break;
                    }
                } else {
                    if (score < beta) {
                        beta = score;
                        best_meta = valid_meta_pos;
                        best_inner = inner_pos;
                    }
                    if (beta <= alpha) {
                        pruned = true;
                        break;
                    }
                }
            }
        }
    }


    int best = ((turn == 1) ? alpha : beta);
    if (search == minimax_cache.end() or depth >= search->second.depth) {
        CacheEntry entry;
        entry.TTmeta = best_meta;
        entry.TTinner = best_inner;
        entry.score = best;
        entry.depth = depth;
        if (best <= orig_alpha) {
            entry.bound = 1;
        } else if (best >= orig_beta) {
            entry.bound = -1;
        } else {
            entry.bound = 0;
        }
        minimax_cache[cache_key] = move(entry);
    }

    return best;
}

struct BestMove {
    array<string, 9> board;
    int played_outer;
    int played_inner;
    int score = 0;
};

BestMove find_best(uint64_t& hash, auto& board, int turn, int next_outer) {
    static bool first_turn = true;
    static int max_depth = 7;
    bool first_move = true;
    
    vector<int> valid_metas;
    if (not first_turn) {
        if (next_outer != 9) {
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
    char new_move = ((turn == 1) ? 'X' : 'O');

    for (int valid_outer_pos : valid_metas) {
        for (int inner_pos : {4,0,2,6,8,1,3,5,7}) {
            if (board[valid_outer_pos][inner_pos] == '.') {
                int new_forced = (valid_board(board[inner_pos]) ? inner_pos : 9);

                board[valid_outer_pos][inner_pos] = new_move;
                zobrist.update(hash, valid_outer_pos, inner_pos, new_move, next_outer, new_forced);
                int score = minimax(hash, board, -turn, new_forced, max_depth);
                zobrist.update(hash, valid_outer_pos, inner_pos, new_move, new_forced, next_outer);
                board[valid_outer_pos][inner_pos] = '.';

                if ((score > best.score && turn == 1) or (score < best.score && turn == -1) or first_move) {
                    first_move = false;
                    best.board = board;
                    best.board[valid_outer_pos][inner_pos] = new_move;
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

bool game_tied(const array<string, 9>& board) {
    for (const string& inner : board) {
        int score = score_inner_board(inner);
        if (score != INT_MAX and score != INT_MIN and !(score == 0 and ranges::count(inner, '.') == 0)) return false;
    }

    return true;
}

int main() {
    bool play_again = true;
    while (play_again) {
        array<string, 9> board = {".........",".........",".........",".........",".........",".........",".........",".........","........."};
        int last_forced = 9;
        int turn = 1;
        uint64_t hash = zobrist.compute(board, last_forced);
        while (score_meta_board(board) != INT_MAX and score_meta_board(board) != INT_MIN and !game_tied(board)) {
            try {
                char move_symbol = ((turn == 1) ? 'X' : 'O');
                BestMove best = find_best(hash, board, turn, last_forced);
                board = best.board;

                int next_forced = valid_board(board[best.played_inner]) ? best.played_inner : 9;
                zobrist.update(hash, best.played_outer, best.played_inner, move_symbol, last_forced, next_forced);

                if (score_meta_board(board) == INT_MAX or score_meta_board(board) == INT_MIN or game_tied(board)) break;
                turn = -turn;
                move_symbol = ((turn == 1) ? 'X' : 'O');
                last_forced = next_forced;

                if (last_forced != 9) {
                    cout << "Opponent turn | Outer: " << last_forced + 1 << " Inner: ";
                    int op_inner;
                    cin >> op_inner;
                    --op_inner;

                    board[last_forced][op_inner] = move_symbol;
                    next_forced = (valid_board(board[op_inner]) ? op_inner : 9);
                    zobrist.update(hash, last_forced, op_inner, move_symbol, last_forced, next_forced);
                } else {
                    cout << "Opponent turn | Outer: ";
                    int actual_outer;
                    cin >> actual_outer;
                    --actual_outer;
                    cout << "Inner: ";
                    int op_inner;
                    cin >> op_inner;
                    --op_inner;
                    board[actual_outer][op_inner] = move_symbol;
                    next_forced = (valid_board(board[op_inner]) ? op_inner : 9);
                    zobrist.update(hash, actual_outer, op_inner, move_symbol, last_forced, next_forced);
                }
                turn = -turn;
                last_forced = next_forced;
                cout << endl;
            } catch (const runtime_error& e) {
                cerr << e.what() << endl;
            }
        }
        cout << game_tied(board) << endl;
        cout << "Game Over!\n" << endl;
    }
}
