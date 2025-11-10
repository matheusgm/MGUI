#pragma once

#include "stdafx.h"

enum class DialogType {
	OK,
	YesNo,
	ConfirmCancel,
	NextPrevious
};

struct DialogNode {
	std::string message;
	DialogType type;
	std::map<std::string, std::shared_ptr<DialogNode>> options;
};

