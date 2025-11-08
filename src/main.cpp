#include <iostream>
#include <sstream>
#include <fstream>
#include <optional>
#include <vector>

#include "./parser.hpp"
#include "./tokenization.hpp"
#include "./generation.hpp"


int main(int argc, char* argv[]) {
    //Expect one arg after the program name
    if (argc != 2)
    {
        std::cerr << "Invalid Usage. Valid Usage is..." << std::endl;
        std::cerr << "nitro <input.ni>" << std::endl;
        return EXIT_FAILURE;
    }


    //Read the entire string file - test.ni
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        if (!input) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    //Tokenizer
    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();

    //Parser
    Parser parser(std::move(tokens));
    std::optional<NodeExit> tree = parser.parse();

    if (!tree.has_value()){
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        exit(EXIT_FAILURE);
    }

    //Generation
    Generator generator(tree.value());

    {
         std::fstream file("output.asm", std::ios::out);
         file << generator.generate();
    }

    //system commands in WIN
    system("wsl nasm -felf64 output.asm");
    system("wsl ld output.o -o output");

    return EXIT_SUCCESS;
}