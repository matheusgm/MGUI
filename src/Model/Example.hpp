#pragma once

struct Example
{
    std::string nome;
    float preco;
    int estoque;

    // Construtor explícito para emplace_back (C++ Moderno)
    Example(std::string name, float price, int stock)
        : nome(std::move(name)), preco(price), estoque(stock) {}

    // O construtor padrão ainda é necessário ou pode ser default-ed,
    // dependendo de onde o vector é redimensionado/alocado.
    Example() = default;
};