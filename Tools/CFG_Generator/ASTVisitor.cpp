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

int lastBlockNo;
int mostRecentIDInCodeBlocks;
std::map<int, std::string> blocks;
std::map<int, int> trueEdges;
std::map<int, int> falseEdges;
std::string functionName;
std::map<int, bool> isWaitingBlock;
std::map<int, bool> isUnlinkedBlock;
std::list<int> conditions;
bool endBlockNeeded;
int branchDepth;

int handle_stmt(ASTNodePtr _node);
int handle_block(BlockNodePtr _block);

bool isCondition(const int& _id) {
    if (std::find(conditions.begin(), conditions.end(), _id) == conditions.end()) {
        return false;
    } else {
        return true;
    }
}

bool conditionHasBranch(const int& _id, const bool& _branch) {
    if (_branch) {
        if (trueEdges.find(_id) != trueEdges.end()) {
            return true;
        } else {
            return false;
        }
    } else {
        if (falseEdges.find(_id) != falseEdges.end()) {
            return true;
        } else {
            return false;
        }
    }
}

void linkEdge(const int& _from, const int& _to, const bool& _edgeType = true) {
    if (conditionHasBranch(_from, _edgeType)) {
        return;
    }
    if (_edgeType) {
        std::cout << "[Link true " << _from << " to " << _to << "\n" << blocks[_from] << "\nTo\n" << blocks[_to] << "\n";
        trueEdges[_from] = _to;
    } else {
        std::cout << "[Link false " << _from << " to " << _to << "\n" << blocks[_from] << "\nTo\n" << blocks[_to] << "\n";
        falseEdges[_from] = _to;
    }
}

void before(std::string arg) {
    lastBlockNo = 0;
    functionName = arg;
    endBlockNeeded = false;
    branchDepth = 0;
}

int finishPreviousBlock(const std::string& _new_text="") {
    int previousFinishedNO = lastBlockNo;
    std::stringstream stringbuilder;
    stringbuilder << "[" << previousFinishedNO << "]\n" << _new_text;
    blocks[lastBlockNo++] = stringbuilder.str();
    return previousFinishedNO;
}

