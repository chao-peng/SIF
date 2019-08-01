# AST Diff

Different from other tools, this tool requires two ASTs to compare. So we have to change the command line slightly.

Usage:

```bash
# Generate ASTs 
# Generate Text AST
solc --ast contract_name.sol >> contract_name.ast
solc --ast contract_name2.sol >> contract_name2.ast

# Generate JSON AST
solc --ast-compact-json contract_name.sol >> contract_name.json
solc --ast-compact-json contract_name2.sol >> contract_name2.json

# Run SIF
sif -v TypeOfFault -a contract_name.ast -j contract_name.json -b contract_name2.ast -k contract_name2.json
```

Example Output

```
Pragma Directive differs.
	In [arithmetic.sol]	pragma solidity ^0.4.16;
	In [arithmetic2.sol]	pragma solidity ^0.4.15;
Parameter List of Function [scoping] differs.
	In [arithmetic.sol]	(uint xxx)
	In [arithmetic2.sol]	()
Statement differs.
	In [arithmetic.sol]	uint b = 100;
	In [arithmetic2.sol]	--------
Statement differs.
	In [arithmetic.sol]	------------
	In [arithmetic2.sol]	uint b = 2;
Statement differs.
	In [arithmetic.sol]	------------
	In [arithmetic2.sol]	uint www;
Statement differs.
	In [arithmetic.sol]	d = c * b;
	In [arithmetic2.sol]	--------
Statement differs.
	In [arithmetic.sol]	------------
	In [arithmetic2.sol]	d = c * 999;
Parameter List of Function [scoping2] differs.
	In [arithmetic.sol]	()
	In [arithmetic2.sol]	(uint yyy)
Function [nobody] in Contract [ScopingErrors] not found in arithmetic2.sol
Contract [aaa] not found in arithmetic2.sol
Library specifier for ccc differs.
	In [arithmetic.sol]	True
	In [arithmetic2.sol]	False
```