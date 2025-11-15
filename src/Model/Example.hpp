#pragma once

struct Example
{
    std::string nome;
    float preco;
    int estoque;

    Example(std::string name, float price, int stock)
        : nome(std::move(name)), preco(price), estoque(stock) {}

    Example() = default;
};