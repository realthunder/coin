#ifndef COIN_SOSHADOWTRANSPARENCYELEMENT_H
#define COIN_SOSHADOWTRANSPARENCYELEMENT_H

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

#include <Inventor/elements/SoFloatElement.h>

class COIN_DLL_API SoShadowTransparencyElement : public SoFloatElement {
  typedef SoFloatElement inherited;

  SO_ELEMENT_HEADER(SoShadowTransparencyElement);
public:
  static void initClass(void);
protected:
  virtual ~SoShadowTransparencyElement();

public:
  virtual void init(SoState * state);

  static  void set(SoState * const state, SoNode * const node,
                   const float value);
  static void set(SoState * const state, const float value);
  static float get(SoState * const state);
  static float getDefault(void);
};

#endif // !COIN_SOSHADOWTRANSPARENCYELEMENT_H
