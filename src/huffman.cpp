#include "../include/tree.h"
#include "../include/heap.h"
#include <algorithm>
#include <fstream>
#include <iostream>

Huffman::Huffman() {
	_root = nullptr;
}

void Huffman::compress(std::string infile_name, std::string outfile_name) {
  std::fstream infile("tests/huffman.in", std::ios::in); // texto de entrada
  std::fstream outfile("tests/huffman.out", std::ios::out | std::ios::binary); // resultado da compressao

  const int char_range = 128; // no de caracteres da tabela ASCII 
  int freqs[char_range] = {0}; // deve ser inicializado com zero

  const std::string instr = "tests/huffman.in"; // Deve conter apenas caracteres ASCII por enquanto
  int char_amount = get_freqs(freqs, instr);

  int leaves_amnt = 0;     // me diz a quantidade de folhas armazenadas
  node *items[char_range]; // igual ao número de chars

  infile.clear(); infile.seekg(0, std::ios::beg); // garante que o arquivo está no começo
  leaves_amnt = get_huffman_nodes(freqs, items, infile);

  node *root = build_tree(items, leaves_amnt);

  infile.clear(); infile.seekg(0, std::ios::beg); // garante que o arquivo está no começo

  write_compression(root, leaves_amnt, char_amount, infile, outfile);
  infile.close(); outfile.close();
	
}

void Huffman::decompress(std::string infile_name, std::string outfile_name) {
  std::fstream infile(infile_name, std::ios::in | std::ios::binary);
  std::fstream outfile(outfile_name, std::ios::out);

	write_decompression(infile, outfile);
  infile.close(); outfile.close();
}


node *Huffman::build_tree(node *items[], int items_amount) {
  MinHeap tree_heap;
  for (int i=0; i<items_amount; i++) {
    tree_heap.push(items[i]);
  }

  for (int i=0; i<items_amount-1; i++) {
    // pegando os dois caracteres de menor frequencia
    node *bottom_one = tree_heap.top(); tree_heap.pop();
    node *bottom_two = tree_heap.top(); tree_heap.pop();

    tree_heap.push(new node('\0', bottom_one->frequency + bottom_two->frequency, bottom_one, bottom_two));
  }
  
  node *root = tree_heap.top(); tree_heap.pop();
  return root;
}

int Huffman::get_huffman_nodes(int freqs[], node *items[], std::fstream &infile) {
  char c;
  int items_amount = 0;
  while (infile >> std::noskipws >> c) {
    int char_index = int(c);
    if (freqs[char_index] == 0) continue;

    items[items_amount] = new node(c, freqs[char_index], nullptr, nullptr); // os nos adicionados aqui são necessáriamente folhas
    freqs[char_index] = 0; // zero pois na proxima iteração tenho como saber se o item já foi incluso
    items_amount++;
  }

  return items_amount;
}

int Huffman::get_freqs(int freqs[], std::string infile_name) {
  std::ifstream infile(infile_name);
  int char_amount = 0;
  char c;

  while (infile >> std::noskipws >> c) {
    freqs[int(c)] += 1;
    char_amount++;
  }

  infile.close();

  return char_amount;
}

node *Huffman::build_from_header(std::string header) {
  std::stack<node*> st;
  for (int i=0; i<header.size(); i++) {
    char c = header[i];

    if (c == '1') {
      // acredito que a frequencia seja irrelevante neste estagio logo deixo 0
      st.push(new node(header[i+1], 0, nullptr, nullptr));
      i++;
      continue;
    }

    if (c == '0') {
      if (st.size() == 1) break;

      node *right_el = st.top(); st.pop();
      node *left_el = st.top(); st.pop();
      st.push(new node('\0', 0, left_el, right_el));
    }
  }

  node *root = st.top(); st.pop();
  return root;
}

