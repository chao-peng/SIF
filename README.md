[![CircleCI](https://circleci.com/gh/chao-peng/SIF.svg?style=svg)](https://circleci.com/gh/chao-peng/SIF)


# SIF
Solidity Instrumentation Framework

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

The executable called sif will be in folder build/sif/sif

# Usage

```bash
# Generate ASTs 
solc --ast contract_name.sol >> contract_name.ast
solc --ast-compact-json contract_name.sol contract_name.json
sif -a contract_name.ast -j contract_name.json -o generated_contract.sol
```

If -o output_file_name is ommited, the generated code will be printed to the standard output (usually the screen).

# Code Instrumentation and Analysis

If you want to use SIF to instrument and analyse the AST, you should implement the function visit() in LibSif/ASTVisitor.cpp and re-build SIF.

Here is an example of implement the code, which prints all function names and changes all function_names to "func".

```c++
void visit(ASTNode* node) {
    if (node->get_node_type() == NodeTypeFunctionDefinition) {
        FunctionDefinitionNode* fd = (FunctionDefinitionNode*) node;
        std::cout << fd.get_name();
        fd->set_name("func");
    }
}
```

Each AST class has sufficient functions to retrieve information and make changes to the AST. Function prototypes can be found in LibSif/ASTNode.hpp. We are writing a documentation on this.

# Contact 

SIF is a very new framework for Solidity smart contract code instrumentation and analysis. We welcome all types of contributions to this project.

Contact details can be found at the author's homepage: [https://chao-peng.github.io](https://chao-peng.github.io)
