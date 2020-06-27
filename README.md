# Update

To simplify the usage of SIF, catch up official syntactic updates and minimise dependencies, SIF will be reimplemented in JavaScript based on the official solc-js compiler.

## Take List

- [ ] Code template
- [ ] AST traversal
- [ ] Code generation
- [ ] Plug-ins for instrumentation
- [ ] Basic tools

I'm working on SIF during my spare time. Please kindly allow some delays.

# Table of Contents

- [Update](#update)
  - [Take List](#take-list)
- [Table of Contents](#table-of-contents)
- [SIF](#sif)
- [Pre-requisite](#pre-requisite)
- [Usage](#usage)
- [Code Instrumentation and Analysis](#code-instrumentation-and-analysis)
- [Documentation](#documentation)
- [Tools](#tools)
- [Cite Our Work](#cite-our-work)
- [Contact](#contact)


![CircleCI](https://img.shields.io/circleci/build/github/chao-peng/SIF/master?token=b3c677431ad059030f63a0b5a53599dc03f524fb)
[![CircleCI](https://circleci.com/gh/chao-peng/SIF.svg?style=svg)](https://circleci.com/gh/chao-peng/SIF)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/32646ea4bd9d4d54a743cba33acb33ec)](https://www.codacy.com/app/chao-peng/SIF?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=chao-peng/SIF&amp;utm_campaign=Badge_Grade)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/chao-peng/SIF.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/chao-peng/SIF/context:cpp)
[![MIT License](https://img.shields.io/github/license/chao-peng/sif)](https://raw.githubusercontent.com/chao-peng/SIF/master/LICENSE)
![Maintenance](https://img.shields.io/maintenance/yes/2019)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://chao-peng.github.io/sif/index.html)
[![Try online](https://img.shields.io/badge/try-online-blue.svg)](https://wandbox.org/permlink/PnaL6bO9zipKRuKu)


# SIF
SIF (Solidity Instrumentation Framework) is a framework to query and instrument Solidity smart contracts.

You can try SIF online by clicking

[![Try online](https://img.shields.io/badge/try-online-blue.svg)](https://wandbox.org/permlink/PnaL6bO9zipKRuKu)

With SIF, you can easily query syntactic information from the AST of smart contracts and make changes to it. SIF is able to generate source code from the AST of Solidity contracts reflecting code instrumentations.

# Pre-requisite

1. Download the Solidity Compiler from [here](https://github.com/ethereum/solidity/releases/tag/v0.4.25)
2. Build SIF

In this folder
```bash
mkdir build
cd build
cmake ..
make
```

The executable called sif will be in folder build/sif/

# Usage

```bash
# Generate ASTs 
# Generate Text AST
solc --ast contract_name.sol >> contract_name.ast
# Generate JSON AST
solc --ast-compact-json contract_name.sol >> contract_name.json
# Run SIF
sif -a contract_name.ast -j contract_name.json -o generated_contract.sol
```

If -o output_file_name is ommited, the generated code will be printed to the standard output (usually the screen).

Command Line Options

```
  -a, --ast arg          AST in plain text file name
  -j, --json arg         AST in compact JSON format file name
  -o, --output arg       Output file name
  -h, --help             Print help message
  -v, --visitor_arg arg  extra arguments for the AST visitor
  -s, --silent           Silent mode (does not print out the generated source code)
```

# Code Instrumentation and Analysis

If you want to use SIF to instrument and analyse the AST, you should implement the function visit() in LibSif/ASTVisitor.cpp and re-build SIF.

Here is an example of implement the code, which prints all function names and changes all function_names to "func".

```c++
void visit(ASTNode* node) {
    if (node->get_node_type() == NodeTypeFunctionDefinition) {
        FunctionDefinitionNode* fd = (FunctionDefinitionNode*) node;
        std::cout << fd->get_name();
        fd->set_name("func");
    }
}
```

Each AST class has sufficient functions to retrieve information and make changes to the AST. Function prototypes can be found in LibSif/ASTNode.hpp.

# Documentation

We started writing the documentation for SIF but it is still in progress. A preliminary version can be found here:[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://chao-peng.github.io/sif/index.html).


# Tools

In the directory **Tools**, we provide 7 examples of using SIF to analyse the source code. You can learn how to use SIF to instrument smart contracts by reading our implementation of these tools.

**Assertion Analyser** inserts property assertions to operations susceptible to arithmetic vulnerabilities.

**AST Diff** compares smart contracts in syntax level.

**CG_Generator** is easy to follow, it generates a control graph for the smart contract by recording function calls.

**CFG_Generator** can be used to generate control-flow graphs for functions. It supports control flows including branches and loops.

**Fault Seeder** seeds faults to smart contracts.

**Function Listing** lists function definitions and in which contracts they are defined.

**Sif Rename** renames existing identifiers.

Tools can be compiled by replacing the original ASTVisitor.cpp by the one you want to use. The graph generation of **CG_Generator** and **CFG_Generator** is based on the graphviz toolkit. Make sure you have the tool on your machine and the command "dot" is available. On Ubuntu, you can simply install grahviz by using the command
```bash
sudo apt install graphviz.
```

# Cite Our Work

If you use SIF for your research, please kindly cite our work describing the SIF framework in your paper.

```bibtex
@inproceedings{peng2019sif,
  title={SIF: A Framework for Solidity Contract Instrumentation and Analysis},
  author={Peng, Chao and Akca, Sefa and Rajan, Ajitha},
  booktitle={2019 26th Asia-Pacific Software Engineering Conference (APSEC)},
  pages={466--473},
  year={2019},
  organization={IEEE}
}
```

> Peng, C., Akca, S. and Rajan, A., 2019, December. SIF: A Framework for Solidity Contract Instrumentation and Analysis. In 2019 26th Asia-Pacific Software Engineering Conference (APSEC) (pp. 466-473). IEEE.

# Contact 

SIF is a very new framework for Solidity smart contract code instrumentation and analysis. 

We welcome issues and pull request raised on Github. All types of contributions to this project are highly appreciated.

Contact details can be found at the author's homepage: [https://chao-peng.github.io](https://chao-peng.github.io)
