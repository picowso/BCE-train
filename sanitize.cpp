// sanitize.cpp: making the data (PGN + FENs) clean for train.cpp
#include <bits/stdc++.h>
#define pb push_back
using namespace std;
using dbl = int;
int main() {
	ifstream pgn("game2.pgn");
	ifstream fen("game2.fen");
	vector<dbl> wins;
	string s;
	while(pgn >> s) {
		if(s == "0-1") wins.pb(-1);
		else if(s == "1-0") wins.pb(1);
		else if(s == "1/2-1/2") wins.pb(0);
	}

	map<array<string, 4>, dbl> uwu;
	vector<string> gg;
	string w;
	while(fen >> w) {
		gg.pb(w);
	}

	int rp = 0;

	// first val
	array<string,4> frst;
	for(int i = 0 ; i < 4 ; i++) frst[i] = gg[i];
	uwu[frst] += wins[rp];

	// not first val :P
	for(int i = 11 ; i < gg.size() ; i += 6) {
		int k = i/6;
		if(k&127) cout << k << endl;
		int a = stoi( gg[i-6] );
		int b = stoi( gg[i] );
		if(a > b) rp++;
		array<string, 4> po;
		for(int j = 0 ; j < 4 ; j++) po[j] = gg[i-5+j];
		uwu[po] += wins[rp];
	}

	for(pair<array<string, 4>, dbl> u: uwu) {
		for(int j = 0 ; j < 4 ; j++) cout << u.first[j] << " ";
		cout << u.second << endl;
	}
}