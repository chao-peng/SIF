#include <cstdlib>
#include "ASTVisitor.hpp"
#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <fstream>
#include <list>
#include <stack>

namespace Sif {

std::map<std::string, std::set<std::string> > cg;
std::string currentFunctionName;

void before(std::string arg) {
    currentFunctionName = "";
}

void visit(ASTNode* node) {
    if (node->get_node_type() == NodeTypeFunctionDefinition) {
        FunctionDefinitionNode* fd = (FunctionDefinitionNode*) node;
        currentFunctionName = fd->get_name();
    }
    if (node->get_node_type() == NodeTypeFunctionCall && currentFunctionName != "") {
        FunctionCallNode* fc = (FunctionCallNode*) node;
        Indentation empty(0);
        std::string functioncall = fc->get_callee()->source_code(empty);
        cg[currentFunctionName].insert(functioncall);
    }
    return;
}

void after() {
    std::stringstream cgstream;
    std::ofstream output("sif_tmp_cg_000.gv");
    cgstream << "digraph CG {\n";
    for (auto it = cg.begin(); it != cg.end(); ++it) {
        for (auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2) {
            cgstream << "    \"" << it->first << "\" -> \"" << *it2 << "\";\n";
        }
    }
    cgstream << "}\n";
    output << cgstream.str();
    output.close();
    system("dot sif_tmp_cg_000.gv -Tpng -o cg.png");
    system("rm -f sif_tmp_cg_000.gv");
}


}
