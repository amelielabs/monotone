#pragma once

//
// monotone.
//
// embeddable cloud-native storage for events
// and time-series data.
//
// Copyright (c) 2023-2025 Dmitry Simonenko
//
// MIT Licensed.
//

typedef uint32_t (*CrcFunction)(uint32_t    crc,
                                const void* data,
                                int         data_size);

uint32_t crc32(uint32_t, const void*, int);
bool     crc32_sse_supported(void);
uint32_t crc32_sse(uint32_t, const void*, int);