int handle_stmt(ASTNodePtr _node) {
    NodeType nodeType = _node->get_node_type();
    Indentation empty(0);
    int thisStmtID = lastBlockNo - 1;
    if (nodeType == NodeTypeIfStatement) {
        IfStatementNodePtr ifstmt = std::static_pointer_cast<IfStatementNode>(_node);
        std::string conditionText = ifstmt->get_condition()->source_code(empty);
        int conditionNo = finishPreviousBlock(conditionText);
        thisStmtID = conditionNo;
        conditions.push_back(conditionNo);
        int thenID = conditionNo + 1;
        auto thenstmt = ifstmt->get_then();
        branchDepth++;
        if (thenstmt->get_node_type() == NodeTypeBlockNode) {
            BlockNodePtr thenblock = std::static_pointer_cast<BlockNode>(thenstmt);
            thenID = handle_block(thenblock);
        } else {
            thenID = handle_stmt(thenstmt);
        }
        branchDepth--;
        linkEdge(conditionNo, thenID);
        auto elsestmt = ifstmt->get_else();
        if (elsestmt != nullptr) {
            int elseID = lastBlockNo;
            branchDepth++;
            if (elsestmt->get_node_type() == NodeTypeBlockNode) {
                BlockNodePtr elseblock = std::static_pointer_cast<BlockNode>(elsestmt);
                elseID = handle_block(elseblock);
            } else {
                elseID = handle_stmt(elsestmt);
            }
            branchDepth--;
            linkEdge(conditionNo, elseID, false);
        } else {
        }
    } else if (nodeType == NodeTypeForStatement) {
        ForStatementNodePtr forstmt = std::static_pointer_cast<ForStatementNode>(_node);
        std::string initText = forstmt->get_init()->source_code(empty);
        int initNo = finishPreviousBlock(initText);
        thisStmtID = initNo;
        std::string conditionText = forstmt->get_condition()->source_code(empty);
        int conditionNo = finishPreviousBlock(conditionText);
        linkEdge(initNo, conditionNo);
        conditions.push_back(conditionNo);
        auto loopbody = forstmt->get_body();
        int bodyFirstID = lastBlockNo - 1;
        int bodyLastID = lastBlockNo - 1;
        if (loopbody->get_node_type() == NodeTypeBlockNode) {
            BlockNodePtr loopbodyblock = std::static_pointer_cast<BlockNode>(loopbody);
            bodyFirstID = handle_block(loopbodyblock);
            bodyLastID = mostRecentIDInCodeBlocks;
        } else {
            bodyFirstID = handle_stmt(loopbody);
            bodyLastID = bodyFirstID;
        }
        std::string incrementText = forstmt->get_increment()->source_code(empty);
        int incrementNo = finishPreviousBlock(incrementText);
        linkEdge(bodyLastID, incrementNo);
        linkEdge(incrementNo, conditionNo);
        linkEdge(conditionNo, bodyFirstID);
        if (branchDepth != 1) {
            linkEdge(conditionNo, incrementNo + 1, false);
        }
    } else if (nodeType == NodeTypeDoWhileStatement) {
        DoWhileStatementNodePtr doStmt = std::static_pointer_cast<DoWhileStatementNode>(_node);
        std::string conditionText = doStmt->get_condition()->source_code(empty);
        auto loopbody = doStmt->get_loop_body();
        int bodyFirstID = lastBlockNo - 1;
        int bodyLastID = lastBlockNo - 1;
        if (loopbody->get_node_type() == NodeTypeBlockNode) {
            BlockNodePtr loopbodyblock = std::static_pointer_cast<BlockNode>(loopbody);
            bodyFirstID = handle_block(loopbodyblock);
            bodyLastID = mostRecentIDInCodeBlocks;
        } else {
            bodyFirstID = handle_stmt(loopbody);
            bodyLastID = bodyFirstID;
        }
        thisStmtID = bodyFirstID;
        int conditionNo = finishPreviousBlock(conditionText);
        conditions.push_back(conditionNo);
        linkEdge(bodyLastID, conditionNo);
        linkEdge(conditionNo, bodyFirstID);
        if (branchDepth != 1) {
            linkEdge(conditionNo, conditionNo + 1, false);
        }
    } else if (nodeType == NodeTypeWhileStatement) {
        WhileStatementNodePtr whileStmt = std::static_pointer_cast<WhileStatementNode>(_node);
        std::string conditionText = whileStmt->get_condition()->source_code(empty);
        int conditionNo = finishPreviousBlock(conditionText);
        thisStmtID = conditionNo;
        conditions.push_back(conditionNo);
        auto loopbody = whileStmt->get_loop_body();
        int bodyFirstID = lastBlockNo - 1;
        int bodyLastID = lastBlockNo - 1;
        if (loopbody->get_node_type() == NodeTypeBlockNode) {
            BlockNodePtr loopbodyblock = std::static_pointer_cast<BlockNode>(loopbody);
            bodyFirstID = handle_block(loopbodyblock);
            bodyLastID = mostRecentIDInCodeBlocks;
        } else {
            bodyFirstID = handle_stmt(loopbody);
            bodyLastID = bodyFirstID;
        }
        linkEdge(bodyLastID, conditionNo);
        linkEdge(conditionNo, bodyFirstID);
        if (branchDepth != 1) {
            linkEdge(conditionNo, bodyLastID + 1, false);
        }
    } else {
        thisStmtID = finishPreviousBlock(_node->source_code(empty));
    }
    return thisStmtID;
}

int handle_block(BlockNodePtr _block) {
    Indentation empty(0);
    int mostRecentID = lastBlockNo - 1;
    int thisBlockFirstID = -100;
    int previousFinishedID = -100;
    for (int i = 0; i < _block->num_statements(); ++i) {
        ASTNodePtr currentNode;
        NodeType nodeType;
        std::string currentBlockText = "";
        while (i < _block->num_statements()) {
            currentNode = _block->get_statement(i);
            nodeType = currentNode->get_node_type();
            if (nodeType != NodeTypeIfStatement && nodeType != NodeTypeWhileStatement && nodeType != NodeTypeDoWhileStatement && nodeType != NodeTypeForStatement) {
                currentBlockText = currentBlockText + currentNode->source_code(empty) + "\n";
                i++;
            } else {
                break;
            }
        }

        if (currentBlockText != "" || _block->num_statements() == 0) {
            mostRecentID = finishPreviousBlock(currentBlockText);
            previousFinishedID = mostRecentID;
            if (thisBlockFirstID == -100) {
                thisBlockFirstID = mostRecentID;
            }
        }
        if (nodeType == NodeTypeIfStatement) {
            mostRecentID = handle_stmt(currentNode);
            if (previousFinishedID != -100 && std::find(conditions.begin(), conditions.end(), previousFinishedID) == conditions.end()) {
                linkEdge(previousFinishedID, mostRecentID);
                previousFinishedID = mostRecentID;
            }
            if (thisBlockFirstID == -100) {
                thisBlockFirstID = mostRecentID;
            }
        } else if (nodeType == NodeTypeForStatement) {
            mostRecentID = handle_stmt(currentNode);
            if (previousFinishedID != -100) {
                linkEdge(previousFinishedID, mostRecentID);
                previousFinishedID = mostRecentID;
            }
            if (thisBlockFirstID == -100) {
                thisBlockFirstID = mostRecentID;
            }
        } else if (nodeType == NodeTypeDoWhileStatement) {
            mostRecentID = handle_stmt(currentNode);
            if (previousFinishedID != -100) {
                linkEdge(previousFinishedID, mostRecentID);
                previousFinishedID = mostRecentID;
            }
            if (thisBlockFirstID == -100) {
                thisBlockFirstID = mostRecentID;
            }
        } else if (nodeType == NodeTypeWhileStatement) {
            mostRecentID = handle_stmt(currentNode);
            if (previousFinishedID != -100 && std::find(conditions.begin(), conditions.end(), previousFinishedID) == conditions.end()) {
                linkEdge(previousFinishedID, mostRecentID);
                previousFinishedID = mostRecentID;
            }
            if (thisBlockFirstID == -100) {
                thisBlockFirstID = mostRecentID;
            }
        } 
        
    }
    mostRecentIDInCodeBlocks = mostRecentID;
    return thisBlockFirstID;
}


