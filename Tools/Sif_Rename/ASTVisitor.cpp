#include "ASTVisitor.hpp"

namespace Sif {


std::string visitor_arg;

std::string structName;
std::string identifierName;
std::string identifierNewName;

void parseSubstr(const std::string& substr) {
    if (substr.find("target=") != substr.npos) {
        structName = substr.substr(7);
    } else if (substr.find("oldname=") != substr.npos) {
        identifierName = substr.substr(8);
    } else if (substr.find("newname=") != substr.npos) {
        identifierNewName = substr.substr(8);
    }
}

void before(std::string _visitor_arg) {
    visitor_arg = _visitor_arg;
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    while ((pos = _visitor_arg.find(delimiter)) != std::string::npos) {
        token = _visitor_arg.substr(0, pos);
        parseSubstr(token);
        _visitor_arg.erase(0, pos + delimiter.length());
    }
    parseSubstr(_visitor_arg);
}

void visit(ASTNode* node) {
    Indentation emptyindentation(0);
    if (structName == "function") {
        if (node->get_node_type() == NodeTypeFunctionDefinition) {
            FunctionDefinitionNode* fdn = (FunctionDefinitionNode*) node;
            if (fdn->get_name() == identifierName) {
                fdn->set_name(identifierNewName);
            }
        } else if (node->get_node_type() == NodeTypeFunctionCall) {
            FunctionCallNode* fcn = (FunctionCallNode*) node;
            ASTNodePtr callee = fcn->get_callee();
            if (callee->get_node_type() == NodeTypeIdentifier) {
                IdentifierNodePtr in = std::dynamic_pointer_cast<IdentifierNode>(callee);
                if (in->get_name() == identifierName) {
                    in->set_name(identifierNewName);
                }
            } else if (callee->get_node_type() == NodeTypeMemberAccess) {
                MemberAccessNodePtr man = std::dynamic_pointer_cast<MemberAccessNode>(callee);
                if (man->get_member() == identifierName) {
                    man->set_member(identifierNewName);
                }
            }
        }
    } else if (structName == "variable") {
        if (node->get_node_type() == NodeTypeVariableDeclaration) {
            VariableDeclarationNode* vdn = (VariableDeclarationNode*) node;
            if (vdn->get_variable_name() == identifierName) {
                vdn->set_variable_name(identifierNewName);
            }
        } else if (node->get_node_type() == NodeTypeIdentifier) {
            IdentifierNode* in = (IdentifierNode*) node;
            if (in->get_name() == identifierName) {
                in->set_name(identifierNewName);
            }
        }
    } else if (structName == "contract") {
        if (node->get_node_type() == NodeTypeContractDefinition) {
            ContractDefinitionNode* cdn = (ContractDefinitionNode*) node;
            if (cdn->get_name() == identifierName) {
                cdn->set_name(identifierNewName);
            } 
            for (auto i = 0; i < cdn->num_inherit_from(); ++i) {
                if (cdn->get_inherit_from(i) == identifierName) {
                    cdn->update_inherit_from(i, identifierNewName);
                }
            }
        } else if (node->get_node_type() == NodeTypeMemberAccess) {
            MemberAccessNode* man = (MemberAccessNode*)(node);
            if (man->get_identifier()->get_node_type() == NodeTypeIdentifier) {
                IdentifierNodePtr in = std::dynamic_pointer_cast<IdentifierNode>(man->get_identifier());
                if (in->get_name() == identifierName) {
                    in->set_name(identifierNewName);
                }
            }
        } else if (node->get_node_type() == NodeTypeUsingForDirective) {
            UsingForDirectiveNode* ufdn = (UsingForDirectiveNode*) node;
            if (ufdn->get_using() == identifierName) {
                ufdn->set_using(identifierNewName);
            }
        }
    }
}

void after() {
    return;
}

}
