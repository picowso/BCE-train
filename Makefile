all:
	g++ train.cpp -O3 -march=native -mtune=native -flto -DNDEBUG -std=c++20 -o train
	./train | tee briwats4.nnue
