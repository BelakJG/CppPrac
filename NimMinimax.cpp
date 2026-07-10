#include <iostream>
#include <array>
#include <unordered_map>
#include <string>

using namespace std;

void print_piles(auto piles) {
    cout << "[";
    for (int i = 0; i < piles.size(); ++i) {
        cout << piles[i] << ((i != piles.size() - 1) ? ", " : "");
    }
    cout << "]" << endl;
}

int sum_piles(auto piles) {
    int sum = 0;
    for (int pile : piles) {
        sum += pile;
    }
    return sum;
}

string create_key(int turn, const auto& piles) {
    string key = to_string(turn);
    for (int pile : piles) {
        key += to_string(pile);
    }
    return key;
}

unordered_map<string, int> minimax_cache;
int minimax(int turn, const auto& piles) {
    string cache_key = create_key(turn, piles);
    if (auto search = minimax_cache.find(cache_key); search != minimax_cache.end()) {
        return search->second;
    }
    if (sum_piles(piles) == 0) {
        int winner = (-turn);
        minimax_cache[cache_key] = winner;
        return winner;
    }
    int score = -turn;
    for (int pile = 0; pile < piles.size(); ++pile) {
        for (int i = 0; i < piles[pile]; ++i) {
            array new_piles = piles;
            new_piles[pile] = i;
            int current_score = minimax(-turn, new_piles);
            if (current_score == turn) {
                minimax_cache[cache_key] = current_score;
                return current_score;
            }
        }
    }

    minimax_cache[cache_key] = score;
    return score;
}

auto find_best(int turn, const auto& piles) {
    auto best = piles;
    for (int i = 0; i < piles.size(); ++i) {
        if (best[i] > 0) {
            --best[i];
            break;
        }
    }

    for (int pile = 0; pile < piles.size(); ++pile) {
        for (int i = 0; i < piles[pile]; ++i) {
            array new_piles = piles;
            new_piles[pile] = i;
            int score = minimax(-turn, new_piles);
            if (score == turn) {
                best = new_piles;
                break;
            }
        }
    }

    for (int i = 0; i < piles.size(); ++i) {
        if (piles[i] != best[i]) {
            cout << "Remove " << (piles[i] - best[i]) << " from pile " << (i + 1) << endl;
            break;
        }
    }
    return best;
}

void opponent_turn(auto& piles) {
    int pile;
    int amount;
    cout << "What pile did they remove from?: ";
    cin >> pile;
    --pile;
    cout << "How many did they remove?: ";
    cin >> amount;
    piles[pile] -= amount;
}

int main() {
    array piles = {1,3,5,7};
    int turn;
    cout << "Are you play 1 or 2?: ";
    cin >> turn;
    if (turn != 1) {
        turn = -1;
        opponent_turn(piles);
    }
    while (sum_piles(piles) > 0) {
        cout << "Current Piles: ";
        print_piles(piles);
        piles = find_best(turn, piles);
        cout << endl;

        if (sum_piles(piles) == 0) break;
        cout << "Piles after your best move: ";
        print_piles(piles);
        opponent_turn(piles);
        cout << endl;
    }
}