#pragma once
#include <climits>
#include <cstdint>

class Isaac
{
	static constexpr uint32_t RATIO = 0x9E3779B9;

	uint32_t state[259];

public:
	constexpr Isaac(const uint32_t(&seed)[256])
		: state{}
	{
		uint32_t tmp[8] = { RATIO, RATIO, RATIO, RATIO, RATIO, RATIO, RATIO, RATIO };

		Mix(tmp);
		Mix(tmp);
		Mix(tmp);
		Mix(tmp);

		for (size_t i = 0; i < 256; i += 8)
		{
			tmp[0] += seed[i];     tmp[1] += seed[i + 1];
			tmp[2] += seed[i + 2]; tmp[3] += seed[i + 3];
			tmp[4] += seed[i + 4]; tmp[5] += seed[i + 5];
			tmp[6] += seed[i + 6]; tmp[7] += seed[i + 7];

			Mix(tmp);

			state[3 + i]     = tmp[0]; state[3 + i + 1] = tmp[1];
			state[3 + i + 2] = tmp[2]; state[3 + i + 3] = tmp[3];
			state[3 + i + 4] = tmp[4]; state[3 + i + 5] = tmp[5];
			state[3 + i + 6] = tmp[6]; state[3 + i + 7] = tmp[7];
		}

		for (size_t i = 0; i < 256; i += 8)
		{
			tmp[0] += state[3 + i];     tmp[1] += state[3 + i + 1];
			tmp[2] += state[3 + i + 2]; tmp[3] += state[3 + i + 3];
			tmp[4] += state[3 + i + 4]; tmp[5] += state[3 + i + 5];
			tmp[6] += state[3 + i + 6]; tmp[7] += state[3 + i + 7];

			Mix(tmp);

			state[3 + i]     = tmp[0]; state[3 + i + 1] = tmp[1];
			state[3 + i + 2] = tmp[2]; state[3 + i + 3] = tmp[3];
			state[3 + i + 4] = tmp[4]; state[3 + i + 5] = tmp[5];
			state[3 + i + 6] = tmp[6]; state[3 + i + 7] = tmp[7];
		}

		Next();
	}

	constexpr uint32_t Next()
	{
		state[2] += 1;
		state[1] += state[2];

		for (size_t i = 0; i < 256; ++i)
		{
			auto x = state[3 + i];

			switch (i % 4)
			{
				case 0:
					state[0] ^= state[0] << 13;
					break;

				case 1:
					state[0] ^= state[0] >> 6;
					break;

				case 2:
					state[0] ^= state[0] << 2;
					break;

				case 3:
					state[0] ^= state[0] >> 16;
					break;
			}

			state[0]     = state[3 + ((i + 128) % 256)] + state[0];
			state[3 + i] = state[3 + ((x >> 2) % 256)] + state[0] + state[1];
			state[1]     = state[3 + ((state[3 + i] >> 10) % 256)] + x;
		}

		return state[1];
	}
	constexpr uint32_t Next(uint32_t min, uint32_t max)
	{
		if (min == max)
			return min;

		auto range = max - min;

		if (range == UINT32_MAX)
			return Next();

		return min + (uint32_t)(((uint64_t)Next() * (uint64_t)(range + 1)) >> 32);
	}

	constexpr uint64_t Next64()
	{
		return ((uint64_t)Next() << 32) | (uint64_t)Next();
	}
	constexpr uint64_t Next64(uint64_t min, uint64_t max)
	{
		if (min == max)
			return min;

		auto range = max - min;

		if (range == UINT64_MAX)
			return Next64();

#if defined(__SIZEOF_INT128__)
		return min + (uint64_t)(((__uint128_t)Next64() * (range + 1)) >> 64);
#else
		auto     a   = Next64();
		auto     b   = range + 1;
		uint64_t c[] = { (uint32_t)a, a >> 32 };
		uint64_t d[] = { (uint32_t)b, b >> 32 };
		uint64_t e[] = { c[0] * d[0], c[0] * d[1], c[1] * d[0], c[1] * d[1] };

		return min + (e[3] + (e[1] >> 32) + (e[2] >> 32) + (((e[0] >> 32) + (uint32_t)e[1] + (uint32_t)e[2]) >> 32));
#endif
	}

private:
	static constexpr void Mix(uint32_t(&value)[8])
	{
		value[0] ^= value[1] << 11;
		value[3] += value[0];
		value[1] += value[2];
		value[1] ^= value[2] >> 2;
		value[4] += value[1];
		value[2] += value[3];
		value[2] ^= value[3] << 8;
		value[5] += value[2];
		value[3] += value[4];
		value[3] ^= value[4] >> 16;
		value[6] += value[3];
		value[4] += value[5];
		value[4] ^= value[5] << 10;
		value[7] += value[4];
		value[5] += value[6];
		value[5] ^= value[6] >> 4;
		value[0] += value[5];
		value[6] += value[7];
		value[6] ^= value[7] << 8;
		value[1] += value[6];
		value[7] += value[0];
		value[7] ^= value[0] >> 9;
		value[2] += value[7];
		value[0] += value[1];
	}
};
