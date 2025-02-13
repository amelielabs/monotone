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

void engine_write(Engine*, EventArg*, int);
void engine_write_replay(Engine*, LogWrite*);
