/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoGLLightIdElement Inventor/elements/SoGLLightIdElement.h
  \brief The SoGLLightIdElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/elements/SoGLLightIdElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/system/gl.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SoGL.h>

int32_t SoGLLightIdElement::maxGLSources = -1;

/*!
  \fn SoGLLightIdElement::maxGLSources

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoGLLightIdElement);

/*!
  This static method initializes static data for the
  SoGLLightIdElement class.
*/

void
SoGLLightIdElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLLightIdElement, inherited);
}

/*!
  The destructor.
*/

SoGLLightIdElement::~SoGLLightIdElement(void)
{
}

//! FIXME: write doc.

void
SoGLLightIdElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoGLLightIdElement::push(SoState * state)
{
  inherited::push(state);
  this->data = ((SoGLLightIdElement*)this->getNextInStack())->data;
}

//! FIXME: write doc.

void
SoGLLightIdElement::pop(SoState * state,
                        const SoElement * prevTopElement)
{
  // capture element since we change the GL state here
  this->capture(state);

  int idx = this->data + 1;
  int prevdata = ((SoGLLightIdElement*)prevTopElement)->data;
  // disable used light sources
  while (idx <= prevdata) {
    glDisable((GLenum)((int32_t)GL_LIGHT0 + idx));
    idx++;
  }
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::increment(SoState * const state,
                              SoNode * const /* node */)
{
  SoGLLightIdElement * element = (SoGLLightIdElement *)
    getElement(state, getClassStackIndex());
  
  if (element) {
    const cc_glglue * glue = sogl_glue_instance(state);
    element->data++;
    int maxl = cc_glglue_get_max_lights(glue);

    // update static variable in case somebody uses the maxGLLights()
    // function
    if (SoGLLightIdElement::maxGLSources != maxl) {
      SoGLLightIdElement::maxGLSources = maxl;
    }

    if (element->data >= maxl) {
      element->data--;
#if COIN_DEBUG
      static SbBool warn = TRUE;

      if (warn) { // warn only once
        warn = FALSE;
        SoDebugError::postWarning("SoGLLightIdElement::increment",
                                  "Number of concurrent light sources in "
                                  "scene exceeds %d, which is the maximum "
                                  "number of concurrent light sources "
                                  "supported by this OpenGL implementation. "
                                  "Some light sources will be ignored.\n\n"
                                  
                                  "(Note to application "
                                  "programmers: this error is often caused by "
                                  "a missing SoState::pop() call in extension "
                                  "shape nodes -- audit your GLRender() "
                                  "method(s)).",

                                  SoGLLightIdElement::getMaxGLSources());
      }
#endif
      return -1;
    }
    glEnable((GLenum)((int32_t)GL_LIGHT0 + element->data));
    
    return element->data;
  }
  return -1;
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::getMaxGLSources(void)
{
  // FIXME: should also make a likewise method available as part of
  // the So*GLWidget classes. 20020802 mortene.

  // FIXME: consider context. pederb, 20001012
  if (SoGLLightIdElement::maxGLSources == -1) {
    // NB: don't try to be clever and move this code to the
    // initClass() method, as it won't work -- there will be no
    // current GL context.
    GLint val;
    glGetIntegerv(GL_MAX_LIGHTS, &val);
    assert(glGetError() == GL_NO_ERROR &&
           "GL error when calling glGetInteger() -- no current GL context?");

    SoGLLightIdElement::maxGLSources = (int32_t)val;
  }

  return SoGLLightIdElement::maxGLSources;
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::increment(SoState * const state)
{
  return increment(state, NULL);
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::get(SoState * const state)
{
  return SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

int32_t
SoGLLightIdElement::getDefault()
{
  return -1;
}
