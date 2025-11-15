#pragma once

#include "Model/DialogNode.hpp"

namespace gui
{
	class DialogTree
	{
	public:
		DialogTree(std::shared_ptr<DialogNode> root);

		void choose(const std::string &label);
		std::shared_ptr<DialogNode> current() const { return currentNode; };
		bool nodeHasChanged() const { return nodeChanged; };
		void resetNodeChangedFlag() { nodeChanged = false; };

	private:
		std::shared_ptr<DialogNode> currentNode;
		bool nodeChanged = false;
	};

}
