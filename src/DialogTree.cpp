#include "stdafx.h"
#include "DialogTree.h"

gui::DialogTree::DialogTree(std::shared_ptr<DialogNode> root) : currentNode(root) {}

void gui::DialogTree::choose(const std::string& label) {
    if (currentNode->options.count(label)) {
        currentNode = currentNode->options[label];
    }
}

std::shared_ptr<DialogNode> gui::DialogTree::current() const {
    return currentNode;
}
