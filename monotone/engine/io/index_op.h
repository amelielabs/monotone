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

void index_open(File*, Source*, Id*, int, Index*);
void index_read(File*, Source*, Index*, Buf*, bool);
