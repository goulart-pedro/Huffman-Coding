#include <iostream>
#include "../include/tree.h"
#include "../include/heap.h"

int main(int argc, char **argv) {
	if (argc < 4) throw std::runtime_error("Argumentados faltando");
	
	// analisa a entrada descobrindo os argumentos
	int argv_places[2];	char flag; int j = 0;
	for (int i=1; i<4; i++) if (argv[i][0] == '-') flag = argv[i][1]; else argv_places[j++] = i;

	Huffman huffman;
	switch (flag) {
		case 'd':
		huffman.decompress(argv[argv_places[0]], argv[argv_places[1]]); break;
		case 'c':
		huffman.compress(argv[argv_places[0]], argv[argv_places[1]]); break;
		default:
		throw std::runtime_error("Flag inválida");
	}
}
