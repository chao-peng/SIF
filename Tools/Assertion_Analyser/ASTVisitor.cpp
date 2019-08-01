#include "ASTVisitor.hpp"

namespace Sif {

void before(std::string arg) {
    return;
}

void visit(ASTNode* node) {
    Indentation emptyIndentation(0);
    if (node->get_node_type() == NodeTypeExpressionStatement) {
        ExpressionStatementNode* esn = (ExpressionStatementNode*) node;
        auto esnExpression = esn->get_expression();
        if (esnExpression->get_node_type() == NodeTypeAssignment) {
            AssignmentNodePtr assignment = std::dynamic_pointer_cast<AssignmentNode>(esnExpression);
            ASTNodePtr operation = assignment->get_right_hand_operand();
            if (operation->get_node_type() == NodeTypeBinaryOperation && assignment->get_operator() == "=") {
                std::string assignmentResult = assignment->get_left_hand_operand()->source_code(emptyIndentation);
                std::string op = assignment->get_operator();
                auto binaryOperation = std::dynamic_pointer_cast<BinaryOperationNode>(operation);
                std::string preCondition;
                std::string postCondition;
                std::string LHD = binaryOperation->get_left_hand_operand()->source_code(emptyIndentation);
                std::string RHD = binaryOperation->get_right_hand_operand()->source_code(emptyIndentation);
                std::string result_type = binaryOperation->get_return_type_str();
                std::string binOperationOperator = binaryOperation->get_operator();
                if (binOperationOperator == "/") {
                    preCondition = "require(" + RHD + " != 0);\n";
                }
                if (result_type.find("uint") != std::string::npos) {
                    if (binOperationOperator == "/") {
                        // Division will not cause overflow/underflow
                    } else if (binOperationOperator == "*") {
                        postCondition = "\n(" + LHD + " != 0 && " + RHD + " != 0)? assert(" + assignmentResult + " >= " + LHD + " && " + assignmentResult + " >= " + RHD + ") : assert(" + assignmentResult + " == 0);\n";
                    } else if (binOperationOperator == "+") {
                        postCondition = "\nassert(" + assignmentResult + " >= " + LHD + " && " + assignmentResult + " >= " + RHD + ");\n";
                    } else if (binOperationOperator == "-") {
                        postCondition = "\nassert(" + LHD + " >= " + assignmentResult + " && " + LHD + " >= " + RHD + ");\n";
                    }
                } else if (result_type.find("int") != std::string::npos) {
                    if (binOperationOperator == "/") {
                        // Division will not cause overflow/underflow
                    } else if (binOperationOperator == "*") {
                        postCondition = "\n(" + LHD + " != 0 && " + RHD + " != 0)? assert((" + assignmentResult + " / " + LHD + " == " + RHD + ") && (" + assignmentResult + " / " + RHD + " == " + LHD + ")) : assert(" + assignmentResult + "== 0);\n";
                    } else if (binOperationOperator == "+") {
                        postCondition = "\nassert((" + RHD + " >= 0 && " + assignmentResult + " >= " + LHD + ") || (" + RHD + " < 0 && " + assignmentResult + " < " + LHD + "));\n";
                    } else if (binOperationOperator == "-") {
                        postCondition = "\nassert((" + RHD + " >= 0 && " + assignmentResult + " <= " + LHD + ") || (" + RHD + " < 0 && " + assignmentResult + " > " + LHD + "));\n";
                    }
                }
                node->insert_text_before(preCondition);
                node->insert_text_after(postCondition);
            }
        }
        
    } else if (node->get_node_type() == NodeTypeVariableDeclarationStatement) {
        VariableDeclarationStatementNode* vdsn = (VariableDeclarationStatementNode*) node;
        auto initial_value = vdsn->get_value();
        if (initial_value != nullptr) {
            if (initial_value->get_node_type() == NodeTypeBinaryOperation) {
                auto decl = vdsn->get_decl();
                std::string assignmentResult = decl->get_variable_name();
                auto binaryOperation = std::dynamic_pointer_cast<BinaryOperationNode>(initial_value);
                std::string preCondition;
                std::string postCondition;
                std::string LHD = binaryOperation->get_left_hand_operand()->source_code(emptyIndentation);
                std::string RHD = binaryOperation->get_right_hand_operand()->source_code(emptyIndentation);
                std::string result_type = binaryOperation->get_return_type_str();
                std::string binOperationOperator = binaryOperation->get_operator();
                if (binOperationOperator == "/") {
                    preCondition = "require(" + RHD + " != 0);\n";
                }
                if (result_type.find("uint") != std::string::npos) {
                    if (binOperationOperator == "/") {
                        // Division will not cause overflow/underflow
                    } else if (binOperationOperator == "*") {
                        postCondition = "\n(" + LHD + " != 0 && " + RHD + " != 0)? assert(" + assignmentResult + " >= " + LHD + " && " + assignmentResult + " >= " + RHD + ") : assert(" + assignmentResult + " == 0);\n";
                    } else if (binOperationOperator == "+") {
                        postCondition = "\nassert(" + assignmentResult + " >= " + LHD + " && " + assignmentResult + " >= " + RHD + ");\n";
                    } else if (binOperationOperator == "-") {
                        postCondition = "\nassert(" + LHD + " >= " + assignmentResult + " && " + LHD + " >= " + RHD + ");\n";
                    }
                } else if (result_type.find("int") != std::string::npos) {
                    if (binOperationOperator == "/") {
                        // Division will not cause overflow/underflow
                    } else if (binOperationOperator == "*") {
                        postCondition = "\n(" + LHD + " != 0 && " + RHD + " != 0)? assert((" + assignmentResult + " / " + LHD + " == " + RHD + ") && (" + assignmentResult + " / " + RHD + " == " + LHD + ")) : assert(" + assignmentResult + "== 0);\n";
                    } else if (binOperationOperator == "+") {
                        postCondition = "\nassert((" + RHD + " >= 0 && " + assignmentResult + " >= " + LHD + ") || (" + RHD + " < 0 && " + assignmentResult + " < " + LHD + "));\n";
                    } else if (binOperationOperator == "-") {
                        postCondition = "\nassert((" + RHD + " >= 0 && " + assignmentResult + " <= " + LHD + ") || (" + RHD + " < 0 && " + assignmentResult + " > " + LHD + "));\n";
                    }
                }
                node->insert_text_before(preCondition);
                node->insert_text_after(postCondition);
            }
        } 
    }
    return;
}

void after() {
    return;
}

}
