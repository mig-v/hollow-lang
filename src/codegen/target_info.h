#pragma once

#include "archive/register.h"

#include <unordered_map>
#include <vector>

enum class TargetArch
{
	X86_64
};

struct TargetInfo
{
	TargetArch architecture;
};