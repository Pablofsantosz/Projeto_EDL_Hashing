# 🚗 Sistema de Cadastro de Carros com Acesso Direto via Hashing

Este projeto implementa um sistema de cadastro e gerenciamento de carros em linguagem C, utilizando **arquivos binários** e uma **tabela de hashing** para acesso direto aos dados.  

Foi desenvolvido com foco em desempenho e organização, permitindo buscas rápidas e manipulação eficiente dos registros.

---

## ⚙️ Funcionalidades

### 📥 Cadastrar Carro
- Adiciona um novo carro ao arquivo.
- Os dados inseridos são: **placa, marca, modelo e cor**.
- O carro é indexado na tabela hash com base na placa.

### 🔍 Consultar Carro
- Busca um carro pela **placa** e exibe suas informações, caso ele esteja ativo.

### ✏️ Alterar Dados
- Permite alterar **marca, modelo ou cor** de um carro cadastrado.

### ❌ Remover Carro
- Realiza uma **remoção lógica** (o carro permanece no arquivo, mas é marcado como inativo).
- O índice da tabela hash também é atualizado.

### 📄 Exibir Cadastros Ativos
- Lista todos os carros que ainda estão ativos no sistema.

### 🧹 Encerrar Programa (e Limpar Arquivo)
- Ao sair, o sistema remove fisicamente do arquivo os registros marcados como removidos.
- Também desaloca toda a tabela hash.

---

## 🗂️ Estrutura dos Dados

- **Arquivo:** `carros.dat`  
  Armazena os registros dos carros no formato binário.

- **Tabela Hash:**  
  Mantém um índice em memória com ponteiros para facilitar o acesso direto aos registros no arquivo.

---

## 📌 Tecnologias Utilizadas

- Linguagem C
- Estrutura de dados: lista duplamente encadeada
- Hashing com resolução por encadeamento ordenado
- Arquivos binários 

---



