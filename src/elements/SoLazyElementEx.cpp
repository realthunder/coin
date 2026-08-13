/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
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

#include "elements/SoLazyElementEx.h"

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>

#include "coindefs.h"
#include "SbBasicP.h"

SO_ELEMENT_SOURCE(SoLazyElementEx);

static SbBool lazyex_installed = FALSE;

void
SoLazyElementEx::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoLazyElementEx, inherited);
}

SoLazyElementEx::~SoLazyElementEx()
{
}

void
SoLazyElementEx::init(SoState * state)
{
  inherited::init(state);
  this->exstate.ambient.clear();
  this->exstate.emissive.clear();
  this->exstate.specular.clear();
  this->exstate.shininess.clear();
}

void
SoLazyElementEx::push(SoState * state)
{
  inherited::push(state);
  const SoLazyElementEx * prev =
    coin_assert_cast<const SoLazyElementEx *>(this->getNextInStack());
  this->exstate = prev->exstate;
}

SbBool
SoLazyElementEx::isInstalled(void)
{
  return lazyex_installed;
}

void
SoLazyElementEx::setMaterialsEx(SoState * state, SoNode * node,
                                uint32_t bitmask,
                                const SbColor * ambient, int numambient,
                                const SbColor * emissive, int numemissive,
                                const SbColor * specular, int numspecular,
                                const float * shininess, int numshininess)
{
  SoLazyElement * elem = SoLazyElement::getInstance(state);
  if (elem->getTypeId() != SoLazyElementEx::getClassTypeId()) return;
  SoLazyElementEx * ex = static_cast<SoLazyElementEx *>(elem);

  const SbUniqueId nodeid = node->getNodeId();
  uint32_t changed = 0;
  if ((bitmask & AMBIENT_MASK) && ex->exstate.ambient.nodeid != nodeid)
    changed |= AMBIENT_MASK;
  if ((bitmask & EMISSIVE_MASK) && ex->exstate.emissive.nodeid != nodeid)
    changed |= EMISSIVE_MASK;
  if ((bitmask & SPECULAR_MASK) && ex->exstate.specular.nodeid != nodeid)
    changed |= SPECULAR_MASK;
  if ((bitmask & SHININESS_MASK) && ex->exstate.shininess.nodeid != nodeid)
    changed |= SHININESS_MASK;
  if (!changed) return;

  // the write must respect the state depth, exactly as the scalar path
  // does, so a deeper separator's pop discards it
  ex = static_cast<SoLazyElementEx *>(SoLazyElement::getWInstance(state));
  if (changed & AMBIENT_MASK) {
    ex->exstate.ambient.values = reinterpret_cast<const float *>(ambient);
    ex->exstate.ambient.num = numambient;
    ex->exstate.ambient.nodeid = nodeid;
  }
  if (changed & EMISSIVE_MASK) {
    ex->exstate.emissive.values = reinterpret_cast<const float *>(emissive);
    ex->exstate.emissive.num = numemissive;
    ex->exstate.emissive.nodeid = nodeid;
  }
  if (changed & SPECULAR_MASK) {
    ex->exstate.specular.values = reinterpret_cast<const float *>(specular);
    ex->exstate.specular.num = numspecular;
    ex->exstate.specular.nodeid = nodeid;
  }
  if (changed & SHININESS_MASK) {
    ex->exstate.shininess.values = shininess;
    ex->exstate.shininess.num = numshininess;
    ex->exstate.shininess.nodeid = nodeid;
  }
}

void
SoLazyElementEx::setAmbientElt(const SbColor * color)
{
  inherited::setAmbientElt(color);
  this->exstate.ambient.clear();
}

void
SoLazyElementEx::setEmissiveElt(const SbColor * color)
{
  inherited::setEmissiveElt(color);
  this->exstate.emissive.clear();
}

void
SoLazyElementEx::setSpecularElt(const SbColor * color)
{
  inherited::setSpecularElt(color);
  this->exstate.specular.clear();
}

void
SoLazyElementEx::setShininessElt(float value)
{
  inherited::setShininessElt(value);
  this->exstate.shininess.clear();
}

void
SoLazyElementEx::setMaterialElt(SoNode * node, uint32_t bitmask,
                                SoColorPacker * packer,
                                const SbColor * diffuse, const int numdiffuse,
                                const float * transp, const int numtransp,
                                const SbColor & ambient,
                                const SbColor & emissive,
                                const SbColor & specular,
                                const float shininess,
                                const SbBool istransparent)
{
  inherited::setMaterialElt(node, bitmask, packer, diffuse, numdiffuse,
                            transp, numtransp, ambient, emissive, specular,
                            shininess, istransparent);
  if (bitmask & AMBIENT_MASK) this->exstate.ambient.clear();
  if (bitmask & EMISSIVE_MASK) this->exstate.emissive.clear();
  if (bitmask & SPECULAR_MASK) this->exstate.specular.clear();
  if (bitmask & SHININESS_MASK) this->exstate.shininess.clear();
}

// *************************************************************************
// the runtime-bound C surface

static int
lazyex_get_field(void * state, const float ** values, uint64_t * nodeid,
                 const SoLazyElementEx::FieldArray & (SoLazyElementEx::*getter)(void) const)
{
  if (!lazyex_installed || !state) return 0;
  SoState * sostate = static_cast<SoState *>(state);
  const SoElement * elem =
    sostate->getConstElement(SoLazyElement::getClassStackIndex());
  if (elem->getTypeId() != SoLazyElementEx::getClassTypeId()) return 0;
  const SoLazyElementEx::FieldArray & field =
    (static_cast<const SoLazyElementEx *>(elem)->*getter)();
  if (values) *values = field.values;
  if (nodeid) *nodeid = field.nodeid;
  return field.num;
}

extern "C" {

int
coin_lazyex_abi_version(void)
{
  return 1;
}

int
coin_lazyex_install(void)
{
  if (lazyex_installed) return coin_lazyex_abi_version();
  if (SoLazyElement::getClassTypeId() == SoType::badType()) return 0;
  SoLazyElementEx::initClass();
  SoCallbackAction::enableElement(SoLazyElementEx::getClassTypeId(),
                                  SoLazyElementEx::getClassStackIndex());
  lazyex_installed = TRUE;
  return coin_lazyex_abi_version();
}

int
coin_lazyex_get_ambient(void * state, const float ** values, uint64_t * nodeid)
{
  return lazyex_get_field(state, values, nodeid, &SoLazyElementEx::getAmbientArray);
}

int
coin_lazyex_get_emissive(void * state, const float ** values, uint64_t * nodeid)
{
  return lazyex_get_field(state, values, nodeid, &SoLazyElementEx::getEmissiveArray);
}

int
coin_lazyex_get_specular(void * state, const float ** values, uint64_t * nodeid)
{
  return lazyex_get_field(state, values, nodeid, &SoLazyElementEx::getSpecularArray);
}

int
coin_lazyex_get_shininess(void * state, const float ** values, uint64_t * nodeid)
{
  return lazyex_get_field(state, values, nodeid, &SoLazyElementEx::getShininessArray);
}

} // extern "C"
