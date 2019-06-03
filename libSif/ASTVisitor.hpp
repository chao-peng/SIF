#ifndef SIF_LIBSIF_ASTVISITOR_H_
#define SIF_LIBSIF_ASTVISITOR_H_

#include "ASTNodes.hpp"

namespace Sif {
    void before();
    void visit(ASTNode* node);
    void after();
}

#endif //SIF_LIBSIF_ASTVISITOR_H_