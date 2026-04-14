// sanitize.cpp: generates engine data for train.cpp (lichess elite db)
// ./pgn-extract -Wfen --notags game.pgn > game.fen

#include <bits/stdc++.h>
#define pb push_back
#define INPUT_SIZE 768
using namespace std;
using dbl = int;
using u64 = unsigned long long;
int inverse(int pc) {
	if(pc == 0) return 0;
	if(pc > 6) return pc - 6;
	return pc + 6;
}

int main() {
	ifstream pgn("game.pgn");
	ifstream fen("game.fen");
	vector<dbl> wins;
	string s;
	while(pgn >> s) {
		if(s == "1-0") wins.pb(1);
		else if(s == "0-1") wins.pb(-1);
		else if(s == "1/2-1/2") wins.pb(0);
	}

	unordered_map<string, int> uwu;
	unordered_map<string, int> uwuc;
	// vector<string> gg;
	string w;
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
	string gg[6];
	int lst = -10000;
	int rp = 0, cnt = 0;
	while(fen >> gg[0] >> gg[1] >> gg[2] >> gg[3] >> gg[4] >> gg[5]) {
		// cnt++;
		// if(cnt&8191 == 0) cout << cnt << endl;
		// gg.pb(w);
		// translate and average (not sum)
		map<array<u64, 12>, float> games;
	   	int av = stoi(gg[5]);
	    int bv = lst;
	    // game finished
	    if(bv > av) rp++;
	    lst = av;
	    // for(int i = 0 ; i < gg.size() ; i += 6) {
        int input[64];
        memset(input, 0, sizeof input);
        string str = gg[0];
        int p = 0;
        int u = 0, j = 0, c = 1;
        bool pers = (gg[1]=="w");
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
            if(!pers) input[u^56] = store[str[j]];
            else input[u] = store[str[j]];
            u++;
        }

        // compressing the board
        // for(int i = 0 ; i < 12 ; i++) {
        // 	unsigned long long yay = 0;
        // 	for(int j = 0 ; j < 64 ; j++) {
        // 		yay <<= 1;
        // 		yay += input[12*i + j];
        // 	}

        // 	cout << yay << " ";
        // }

        // encoding info
        int pe = -1;
        if(pers) pe *= -1;
        // cout << pe*wins[rp] << endl;

        string fenny = gg[0];
        // flipping board
        if(!pers) {
        	fenny = "";
        	int ninput[64];
        	memset(ninput, 0, sizeof ninput);
        	for(int i = 0 ; i < 8 ; i++) {
        		for(int j = 0 ; j < 8 ; j++) {
        			int r = 8*i + j;
        			ninput[r^56] = inverse(input[r]);
        		}
        	}

        	for(int i = 0 ; i < 8 ; i++) {
	    		int c = 0;
	    		for(int j = 0 ; j < 8 ; j++) {
	    			int r = 8*i + j;
	    			if(ninput[r]) {
						if(c) {
	    					fenny += '0' + c;
	    					c = 0;
	    				}

	    				fenny += mar[ninput[r]];
	    			}

	    			else c++;
	    		}

	    		if(c) fenny += '0' + c;
	    		if(i!=7) fenny += "/";
    		}
        }

        uwu[fenny] += pe*wins[rp];
        uwuc[fenny]++;
        // pers ^= 1;
	}

    for(auto w: uwu) {
    	cout << w.first << " ";
    	cout << (float)w.second / (float)uwuc[w.first] << endl;
    }
}