void Huffman::write_tree(node *n, std::fstream &outfile) {
  if (!n) return;

  write_tree(n->left, outfile);
  write_tree(n->right, outfile);

  if (n->value != '\0') {
    bool leaf_flag = 1;
    outfile.write((char*) &leaf_flag, sizeof(bool));
    outfile.write((char*) &n->value, sizeof(char));
  }
  else {
    bool leaf_flag = 0;
    outfile.write((char*) &leaf_flag, sizeof(bool));
  }
}

void Huffman::print_codes(node *n, std::string code, std::string dict[], int &node_number_acc) {
  if (n == nullptr) return;
  node_number_acc++;

  if (n->left == nullptr && n->right == nullptr) {
    dict[int(n->value)] = code;
    std::cout << n->value << '\t' << code << std::endl;
  }

  print_codes(n->left, code+'0', dict, node_number_acc);
  print_codes(n->right, code+'1', dict, node_number_acc);
}

void Huffman::write_decompression(std::fstream &infile, std::fstream &outfile) {
  // pegando as informações de quantidade de nós na árvore e caracteres no texto
  int node_amnt = 0; infile.read((char*) &node_amnt, sizeof(int));
  int char_amnt = 0; infile.read((char*) &char_amnt, sizeof(int));

  std::string inline_tree; // armazena a representação da árvore em texto

  // constrói a árvore a partir de sua representação em texto
  for (int i=0; i<node_amnt; i++) {
    char buffer; infile.read((char*) &buffer, sizeof(char));
    char code;
    inline_tree.push_back(int(buffer) == 1 ? '1' : '0');

    if (int(buffer) == 1) {
      infile.read((char*) &code, sizeof(char));
      inline_tree.push_back(code);
    }
  }

  node *root = build_from_header(inline_tree);
  int t=0;

  node *search_pointer = root;
  // lê o texto comprimido byte e byte até que char_amnt caracteres sejam encontrados
  int matched_chars = 0;
  while (matched_chars != char_amnt) {
    // armazena um byte em curr
    char buffer = 0;
    infile.read(&buffer, 1);
    // com esse ponteiro eu vou fazendo a busca na árvore

    // percorre um byte (armazenado em buffer)
    for (int i=0; i<8; i++) {
      if (matched_chars == char_amnt) break;
      // determina o i-ésimo bit do buffer
      int curr_bit = int((buffer >> i) & 1);
      std::cout << curr_bit << ' ';

      // se o nó atual for uma folha, escreve ele e volta para o começo da árvore
      if (search_pointer->left == nullptr && search_pointer->right == nullptr) {
        std::cout << "| ";
        outfile << search_pointer->value;
        search_pointer = root;
        matched_chars++;
      }

      // se o bit for 0 vá para a esquerda na árvore, direita senão
      search_pointer =
          curr_bit == 0 ? search_pointer->left : search_pointer->right;
    }
  }

}

void Huffman::write_compression(node *root, int leaves_amnt, int char_amnt, std::fstream &infile, std::fstream &outfile) {
  // gerar o dicionario das palavras
  std::string dict[128];
  std::string path;
  int node_number = 0;
  print_codes(root, "", dict, node_number);

  // escrevendo a header
  outfile.write((char*) &node_number, sizeof(node_number));
  outfile.write((char*) &char_amnt, sizeof(char_amnt));
  write_tree(root, outfile);

  // escrevendo o texto comprimido com padding caso necessário
  char curr_infile_chr;
  char buffer = 0;
  int curr_bit = 0;

  while (infile >> curr_infile_chr) {
    std::string code = dict[int(curr_infile_chr)];
    std::cout << "char: " << curr_infile_chr << '\t' << "code: " << code << std::endl;

    for (char &c: code) {
      // se o buffer estiver cheio, o escrevemos e zeramos
      if (curr_bit == 8) {
        outfile.write(&buffer, sizeof(char));
        buffer = curr_bit = 0;
      }
      
      if (c == '1') {
        buffer |= 1 << curr_bit;
      }

      curr_bit++;
    }

    // adicione o ultimo byte caso não tenha sido preenchido completamente
  }
  if (curr_bit != 0) {
    outfile.write(&buffer, sizeof(char));
  }
}
