#ifndef COIN_SOLAZYELEMENTEX_H
#define COIN_SOLAZYELEMENTEX_H

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

// SoLazyElement carries diffuse color and transparency as
// pointer-plus-count but ambient, emissive, specular and shininess as
// single values, so under a per-part material binding only the first
// two survive traversal. This derived element additionally carries the
// other four fields as pointer-plus-count, exactly the way the base
// carries diffuse: pointers into the owning node's fields, valid for
// the duration of the traversal, guarded by the node's id.
//
// The element is inert until a client calls install(), which registers
// the type and enables it on SoCallbackAction, where it substitutes for
// SoLazyElement at the same stack index (the SoGLLazyElement idiom).
// The GL render path is deliberately left alone: per-face
// ambient/specular/emissive are glMaterial* state changes there, which
// is why Open Inventor never indexed them, and the consumer of this
// element is an external render-cache builder traversing with an
// SoCallbackAction.

#include <Inventor/elements/SoLazyElement.h>

class COIN_DLL_API SoLazyElementEx : public SoLazyElement {
  typedef SoLazyElement inherited;

  SO_ELEMENT_HEADER(SoLazyElementEx);

public:
  static void initClass(void);

  // Registers the type and enables it on SoCallbackAction, in place of
  // SoLazyElement. Idempotent, and must be called after SoDB::init();
  // returns FALSE if it was not (nothing is installed then).
  static SbBool install(void);
  static SbBool isInstalled(void);

  // The element of the given traversal state, or NULL when that state's
  // lazy element is the stock one -- which is every state until
  // install() has run, and every action other than SoCallbackAction.
  static const SoLazyElementEx * getInstance(const SoState * state);

protected:
  virtual ~SoLazyElementEx();
public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);

  // One extended field: a borrowed array and the id of the node it was
  // borrowed from. num == 0 means the field has only its scalar value
  // in the base element; num == 1 duplicates the scalar (kept so the
  // producer does not need to special-case uniform nodes).
  struct FieldArray {
    const float * values;
    int32_t num;
    SbUniqueId nodeid;

    void clear(void) {
      this->values = NULL;
      this->num = 0;
      this->nodeid = 0;
    }
  };

  // Companion to SoLazyElement::setMaterials(): records the array form
  // of the four fields that setMaterials() collapses to single values.
  // Fields not present in bitmask are left inherited. Called by
  // SoMaterial::doAction() when the element at the lazy stack index is
  // of this type; a no-op otherwise.
  static void setMaterialsEx(SoState * state, SoNode * node,
                             uint32_t bitmask,
                             const SbColor * ambient, int numambient,
                             const SbColor * emissive, int numemissive,
                             const SbColor * specular, int numspecular,
                             const float * shininess, int numshininess);

  const FieldArray & getAmbientArray(void) const { return this->exstate.ambient; }
  const FieldArray & getEmissiveArray(void) const { return this->exstate.emissive; }
  const FieldArray & getSpecularArray(void) const { return this->exstate.specular; }
  const FieldArray & getShininessArray(void) const { return this->exstate.shininess; }

protected:
  // A scalar write from any other producer (SoVRMLMaterial and friends
  // go through these virtuals) makes the corresponding array stale, so
  // it is dropped; SoMaterial::doAction() re-fills right after through
  // setMaterialsEx().
  virtual void setAmbientElt(const SbColor * color);
  virtual void setEmissiveElt(const SbColor * color);
  virtual void setSpecularElt(const SbColor * color);
  virtual void setShininessElt(float value);
  virtual void setMaterialElt(SoNode * node, uint32_t bitmask,
                              SoColorPacker * packer,
                              const SbColor * diffuse, const int numdiffuse,
                              const float * transp, const int numtransp,
                              const SbColor & ambient,
                              const SbColor & emissive,
                              const SbColor & specular,
                              const float shininess,
                              const SbBool istransparent);

private:
  struct {
    FieldArray ambient;
    FieldArray emissive;
    FieldArray specular;
    FieldArray shininess;
  } exstate;
};

#endif // !COIN_SOLAZYELEMENTEX_H
