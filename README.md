# gosh

[![Build](https://github.com/iaglourenco/gosh/actions/workflows/builld.yaml/badge.svg)](https://github.com/iaglourenco/gosh/actions/workflows/builld.yaml)

gosh stands for **G**reat, An**o**ther **Sh**ell. It is a very simple Linux shell written in C. It was made only for educational purposes and it is not intended to be used in a production environment.

# Concepts
- System calls
- Creation and management of processes
- File management


# Features
- [x] Execute commands interactively, obviously.
- [x] Execute commands from a file.
- [ ] Execute commands in parallel.(Work in progress)
- [ ] Output redirection to a file. (Work in progress)
- [x] A clone of cat command.
- [x] A clone of ls command.


# How to use

## Precompiled binaries

You can download the latest precompiled binary from the [releases](https://github.com/iaglourenco/gosh/releases) page.

Extract the files and navigate to the directory where the binary is located.

```bash
unzip gosh.zip
cd gosh
```

To run the shell, just execute the binary file.

```bash
./gosh
```

If you want to use the `cat` or `ls` commands, you need to register the path of the binaries using the `path` command while running the shell.

```bash

./gosh

gosh> path </path/to/binaries>
```


## Compile from source

To compile the shell from source, you need to have the `build-essential` and `git` packages installed on your system.

```bash
sudo apt install build-essential
```

Then, clone the repository and compile the source code using the `make` command.

```bash
git clone https://github.com/iaglourenco/gosh.git

cd gosh

make all
```

# Commands

- `cd` - Changes the current directory.
- `exit` - Exits the shell.
- `cat` - Concatenates and displays the content of files. (Needs to be in the same directory as the shell binary)
- `ls` - Lists the files in the current directory. (Needs to be in the same directory as the shell binary)
- `path`- Register the path where the shell will look for programs to execute, it's the same as the PATH environment variable.
- `help` - Displays the help message.


## Execute commands from a file

You can execute commands from a file using the following syntax:

```bash
./gosh <file>
```

# License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.
