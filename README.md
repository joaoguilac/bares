﻿# Trabalho 05: Projeto BARES (_Basic ARithmetic Expression Evaluator based on Stacks_)

# Apresentação

Nesse trabalho você terá a oportunidade de implementar um avaliador de expressões simples.
A descrição completa do trabalho se encontra [**aqui**](docs/bares.pdf).


Além da implementação do trabalho você precisa fornecer algumas informações no arquivo [author.md](author.md), indicando a autoria do trabalho e quais itens da tarefa foram realizados com sucesso. 

# Compilando e Executando

## Manualmente com g++

Para compilar e executar o programa manualmente basta digitar os seguintes comandos em seu terminal:

```bash
# Caso não tenha uma pasta para os executáveis, você deve criá-la com esse comando
$ mkdir bin

# Compilar
$ g++ -Wall -std=c++11 -g source/src/main.cpp source/src/parser.cpp source/src/bares_manager.cpp -I source/include -o bin/bares

# Executar
$ ./bin/bares
$ Digite a expressão a ser calculada
```

## Cmake

Para compilar e executar o programa com o Cmake você precisa ter instalado pelo menos a versão 3.5. Em seguida, abra seu terminal e execute os seguintes comandos:

```bash
# Pede ao cmake para criar o diretório de compilação
$ cmake -S source -B build

# Compilar
$ cmake --build build

 # Executar
$ ./build/bares
$ Digite a expressão a ser calculada
```

--------
&copy; DIMAp/UFRN 2021.
