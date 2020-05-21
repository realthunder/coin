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

/*!
  \class SoShadowTransparencyElement SoShadowTransparencyElement.h Inventor/annex/FXViz/elements/SoShadowTransparencyElement.h
  \brief The SoShadowTransparencyElement class is yet to be documented.

  \ingroup elements

  FIXME: write doc.

  \since Coin 2.5
*/

#include <Inventor/annex/FXViz/elements/SoShadowTransparencyElement.h>


#include <cassert>

SO_ELEMENT_SOURCE(SoShadowTransparencyElement);

/*!
  \copydetails SoElement::initClass(void)
*/
void
SoShadowTransparencyElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoShadowTransparencyElement, inherited);
}

/*!
  Destructor.
*/

SoShadowTransparencyElement::~SoShadowTransparencyElement(void)
{
}

//! FIXME: write doc.

void
SoShadowTransparencyElement::set(SoState * const state,
                          SoNode * const node,
                          const float value)
{
  inherited::set(classStackIndex, state, node, value);
}

/*!
  Initializes the element to its default value.
*/

void
SoShadowTransparencyElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoShadowTransparencyElement::set(SoState * const state, const float value)
{
  SoShadowTransparencyElement::set(state, NULL, value);
}

//! FIXME: write doc.

float
SoShadowTransparencyElement::get(SoState * const state)
{
  return inherited::get(classStackIndex, state);
}

//! FIXME: write doc.

float
SoShadowTransparencyElement::getDefault(void)
{
  return 0.2;
}


#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  BOOST_CHECK_MESSAGE(SoShadowTransparencyElement::getClassStackIndex() != -1,
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
