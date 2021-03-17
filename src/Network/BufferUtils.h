#pragma once

#include "../stdafx.h"

typedef unsigned int Remove;

#define Remove_CMD			1
#define Remove_LENGTH		2
#define Remove_IS_SERVER	1
#define Remove_UDP_ID		2

class BufferUtils {
public:
	static std::vector<char> AddLength(std::vector<char> data) {
		UINT16 len = data.size();
		char lo = len & 0xFF;
		char hi = len >> 8;
		std::vector<char> len_v = {lo, hi};
		return Add({ len_v , data });
	}

	static std::vector<char> RemoveFront(Remove numToRemove, std::vector<char> origin)
	{
		std::vector<char> res;
		if ((int)numToRemove > origin.size())
		{
			return res;
			//throw new Exception(string.Format("RemoveFront: received remove length ({0}) longer than buffer: {1}", (int)numToRemove, BitConverter.ToString(origin)));
		}

		res.assign(origin.begin() + numToRemove, origin.end());
		return res;
	}

	static std::vector<char> AddFirst(char byteToAdd, std::vector<char> origin)
	{
		origin.insert(origin.begin(), byteToAdd);
		return origin;
	}

	static std::vector<char> Add(std::vector<std::vector<char>> buffers) {
		std::vector<char> res;
		for (int i = 0; i < buffers.size(); i++)
		{
			res.insert(res.end(), buffers[i].begin(), buffers[i].end());
		}
		return res;
	}
};