#pragma once

#include "arena_block.h"

#include <utility>
#include <vector>


class MemoryArena
{
public:
	MemoryArena(const MemoryArena&) = delete;
	MemoryArena& operator=(const MemoryArena&) = delete;

	MemoryArena(size_t blockSize = DEFAULT_ARENA_BLOCK_SIZE)
	{
		this->defaultBlockSize = blockSize;
		blocks.emplace_back(new ArenaBlock(blockSize));
	}

	~MemoryArena()
	{
		for (ArenaBlock* block : blocks)
			delete block;
	}

	template<typename T, typename... Args>
	T* alloc(Args&&... args)
	{
		// try to create a node of type T in the newest ASTArenaBlock. If it succeeds, we're done and we return the node
		if (T* node = blocks.back()->tryAlloc<T>(std::forward<Args>(args)...))
			return node;

		// if it fails, we need to create a new arena block. To be safe, we check to make sure the block will be big enough to hold the node
		size_t newBlockSize = std::max(defaultBlockSize, sizeof(T) + alignof(T));
		blocks.emplace_back(new ArenaBlock(newBlockSize));
		T* node = blocks.back()->tryAlloc<T>(std::forward<Args>(args)...);
		return node;
	}
private:
	std::vector<ArenaBlock*> blocks;
	size_t defaultBlockSize;
};