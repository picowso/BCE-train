// eng_train.cpp: generates engine data for train.cpp (LC0)
// takes FEN + eval, returns Piece placement + eval
#include <bits/stdc++.h>
#define pb push_back
#define INPUT_SIZE 768
using namespace std;
using dbl = int;
using u64 = unsigned long long;

int inv(int n) {
	if(n == 0) return 0;
	if(n > 6) return n - 6;
	else return n + 6;
}

int main() {
	string str; cin >> str;
	ifstream pgn(str); // should've been called fens :P
	int store[1000];
    memset(store, 0, sizeof store);
    store['p'] = 1;
    store['n'] = 2;
    store['b'] = 3;
    store['r'] = 4;
    store['q'] = 5;
    store['k'] = 6;
    store['P'] = 7;
    store['N'] = 8;
    store['B'] = 9;
    store['R'] = 10;
    store['Q'] = 11;
    store['K'] = 12;
    char mar[13];
    // for(int i = 0 ; i < 1000 ; i++) mar[store[i]] = (char)i;
    mar[1] = 'p';
	mar[2] = 'n';
	mar[3] = 'b';
	mar[4] = 'r';
	mar[5] = 'q';
	mar[6] = 'k';
	mar[7] = 'P';
	mar[8] = 'N';
	mar[9] = 'B';
	mar[10] = 'R';
	mar[11] = 'Q';
	mar[12] = 'K';
	string fen, turn;
	float eval = 0.f;
	while(pgn >> fen >> turn >> eval) {
		// if(turn == 'b') {
		// 	// flip board
		// }

		// else {
		// 	cout << pgn << " " << eval << endl;
		// }

		// decode pgn
		int input[64];
        memset(input, 0, sizeof input);
        string str = fen;
        int p = 0;
        int u = 0, j = 0, c = 1;
        bool pers = (turn=="w");
        for(; j < str.size() ; j++) {
            if(str[j] == '/') {
                u = c << 3;
                c++;
                continue;
            }

            if('0' <= str[j] and str[j] <= '9') {
                u += str[j] - '0';
                continue;
            }

            // cout << u << endl;
            // lower = white
            if(!pers) input[u^56] = inv(store[str[j]]);
            else input[u] = store[str[j]];
            u++;
        }

        // encode pgn
        string fenny = "";
    	for(int i = 0 ; i < 8 ; i++) {
    		int c = 0;
    		for(int j = 0 ; j < 8 ; j++) {
    			int r = 8*i + j;
    			if(input[r]) {
					if(c) {
    					fenny += '0' + c;
    					c = 0;
    				}

    				fenny += mar[input[r]];
    			}

    			else c++;
    		}

    		if(c) fenny += '0' + c;
    		if(i!=7) fenny += "/";
		}

		cout << fenny << " ";
		if(turn == "w") cout << eval << endl;
		else cout << -eval << endl;
		// string fenny = fen;
		// if(turn == "b") {
		// 	vector<string> gg;
		// 	string u = "";
		// 	fenny = "";
		// 	for(int j = 0 ; j < fen.size() ; j++) {
		// 		if(fen[j] == '/') {
		// 			gg.push_back(u);
		// 			u = "";
		// 		}

		// 		else u.push_back(fen[j]);
		// 	}

		// 	if(u.size()) gg.push_back(u);
		// 	// reverse :3
		// 	// for(int i = 3, j = 4 ; j < 8 ; j++, i--) swap(gg[i], gg[j]);
		// 	reverse(gg.begin(), gg.end());
		// 	for(int i = 0 ; i < gg.size() ; i++) {
		// 		// lower -> upper
		// 		// upper -> lower
		// 		for(int j = 0 ; j < gg[i].size() ; j++) {
		// 			if(islower(gg[i][j])) gg[i][j] = toupper(gg[i][j]);
		// 			else if(isupper(gg[i][j])) gg[i][j] = tolower(gg[i][j]);
		// 		}

		// 		fenny += gg[i];
		// 		if(i != gg.size()-1) fenny += "/";
		// 	}

		// 	cout << fenny << " " << -eval << endl;
		// }

		// else cout << fenny << " " << eval << endl;
	}
}