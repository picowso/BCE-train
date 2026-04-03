// train.cpp: simple MLP implementation, trains on a set of labeled FENs
// Quantizate weights and biases to plug in the NNUE
// let's only have 1 hidden layer for now.
#include <iostream>
#include <random>
#include <cstring>
using namespace std;
using dbl = double;
#define INPUT_SIZE 1
#define HL1_SIZE 128

dbl hidden1_w[INPUT_SIZE][HL1_SIZE];
dbl hidden1_b[HL1_SIZE];
dbl output_w[HL2_SIZE];
dbl output_b;
dbl input[INPUT_SIZE];
dbl act(dbl x) {
	return max(0., x);
}

dbl d_act(dbl x) {
	if(x < 0) return 0;
	return 1;
}

dbl sigmoid(dbl x) {
	return 1 / (1 + exp(-x));
}

dbl d_sigmoid(dbl x) {
	return x*(1-x);
}

dbl feed_forward() {
    dbl z1[HL1_SIZE], a1[HL1_SIZE];
    for(int i = 0; i < HL1_SIZE; i++) {
        z1[i] = hidden1_b[i];
        for(int j = 0; j < INPUT_SIZE; j++) z1[i] += input[j] * hidden1_w[j][i];
        a1[i] = act(z1[i]);
    }

    dbl zout = output_b;
    for(int i = 0; i < HL1_SIZE; i++) zout += a1[i] * output_w[i];
    return sigmoid(zout);
}

dbl loss(dbl i, dbl ex) {
	input[0] = i;
	dbl u = feed_forward();
	return 0.5 * (u - ex) * (u - ex);
}

dbl alpha = 0.1;
// we learning cuh we so intelligent
void backprop(dbl in, dbl ex) {
    input[0] = in;
    // feed forward
    dbl z1[HL1_SIZE], a1[HL1_SIZE];
    for(int i = 0; i < HL1_SIZE; i++) {
        z1[i] = hidden1_b[i];
        for(int j = 0; j < INPUT_SIZE; j++) z1[i] += input[j] * hidden1_w[j][i];
        a1[i] = act(z1[i]);
    }

    dbl zout = output_b;
    for(int i = 0; i < HL1_SIZE; i++) zout += a1[i] * output_w[i];
    dbl aout = sigmoid(zout);

    // output delta
    dbl delta = (aout - ex) * d_sigmoid(aout);

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
    uniform_real_distribution<dbl> dist(-100'000, 100'000);
    for(int j = 0; j < INPUT_SIZE; j++) {
        for(int i = 0; i < HL1_SIZE; i++) {
        	hidden1_w[j][i] = dist(rng);
        }
    }

    for(int i = 0; i < HL1_SIZE; i++) hidden1_b[i] = dist(rng);
    for(int i = 0; i < HL2_SIZE; i++) hidden2_b[i] = dist(rng);
    for(int i = 0; i < HL2_SIZE; i++) output_w[i] = dist(rng);
    output_b = dist(rng);

    // TRAIN:

    // OUTPUT:
    
}