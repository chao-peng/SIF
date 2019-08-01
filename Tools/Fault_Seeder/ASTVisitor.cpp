#include "ASTVisitor.hpp"

namespace Sif {

std::string mutation;

void before(std::string arg) {
    std::string mutation_type = arg;
    if (mutation_type == "overflow") {
        mutation = "    //unsigned overflow added\n    uint8 t1=250;\n    uint8 test=255+t1;\n    //********\n";
    } else if (mutation_type == "underflow") {
        mutation = "    //underflow added\n    uint256 underflow=20;\n    uint256 subtrahend =250;\n    uint256 result = underflow -subtrahend;\n    //********\n";
    } else if (mutation_type == "divbyzero") {
        mutation = "    //division by zero added\n    uint input=10;\n    uint divisor =input - 10;\n    uint quotient = 100/divisor;\n    //********\n";
    }
}

void visit(ASTNode* node) {
    if (node->get_node_type() == NodeTypeFunctionDefinition) {
        FunctionDefinitionNode* fdn = (FunctionDefinitionNode*) node;
        if (fdn->get_function_body()->num_statements()) {
            fdn->get_function_body()->get_statement(0)->insert_text_before(mutation + "\n");
        } else {
            auto nullStatement = std::make_shared<ReturnNode>();
            fdn->get_function_body()->add_statement(nullStatement);
            fdn->get_function_body()->get_statement(0)->insert_text_before(mutation + "\n");
        }
    }
}

void after() {
    return;
}

}
