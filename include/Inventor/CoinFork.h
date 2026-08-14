#ifndef COIN_COINFORK_H
#define COIN_COINFORK_H

/**************************************************************************\
 * Copyright (c) Zheng, Lei (realthunder) <realthunder.dev@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <Inventor/C/basic.h>

/*
  ABI and feature identification for this Coin fork.

  The fork's binary is deliberately renamed (libCoinRT, see
  COIN_OUTPUT_NAME in the top-level CMakeLists.txt) because its ABI
  diverges from stock Coin: node classes have grown fields, and more
  layout-affecting changes are expected. The rename keeps a stock
  libCoin from ever being loaded in place of the fork; the functions
  below catch the remaining hazard, a STALE build of the fork itself.

  COIN_FORK_ABI_VERSION is a monotonic counter. Bump it on EVERY
  layout-affecting change: a field or virtual added to an installed
  class, a struct resized, an enum value renumbered. Clients compare
  their compile-time COIN_FORK_ABI_VERSION against the runtime
  coin_fork_abi() at startup and refuse to run on mismatch.

  coin_fork_features() lists optional capabilities as space-separated
  tags. Clients gate optional behaviour on a tag being present and
  degrade gracefully when it is not, instead of aborting.
*/

#define COIN_FORK_ABI_VERSION 1

#ifdef __cplusplus
extern "C" {
#endif

/* ABI tag of the loaded library; compare with COIN_FORK_ABI_VERSION. */
COIN_DLL_API int coin_fork_abi(void);

/* Space-separated feature tags of the loaded library. */
COIN_DLL_API const char * coin_fork_features(void);

#ifdef __cplusplus
}
#endif

#endif /* !COIN_COINFORK_H */
