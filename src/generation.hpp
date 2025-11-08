#pragma once

#include "parser.hpp"

class Generator
{
public:
    inline Generator(NodeExit root)
        :m_root(std::move(root))
    {
    }

    [[nodiscard]] std::string generate() const{
        std::stringstream output;
        output << "   mov rax, 60\n";
        output << "   mov rdi, " << m_root.expr.int_lit.value.value() << std::endl;
        output << "   syscall";
        return output.str();
    }

private:
    const NodeExit m_root;

};