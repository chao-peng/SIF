#include "ASTVisitor.hpp"

namespace Sif {

void before(std::string arg) {
    return;
}

void visit(ASTNode* node) {
    
    if (node->get_node_type() == NodeTypeAssignment) {
        Indentation emptyIndentation(0);
        AssignmentNode* assignment = (AssignmentNode*) node;
        ASTNodePtr operation = assignment->get_right_hand_operand();
        if (operation->get_node_type() == NodeTypeBinaryOperation && assignment->get_operator() == "=") {
            std::string assignmentResult = assignment->get_left_hand_operand()->source_code(emptyIndentation);
            std::string op = assignment->get_operator();
            auto binaryOperation = std::dynamic_pointer_cast<BinaryOperationNode>(operation);
            std::string requireAssertion;
            std::string assertAssertion;
            std::string LHD = binaryOperation->get_left_hand_operand()->source_code(emptyIndentation);
            std::string RHD = binaryOperation->get_right_hand_operand()->source_code(emptyIndentation);
            if (op == "/") {
                requireAssertion = "require(" + RHD + " != 0)";
            }
            std::string return_type = binaryOperation->get_return_type_str();
            if (return_type.find("uint") != std::string::npos) {
                if (op == "/") {
                    
                } else if (op == "*") {

                } else if (op == "+") {
                    
                } else if (op == "-") {

                }
            }
            node->insert_text_before(requireAssertion);
            node->insert_text_after(assertAssertion);
        }
    }
    return;
}

void after() {
    return;
}

}
