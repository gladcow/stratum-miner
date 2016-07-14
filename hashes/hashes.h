#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
	int has_aes_ni_support();

	// cryptonight
	void cryptonight_hash(void* output, const void* input, size_t len);

	struct cryptonight_ctx;
	struct cryptonight_ctx* cryptonight_alloc_ctx();
	void cryptonight_free_ctx(struct cryptonight_ctx* ctx);
	void cryptonight_hash_ctx(void* output, const void* input, size_t len, struct cryptonight_ctx* ctx);
	void cryptonight_hash_ctx_aes_ni(void* output, const void* input, size_t len, struct cryptonight_ctx* ctx);

	// 

#ifdef __cplusplus
}
#endif
