//
// Created by fatih on 5/7/17.
//

#pragma once

#if !defined(MALT_STATIC_LIBS) && !defined(MALT_GAME_IMPL)
#define MALT_WEAK_SYMBOL __attribute__((weak))
#else
#define MALT_WEAK_SYMBOL
#endif

#define MALT_PUBLIC __attribute__((visibility("default")))
#define MALT_PRIVATE __attribute__((visibility("hidden")))

