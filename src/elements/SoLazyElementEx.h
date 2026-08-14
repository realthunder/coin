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
// The element is inert unless installed at runtime through the
// exported C entry point coin_lazyex_install(), which registers the
// type and enables it on SoCallbackAction, where it substitutes for
// SoLazyElement at the same stack index (the SoGLLazyElement idiom).
// The GL render path is deliberately left alone: per-face
// ambient/specular/emissive are glMaterial* state changes there, which
// is why Open Inventor never indexed them, and the consumer of this
// element (an external render-cache builder traversing with an
// SoCallbackAction) reads it through the C accessors declared below
// rather than through this header, so the class layout is not ABI.

#include <Inventor/elements/SoLazyElement.h>

class COIN_DLL_API SoLazyElementEx : public SoLazyElement {
  typedef SoLazyElement inherited;

  SO_ELEMENT_HEADER(SoLazyElementEx);

public:
  static void initClass(void);
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

  static SbBool isInstalled(void);

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

// The C surface consumers bind to at runtime (dlsym/GetProcAddress);
// present means extended, absent means stock Coin. Keep it stable.
extern "C" {

// Feature level of this surface; bump when the surface grows.
COIN_DLL_API int coin_lazyex_abi_version(void);

// Registers the element type and enables it on SoCallbackAction.
// Idempotent. Must be called after SoDB::init(). Returns the abi
// version on success, 0 on failure.
COIN_DLL_API int coin_lazyex_install(void);

// Each returns the number of entries of the field's array form in the
// given traversal state (0 when not installed, the element is not the
// extended one, or the field holds only its scalar), and fills in a
// borrowed pointer -- 3 floats per entry for the colors, 1 for
// shininess -- plus the id of the node the array was captured from.
// The pointer is only valid during the traversal that set it.
COIN_DLL_API int coin_lazyex_get_ambient(void * state, const float ** values,
                                         uint64_t * nodeid);
COIN_DLL_API int coin_lazyex_get_emissive(void * state, const float ** values,
                                          uint64_t * nodeid);
COIN_DLL_API int coin_lazyex_get_specular(void * state, const float ** values,
                                          uint64_t * nodeid);
COIN_DLL_API int coin_lazyex_get_shininess(void * state, const float ** values,
                                           uint64_t * nodeid);

} // extern "C"

#endif // !COIN_SOLAZYELEMENTEX_H
