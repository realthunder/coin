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
  \class SoShadowTransparency SoShadowTransparency.h Inventor/annex/FXViz/nodes/SoShadowTransparency.h
  \brief The SoShadowTransparency class is a node for setting the shadow transparency on nodes.

  \ingroup nodes

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    ShadowTransparency {
        data 0.5
    }
  \endcode

  \since Coin 4.0
*/

// *************************************************************************

#include <Inventor/annex/FXViz/nodes/SoShadowTransparency.h>

#include <cstdio>

#include <Inventor/actions/SoGLRenderAction.h>

#include "nodes/SoSubNodeP.h"
// *************************************************************************

// *************************************************************************


SO_NODE_SOURCE(SoShadowTransparency);

/*!
  Constructor.
*/
SoShadowTransparency::SoShadowTransparency(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShadowTransparency);
  SO_NODE_ADD_FIELD(transparency, (0.2f));
}

/*!
  Destructor.
*/
SoShadowTransparency::~SoShadowTransparency()
{
}

/*!
  \copydetails SoNode::initClass(void)
*/
void
SoShadowTransparency::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShadowTransparency, SO_FROM_COIN_4_0);
  SO_ENABLE(SoGLRenderAction, SoShadowTransparencyElement);
}

// Doc from superclass.
void
SoShadowTransparency::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  SoShadowTransparencyElement::set(state,
                                   this,
                                   this->transparency.getValue());
}



#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoShadowTransparency * node = new SoShadowTransparency;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
