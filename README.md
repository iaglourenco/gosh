# gosh

[![Build](https://github.com/iaglourenco/gosh/actions/workflows/builld.yaml/badge.svg)](https://github.com/iaglourenco/gosh/actions/workflows/builld.yaml)

gosh significa **G**reat, An**o**ther **Sh**ell. É um shell Linux muito simples escrito em C. Foi feito apenas para fins educacionais e não se destina a ser usado em um ambiente de produção.

# Conceitos
- Chamadas de sistema
- Criação e gerenciamento de processos
- Gerenciamento de arquivos

# Recursos
- [x] Executar comandos interativamente, obviamente.
- [x] Executar comandos a partir de um arquivo.
- [x] Executar comandos em paralelo.
- [x] Redirecionamento de saída para um arquivo.
- [x] Um clone do comando cat.
- [x] Um clone do comando ls.

# Como usar

## Binários pré-compilados

Você pode baixar o último binário pré-compilado na página de [lançamentos](https://github.com/iaglourenco/gosh/releases).

Extraia os arquivos e navegue até o diretório onde o binário está localizado.

```bash
unzip gosh.zip
cd gosh
```

Para executar o shell, basta executar o arquivo binário.

```bash
./gosh
```

Se você quiser usar os comandos `cat` ou `ls`, você precisa registrar o caminho dos binários usando o comando `path` ao executar o shell.

```bash
./gosh
gosh> path </caminho/para/os/binários>
```

## Compilar a partir do código fonte

Para compilar o shell a partir do código fonte, você precisa ter os pacotes `build-essential` e `git` instalados em seu sistema.

```bash
sudo apt install build-essential git make
```

Depois, clone o repositório e compile o código fonte usando o comando `make`.

```bash
git clone https://github.com/iaglourenco/gosh.git
cd gosh
make all
```

# Comandos

- `cd` - Muda o diretório atual.
- `exit` - Sai do shell.
- `cat` - Concatena e exibe o conteúdo dos arquivos. (Precisa estar no mesmo diretório que o binário do shell)
- `ls` - Lista os arquivos no diretório atual. (Precisa estar no mesmo diretório que o binário do shell)
- `path` - Registra o caminho onde o shell procurará programas para executar, é o mesmo que a variável de ambiente PATH.
- `help` - Exibe a mensagem de ajuda.

## Executar comandos a partir de um arquivo

Você pode executar comandos a partir de um arquivo usando a seguinte sintaxe:

```bash
./gosh <arquivo>
```

# Licença

Este projeto está licenciado sob a Licença Pública Geral GNU v3.0 - veja o arquivo [LICENSE](LICENSE) para detalhes.