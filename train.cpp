// train.cpp: MLP implementation to train NNUE
// Quantizate weights and biases to plug in the NNUE
// let's only have 1 hidden layer for now :P
#include <iostream>
#include <random>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <cctype>

using namespace std;
using dbl = double;
#define INPUT_SIZE 768
#define HL1_SIZE 128
#define pb push_back

// const int epoch = 3;
dbl hidden1_w[INPUT_SIZE][HL1_SIZE];
dbl hidden1_b[HL1_SIZE];
dbl output_w[HL1_SIZE];
dbl output_b;
dbl input[INPUT_SIZE];

// let's just use relu gang
dbl act(dbl x) {
    return max(0.0, x);
}

dbl d_act(dbl x) {
    return x > 0 ? 1.0 : 0.0;
}

// we learning cuh we so intelligent
dbl alpha = 0.0045;
dbl feed_forward() {
    dbl z1[HL1_SIZE], a1[HL1_SIZE];
    for(int i = 0; i < HL1_SIZE; i++) {
        z1[i] = hidden1_b[i];
        for(int j = 0; j < INPUT_SIZE; j++) z1[i] += input[j] * hidden1_w[j][i];
        a1[i] = act(z1[i]);
    }

    dbl zout = output_b;
    for(int i = 0; i < HL1_SIZE; i++) zout += a1[i] * output_w[i];
    return zout; // act=linear
}

dbl loss(dbl ex) {
	dbl u = feed_forward();
	return 0.5 * (u - ex) * (u - ex);
}

void backprop(dbl ex) {
    // feed forward
    dbl z1[HL1_SIZE], a1[HL1_SIZE];
    for(int i = 0; i < HL1_SIZE; i++) {
        z1[i] = hidden1_b[i];
        for(int j = 0; j < INPUT_SIZE; j++) z1[i] += input[j] * hidden1_w[j][i];
        a1[i] = act(z1[i]);
    }

    dbl zout = output_b;
    for(int i = 0; i < HL1_SIZE; i++) zout += a1[i] * output_w[i];
    // ###bug###
    // dbl aout = act(zout);

    // output delta (act=linear)
    dbl delta = (zout - ex);

    // hidden delta
    dbl delta1[HL1_SIZE];
    for(int i = 0; i < HL1_SIZE; i++) {
        delta1[i] = output_w[i] * delta * d_act(z1[i]);
    }

    // output
    for(int i = 0; i < HL1_SIZE; i++) output_w[i] -= alpha * delta * a1[i];
    output_b -= alpha * delta;

    // hidden
    for(int i = 0; i < HL1_SIZE; i++) {
        for(int j = 0; j < INPUT_SIZE; j++) {
            hidden1_w[j][i] -= alpha * delta1[i] * input[j];
        }

        hidden1_b[i] -= alpha * delta1[i];
    }
}

int main() {
	mt19937 rng(std::random_device{}());
    uniform_real_distribution<dbl> dist(-0.1, 0.1);
    for(int j = 0; j < INPUT_SIZE; j++) {
        for(int i = 0; i < HL1_SIZE; i++) {
        	hidden1_w[j][i] = dist(rng);
        }
    }

    for(int i = 0; i < HL1_SIZE; i++) hidden1_b[i] = dist(rng);
    for(int i = 0; i < HL1_SIZE; i++) output_w[i] = dist(rng);
    output_b = dist(rng);

    // TRAIN:
    vector<pair<string, int>> data;
    vector<string> inp;
    string nnue; cin >> nnue;
    ifstream dt(nnue);
    string str;
    float eval;
    cout << "YAY" << endl;
    int store[1000];
    store['p'] = 0;
    store['n'] = 1;
    store['b'] = 2;
    store['r'] = 3;
    store['q'] = 4;
    store['k'] = 5;
    store['P'] = 0;
    store['N'] = 1;
    store['B'] = 2;
    store['R'] = 3;
    store['Q'] = 4;
    store['K'] = 5;
    // READ+TRAIN:
    int i = 0;
    while(dt >> str >> eval) {
        // bool input[INPUT_SIZE];
        memset(input, 0, sizeof input);
        // string str = data[i].first;
        int p = 0;
        int u = 0, j = 0, c = 1;
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
            // upper = white
            if(isupper(str[j])) input[store[str[j]] * 64 + u] = 1;
            else input[64 * 6 + store[str[j]] * 64 + u] = 1;
            
            u++;
        }

        // int epoch = max(1, rand()%4);
        // for(int j = 0 ; j < epoch ; j++) backprop(eval);
        backprop(eval);
        if((i&1023) == 0) cout << "GAME #: " << i << ", LOSS: " << loss(eval) << endl;
        // if(i > 50'000) break;
        i++;
    }
    
    // (quan + feed to NNUE)
    // OUTPUT:
    cout << INPUT_SIZE << " " << HL1_SIZE << endl;
    for(int i = 0 ; i < INPUT_SIZE ; i++) {
        for(int j = 0 ; j < HL1_SIZE ; j++) {
            cout << hidden1_w[i][j] << " ";
        }

        cout << endl;
    }

    cout << HL1_SIZE << endl;
    for(int i = 0 ; i < HL1_SIZE ; i++) cout << hidden1_b[i] << " ";

    cout << HL1_SIZE << endl;
    for(int i = 0 ; i < HL1_SIZE ; i++) cout << output_w[i] << " ";
    cout << output_b;
}