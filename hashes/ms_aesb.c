#ifdef _MSC_VER
#include <wmmintrin.h>
#include <stdint.h>

int has_aes_ni_support()
{
#ifdef __arm__
	return false;
#else
	uint32_t cpu_info[4];
	__cpuid(cpu_info, 1);
	return cpu_info[2] & (1 << 25);
#endif
}

void ms_fast_aesb_single_round(const uint8_t *in, uint8_t*out,
	const uint8_t *expandedKey)
{
	__m128i t = _mm_loadu_si128(in);
	__m128i av = _mm_loadu_si128(expandedKey);
	t = _mm_aesenc_si128(t, av);
	_mm_storeu_si128(out, t);

}

void ms_fast_aesb_pseudo_round_mut(uint8_t *val, uint8_t *expandedKey)
{
	__m128i t = _mm_loadu_si128(val);
	__m128i* av = (__m128i*)expandedKey;
	for (int i = 0; i < 10; ++i)
		t = _mm_aesenc_si128(t, av[i]);
	_mm_storeu_si128(val, t);
}

#endif