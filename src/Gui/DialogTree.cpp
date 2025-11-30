#include "../stdafx.hpp"
#include "DialogTree.hpp"

gui::DialogTree::DialogTree(std::shared_ptr<DialogNode> root) : currentNode(root) {}

void gui::DialogTree::choose(const std::string &label)
{
    cout << "Choose: " << label << endl;
    if (currentNode->options.count(label))
    {
        cout << "lalala" << endl;
        currentNode = currentNode->options[label];
        nodeChanged = true;
    }
}
