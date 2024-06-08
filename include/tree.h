#pragma once
#include <string>

struct node {
  char value; // \0 para os nós internos, 'A,B,C ... para as folhas'
  int frequency;
  node *left, *right;

  node(char value, int freq, node *left, node *right) {
    this->value = value;
    this->frequency = freq;
    this->left = left;
    this->right = right;
  }

  node() {
    this->value = '\0';
    this->frequency = 0;
    this->left = nullptr;
    this->right = nullptr;
  }
};

class Huffman {
  node *_root;

  // realiza um caminhamento pre order na árvore e printa ela num arquivo binario
  void write_tree(node *n, std::fstream &outfile);

  // realiza um caminhamento pre order na árvore
  void walk(node *n);

  // gera os códigos e os insere em um dicionário
  void print_codes(node *n, std::string code, std::string dict[], int &node_number_acc);

  // Determina a frequencia de cada caractere na entrada e retorna o numero de caracteres no arquivo
  int get_freqs(int freqs[], std::string infile_name);

  // Constroi a árvore de huffman dados os nós associados a entrada
  node *build_tree(node *items[], int items_amount);

  // preenche o vetor de caracteres unicos
  // retorna a quantidade de itens adicionados
  int get_huffman_nodes(int freqs[], node *items[], std::fstream &infile);

  // constroi a árvore de huffman com base na header gerada na compressão
  node *build_from_header(std::string header);

  // descomprime o arquivo comprimido
  void write_decompression(std::fstream &infile, std::fstream &outfile);

  // escreve as informações, topologia e texto comprimido
  void write_compression(node* root, int leaves_amount, int char_amnt, std::fstream &infile, std::fstream &outfile);

  public:
  Huffman();
  void compress(std::string infile_name, std::string outfile_name);
  void decompress(std::string infile_name, std::string outfile_name);
};