void visit(ASTNode* node) {
    auto currentNodeType = node->get_node_type();
    if (currentNodeType == NodeTypeFunctionDefinition) {
        FunctionDefinitionNode* fd = (FunctionDefinitionNode*) node;
        if (fd->get_name() == functionName) {
            auto block = fd->get_function_body();
            Indentation empty(0);
            std::string parameters = fd->get_name() + fd->get_params()->source_code(empty);
            int parametersBlockID = finishPreviousBlock(parameters);
            handle_block(block);
            linkEdge(parametersBlockID, parametersBlockID + 1);
            auto lastNodeType = block->get_statement(block->num_statements() - 1)->get_node_type();
            if (lastNodeType == NodeTypeIfStatement || lastNodeType == NodeTypeWhileStatement || lastNodeType == NodeTypeDoWhileStatement || lastNodeType == NodeTypeForStatement) {
                blocks[lastBlockNo++] = "END";
            } 
        }
        
    }
}

void after() {
    std::stringstream cfgstream;
    std::ofstream output("sif_tmp_cfg_000.gv");
    cfgstream << "digraph CFG {\n";
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        std::string shape = "shape=\"box\",style=\"rounded\"";
        if (std::find(conditions.begin(), conditions.end(), it->first) != conditions.end()) {
            shape = "shape=\"diamond\"";
        }
        isWaitingBlock[it->first] = true;
        isUnlinkedBlock[it->first] = true;
        cfgstream << "    nd_" << it->first << " [" << shape << ",labelloc=l, label=\"" << it->second << "\"];\n";
    }
    for (auto it = trueEdges.begin(); it != trueEdges.end(); ++it) {
        if (std::find(conditions.begin(), conditions.end(), it->first) != conditions.end()) {
            cfgstream << "    nd_" << it->first << " -> nd_" << it->second << "[label=\"True\"];\n";
            isUnlinkedBlock[it->second] = false;
        }
        else {
            cfgstream << "    nd_" << it->first << " -> nd_" << it->second << ";\n"; 
            isWaitingBlock[it->first] = false;
            isUnlinkedBlock[it->second] = false;
        }
    }
    for (auto it = falseEdges.begin(); it != falseEdges.end(); ++it) {
        cfgstream << "    nd_" << it->first << " -> nd_" << it->second << "[label=\"False\"];\n";
        isWaitingBlock[it->first] = false;
        isUnlinkedBlock[it->second] = false;
    }

    isWaitingBlock[isWaitingBlock.size()-1] = false;
    /*
    for (auto it = conditions.begin(); it != conditions.end(); ++it) {
        if (isWaitingBranch[*it]) {
            int id = *it + 1;
            while (isUnlinkedBlock[id]) {
                id++;
            }
            cfgstream << "    nd_" << *it << " -> nd_" << id << "[label=\"False\"];\n";
        }
    }
*/

    
    for (auto it = isWaitingBlock.begin(); it != isWaitingBlock.end(); ++it) {
        if (it->second) {
            int id = it->first + 1;
            while (id < blocks.size() && !isUnlinkedBlock[id]) {
                id++;
            }
            if (id >= blocks.size()) id = blocks.size()-1;
            if (isCondition(it->first)) {
                cfgstream << "    nd_" << it->first << " -> nd_" << id << "[label=\"False\"];\n";
            } else {
                cfgstream << "    nd_" << it->first << " -> nd_" << id << ";\n";
            }
        }
    }

    cfgstream << "}\n";
    output << cfgstream.str();
    output.close();
    system("dot sif_tmp_cfg_000.gv -Tpng -o cfg.png");
    //system("rm -f sif_tmp_cfg_000.gv");
    
}

}
