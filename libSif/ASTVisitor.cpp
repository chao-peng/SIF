#include <cstdlib>
#include "ASTVisitor.hpp"
#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <fstream>

namespace Sif {

std::map<std::string, std::set<std::string> > cfg;
std::string currentFunctionName;

void before() {
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
        cfg[currentFunctionName].insert(functioncall);
    }
    return;
}

void after() {
    std::stringstream cfgstream;
    std::ofstream output("sif_tmp_cfg_000.gv");
    cfgstream << "digraph CFG {\n";
    for (auto it = cfg.begin(); it != cfg.end(); ++it) {
        for (auto it2 = (it->second).begin(); it2 != (it->second).end(); ++it2) {
            cfgstream << "    \"" << it->first << "\" -> \"" << *it2 << "\";\n";
        }
    }
    cfgstream << "}\n";
    output << cfgstream.str();
    output.close();
    system("dot sif_tmp_cfg_000.gv -Tpng -o cfg.png");
    system("rm -f sif_tmp_cfg_000.gv");
}

}
