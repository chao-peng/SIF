# Assertion Analyser

Usage:

```bash
# Generate ASTs 
# Generate Text AST
solc --ast contract_name.sol >> contract_name.ast
# Generate JSON AST
solc --ast-compact-json contract_name.sol >> contract_name.json
# Run SIF
sif -a contract_name.ast -j contract_name.json [-o generated_contract.sol]
```