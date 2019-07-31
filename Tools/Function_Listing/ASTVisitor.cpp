#include "ASTVisitor.hpp"

namespace Sif {

std::string currentContractName;

void before(std::string arg) {
    currentContractName = "";
}

std::stringstream iterateParamList(ParameterListNodePtr param) {
    std::stringstream ss;
    Indentation empty(0);
    for (int i = 0; i < param->num_parameters(); ++i) {
        VariableDeclarationNodePtr varDecl = std::dynamic_pointer_cast<VariableDeclarationNode>(param->get_parameter(i));
        if (varDecl->get_variable_name() != "") {
            ss << "        [Name] " << varDecl->get_variable_name() << " [Type] " << varDecl->get_type()->source_code(empty) << "\n";
        } else {
            ss << "        [Type] " << varDecl->get_type()->source_code(empty) << "\n";
        }
    }
    return ss;
}

void visit(ASTNode* node) {
    Indentation empty(0);
    if (node->get_node_type() == NodeTypeContractDefinition) {
        ContractDefinitionNode* cd = (ContractDefinitionNode*) node;
        currentContractName = cd->get_name();
    }
    if (node->get_node_type() == NodeTypeFunctionDefinition) {
        FunctionDefinitionNode* fd = (FunctionDefinitionNode*) node;


        auto functionName = fd->get_name();
        ParameterListNodePtr returns = fd->get_returns();
        ParameterListNodePtr parameters = fd->get_params();
        std::cout << "[In " << currentContractName << "] " << functionName << parameters->source_code(empty) << " returns " << returns->source_code(empty) << "\n";
/*
        std::cout << "[Function Name] " << functionName << "\n";

        if (returns->num_parameters()) {

                std::cout << "    [Return List]\n";
                std::cout << iterateParamList(returns).str();
            
        } else {
            std::cout << "    [No Return Value]\n";
        }

        if (parameters->num_parameters()) {

                std::cout << "    [Parameter List]\n";
                std::cout << iterateParamList(parameters).str();
            
        } else {
            std::cout << "    [No Parameter]\n";
        }
        std::cout << "\n";
*/
    }
}

void after() {

}

}
