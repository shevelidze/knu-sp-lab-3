#include <iostream>
#include <fstream>

#include "LexicalAnalyzer.hpp"

int main(int argc, char **argv)
{
  std::ifstream inputFile;

  inputFile.open(argv[1]);

  if (!inputFile.is_open())
  {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }

  LexicalAnalyzer lexicalAnalyzer(inputFile);

  auto tokens = lexicalAnalyzer.analyze();

  for (auto token : tokens)
  {
    std::cout << token.getANSIEscapeCode() << token.getContent();
  }

  std::cout << '\n';

  return 0;
}