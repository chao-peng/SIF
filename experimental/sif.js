var solc = require('solc');

var input = {
    language: 'Solidity',
    sources: {
        'test.sol': {
            content: 'contract C { function f() public { } }'
        }
    },
    settings: {
        outputSelection: {
            '*': {
                '*': ['*'],
                '': ['ast']
            }
        }
}
};

var output = JSON.parse(solc.compile(JSON.stringify(input)));

console.log(output.sources['test.sol'].ast);