# SIF Rename

Usage:

```bash
# Generate ASTs 
# Generate Text AST
solc --ast contract_name.sol >> contract_name.ast
# Generate JSON AST
solc --ast-compact-json contract_name.sol >> contract_name.json
# Run SIF
sif -v "target=function oldname=oldname newname=newname" -a contract_name.ast -j contract_name.json -o generated_contract.sol
```

Target can be:
- function
- variable
- contract