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

/*!
  \class SoShadowGroup SoShadowGroup.h FXViz/nodes/SoShadowGroup.h
  \brief The SoShadowGroup node is a group node used for shadow rendering.

  \ingroup coin_fxviz

  Children of this node can receive shadows, and cast shadows on other children.
  Use the SoShadowStyle node to control shadow casters and shadow receivers.

  Please note that all shadow casters will be rendered twice. Once to
  create the shadow map, and once for normal rendering. If you're
  having performance issues, you should consider reducing the number of
  shadow casters.

  The algorithm used to render the shadows is Variance Shadow Maps
  (http://www.punkuser.net/vsm/). As an extra bonus, all geometry
  rendered with shadows can also be rendered with per fragment Phong
  lighting.

  This node will search its subgraph and calculate shadows for all
  SoSpotLight nodes. The node will use one texture unit for each spot
  light, so for this node to work 100%, you need to have
  num-spotlights free texture units while rendering the subgraph.

  Currently, we only support scenes with maximum two texture units
  active while doing shadow rendering (unit 0 and unit 1). This is due
  to the fact that we emulate the OpenGL shading model in a shader
  program, and we're still working on creating a solution that updates
  the shader program during the scene graph traversal. Right now a
  shader program is created when entering the SoShadowGroup node, and
  this is used for the entire subgraph.


  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    SoShadowGroup {
      isActive TRUE
      intensity 0.5
      precision 0.5
      quality 0.5
      shadowCachingEnabled TRUE
      visibilityRadius -1.0
      visibilityFlag LONGEST_BBOX_EDGE_FACTOR

      epsilon 0.00001
      threshold 0.1
      smoothBorder 0.0

    }
  \endcode

  Example scene graph:
  \code
  #Inventor V2.1 ascii

  # to get some lighting when headlight is turned off in the viewer
  DirectionalLight { direction 0 0 -1 intensity 0.2 }

  ShadowGroup {
    quality 1 # to get per pixel lighting

    ShadowStyle { style NO_SHADOWING }

    SpotLight {
      location -8 -8 8.0
      direction 1 1 -1
      cutOffAngle 0.35
      dropOffRate 0.7
    }

    ShadowStyle { style CASTS_SHADOW_AND_SHADOWED }

    Separator {
      Complexity { value 1.0 }
      Material { diffuseColor 1 1 0 specularColor 1 1 1 shininess 0.9 }
      Shuttle { translation0 -3 1 0 translation1 3 -5 0 speed 0.25 on TRUE }
      Translation { translation -5 0 2 }
      Sphere { radius 2.0 }
    }

    Separator {
      Material { diffuseColor 1 0 0 specularColor 1 1 1 shininess 0.9 }
      Shuttle { translation0 0 -5 0 translation1 0 5 0 speed 0.15 on TRUE }
      Translation { translation 0 0 -3 }
      Cube { depth 1.8 }
    }
    Separator {
      Material { diffuseColor 0 1 0 specularColor 1 1 1 shininess 0.9 }
      Shuttle { translation0 -5 0 0 translation1 5 0 0 speed 0.3 on TRUE }
      Translation { translation 0 0 -3 }
      Cube { }
    }

    ShadowStyle { style SHADOWED }
    Coordinate3 { point [ -10 -10 -3, 10 -10 -3, 10 10 -3, -10 10 -3 ] }
    Material { specularColor 1 1 1 shininess 0.9 }

    Complexity { textureQuality 0.1 }
    Texture2 { image 2 2 3 0xffffff 0x225588 0x225588 0xffffff }
    Texture2Transform { scaleFactor 4 4 }
    FaceSet { numVertices 4 }
  }

  \endcode

  \since Coin 2.5
*/


/*!
  \var SoSFBool SoShadowGroup::isActive

  Use this field to turn shadow rendering for the subgraph
  on/off. Default value is TRUE.
*/

/*!
  \var SoSFFloat SoShadowGroup::intensity

  Not used yet. Provided for TGS Inventor compatibility.
*/

/*!
  \var SoSFFloat SoShadowGroup::precision

  Use to calculate the size of the shadow map. A precision of 1.0
  means the maximum shadow buffer size will be used (typically
  2048x2048 on current graphics cards). Default value is 0.5.
*/

/*!
  \var SoSFFloat SoShadowGroup::quality

  Can be used to tune the shader program complexity. A higher value
  will mean that more calculations are done per fragment instead of
  per vertex. Default value is 0.5.

*/

/*!
  \var SoSFBool SoShadowGroup::shadowCachingEnabled

  Not used yet. Provided for TGS Inventor compatibility.
*/

/*!
  \var SoSFFloat SoShadowGroup::visibilityNearRadius

  Can be used to manually set the near clipping plane of the shadow
  maps.  If a negative value is provided, the group will calculate a
  near plane based on the bounding box of the children. Default value
  is -1.0.

  \sa visibilityFlag
*/

/*!
  \var SoSFFloat SoShadowGroup::visibilityRadius

  Can be used to manually set the far clipping plane of the shadow
  maps.  If a negative value is provided, the group will calculate a
  near plane based on the bounding box of the children. Default value
  is -1.0.

  \sa visibilityFlag
*/

/*!
  \var SoSFEnum SoShadowGroup::visibilityFlag

  Determines how visibilityRadius and visibilitNearRadius are used to
  calculate near and far clipping planes for the shadow volume.
*/

/*!
  SoShadowGroup::VisibilityFlag SoShadowGroup::ABSOLUTE_RADIUS

  The absolute values of visibilityNearRadius and visibilityRadius will be used.
*/

/*!
  SoShadowGroup::VisibilityFlag SoShadowGroup::LONGEST_BBOX_EDGE_FACTOR

  The longest bounding box edge will be used to determine near and far clipping planes.

*/

/*!
  SoShadowGroup::VisibilityFlag SoShadowGroup::PROJECTED_BBOX_DEPTH_FACTOR

  The bounding box depth (projected to face the camera) will be used to calculate the clipping planes.

*/

/*!
  \var SoSFInt32 SoShadowGroup::smoothBorder

  We have some problems with this feature so it's not supported at the
  moment.

  Used to add shadow border smoothing. This is currently done as a
  post processing step on the shadow map. The algorithm used is Gauss
  Smoothing, but in the future we'll probably change this, and use a
  summed area sampling method instead. The value should be a
  number between 0 (no smoothing), and 1 (max smoothing).

  If you want to enable smoothing, choosing a low value (~0.1) works
  best in the current implementation.

  Default value is 0.0.
*/

/*!
  \var SoSFFloat SoShadowGroup::epsilon

  Epsilon is used to offset the shadow map depth from the model depth.
  Should be set to as low a number as possible without causing
  flickering in the shadows or on non-shadowed objects. Default value
  is 0.00001.
*/

/*!
  \var SoSFFloat SoShadowGroup::threshold

  Can be used to avoid light bleeding in merged shadows cast from different objects.

  A threshold to completely eliminate all light bleeding can be
  computed from the ratio of overlapping occluder distances from the
  light's perspective. See
  http://forum.beyond3d.com/showthread.php?t=38165 for a discussion
  about this problem.

*/


// use to increase the VSM precision by using all four components
#define DISTRIBUTE_FACTOR 64.0

// use to increase precision by one bit at the cost of some extra processing
#define USE_NEGATIVE 1

// use box filter instead of gaussian blur.
// #define USE_BOX_FILTER 1

// *************************************************************************

#include <FXViz/nodes/SoShadowGroup.h>
#include "coindefs.h"

#include <cmath>

#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSceneTexture2.h>
#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoDepthBufferElement.h>
#include <Inventor/elements/SoLightElement.h>
#include <Inventor/elements/SoMultiTextureMatrixElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoEnvironmentElement.h>
#include <Inventor/elements/SoClipPlaneElement.h>
#include <Inventor/annex/FXViz/elements/SoShadowStyleElement.h>
#include <Inventor/annex/FXViz/elements/SoShadowTransparencyElement.h>
#include <Inventor/annex/FXViz/elements/SoGLShadowCullingElement.h>
#include <Inventor/annex/FXViz/nodes/SoShadowStyle.h>
#include <Inventor/annex/FXViz/nodes/SoShadowTransparency.h>
#include <Inventor/annex/FXViz/nodes/SoShadowCulling.h>
#include <Inventor/annex/FXViz/nodes/SoShadowSpotLight.h>
#include <Inventor/annex/FXViz/nodes/SoShadowDirectionalLight.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/C/glue/gl.h>

#include "nodes/SoSubNodeP.h"
#include "shaders/SoShader.h"
#include "glue/glp.h"
#include "misc/SoShaderGenerator.h"
#include "caches/SoShaderProgramCache.h"
#include "rendering/SoGL.h"

// *************************************************************************

struct GaussianFilter {
  GaussianFilter()
    :texture(0),programH(0),programV(0),smoothing(0.0),packed(false)
  {}

  ~GaussianFilter()
  {
    if (this->texture)
      this->texture->unref();
    if (this->programH)
      this->programH->unref();
    if (this->programV)
      this->programV->unref();
  }

  void update(int type, const SbVec2s &size, float smoothing, bool packed)
  {
    if (smoothing == 0.0) {
      if (this->texture) {
        this->texture->unref();
        this->texture = NULL;
      }
      return;
    }

    if (!this->texture
        || this->texture->size.getValue()[0]<size[0]
        || this->texture->size.getValue()[1]<size[1]
        || this->texture->type.getValue() != type) {
      if (!this->texture) {
        this->texture = new SoSceneTexture2;
        this->texture->ref();
        this->texture->transparencyFunction = SoSceneTexture2::NONE;
        this->texture->wrapS = SoSceneTexture2::CLAMP_TO_BORDER;
        this->texture->wrapT = SoSceneTexture2::CLAMP_TO_BORDER;

        SoTransparencyType * tt = new SoTransparencyType;
        tt->value = SoTransparencyType::NONE;
        this->texture->sceneTransparencyType = tt;
      }
      this->texture->size = size;
      this->texture->type = type;
    }

    if (this->programH && this->smoothing == smoothing && this->packed == packed)
      return;

    if (this->programH) {
      this->programH->unref();
      this->programH = NULL;
      this->programV->unref();
      this->programV = NULL;
    }
    this->smoothing = smoothing;
    this->packed = packed;

    createFilter(true);
    createFilter(false);
  }

  static inline int binomial(int n, int k)
  {
    double res = 1;
    for (int i = 1; i <= k; ++i)
      res = res * (n - k + i) / i;
    return (int)(res + 0.01);
  }

  static void initGaussian(SbList<float> &weights, int size)
  {
    if (weights.getLength() == size+1) return;
    assert(size > 0);
    weights.truncate(0);
    weights.ensureCapacity(size+1);
    int n = 2*size + 4;
    double s = pow(2, n) - (n+1)*2;
    for (int i=0; i<=size; ++i)
      weights.append(float(binomial(n, i+2) / s));
  }

  SoShaderProgram *createFilter(bool horizontal)
  {
    SoShaderProgram *&program = horizontal ? this->programH : this->programV;
    if (!program) {
      program = new SoShaderProgram;
      program->ref();
    }

    SoVertexShader * vshader = new SoVertexShader;
    SoFragmentShader * fshader = new SoFragmentShader;

    SoShaderParameter1i * baseimage = new SoShaderParameter1i;
    baseimage->name = "baseimage";
    baseimage->value = 0;

    int size = int(smoothing);

    SoShaderGenerator fgen;
    SbString str;

    fgen.addDeclaration("uniform sampler2D baseimage;", FALSE);

#ifdef DISTRIBUTE_FACTOR
    str.sprintf("const float DISTRIBUTE_FACTOR = %.1f;\n", DISTRIBUTE_FACTOR);
    fgen.addDeclaration(str, FALSE);
#endif

    fgen.addMainStatement("vec4 m = vec4(0.0);\n");
    fgen.addMainStatement("vec4 map;\n");
    fgen.addMainStatement(str);

    float dt = 1.0f / float(this->texture->size.getValue()[0]);

    const char *fmt;
    if (horizontal)
      fmt = "map = texture2D(baseimage, gl_TexCoord[0].st + vec2(%f,0.0));\n";
    else
      fmt = "map = texture2D(baseimage, gl_TexCoord[0].st + vec2(0.0,%f));\n";

    const char *fmt2;
    if (!this->packed) {
      fmt2 = "m += map * %f;\n";
    }
    else {
      fmt2 =
#ifdef USE_NEGATIVE
        "map = (map + vec4(1.0)) * 0.5;\n"
#endif // USE_NEGATIVE
#ifdef DISTRIBUTE_FACTOR
        "map.xy += map.zw / DISTRIBUTE_FACTOR;\n"
#endif
        "m += map * %f;\n";
    }

#ifdef USE_BOX_FILTER
    float weight = 1.0f/(size*2+1);
#else
    initGaussian(this->weights, size);
    float weight = this->weights[size];
#endif
    str.sprintf(fmt, 0.0f);
    fgen.addMainStatement(str);
    str.sprintf(fmt2, weight);
    fgen.addMainStatement(str);
    for (int s = 0; s < size; s++) {
      int d = size-s;
#ifndef USE_BOX_FILTER
      weight = this->weights[s];
#endif
      float offset = float(d)*dt;
      str.sprintf(fmt, offset);
      fgen.addMainStatement(str);
      str.sprintf(fmt2, weight);
      fgen.addMainStatement(str);
      str.sprintf(fmt, -offset);
      fgen.addMainStatement(str);
      str.sprintf(fmt2, weight);
      fgen.addMainStatement(str);
    }

    if (!this->packed) {
      fgen.addMainStatement("gl_FragColor = m;\n");
    }
    else {
      fgen.addMainStatement(
#ifdef DISTRIBUTE_FACTOR
          "vec2 f = fract(m.rg * DISTRIBUTE_FACTOR);\n"
#   ifdef USE_NEGATIVE
          "gl_FragColor.rg = (m.rg - (f / DISTRIBUTE_FACTOR)) * 2.0 - vec2(1.0, 1.0);\n"
          "gl_FragColor.ba = f * 2.0 - vec2(1.0, 1.0);\n"
#   else // USE_NEGATIVE
          "gl_FragColor.rg = m.rg - (f / DISTRIBUTE_FACTOR);\n"
          "gl_FragColor.ba = f;\n"
#   endif // ! USE_NEGATIVE
#else  // DISTRIBUTE_FACTOR
#   ifdef USE_NEGATIVE
          "gl_FragColor = vec4(m.r*2.0 - 1.0, m.g*2.0 - 1.0, 0.0, 0.0);"
#   else // USE_NEGATIVE
          "gl_FragColor = vec4(m.r, m.g, 0.0, 0.0);"
#   endif // !USE_NEGATIVE
#endif // !DISTRIBUTE_FACTOR
          );
    }

    program->shaderObject = vshader;
    program->shaderObject.set1Value(1, fshader);

    fshader->sourceProgram = fgen.getShaderProgram();
    fshader->sourceType = SoShaderObject::GLSL_PROGRAM;

    fshader->parameter = baseimage;

    SoShaderGenerator vgen;
    vgen.addMainStatement("gl_TexCoord[0] = gl_Vertex;\n");
    vgen.addMainStatement("gl_Position = ftransform();");

    vshader->sourceProgram = vgen.getShaderProgram();
    vshader->sourceType = SoShaderObject::GLSL_PROGRAM;

    return program;
  }

  void apply(SoGLRenderAction *action, SoSceneTexture2 *input, SoNode *sceneH, SoNode *sceneV)
  {
    SoNode *scene;
    if (!this->texture) return;
    this->texture->backgroundColor = SbVec4f(1.0f, 1.0f, 1.0f, 1.0f);
    this->texture->scene = sceneH;

    // Two pass gauss filter

    // First pass, output to internal texture
    this->texture->GLRender(action);

    // Second pass, use internal texture as input and original input as output
    input->enableNotify(FALSE);
    SoNode *inputscene = input->scene.getValue();
    inputscene->ref();
    input->scene = sceneV;
    input->GLRender(action);

    // Now disable notifying, and restore input's original scene
    input->scene.enableNotify(FALSE);
    input->scene = inputscene;
    inputscene->unref();
    input->scene.enableNotify(TRUE);
    input->enableNotify(TRUE);
  }

  SoSceneTexture2 *texture;
  SoShaderProgram *programH;
  SoShaderProgram *programV;
  SbList <float> weights;
  float smoothing;
  bool packed;
};

class SoShadowLightCache {
public:
  SoShadowLightCache(SoState * state,
                     const SoPath * path,
                     SoShadowGroup * sg,
                     SoNode * scene,
                     SoNode * bboxscene,
                     GaussianFilter *filter,
                     GaussianFilter *alphafilter)
  {
    const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));

    GLint maxsize = 2048;
    GLint maxtexsize = 2048;

    // Testing for maximum proxy texture size doesn't seem to work, so
    // we just have to hardcode the maximum size to 2048 for now.  We
    // still use the proxy texture test in case the maximum size is
    // something smaller than 2048 though.  pederb, 2007-05-03

    // glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &maxsize);
    // glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxtexsize);
    // if (maxtexsize < maxsize) maxsize = maxtexsize;

    GLenum internalformat = GL_RGBA16F_ARB;
    GLenum format = GL_RGBA;
    GLenum type = GL_FLOAT;

    while (!coin_glglue_is_texture_size_legal(glue, maxsize, maxsize, 0, internalformat, format, type, TRUE) && (maxsize != 0)) {
      maxsize >>= 1;
    }
    if (maxsize == 0) { // Can happen on CentOS 7 in VirtualBox
      glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxsize);
      glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxtexsize);
      if (maxtexsize < maxsize) maxsize = maxtexsize;
    }
    const int TEXSIZE = coin_geq_power_of_two((int) (sg->precision.getValue() * SbMin(maxsize, maxtexsize)));

    this->master = sg;
    this->alphamap = NULL;
    this->alphadepthmap = NULL;

    this->lightid = -1;
    this->vsm_farval = NULL;
    this->vsm_nearval = NULL;
    this->gaussfilter = filter;
    this->gaussalphafilter = alphafilter;
    this->gaussdepthsceneH = NULL;
    this->gaussdepthsceneV = NULL;
    this->gaussalphasceneH = NULL;
    this->gaussalphasceneV = NULL;
    this->depthnodeid = 0;
    this->alphanodeid = 0;
    this->texunit = -1;
    this->depthtest = NULL;

    this->bboxnode = new SoSeparator;
    this->bboxnode->ref();

    this->shadowmapid = new SoShaderParameter1i;
    this->shadowmapid->ref();

    this->alphamapid = new SoShaderParameter1i;
    this->alphamapid->ref();

    this->alphadepthmapid = new SoShaderParameter1i;
    this->alphadepthmapid->ref();

    this->fragment_farval = new SoShaderParameter1f;
    this->fragment_farval->ref();

    this->fragment_nearval = new SoShaderParameter1f;
    this->fragment_nearval->ref();

    this->fragment_lightplane = new SoShaderParameter4f;
    this->fragment_lightplane->ref();

    this->maxshadowdistance = new SoShaderParameter1f;
    this->maxshadowdistance->ref();

    this->dropoffrate = new SoShaderParameter1f;
    this->dropoffrate->ref();

    this->coscutoff = new SoShaderParameter1f;
    this->coscutoff->ref();

    this->path = path->copy();
    this->path->ref();
    assert(((SoFullPath*)path)->getTail()->isOfType(SoLight::getClassTypeId()));

    this->light = (SoLight*)((SoFullPath*)path)->getTail();
    this->light->ref();

    this->hastransp = FALSE;

    this->depthmap = new SoSceneTexture2;
    this->depthmap->ref();
    this->depthmap->transparencyFunction = SoSceneTexture2::NONE;
    this->depthmap->size = SbVec2s(TEXSIZE, TEXSIZE);
    this->depthmap->wrapS = SoSceneTexture2::CLAMP_TO_BORDER;
    this->depthmap->wrapT = SoSceneTexture2::CLAMP_TO_BORDER;

    this->depthmap->type = SoSceneTexture2::RGBA32F;
    this->depthmap->backgroundColor = SbVec4f(1.0f, 1.0f, 1.0f, 1.0f);

    SoTransparencyType * tt = new SoTransparencyType;
    tt->value = SoTransparencyType::NONE;

    this->depthmap->sceneTransparencyType = tt;

    if (this->light->isOfType(SoDirectionalLight::getClassTypeId())) {
      this->camera = new SoOrthographicCamera;
    }
    else {
      this->camera = new SoPerspectiveCamera;
    }
    this->camera->ref();
    this->camera->viewportMapping = SoCamera::LEAVE_ALONE;

    SoSeparator * sep = new SoSeparator;
    sep->addChild(this->camera);

    SoCallback * cb = new SoCallback;
    cb->setCallback(shadowmap_glcallback, this);

    sep->addChild(cb);

    sep->addChild(this->createVSMProgram());

    if (scene == sg)
      this->scene = NULL;
    else {
      this->scene = scene;
      this->scene->ref();
    }

    if (scene == sg) {
      SoShadowGroup * g = (SoShadowGroup*) scene;
      for (int i = 0; i < g->getNumChildren(); i++) {
        sep->addChild(g->getChild(i));
      }
    }
    else sep->addChild(scene);

    if (bboxscene->isOfType(SoShadowGroup::getClassTypeId())) {
      SoShadowGroup * g = (SoShadowGroup*) bboxscene;
      for (int i = 0; i < g->getNumChildren(); i++) {
        this->bboxnode->addChild(g->getChild(i));
      }
    }
    else {
      this->bboxnode->addChild(bboxscene);
    }

    cb = new SoCallback;
    cb->setCallback(shadowmap_post_glcallback, this);
    sep->addChild(cb);

    this->depthmap->scene = sep;
    this->depthmapscene = sep;
    this->depthmapscene->ref();
    this->matrix = SbMatrix::identity();
  }

  ~SoShadowLightCache() {
    if (this->scene) this->scene->unref();
    if (this->depthmapscene) this->depthmapscene->unref();
    if (this->gaussdepthsceneH) this->gaussdepthsceneH->unref();
    if (this->gaussdepthsceneV) this->gaussdepthsceneV->unref();
    if (this->gaussalphasceneH) this->gaussalphasceneH->unref();
    if (this->gaussalphasceneV) this->gaussalphasceneV->unref();
    if (this->depthtest) this->depthtest->unref();
    if (this->bboxnode) this->bboxnode->unref();
    if (this->maxshadowdistance) this->maxshadowdistance->unref();
    if (this->dropoffrate) this->dropoffrate->unref();
    if (this->coscutoff) this->coscutoff->unref();
    if (this->vsm_farval) this->vsm_farval->unref();
    if (this->vsm_nearval) this->vsm_nearval->unref();
    if (this->fragment_farval) this->fragment_farval->unref();
    if (this->shadowmapid) this->shadowmapid->unref();
    if (this->alphamapid) this->alphamapid->unref();
    if (this->alphadepthmapid) this->alphadepthmapid->unref();
    if (this->fragment_nearval) this->fragment_nearval->unref();
    if (this->fragment_lightplane) this->fragment_lightplane->unref();
    if (this->light) this->light->unref();
    if (this->path) this->path->unref();
    if (this->depthmap) this->depthmap->unref();
    if (this->camera) this->camera->unref();
    if (this->alphamap) this->alphamap->unref();
    if (this->alphadepthmap) this->alphadepthmap->unref();
  }

  void createAlphaMap()
  {
    assert(this->alphamap == NULL);
    this->alphamap = new SoSceneTexture2;
    this->alphamap->ref();
    this->alphamap->transparencyFunction = SoSceneTexture2::NONE;
    this->alphamap->size = this->depthmap->size;
    this->alphamap->wrapS = SoSceneTexture2::CLAMP_TO_BORDER;
    this->alphamap->wrapT = SoSceneTexture2::CLAMP_TO_BORDER;

    this->alphamap->type = SoSceneTexture2::RGBA8;
    this->alphamap->backgroundColor = SbVec4f(1.0f, 1.0f, 1.0f, 0.0f);

    SoTransparencyType * tt = new SoTransparencyType;
    tt->value = SoTransparencyType::BLEND;

    this->alphamap->sceneTransparencyType = tt;

    SoSeparator * sep = new SoSeparator;
    sep->addChild(this->camera);

    SoCallback * cb = new SoCallback;
    cb->setCallback(alphamap_glcallback, this);

    sep->addChild(cb);
    // sep->addChild(this->createAlphaProgram());

    if (this->scene)
      sep->addChild(this->scene);
    else {
      for (int i = 0; i < this->master->getNumChildren(); i++) {
        sep->addChild(this->master->getChild(i));
      }
    }

    this->alphamap->scene = sep;

    updateGaussMap(this->gaussalphafilter->smoothing);

    createAlphaDepthMap();
  }

  void createAlphaDepthMap()
  {
    assert(this->alphadepthmap == NULL);
    this->alphadepthmap = new SoSceneTexture2;
    this->alphadepthmap->ref();
    this->alphadepthmap->transparencyFunction = SoSceneTexture2::NONE;
    this->alphadepthmap->size = this->depthmap->size;
    this->alphadepthmap->wrapS = SoSceneTexture2::CLAMP_TO_BORDER;
    this->alphadepthmap->wrapT = SoSceneTexture2::CLAMP_TO_BORDER;

    this->alphadepthmap->type = SoSceneTexture2::DEPTH;
    this->alphadepthmap->setDepthFunc(GL_LESS);

    SoTransparencyType * tt = new SoTransparencyType;
    tt->value = SoTransparencyType::NONE;

    this->alphadepthmap->sceneTransparencyType = tt;

    SoSeparator * sep = new SoSeparator;
    sep->addChild(this->camera);

    SoCallback * cb = new SoCallback;
    cb->setCallback(alphadepthmap_glcallback, this);

    sep->addChild(cb);

    if (this->scene)
      sep->addChild(this->scene);
    else {
      for (int i = 0; i < this->master->getNumChildren(); i++) {
        sep->addChild(this->master->getChild(i));
      }
    }

    this->alphadepthmap->scene = sep;
  }

  void updateGaussMap(float smoothing)
  {
    if (this->depthmap) {
      this->gaussfilter->update(this->depthmap->type.getValue(),
          this->depthmap->size.getValue(), smoothing, false);

      if (!this->gaussfilter->texture) {
        if (this->gaussdepthsceneH) {
          this->gaussdepthsceneH->unref();
          this->gaussdepthsceneH = NULL;
        }
        if (this->gaussdepthsceneV) {
          this->gaussdepthsceneV->unref();
          this->gaussdepthsceneV = NULL;
        }
      }
      else if (!this->gaussdepthsceneH) {
        this->gaussdepthsceneH = createGaussScene(
            this->gaussfilter->programH, this->depthmap);
        this->gaussdepthsceneH->ref();
        this->gaussdepthsceneV = createGaussScene(
            this->gaussfilter->programV, this->gaussfilter->texture);
        this->gaussdepthsceneV->ref();
      }
    }

    if (this->alphamap) {
      this->gaussalphafilter->update(this->alphamap->type.getValue(),
          this->alphamap->size.getValue(), smoothing, false);

      if (!this->gaussalphafilter->texture) {
        if (this->gaussalphasceneH) {
          this->gaussalphasceneH->unref();
          this->gaussalphasceneH = NULL;
        }
        if (this->gaussalphasceneV) {
          this->gaussalphasceneV->unref();
          this->gaussalphasceneV = NULL;
        }
      }
      else if (!this->gaussalphasceneH) {
        this->gaussalphasceneH = createGaussScene(
            this->gaussalphafilter->programH, this->alphamap);
        this->gaussalphasceneH->ref();
        this->gaussalphasceneV = createGaussScene(
            this->gaussalphafilter->programV, this->gaussalphafilter->texture);
        this->gaussalphasceneV->ref();
      }
    }
  }

  static int
  write_short(FILE * fp, unsigned short val)
  {
    unsigned char tmp[2];
    tmp[0] = (unsigned char)(val >> 8);
    tmp[1] = (unsigned char)(val & 0xff);
    return (int)fwrite(&tmp, 2, 1, fp);
  }

  int dumpBitmap(const char * filename) const {
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    int width = vp[2];
    int height = vp[3];
    int comp = 1;

    unsigned char * bytes = new unsigned char[width*height*comp];
    glFlush();
    glReadPixels(0,0, width, height, GL_RED, GL_UNSIGNED_BYTE, bytes);

    FILE * fp = fopen(filename, "wb");
    if (!fp) {
      return 0;
    }

    write_short(fp, 0x01da); /* imagic */
    write_short(fp, 0x0001); /* raw (no rle yet) */

    if (comp == 1)
      write_short(fp, 0x0002); /* 2 dimensions (heightmap) */
    else
      write_short(fp, 0x0003); /* 3 dimensions */

    write_short(fp, (unsigned short) width);
    write_short(fp, (unsigned short) height);
    write_short(fp, (unsigned short) comp);

    unsigned char buf[500];
    memset(buf, 0, 500);
    buf[7] = 255; /* set maximum pixel value to 255 */
    strcpy((char *)buf+8, "https://github.com/coin3d/");
    fwrite(buf, 1, 500, fp);

    unsigned char * tmpbuf = new unsigned char[width];

    int x, y, c;
    for (c = 0; c < comp; c++) {
      for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
          tmpbuf[x] = bytes[x * comp + y * comp * width + c];
        }
        fwrite(tmpbuf, 1, width, fp);
      }
    }
    delete [] tmpbuf;
    fclose(fp);
    delete [] bytes;
    return 1;
  }	
  SbBox3f toCameraSpace(const SbXfBox3f & worldbox) const;
  static void shadowmap_glcallback(void * closure, SoAction * action);
  static void shadowmap_post_glcallback(void * closure, SoAction * action);
  static void alphamap_glcallback(void * closure, SoAction * action);
  static void alphadepthmap_glcallback(void * closure, SoAction * action);
  SoShaderProgram * createVSMProgram();
  SoShaderProgram * createAlphaProgram();
  SoShaderProgram * createGaussFilter(float smoothing, bool horizontal, bool transp);
  SoSeparator * createGaussScene(SoShaderProgram *program, SoSceneTexture2 * tex);

  SoShadowGroup *master;
  SbMatrix matrix;
  SoPath * path;
  SoLight * light;
  SoNode *scene;
  SoSceneTexture2 * depthmap;
  SoSceneTexture2 * alphamap;
  SoSceneTexture2 * alphadepthmap;
  SbBool hastransp;
  SoNode * depthmapscene;
  SoNode * gaussdepthsceneH;
  SoNode * gaussdepthsceneV;
  SoNode * gaussalphasceneH;
  SoNode * gaussalphasceneV;
  GaussianFilter *gaussfilter;
  GaussianFilter *gaussalphafilter;
  SoDepthBuffer *depthtest;
  SoCamera * camera;
  float farval;
  float nearval;
  int texunit;
  int lightid;
  SbUniqueId depthnodeid;
  SbUniqueId alphanodeid;
  float smoothing;

  SoSeparator * bboxnode;
  SoShaderProgram * vsm_program;
  SoShaderParameter1i * shadowmapid;
  SoShaderParameter1i * alphamapid;
  SoShaderParameter1i * alphadepthmapid;
  SoShaderParameter1f * vsm_farval;
  SoShaderParameter1f * vsm_nearval;
  SoShaderParameter1f * fragment_farval;
  SoShaderParameter1f * fragment_nearval;
  SoShaderParameter4f * fragment_lightplane;
  SoShaderGenerator vsm_vertex_generator;
  SoShaderGenerator vsm_fragment_generator;
  SoShaderParameter1f * maxshadowdistance;
  SoShaderParameter1f * dropoffrate;
  SoShaderParameter1f * coscutoff;
};

class SoShadowGroupP {
public:
  SoShadowGroupP(SoShadowGroup * master) :
    master(master),
    bboxaction(SbViewportRegion(SbVec2s(100,100))),
    matrixaction(SbViewportRegion(SbVec2s(100,100))),
    shadowlightsvalid(FALSE),
    needscenesearch(TRUE),
    shaderprogram(NULL),
    vertexshader(NULL),
    fragmentshader(NULL),
    vertexshadercache(NULL),
    fragmentshadercache(NULL),
    texunit0(NULL),
    texunit1(NULL),
    lightmodel(NULL),
    twosided(NULL),
    shadowalpha(NULL),
    numtexunitsinscene(1),
    hasclipplanes(FALSE),
    subgraphsearchenabled(TRUE)
  {
    this->shaderprogram = new SoShaderProgram;
    this->shaderprogram->ref();
    this->shaderprogram->setEnableCallback(shader_enable_cb, this);
    this->vertexshader = new SoVertexShader;
    this->vertexshader->ref();
    this->fragmentshader = new SoFragmentShader;
    this->fragmentshader->ref();

    this->cameratransform = new SoShaderParameterMatrix;
    this->cameratransform->name = "cameraTransform";
    this->cameratransform->ref();

    this->shaderprogram->shaderObject.set1Value(0, this->vertexshader);
    this->shaderprogram->shaderObject.set1Value(1, this->fragmentshader);

    this->twosided = new SoShaderParameter1i;
    this->twosided->ref();
    this->twosided->name = "coin_two_sided_lighting";
    this->twosided->value = 0;
  }
  ~SoShadowGroupP() {
    this->clearLightPaths();
    if (this->lightmodel) this->lightmodel->unref();
    if (this->twosided) this->twosided->unref();
    if (this->texunit0) this->texunit0->unref();
    if (this->texunit1) this->texunit1->unref();
    if (this->shadowalpha) this->shadowalpha->unref();
    if (this->vertexshadercache) this->vertexshadercache->unref();
    if (this->fragmentshadercache) this->fragmentshadercache->unref();
    if (this->cameratransform) this->cameratransform->unref();
    if (this->vertexshader) this->vertexshader->unref();
    if (this->fragmentshader) this->fragmentshader->unref();
    if (this->shaderprogram) this->shaderprogram->unref();
    this->deleteShadowLights();
  }

  SbBool transparentShadow() const
  {
    return this->master->isActive.getValue() & SoShadowGroup::TRANSPARENT_SHADOW;
  }
  void clearLightPaths(void) {
    for (int i = 0; i < this->lightpaths.getLength(); i++) {
      this->lightpaths[i]->unref();
    }
    this->lightpaths.truncate(0);
  }
  void copyLightPaths(const SoPathList & pl) {
    for (int i = 0; i < pl.getLength(); i++) {
      SoFullPath * p = (SoFullPath*) pl[i];
      SoNode * tail = p->getTail();
      if (tail->isOfType(SoSpotLight::getClassTypeId()) ||
          tail->isOfType(SoDirectionalLight::getClassTypeId())) {
        SoTempPath * tp = new SoTempPath(p->getLength());
        tp->ref();
        tp->setHead(p->getHead());

        for (int j = 1; j < p->getLength(); j++) {
          tp->append(p->getNode(j));
        }
        this->lightpaths.append(tp);
      }
    }
  }
  void getQuality(SoState * COIN_UNUSED_ARG(state), SbBool & perpixelspot, SbBool & perpixelother) {
    float quality = this->master->quality.getValue();
    perpixelspot = FALSE;
    perpixelother = FALSE;

    if (quality > 0.3) {
      perpixelspot = TRUE;
    }
    if (quality > 0.7) {
      perpixelother = TRUE;
    }
  }
  void deleteShadowLights(void) {
    for (int i = 0; i < this->shadowlights.getLength(); i++) {
      delete this->shadowlights[i];
    }
    this->shadowlights.truncate(0);
  }

  static bool supported(const cc_glglue * glctx, SbString& reason);

  static void shader_enable_cb(void * closure,
                               SoState * state,
                               const SbBool enable);

  void GLRender(SoGLRenderAction * action, const SbBool inpath);
  void setVertexShader(SoState * state);
  void setFragmentShader(SoState * state);
  void updateSpotCamera(SoState * state, SoShadowLightCache * cache, const SbMatrix & transform);
  void updateDirectionalCamera(SoState * state, SoShadowLightCache * cache, const SbMatrix & transform, SbBool fullupdate);
  const SbXfBox3f & calcBBox(SoShadowLightCache * cache);

  void renderDepthMap(SoShadowLightCache * cache,
                      SoGLRenderAction * action);
  void updateShadowLights(SoGLRenderAction * action);

  int32_t getFog(SoState * state) {
    return SoEnvironmentElement::getFogType(state);
  }

  SoShadowGroup * master;
  SoSearchAction searchaction;
  SbList <SoTempPath*> lightpaths;
  SoGetBoundingBoxAction bboxaction;
  SoGetMatrixAction matrixaction;

  SbBool shadowlightsvalid;
  SbBool needscenesearch;
  SbList <SoShadowLightCache*> shadowlights;

  SoShaderProgram * shaderprogram;
  SoVertexShader * vertexshader;
  SoFragmentShader * fragmentshader;

  SoShaderGenerator vertexgenerator;
  SoShaderGenerator fragmentgenerator;
  SoShaderParameterMatrix * cameratransform;

  SoShaderProgramCache * vertexshadercache;
  SoShaderProgramCache * fragmentshadercache;
  SoShaderParameter1i * texunit0;
  SoShaderParameter1i * texunit1;
  SoShaderParameter1i * lightmodel;
  SoShaderParameter1i * twosided;
  SoShaderParameter1f * shadowalpha;

  int numtexunitsinscene;
  SbBool hasclipplanes;
  SbBool subgraphsearchenabled;

  GaussianFilter gaussfilter;
  GaussianFilter gaussalphafilter;
};

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->master)

SO_NODE_SOURCE(SoShadowGroup);

/*!
  Default constructor.
*/
SoShadowGroup::SoShadowGroup(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShadowGroup);

  PRIVATE(this) = new SoShadowGroupP(this);

  SO_NODE_ADD_FIELD(isActive, (TRUE));
  SO_NODE_ADD_FIELD(intensity, (0.5f));
  SO_NODE_ADD_FIELD(precision, (0.5f));
  SO_NODE_ADD_FIELD(quality, (0.5f));
  SO_NODE_ADD_FIELD(shadowCachingEnabled, (TRUE));
  SO_NODE_ADD_FIELD(visibilityNearRadius, (-1.0f));
  SO_NODE_ADD_FIELD(visibilityRadius, (-1.0f));
  SO_NODE_ADD_FIELD(epsilon, (0.00001f));
  SO_NODE_ADD_FIELD(threshold, (0.1f));
  SO_NODE_ADD_FIELD(smoothBorder, (0.0f));

  SO_NODE_ADD_FIELD(visibilityFlag, (LONGEST_BBOX_EDGE_FACTOR));

  SO_NODE_DEFINE_ENUM_VALUE(VisibilityFlag, LONGEST_BBOX_EDGE_FACTOR);
  SO_NODE_DEFINE_ENUM_VALUE(VisibilityFlag, ABSOLUTE_RADIUS);
  SO_NODE_DEFINE_ENUM_VALUE(VisibilityFlag, PROJECTED_BBOX_DEPTH_FACTOR);
  SO_NODE_SET_SF_ENUM_TYPE(visibilityFlag, VisibilityFlag);

}

/*!
  Destructor.
*/
SoShadowGroup::~SoShadowGroup()
{
  delete PRIVATE(this);
}

/*!
  \copydetails SoNode::initClass(void)
*/
void
SoShadowGroup::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShadowGroup, SO_FROM_COIN_2_5);
}

void
SoShadowGroup::init(void)
{
  SoShadowGroup::initClass();
  SoShadowStyleElement::initClass();
  SoShadowTransparencyElement::initClass();
  SoGLShadowCullingElement::initClass();
  SoShadowStyle::initClass();
  SoShadowTransparency::initClass();
  SoShadowSpotLight::initClass();
  SoShadowDirectionalLight::initClass();
  SoShadowCulling::initClass();
}

// *************************************************************************

/*!
  Reports whether or not the shadow nodes can be used successfully on
  the current system.

  The result will depend on the specific qualities of the graphics
  card and OpenGL driver on the system.

  An important note about this function:

  The API design of this function has a serious shortcoming, as
  features of OpenGL should be tested within an OpenGL context, and
  this function does not provide any means of specifying the
  context. It is implemented in this manner to match the function
  signature in TGS Inventor, for compatibility reasons.

  (A temporary offscreen OpenGL context is set up for the feature
  tests. This should usually be sufficient to decide whether or not
  the graphics driver / card supports the features needed for
  rendering shadows.)

  \since Coin 3.1
*/
SbBool
SoShadowGroup::isSupported(void)
{
  static int supp = -1;
  if (supp != -1) { return supp ? true : false; }

  void * glctx = cc_glglue_context_create_offscreen(256, 256);
  SbBool ok = cc_glglue_context_make_current(glctx);
  if (!ok) {
    SoDebugError::postWarning("SoShadowGroupP::isSupported",
                              "Could not open an OpenGL context.");
    return false;
  }

  const cc_glglue * glue = cc_glglue_instance_from_context_ptr(glctx);

  SbString unused;
  const bool supported = SoShadowGroupP::supported(glue, unused);
  supp = supported ? 1 : 0;

  cc_glglue_context_reinstate_previous(glctx);
  cc_glglue_context_destruct(glctx);

  return supported;
}

// *************************************************************************

void
SoShadowGroup::GLRenderBelowPath(SoGLRenderAction * action)
{
  PRIVATE(this)->GLRender(action, FALSE);
}

void
SoShadowGroup::GLRenderInPath(SoGLRenderAction * action)
{
  PRIVATE(this)->GLRender(action, TRUE);
}

void
SoShadowGroup::notify(SoNotList * nl)
{
  // FIXME: examine notification chain, and detect when an
  // SoSpotLight/SoShadowDirectionalLight is changed. When this
  // happens we can just invalidate the depth map for that spot light,
  // and not the others.

  SoNotRec * rec = nl->getLastRec();
  if (rec->getBase() != this) {
    // was not notified through a field, subgraph was changed

    rec = nl->getFirstRecAtNode();
    if (rec) {
      SoNode * node = (SoNode*) rec->getBase();
      if (node->isOfType(SoGroup::getClassTypeId())) {
        // first rec was from a group node, we need to search the scene graph again
        PRIVATE(this)->shadowlightsvalid = FALSE;

        if (PRIVATE(this)->subgraphsearchenabled) {
          PRIVATE(this)->needscenesearch = TRUE;
        }
      }
      else {
        PRIVATE(this)->shadowlightsvalid = FALSE;
      }
    }
  }
  else if (nl->getLastField() == &this->precision
          || nl->getLastField() == &this->isActive)
    PRIVATE(this)->deleteShadowLights();

  if (PRIVATE(this)->vertexshadercache) {
    PRIVATE(this)->vertexshadercache->invalidate();
  }
  if (PRIVATE(this)->fragmentshadercache) {
    PRIVATE(this)->fragmentshadercache->invalidate();
  }
  inherited::notify(nl);
}

/*!

  By default, the SoShadowGroup node will search its subgraph for new
  spot lights whenever a group node under it is touched. However, this
  might lead to bad performance in some cases so it is possible to
  disable this feature using this method. If you do disable this
  feature, make sure you enable it again before inserting a new spot
  light, or insert all spot lights in the scene graph before you
  render the scene once, and just set "on" to FALSE if you want to toggle
  spot lights on/off on the fly.

  \since Coin 2.6
 */
void
SoShadowGroup::enableSubgraphSearchOnNotify(const SbBool onoff)
{
  PRIVATE(this)->subgraphsearchenabled = onoff;
}

#undef PRIVATE

// *************************************************************************

void
SoShadowGroupP::updateShadowLights(SoGLRenderAction * action)
{
  int i;
  SoState * state = action->getState();

  float smoothing = PUBLIC(this)->smoothBorder.getValue();

  if (!this->shadowlightsvalid) {
    int lightidoffset = SoLightElement::getLights(state).getLength();

    const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));

    if (this->needscenesearch) {
      this->hasclipplanes = SoClipPlaneElement::getInstance(state)->getNum() > 0;
      if (!this->hasclipplanes) {
        this->searchaction.setType(SoClipPlane::getClassTypeId());
        this->searchaction.setInterest(SoSearchAction::FIRST);
        this->searchaction.setSearchingAll(FALSE);
        this->searchaction.apply(PUBLIC(this));
        if (this->searchaction.getPath()) {
          this->hasclipplanes = TRUE;
        }
        this->searchaction.reset();
      }
      // first, search for texture unit nodes
      this->searchaction.setType(SoTextureUnit::getClassTypeId());
      this->searchaction.setInterest(SoSearchAction::ALL);
      this->searchaction.setSearchingAll(FALSE);
      this->searchaction.apply(PUBLIC(this));

      int lastenabled;
      (void) SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
      this->numtexunitsinscene = lastenabled + 1;

      for (i = 0; i < this->searchaction.getPaths().getLength(); i++) {
        SoFullPath * p = (SoFullPath*) this->searchaction.getPaths()[i];
        SoTextureUnit * unit = (SoTextureUnit*) p->getTail();
        if (unit->unit.getValue() >= this->numtexunitsinscene) {
          this->numtexunitsinscene = unit->unit.getValue() + 1;
        }
      }
      if (this->numtexunitsinscene == 0) this->numtexunitsinscene = 1;

      this->searchaction.reset();
      this->searchaction.setType(SoLight::getClassTypeId());
      this->searchaction.setInterest(SoSearchAction::ALL);
      this->searchaction.setSearchingAll(FALSE);
      this->searchaction.apply(PUBLIC(this));
      this->clearLightPaths();
      this->copyLightPaths(this->searchaction.getPaths());
      this->searchaction.reset();
      this->needscenesearch = FALSE;
    }
    int maxunits = cc_glglue_max_texture_units(glue);

    int maxlights = maxunits - this->numtexunitsinscene;
    SbList <SoTempPath*> & pl = this->lightpaths;

    SbBool lightschanged = FALSE;
    int numlights = 0;
    for (i = 0; i < pl.getLength(); i++) {
      SoLight * light = (SoLight*)((SoFullPath*)(pl[i]))->getTail();
      if (light->on.getValue() && (numlights < maxlights)){
        if (numlights < this->shadowlights.getLength() && this->shadowlights[numlights]->light != light)
          lightschanged = TRUE;
        numlights++;
      }
    }
    if (lightschanged || numlights != this->shadowlights.getLength()) {
      // just delete and recreate all if the number of spot lights have changed
      this->deleteShadowLights();
      int id = lightidoffset;
      for (i = 0; i < pl.getLength(); i++) {
        SoLight * light = (SoLight*)((SoFullPath*)pl[i])->getTail();
        if (light->on.getValue() && (this->shadowlights.getLength() < maxlights)) {
          SoNode * scene = PUBLIC(this);
          SoNode * bboxscene = PUBLIC(this);
          if (light->isOfType(SoShadowSpotLight::getClassTypeId())) {
            SoShadowSpotLight * ssl = (SoShadowSpotLight*) light;
            if (ssl->shadowMapScene.getValue()) {
              scene = ssl->shadowMapScene.getValue();
            }
          }
          else if (light->isOfType(SoShadowDirectionalLight::getClassTypeId())) {
            SoShadowDirectionalLight * sl = (SoShadowDirectionalLight*) light;
            if (sl->shadowMapScene.getValue()) {
              scene = sl->shadowMapScene.getValue();
            }
          }
          SoShadowLightCache * cache = new SoShadowLightCache(state, pl[i],
                                                              PUBLIC(this),
                                                              scene,
                                                              bboxscene,
                                                              &this->gaussfilter,
                                                              &this->gaussalphafilter);
          cache->lightid = id++;
          this->shadowlights.append(cache);
        }
      }
    }
    int unit_scale = transparentShadow() ? 3 : 1;
    int i2 = 0;
    int id = lightidoffset;
    // validate if spot light paths are still valid
    for (i = 0; i < pl.getLength(); i++) {
      SoPath * path = pl[i];
      SoLight * light = (SoLight*) ((SoFullPath*)path)->getTail();
      if (light->on.getValue() && (i2 < maxlights)) {
        SoShadowLightCache * cache = this->shadowlights[i2];
        int unit = (maxunits - 1) - i2 * unit_scale;
        int lightid = id++;
        if (unit != cache->texunit || lightid != cache->lightid) {
          if (this->vertexshadercache) this->vertexshadercache->invalidate();
          if (this->fragmentshadercache) this->fragmentshadercache->invalidate();
          cache->texunit = unit;
          cache->lightid = lightid;
        }
        if (*(cache->path) != *path) {
          cache->path->unref();
          cache->path = path->copy();
        }
        if (cache->light->isOfType(SoSpotLight::getClassTypeId())) {
          this->matrixaction.apply(path);
          this->updateSpotCamera(state, cache, this->matrixaction.getMatrix());
        }
        else if (cache->light->isOfType(SoDirectionalLight::getClassTypeId())) {
          this->matrixaction.apply(path);
          this->updateDirectionalCamera(state, cache, this->matrixaction.getMatrix(), TRUE);
        }
        i2++;
      }
    }
    this->shadowlightsvalid = TRUE;
  }

  smoothing = floorf((smoothing+1e-6f)*10.0f);

  for (i = 0; i < this->shadowlights.getLength(); i++) {
    SoShadowLightCache * cache = this->shadowlights[i];
    if (cache->light->isOfType(SoDirectionalLight::getClassTypeId())) {
      this->matrixaction.apply(cache->path);
      this->updateDirectionalCamera(state, cache, this->matrixaction.getMatrix(), FALSE);
    }
    cache->updateGaussMap(smoothing);
    assert(cache->texunit >= 0);
    assert(cache->lightid >= 0);
    SoTextureUnitElement::set(state, PUBLIC(this), cache->texunit);
    SoMultiTextureMatrixElement::set(state, PUBLIC(this), cache->texunit, cache->matrix);
    this->renderDepthMap(cache, action);
    SoGLMultiTextureEnabledElement::set(state, PUBLIC(this), cache->texunit,
                                        SoGLMultiTextureEnabledElement::DISABLED);
  }
  SoTextureUnitElement::set(state, PUBLIC(this), 0);
}

const SbXfBox3f &
SoShadowGroupP::calcBBox(SoShadowLightCache * cache)
{
  if (cache->light->isOfType(SoShadowDirectionalLight::getClassTypeId())) {
    SoShadowDirectionalLight * sl = static_cast<SoShadowDirectionalLight*> (cache->light);
    SbVec3f size = sl->bboxSize.getValue();
    if (size[0] >= 0.0f && size[1] >= 0.0f && size[2] >= 0.0f) {
      SbVec3f center = sl->bboxCenter.getValue();
      size *= 0.5f;
      this->bboxaction.getXfBoundingBox() = SbXfBox3f(center-size, center+size);
    }
    else {
      this->bboxaction.apply(cache->bboxnode);
    }
  }
  else {
    this->bboxaction.apply(cache->bboxnode);
  }
  return this->bboxaction.getXfBoundingBox();
}

SbBox3f
SoShadowLightCache::toCameraSpace(const SbXfBox3f & worldbox) const
{
  SoCamera * cam = this->camera;
  SbMatrix mat;
  SbXfBox3f xbox = worldbox;
  mat.setTranslate(- cam->position.getValue());
  xbox.transform(mat);
  mat = cam->orientation.getValue().inverse();
  xbox.transform(mat);
  return xbox.project();
}

void
SoShadowGroupP::updateSpotCamera(SoState * COIN_UNUSED_ARG(state), SoShadowLightCache * cache, const SbMatrix & transform)
{
  SoCamera * cam = cache->camera;
  SoSpotLight * light = static_cast<SoSpotLight*> (cache->light);

  assert(cam->isOfType(SoPerspectiveCamera::getClassTypeId()));
  SbVec3f pos = light->location.getValue();
  transform.multVecMatrix(pos, pos);

  SbVec3f dir = light->direction.getValue();
  transform.multDirMatrix(dir, dir);
  (void) dir.normalize();
  float cutoff = light->cutOffAngle.getValue();
  cam->position.setValue(pos);

  // Realthunder: not sure about the following remarks. Can't we just move the
  // far plane back a bit to fully enclose the circle casted by the line cone?
  // A far plane that is too far would result a low quanlity shadow, but beats
  // no shadow at all times.
  //
  // Old remarks: the maximum heightAngle we can render with a camera is < PI/2,.
  // The max cutoff is therefore PI/4. Some slack is needed, and 0.78
  // is about the maximum angle we can do.
  // if (cutoff > 0.78f) cutoff = 0.78f;

  cam->orientation.setValue(SbRotation(SbVec3f(0.0f, 0.0f, -1.0f), dir));
  static_cast<SoPerspectiveCamera*> (cam)->heightAngle.setValue(cutoff * 2.0f);
  SoShadowGroup::VisibilityFlag visflag = (SoShadowGroup::VisibilityFlag) PUBLIC(this)->visibilityFlag.getValue();

  float visnear = PUBLIC(this)->visibilityNearRadius.getValue();
  float visfar = PUBLIC(this)->visibilityRadius.getValue();

  SbBool needbbox =
    (visflag == SoShadowGroup::LONGEST_BBOX_EDGE_FACTOR) ||
    (visflag == SoShadowGroup::PROJECTED_BBOX_DEPTH_FACTOR) ||
    ((visnear < 0.0f) || (visfar < 0.0f));

  if (light->isOfType(SoShadowSpotLight::getClassTypeId())) {
    SoShadowSpotLight * sslight = static_cast<SoShadowSpotLight*> (light);
    const float ssnear = sslight->nearDistance.getValue();
    const float ssfar = sslight->farDistance.getValue();

    if (ssnear > 0.0f && ssfar > ssnear) {
      visnear = ssnear;
      visfar = ssfar;
      needbbox = FALSE;
    }
  }
  if (needbbox) {
    const SbXfBox3f & worldbox = this->calcBBox(cache);
    SbBox3f box = cache->toCameraSpace(worldbox);

    // Bounding box was calculated in camera space, so we need to "flip"
    // the box (because camera is pointing in the (0,0,-1) direction
    // from origo.
    cache->nearval = -box.getMax()[2];
    cache->farval = -box.getMin()[2];

    const int depthbits = 16;
    float r = (float) pow(2.0, (double) depthbits);
    float nearlimit = cache->farval / r;

    if (cache->nearval < nearlimit) {
      cache->nearval = nearlimit;
    }
    const float SLACK = 0.001f;

    cache->nearval = cache->nearval * (1.0f - SLACK);
    cache->farval = cache->farval * (1.0f + SLACK);

    if (visflag == SoShadowGroup::LONGEST_BBOX_EDGE_FACTOR) {
      float sx,sy,sz;
      worldbox.getSize(sx, sy, sz);
      float smax =  SbMax(SbMax(sx, sy), sz);
        if (visnear > 0.0f) visnear = smax * visnear;
        if (visfar > 0.0f) visfar = smax  * visfar;
    }
    else if (visflag == SoShadowGroup::PROJECTED_BBOX_DEPTH_FACTOR) {
      if (visnear > 0.0f) visnear = cache->farval * visnear; // should be calculated from farval, not nearval
      if (visfar > 0.0f) visfar = cache->farval * visfar;
    }
  }

  if (visnear > 0.0f) cache->nearval = visnear;
  if (visfar > 0.0f) cache->farval = visfar;

  if (cache->nearval != cam->nearDistance.getValue()) {
    cam->nearDistance = cache->nearval;
  }
  if (cache->farval != cam->farDistance.getValue()) {
    cam->farDistance = cache->farval;
  }

  // float realfarval = cutoff >= 0.0f ? cache->farval / float(cos(cutoff)) : cache->farval;
  //
  // Just move the far plane a bit further to enclose the light cone circle. 1/0.7 = 1/cos(PI/4) 
  float realfarval = cache->farval / 0.7f;

  cache->fragment_farval->value = realfarval;
  cache->vsm_farval->value = realfarval;

  cache->coscutoff->value = float(cos(cutoff>=0.0f?cutoff:0.0f));

  cache->fragment_nearval->value = cache->nearval;
  cache->vsm_nearval->value = cache->nearval;

  SbViewVolume vv = cam->getViewVolume(1.0f);
  SbMatrix affine, proj;

  vv.getMatrices(affine, proj);
  cache->matrix = affine * proj;
}

void
SoShadowGroupP::updateDirectionalCamera(SoState * state, SoShadowLightCache * cache, const SbMatrix & transform, SbBool fullupdate)
{
  SoOrthographicCamera * cam = static_cast<SoOrthographicCamera*>(cache->camera);
  assert(cache->light->isOfType(SoDirectionalLight::getClassTypeId()));
  SoDirectionalLight * light = static_cast<SoDirectionalLight*> (cache->light);

  float maxdist = 0.0f;
  if (cache->light->isOfType(SoShadowDirectionalLight::getClassTypeId()))
    maxdist = static_cast<SoShadowDirectionalLight*>(light)->maxShadowDistance.getValue();

  SbVec3f dir = light->direction.getValue();
  dir.normalize();
  transform.multDirMatrix(dir, dir);
  dir.normalize();

  SbPlane plane(dir, cam->position.getValue());
  // move to eye space
  plane.transform(SoViewingMatrixElement::get(state));
  SbVec3f N = plane.getNormal();
  float D = plane.getDistanceFromOrigin();

#if 0
  fprintf(stderr,"isect: %g %g %g, %g %g %g\n",
          isect.getMin()[0],
          isect.getMin()[1],
          isect.getMin()[2],
          isect.getMax()[0],
          isect.getMax()[1],
          isect.getMax()[2]);
  fprintf(stderr,"plane: %g %g %g, %g\n", N[0], N[1], N[2], D);
  fprintf(stderr,"nearfar: %g %g\n", cam->nearDistance.getValue(), cam->farDistance.getValue());
  fprintf(stderr,"aspect: %g\n", SoViewportRegionElement::get(state).getViewportAspectRatio());
#endif

  cache->fragment_lightplane->value.setValue(N[0], N[1], N[2], D);

  if (!fullupdate) return;

  cam->orientation.setValue(SbRotation(SbVec3f(0.0f, 0.0f, -1.0f), dir));
  SbViewVolume vv = SoViewVolumeElement::get(state);
  const SbXfBox3f & worldbox = this->calcBBox(cache);
  SbBool visible = TRUE;
  if (maxdist > 0.0f) {
    float nearv = vv.getNearDist();
    if (maxdist < nearv) visible = FALSE;
    else {
      maxdist -= nearv;
      float depth = vv.getDepth();
      if (maxdist > depth) maxdist = depth;
      vv = vv.zNarrow(1.0f, 1.0f - maxdist/depth);
    }
  }
  SbBox3f isect;
  if (visible) {
    isect = vv.intersectionBox(worldbox);
    if (isect.isEmpty()) visible = FALSE;
  }
  if (!visible) {
    if (cache->depthmap->scene.getValue() == cache->depthmapscene) {
      cache->depthmap->scene = new SoInfo;
    }
    return;
  }
  if (cache->depthmap->scene.getValue() != cache->depthmapscene) {
    cache->depthmap->scene = cache->depthmapscene;
  }
  cam->viewBoundingBox(isect, 1.0f, 1.0f);

  SbBox3f box = cache->toCameraSpace(isect);

  // Bounding box was calculated in camera space, so we need to "flip"
  // the box (because camera is pointing in the (0,0,-1) direction
  // from origo. Add a little slack (multiply by 1.01)
  cam->nearDistance = -box.getMax()[2]*1.01f;
  cam->farDistance = -box.getMin()[2]*1.01f;

  //SoShadowGroup::VisibilityFlag visflag = (SoShadowGroup::VisibilityFlag) PUBLIC(this)->visibilityFlag.getValue();

  float visnear = cam->nearDistance.getValue();
  float visfar = cam->farDistance.getValue();

  cache->nearval = visnear;
  cache->farval = visfar;

  if (cache->nearval != cam->nearDistance.getValue()) {
    cam->nearDistance = cache->nearval;
  }
  if (cache->farval != cam->farDistance.getValue()) {
    cam->farDistance = cache->farval;
  }

  float realfarval = cache->farval * 1.1f;
  cache->fragment_farval->value = realfarval;
  cache->vsm_farval->value = realfarval;

  cache->fragment_nearval->value = cache->nearval;
  cache->vsm_nearval->value = cache->nearval;

  vv = cam->getViewVolume(1.0f);
  SbMatrix affine, proj;
  vv.getMatrices(affine, proj);
  cache->matrix = affine * proj;
}

void
SoShadowGroupP::renderDepthMap(SoShadowLightCache * cache,
                               SoGLRenderAction * action)
{
  SoState *state = action->getState();

  // Make sure the depth buffer inside is cleared before rendering
  cache->depthmap->setDepthBuffer(state, GL_INVALID_VALUE, TRUE);

  cache->depthmap->GLRender(action);

  // Prevent depth buffer clearing, because the same depthmap will
  // be used for gauss filtering. We need to keep the depth buffer
  // to be reused for alphamap (i.e. transparent shadow) rendering.
  cache->depthmap->setDepthBuffer(state, GL_INVALID_VALUE, FALSE);

  if (cache->depthmap->scene.getValue() != cache->depthmapscene)
    return;

  bool redraw = false;
  if(cache->depthmap->getNodeId() != cache->depthnodeid) {
    cache->gaussfilter->apply(action, cache->depthmap, cache->gaussdepthsceneH, cache->gaussdepthsceneV);
    cache->depthnodeid = cache->depthmap->getNodeId();
    redraw = true;
  }

  if (!this->transparentShadow() || !cache->hastransp) {
    // if (cache->alphamap) {
    //   cache->alphamap->unref();
    //   cache->alphamap = NULL;
    // }
    // if (cache->alphadepthmap) {
    //   cache->alphadepthmap->unref();
    //   cache->alphadepthmap = NULL;
    // }
    return;
  }

  if (!cache->alphamap) {
    redraw = true;
    cache->createAlphaMap();
  }

  // Transparent shadow map is stored using two extra textures.
  //
  // One RGBA8 color texutre (alphamap) is generated by rendering the scene
  // with only transparent objects, using the depth buffer of the previous
  // opaque object only rendering pass (depthmap), with alpha blending and
  // depth write disabled. This is for cases where there is an oqapue objects
  // inbetween two transparent objects, the lower transparent object can be
  // correctly occluded.
  //
  // The other depth texture (alphadepthmap) is generated by a second pass
  // rendering of transparent objects only. This is to prevent casting upper
  // transparent object shadows from lower transparent object.
  //
  // When rendering transparent shadow, consult alphamap only if the fragment
  // is not under opaque shadow. When casting on an opaque objects, simply
  // multiply the shade color with the color in the alphamap. When casting on
  // a transparent object, only multiply if the object pass the depth test
  // against alphadepthmap.

  SoTextureUnitElement::set(state, PUBLIC(this), cache->texunit-1);
  SoMultiTextureMatrixElement::set(state, PUBLIC(this), cache->texunit-1, cache->matrix);

  if(!redraw && cache->alphamap->getNodeId() == cache->alphanodeid
             && cache->depthmap->getDepthBuffer() == cache->alphamap->getDepthBuffer())
    cache->alphamap->GLRender(action);
  else {
    cache->alphamap->setDepthBuffer(state, cache->depthmap->getDepthBuffer(), FALSE);
    cache->alphamap->GLRender(action);
    cache->gaussalphafilter->apply(action, cache->alphamap, cache->gaussalphasceneH, cache->gaussalphasceneV);
    cache->alphanodeid = cache->alphamap->getNodeId();
  }

  SoTextureUnitElement::set(state, PUBLIC(this), cache->texunit-2);
  SoMultiTextureMatrixElement::set(state, PUBLIC(this), cache->texunit-2, cache->matrix);

  cache->alphadepthmap->GLRender(action);

  SoGLMultiTextureEnabledElement::set(state, PUBLIC(this), cache->texunit-2,
                                      SoGLMultiTextureEnabledElement::DISABLED);

  SoGLMultiTextureEnabledElement::set(state, PUBLIC(this), cache->texunit-1,
                                      SoGLMultiTextureEnabledElement::DISABLED);
}

namespace {
  void initLightMaterial(SoShaderGenerator & gen, int i) {
    SbString str;
    str.sprintf("ambient = gl_LightSource[%d].ambient;\n"
                "diffuse = gl_LightSource[%d].diffuse;\n"
                "specular = gl_LightSource[%d].specular;\n", i,i,i);
    gen.addMainStatement(str);
  }

  void addDirectionalLight(SoShaderGenerator & gen, int i) {
    initLightMaterial(gen, i);
    SbString str;
    str.sprintf("DirectionalLight(normalize(vec3(gl_LightSource[%d].position)),"
                "vec3(gl_LightSource[%d].halfVector), normal, diffuse, specular);", i,i);
    gen.addMainStatement(str);
  }
  void addSpotLight(SoShaderGenerator & gen, int i, SbBool needdist = FALSE) {
    initLightMaterial(gen, i);
    const char * dist = needdist ? "dist = " : "";
    SbString str;
    str.sprintf("%s SpotLight("
                "vec3(gl_LightSource[%d].position),"
                "vec3(gl_LightSource[%d].constantAttenuation,"
                "     gl_LightSource[%d].linearAttenuation,"
                "     gl_LightSource[%d].quadraticAttenuation),"
                "gl_LightSource[%d].spotDirection,"
                "gl_LightSource[%d].spotExponent,"
                "gl_LightSource[%d].spotCosCutoff,"
                "eye, ecPosition3, normal, ambient, diffuse, specular);",
                dist, i,i,i,i,i,i,i);
    gen.addMainStatement(str);
  }
  void addCachedSpotLight(SoShaderGenerator & gen, int i, SbBool needdist = FALSE) {
    initLightMaterial(gen, i);
    const char * dist = needdist ? "dist = " : "";
    SbString str;
    str.sprintf("%s SpotLight("
                "vec3(gl_LightSource[%d].position),"
                "vec3(gl_LightSource[%d].constantAttenuation,"
                "     gl_LightSource[%d].linearAttenuation,"
                "     gl_LightSource[%d].quadraticAttenuation),"
                "gl_LightSource[%d].spotDirection,"
                "dropoffrate%d * 128.0,"
                "coscutoff%d,"
                "eye, ecPosition3, normal, ambient, diffuse, specular);",
                dist, i,i,i,i,i,i,i);
    gen.addMainStatement(str);
  }
  void addDirSpotLight(SoShaderGenerator & gen, int i, SbBool needdist = FALSE) {
    initLightMaterial(gen, i);
    const char * dist = needdist ? "dist = " : "";
    SbString str;
    str.sprintf("%s DirSpotLight("
                " -normalize(vec3(gl_LightSource[%d].spotDirection)),"
                " vec3(gl_LightSource[%d].position),"
                " eye, ecPosition3, normal, diffuse, specular);", dist, i, i);
    gen.addMainStatement(str);
  }

  void addPointLight(SoShaderGenerator & gen, int i) {
    initLightMaterial(gen, i);
    SbString str;
    str.sprintf("PointLight("
                "vec3(gl_LightSource[%d].position),"
                "vec3(gl_LightSource[%d].constantAttenuation,"
                "     gl_LightSource[%d].linearAttenuation,"
                "     gl_LightSource[%d].quadraticAttenuation),"
                " eye, ecPosition3, normal, ambient, diffuse, specular);", i,i,i,i);

    gen.addMainStatement(str);

  }

};

void
SoShadowGroupP::setVertexShader(SoState * state)
{
  int i;
  SoShaderGenerator & gen = this->vertexgenerator;
  gen.reset(FALSE);
  gen.setVersion("#version 120");

  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);

  state->push();

  SbBool perpixelspot = FALSE;
  SbBool perpixelother = FALSE;

  this->getQuality(state, perpixelspot, perpixelother);

  if (this->vertexshadercache) {
    this->vertexshadercache->unref();
  }
  this->vertexshadercache = new SoShaderProgramCache(state);
  this->vertexshadercache->ref();

  const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));

  // set active cache to record cache dependencies
  SoCacheElement::set(state, this->vertexshadercache);
  const SoNodeList & lights = SoLightElement::getLights(state);

  int numshadowlights = this->shadowlights.getLength();

  for (i = 0; i < numshadowlights; i++) {
    SbString str;
    str.sprintf("varying vec4 shadowCoord%d;", i);
    gen.addDeclaration(str, FALSE);

#ifdef PER_VERTEX_SHADOW
    if (!perpixelspot) {
      str.sprintf("varying vec3 spotVertexColor%d;", i);
      gen.addDeclaration(str, FALSE);
    }
#endif
  }

  if (numshadowlights) {
    gen.addDeclaration("uniform mat4 cameraTransform;", FALSE);
  }

  if (!this->transparentShadow())
    gen.addDeclaration("uniform int coin_two_sided_lighting;\n", FALSE);

  gen.addDeclaration("varying vec3 ecPosition3;", FALSE);
  gen.addDeclaration("varying vec3 fragmentNormal;", FALSE);
  gen.addDeclaration("varying vec3 perVertexColor;", FALSE);

  SbBool dirlight = FALSE;
  SbBool pointlight = FALSE;
  SbBool spotlight = FALSE;
  SbString str;

  gen.addMainStatement("vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;\n"
                       "ecPosition3 = ecPosition.xyz / ecPosition.w;");

  gen.addMainStatement("vec3 normal = normalize(gl_NormalMatrix * gl_Normal);\n"
                       "vec3 eye = -normalize(ecPosition3);\n"
                       "vec4 ambient;\n"
                       "vec4 diffuse;\n"
                       "vec4 specular;\n"
                       "vec4 accambient = vec4(0.0);\n"
                       "vec4 accdiffuse = vec4(0.0);\n"
                       "vec4 accspecular = vec4(0.0);\n"
                       "vec4 color;\n");

  gen.addMainStatement("fragmentNormal = normal;");

  gen.addMainStatement(
      "vec4 sceneColor = gl_FrontLightModelProduct.sceneColor;\n"
      "vec4 mambient = gl_FrontMaterial.ambient;\n"
      "vec4 mspecular = gl_FrontMaterial.specular;\n"
      "if (normal.z < 0.0 ");
  if (!this->transparentShadow())
    gen.addMainStatement(
        "&& coin_two_sided_lighting != 0");
  gen.addMainStatement(
      ") {\n"
      "   normal = -normal;\n"
      "   sceneColor = gl_BackLightModelProduct.sceneColor;\n"
      "   mambient = gl_BackMaterial.ambient;\n"
      "   mspecular = gl_BackMaterial.specular;\n"
      "}\n");

  if (!perpixelother) {
    for (i = 0; i < lights.getLength(); i++) {
      SoLight * l = (SoLight*) lights[i];
      if (l->isOfType(SoDirectionalLight::getClassTypeId())) {
        addDirectionalLight(gen, i);
        dirlight = TRUE;
      }
      else if (l->isOfType(SoSpotLight::getClassTypeId())) {
        addSpotLight(gen, i);
        spotlight = TRUE;
      }
      else if (l->isOfType(SoPointLight::getClassTypeId())) {
        addPointLight(gen, i);
        gen.addMainStatement(str);
        pointlight = TRUE;
      }
      else {
        SoDebugError::postWarning("SoShadowGroupP::setVertexShader",
                                  "Unknown light type: %s",
                                  l->getTypeId().getName().getString());
      }
      gen.addMainStatement("accambient += ambient; accdiffuse += diffuse; accspecular += specular;\n");
    }

    if (dirlight) gen.addNamedFunction(SbName("lights/DirectionalLight"), FALSE);
    if (pointlight) gen.addNamedFunction(SbName("lights/PointLight"), FALSE);

    gen.addMainStatement("color = sceneColor + "
                         "  accambient * mambient + "
                         "  accdiffuse * gl_Color +"
                         "  accspecular * mspecular;\n"
                         );
  }
  else {
    gen.addMainStatement("color = sceneColor;\n");
  }

  if (numshadowlights) {
    gen.addMainStatement("vec4 pos = cameraTransform * ecPosition;\n"); // in world space
  }
  for (i = 0; i < numshadowlights; i++) {
    SoShadowLightCache * cache = this->shadowlights[i];
    str.sprintf("shadowCoord%d = gl_TextureMatrix[%d] * pos;\n", i, cache->texunit); // in light space
    gen.addMainStatement(str);

#ifdef PER_VERTEX_SHADOW
    if (!perpixelspot) {
      spotlight = TRUE;
      addCachedSpotLight(gen, cache->lightid);
      str.sprintf("spotVertexColor%d = \n"
                  "  ambient.rgb * mambient.rgb + "
                  "  diffuse.rgb * gl_Color.rgb + "
                  "  specular.rgb * mspecular.rgb;\n", i);
      gen.addMainStatement(str);
    }
#endif
  }

  if (spotlight) gen.addNamedFunction(SbName("lights/SpotLight"), FALSE);
  int32_t fogType = this->getFog(state);

  switch (fogType) {
  default:
    assert(0 && "unknown fog type");
  case SoEnvironmentElement::NONE:
    // do nothing
    break;
  case SoEnvironmentElement::HAZE:
  case SoEnvironmentElement::FOG:
  case SoEnvironmentElement::SMOKE:
    gen.addMainStatement("gl_FogFragCoord = abs(ecPosition3.z);\n");
    break;
  }
  gen.addMainStatement("perVertexColor = vec3(clamp(color.r, 0.0, 1.0), clamp(color.g, 0.0, 1.0), clamp(color.b, 0.0, 1.0));"
                       "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"
                       "gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord1;\n"
                       "gl_Position = ftransform();\n"
                       "gl_FrontColor = gl_Color;\n");

  if (this->hasclipplanes) {
    if (SoGLDriverDatabase::isSupported(glue, SO_GL_GLSL_CLIP_VERTEX_HW)) {
      gen.addMainStatement("gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;\n");
    }
  }

  this->vertexshader->parameter.setNum(0);
  if (numshadowlights)
    this->vertexshader->parameter.set1Value(0, this->cameratransform);

#ifdef PER_VERTEX_SHADOW
  if (!perpixelspot) {
    for (i = 0; i < numshadowlights; i++) {
      SoShadowLightCache * cache = this->shadowlights[i];
      if (cache->light->isOfType(SoSpotLight::getClassTypeId())) {
        SbString str;
        SoSpotLight * sl = static_cast<SoSpotLight*> (cache->light);
        SoShaderParameter1f * dropoffrate = cache->dropoffrate;
        dropoffrate->value.connectFrom(&sl->dropOffRate);
        str.sprintf("dropoffrate%d", cache->lightid);
        if (dropoffrate->name.getValue() != str) {
            dropoffrate->name = str;
        }
        SbString uniform;
        uniform.sprintf("uniform float %s;\n", str.getString());
        gen.addDeclaration(uniform, FALSE);
        this->vertexshader->parameter.set1Value(this->vertexshader->parameter.getNum(), dropoffrate);

        SoShaderParameter1f * coscutoff = cache->coscutoff;
        str.sprintf("coscutoff%d", cache->lightid);
        if (coscutoff->name.getValue() != str) {
            coscutoff->name = str;
        }
        uniform.sprintf("uniform float %s;\n", str.getString());
        gen.addDeclaration(uniform, FALSE);
        this->vertexshader->parameter.set1Value(this->vertexshader->parameter.getNum(), coscutoff);
      }
    }
  }
#endif

  if (!this->transparentShadow())
    this->vertexshader->parameter.set1Value(this->vertexshader->parameter.getNum(), this->twosided);

  // never update unless the program has actually changed. Creating a
  // new GLSL program is very slow on current drivers.
  if (this->vertexshader->sourceProgram.getValue() != gen.getShaderProgram()) {
    this->vertexshader->sourceProgram = gen.getShaderProgram();
    this->vertexshader->sourceType = SoShaderObject::GLSL_PROGRAM;
  }

  this->vertexshadercache->set(gen.getShaderProgram());

  state->pop();
  SoCacheElement::setInvalid(storedinvalid);

}

void
SoShadowGroupP::setFragmentShader(SoState * state)
{
  int i;

  int smoothing = 0;
  float s = PUBLIC(this)->smoothBorder.getValue();
  if (s > 0)
    smoothing = int(round(s*1e7)) % 1000000;

  SoShaderGenerator & gen = this->fragmentgenerator;
  gen.reset(FALSE);
  gen.setVersion("#version 120");

  SbBool perpixelspot = FALSE;
  SbBool perpixelother = FALSE;
  this->getQuality(state, perpixelspot, perpixelother);

  const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));
  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);
  state->push();

  if (this->fragmentshadercache) {
    this->fragmentshadercache->unref();
  }
  this->fragmentshadercache = new SoShaderProgramCache(state);
  this->fragmentshadercache->ref();

  // set active cache to record cache dependencies
  SoCacheElement::set(state, this->fragmentshadercache);

  int numshadowlights = this->shadowlights.getLength();
  SbBool dirspot = FALSE;

  if (numshadowlights) {
    SbString eps;
    eps.sprintf("const float EPSILON = %f;",
                PUBLIC(this)->epsilon.getValue());
    gen.addDeclaration(eps, FALSE);
    eps.sprintf("const float THRESHOLD = %f;",
                PUBLIC(this)->threshold.getValue());
    gen.addDeclaration(eps, FALSE);
  }
  for (i = 0; i < numshadowlights; i++) {
    SbString str;
    str.sprintf("uniform sampler2D shadowMap%d;", i);
    gen.addDeclaration(str, FALSE);

    if (this->shadowlights[i]->hastransp && this->shadowlights[i]->alphamap) {
      str.sprintf("uniform sampler2D alphaMap%d;", i);
      gen.addDeclaration(str, FALSE);

      str.sprintf("uniform sampler2DShadow alphaDepthMap%d;", i);
      gen.addDeclaration(str, FALSE);
    }

    str.sprintf("uniform float farval%d;", i);
    gen.addDeclaration(str, FALSE);

    str.sprintf("uniform float nearval%d;", i);
    gen.addDeclaration(str, FALSE);

    str.sprintf("varying vec4 shadowCoord%d;", i);
    gen.addDeclaration(str, FALSE);

#ifdef PER_VERTEX_SHADOW
    if (!perpixelspot) {
      str.sprintf("varying vec3 spotVertexColor%d;", i);
      gen.addDeclaration(str, FALSE);
    }
#endif
    if (this->shadowlights[i]->light->isOfType(SoDirectionalLight::getClassTypeId())) {
      str.sprintf("uniform vec4 lightplane%d;", i);
      gen.addDeclaration(str, FALSE);
    }
  }

  SbString str;
  if (numshadowlights) {
#ifdef DISTRIBUTE_FACTOR
    str.sprintf("const float DISTRIBUTE_FACTOR = %.1f;\n", DISTRIBUTE_FACTOR);
    gen.addDeclaration(str, FALSE);
#endif
  }
  gen.addDeclaration("varying vec3 ecPosition3;", FALSE);
  gen.addDeclaration("varying vec3 fragmentNormal;", FALSE);
  gen.addDeclaration("varying vec3 perVertexColor;", FALSE);

  const SoNodeList & lights = SoLightElement::getLights(state);

  if (numshadowlights) {
    gen.addNamedFunction("vsm/VsmLookup", FALSE);
  }
  gen.addMainStatement("vec3 normal = normalize(fragmentNormal);\n");

  gen.setVersion("#version 110");
  gen.addMainStatement(
      "vec4 mambient = gl_FrontMaterial.ambient;\n"
      "vec4 mspecular = gl_FrontMaterial.specular;\n"
      "vec3 fdx = dFdx(ecPosition3);\n"
      "vec3 fdy = dFdy(ecPosition3);\n"
      "vec3 faceNormal = normalize(cross(fdx,fdy));\n"
      "if (dot (normal, faceNormal) < 0.0 ");
  if (!this->transparentShadow())
    gen.addMainStatement(
        "&& coin_two_sided_lighting != 0");
  gen.addMainStatement(
      ") {\n"
      "   normal = -normal;\n"
      "   mambient = gl_BackMaterial.ambient;\n"
      "   mspecular = gl_BackMaterial.specular;\n"
      "}\n");

  gen.addMainStatement("vec3 eye = -normalize(ecPosition3);\n");
  gen.addMainStatement("vec4 ambient = vec4(0.0);\n"
                       "vec4 diffuse = vec4(0.0);\n"
                       "vec4 specular = vec4(0.0);"
                       "vec4 mydiffuse = gl_Color;\n"
                       "vec4 texcolor = (coin_texunit0_model != 0) ? texture2D(textureMap0, gl_TexCoord[0].xy) : vec4(1.0);\n");

  if (this->numtexunitsinscene > 1) {
    gen.addMainStatement("if (coin_texunit1_model != 0) texcolor *= texture2D(textureMap1, gl_TexCoord[1].xy);\n");
  }
  gen.addMainStatement("vec3 color = perVertexColor;\n"
                       "vec3 scolor = vec3(0.0);\n"
                       "vec3 accumAlphaColor = vec3(1.0);\n"
                       "vec4 acolor = vec4(1.0);\n"
                       "float dist;\n"
                       "float swidth;\n"
                       "float accumShade = 0.0;\n"
                       "float shadeFactor;\n"
                       "float alphaFactor;\n"
                       "float s;\n"
                       "vec3 coord;\n"
                       "vec4 map;\n"
                       "mydiffuse.a *= texcolor.a;\n");

  float swidth = (smoothing % 100000)*5e-5f;

  // To simplify shadow rendering a bit, we'll always perform perpixel shading for now
#ifdef PER_VERTEX_SHADOW
  if (perpixelspot)
#endif
  {
    SbString str;
    int count;
#define MAX_OFFSET_COUNT 8
    float offsets[MAX_OFFSET_COUNT*MAX_OFFSET_COUNT*2];
    if (smoothing >= 100000) {
      gen.addMainStatement("vec2 offset = vec2(0.0, 0.0);\n");
      gen.addMainStatement(str);
      count = smoothing/100000*2 + 1;
      if (count > MAX_OFFSET_COUNT) count = MAX_OFFSET_COUNT;
      int center = count/2;
      for (int j=0;j<count;++j) {
        for (int k=0;k<count;++k) {
            offsets[j*count*2 + k*2] = float(k-center);
            offsets[j*count*2 + k*2 + 1] = float(j-center);
        }
      }
      count *= count;
    }
    else if (smoothing) {
      gen.addMainStatement("vec2 offset = mod(floor(gl_FragCoord.xy),2.0);\n");
      gen.addMainStatement("offset.y = -offset.y;\n");
      gen.addMainStatement(str);
      count = 4;
      offsets[0] = -1.5f;
      offsets[1] = 1.5f;
      offsets[2] = -1.5f;
      offsets[3] = -0.5f;
      offsets[4] = 0.5f;
      offsets[5] = 1.5f;
      offsets[6] = 0.5f;
      offsets[7] = -0.5f;
    }
    else {
      gen.addMainStatement("vec2 offset = vec2(0.0, 0.0);\n");
      count = 1;
      offsets[0] = 0.0f;
      offsets[1] = 0.0f;
      swidth = 0.0f;
    }

    SbBool spotlight = FALSE;
    SbBool dirlight = FALSE;

    for (i = 0; i < numshadowlights; i++) {
      SoShadowLightCache * cache = this->shadowlights[i];
      SbBool dirshadow = FALSE;
      SbBool normalspot = FALSE;
      SbBool insidetest = TRUE;

      SoLight * light = this->shadowlights[i]->light;
      if (light->isOfType(SoSpotLight::getClassTypeId())) {
        SoSpotLight * sl = static_cast<SoSpotLight*> (light);
        if (sl->dropOffRate.getValue() >= 0.0f) {
          insidetest = FALSE;
          spotlight = TRUE;
          normalspot = TRUE;
        }
        else {
          insidetest = FALSE;
          dirspot = TRUE;
        }
      }
      else {
        dirshadow = TRUE;
        dirlight = TRUE;
      }

      if (dirshadow) {
        str.sprintf("dist = dot(ecPosition3.xyz, lightplane%d.xyz) - lightplane%d.w;\n", i,i);
        gen.addMainStatement(str);
        addDirectionalLight(gen, cache->lightid);
      }
      else {
        if (normalspot) {
          addCachedSpotLight(gen, cache->lightid, TRUE);
        }
        else {
          addDirSpotLight(gen, cache->lightid, TRUE);
        }
      }

      str.sprintf("swidth = %f;\n", dirshadow?swidth:(swidth*0.1f));
      gen.addMainStatement(str);

      str.sprintf("coord = 0.5 * (shadowCoord%d.xyz / shadowCoord%d.w + vec3(1.0));\n", i , i);
      gen.addMainStatement(str);

      str.sprintf("if (shadowCoord%d.z < -1.0", i);
      gen.addMainStatement(str);
      if (insidetest)
        gen.addMainStatement(" || coord.x < 0.001 || coord.x > 0.999 || coord.y < 0.001 || coord.y > 0.999");
      gen.addMainStatement(
          ") {\n"
          "  shadeFactor = 1.0;\n"
          "  acolor = vec4(1.0);\n"
          "} else {\n"
          "  shadeFactor = 0.0;\n"
          "  alphaFactor = 0.0;\n");

      for (int j=0; j<count; ++j) {
        str.sprintf("map = texture2D(shadowMap%d, coord.xy + "
            "(vec2(%f,%f)+offset.xy)*swidth*0.001*shadowCoord%d.w);\n",
            i, offsets[j*2], offsets[j*2+1], i);
        gen.addMainStatement(str);
#ifdef USE_NEGATIVE
        gen.addMainStatement("map = (map + vec4(1.0)) * 0.5;\n");
#endif // USE_NEGATIVE
#ifdef DISTRIBUTE_FACTOR
        gen.addMainStatement("map.xy += map.zw / DISTRIBUTE_FACTOR;\n");
#endif
        str.sprintf("shadeFactor += map.x < 0.9999 ? "
              "VsmLookup(map, (dist - nearval%d) / (farval%d - nearval%d), EPSILON, THRESHOLD) : 1.0;\n",
              i, i, i);
        gen.addMainStatement(str);
      }
      if (count > 1) {
        str.sprintf("shadeFactor /= %d.0;\n", count);
        gen.addMainStatement(str);
      }

      if (cache->hastransp && cache->alphamap) {
        gen.addMainStatement(
            "if (shadeFactor < 0.01) \n"
            "  acolor = vec4(1.0);\n"
            "else {\n"
            "  acolor = vec4(0.0);\n");
        for (int j=0; j<count; ++j) {
          str.sprintf("map = texture2D(alphaMap%d, coord.xy + "
                "(vec2(%f,%f)+offset.xy)*swidth*0.001*shadowCoord%d.w);\n",
                i, offsets[j*2], offsets[j*2+1], i);
          gen.addMainStatement(str);
          gen.addMainStatement("acolor += map;\n");
        }
        if (count > 1) {
          str.sprintf("acolor /= %d.0;\n", count);
          gen.addMainStatement(str);
        }
        // If casting transparent shadow on a transparent fragment, use the
        // alpha depth map to get a depth comparison result and use it as alpha
        // to blend with pure white. So if shadow2D() returns 1.0, it means the
        // current fragment is completly above the shadow, so acolor will
        // become white, so no change of fragment color, i.e. no transparent
        // color shadow.
        str.sprintf("if(acolor.a > 0.0 && mydiffuse.a < 1.0) {\n"
                    "  float d = shadow2D(alphaDepthMap%d, coord).z;\n"
                    "  acolor *= 1.0 - d;\n"
                    "  acolor.rgb += d;\n"
                    "}\n"
                    "shadeFactor *= 1.0 - acolor.a;\n"
                    , i);
        gen.addMainStatement(str);
        gen.addMainStatement("}\n");
      }

      if (dirshadow && light->isOfType(SoShadowDirectionalLight::getClassTypeId())) {
        SoShadowDirectionalLight * sl = static_cast<SoShadowDirectionalLight*> (light);
        if (sl->maxShadowDistance.getValue() > 0.0f) {
          gen.addMainStatement("shadeFactor = 1.0 - shadeFactor;\n");

          // linear falloff
          // str.sprintf("shadeFactor *= max(0.0, min(1.0, 1.0 + ecPosition3.z/maxshadowdistance%d));\n", i);

          // See SoGLEnvironemntElement.cpp (updategl()) to see how the magic exp() constants here are calculated

          // exp(f) falloff
          // str.sprintf("shadeFactor *= min(1.0, exp(5.545*ecPosition3.z/maxshadowdistance%d));\n", i);
          // just use exp(f^2) as a falloff formula for now, consider making this configurable
          str.sprintf("shadeFactor *= min(1.0, exp(2.35*ecPosition3.z*abs(ecPosition3.z)/(maxshadowdistance%d*maxshadowdistance%d)));\n", i,i);
          gen.addMainStatement(str);
          gen.addMainStatement("shadeFactor = 1.0 - shadeFactor;\n");
        }
      }
      gen.addMainStatement(
          "}\n"
          "accumShade += shadeFactor;\n"
          "accumAlphaColor *= acolor.rgb;\n"
          "color += shadeFactor * diffuse.rgb * mydiffuse.rgb * acolor.rgb;\n"
          "scolor += shadeFactor * mspecular.rgb * specular.rgb;\n"
          "color += ambient.rgb * mambient.rgb;\n");
    }
    str.sprintf("accumShade /= %d.0;\n", numshadowlights);
    gen.addMainStatement(str);

    if (perpixelother) {
      SbBool pointlight = FALSE;
      for (i = 0; i < lights.getLength(); i++) {
        SoLight * l = (SoLight*) lights[i];
        if (l->isOfType(SoDirectionalLight::getClassTypeId())) {
          addDirectionalLight(gen, i);
          dirlight = TRUE;
        }
        else if (l->isOfType(SoSpotLight::getClassTypeId())) {
          addSpotLight(gen, i);
          spotlight = TRUE;
        }
        else if (l->isOfType(SoPointLight::getClassTypeId())) {
          addPointLight(gen, i);
          pointlight = TRUE;
        }
        else {
          SoDebugError::postWarning("SoShadowGroupP::setFragmentShader",
                                    "Unknown light type: %s",
                                    l->getTypeId().getName().getString());
        }
        gen.addMainStatement("color += ambient.rgb * mambient.rgb + "
                             "diffuse.rgb * mydiffuse.rgb;\n");
        gen.addMainStatement("scolor += specular.rgb * mspecular.rgb;\n");
      }

      if (pointlight) gen.addNamedFunction(SbName("lights/PointLight"), FALSE);
    }
    if (dirlight) gen.addNamedFunction(SbName("lights/DirectionalLight"), FALSE);
    if (spotlight) gen.addNamedFunction(SbName("lights/SpotLight"), FALSE);
  }
#ifdef PER_VERTEX_SHADOW
  else {
    for (i = 0; i < numshadowlights; i++) {
      SbString insidetest = "&& coord.x >= 0.0 && coord.x <= 1.0 && coord.y >= 0.0 && coord.y <= 1.0)";

      SoLight * light = this->shadowlights[i]->light;
      if (light->isOfType(SoSpotLight::getClassTypeId())) {
        SoSpotLight * sl = static_cast<SoSpotLight*> (light);
        if (sl->dropOffRate.getValue() >= 0.0f) {
          insidetest = ")";
        }
      }
      SbString str;
      str.sprintf("dist = length(vec3(gl_LightSource[%d].position) - ecPosition3);\n"
                  "coord = 0.5 * (shadowCoord%d.xyz / shadowCoord%d.w + vec3(1.0));\n"
                  "map = texture2D(shadowMap%d, coord.xy);\n"
#ifdef USE_NEGATIVE
                  "map = (map + vec4(1.0)) * 0.5;\n"
#endif // USE_NEGATIVE
#ifdef DISTRIBUTE_FACTOR
                  "map.xy += map.zw / DISTRIBUTE_FACTOR;\n"
#endif
                  "shadeFactor = (shadowCoord%d.z > -1.0%s ? VsmLookup(map, (dist - nearval%d)/(farval%d-nearval%d), EPSILON, THRESHOLD) : 1.0;\n"
                  "color += shadeFactor * spotVertexColor%d;\n",
                  lights.getLength()+i, i , i, i, i,insidetest.getString(), i,i,i,i);
      gen.addMainStatement(str);
    }
  }
#endif

  gen.addMainStatement(
      "if (coin_light_model != 0) {\n"
      "  color *= texcolor.rgb;\n"
      "  color += scolor;\n"
      "} else \n"
      // When lighting is off, the shadeFactor seems too aggressive. The clamp
      // is added here to mix the shadow color with the ground. It would be better
      // to expose this as an attribute somehow.
      "  color = clamp(accumShade,0.5,1.0) * accumAlphaColor * mydiffuse.rgb * texcolor.rgb;\n");

  gen.addMainStatement("if (shadow_alpha != 1.0 && mydiffuse.a == 0.0 && accumShade < 0.999) {"
                            "mydiffuse.a = shadow_alpha;"
                            "color = vec3(clamp(color.r, 0.0, mydiffuse.r),"
                                         "clamp(color.g, 0.0, mydiffuse.g),"
                                         "clamp(color.b, 0.0, mydiffuse.b));}\n"
                      "else color = vec3(clamp(color.r, 0.0, 1.0),"
                                        "clamp(color.g, 0.0, 1.0),"
                                        "clamp(color.b, 0.0, 1.0));");

  int32_t fogType = this->getFog(state);

  switch (fogType) {
  default:
    assert(0 && "unknown fog type");
  case SoEnvironmentElement::NONE:
    // do nothing
    break;
  case SoEnvironmentElement::HAZE:
    gen.addMainStatement("float fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;\n");
    break;
  case SoEnvironmentElement::FOG:
    gen.addMainStatement("float fog = exp(-gl_Fog.density * gl_FogFragCoord);\n");
    break;
  case SoEnvironmentElement::SMOKE:
    gen.addMainStatement("float fogfrag =  gl_FogFragCoord;");
    gen.addMainStatement("float fogdens =  gl_Fog.density;");
    gen.addMainStatement("float fog = exp(-fogdens * fogdens * fogfrag * fogfrag);\n");
    break;
  }
  if (fogType != SoEnvironmentElement::NONE) {
    gen.addMainStatement("color = mix(gl_Fog.color.rgb, color, clamp(fog, 0.0, 1.0));\n");
  }

  gen.addMainStatement("gl_FragColor = vec4(color, mydiffuse.a);");

  gen.addDeclaration("uniform sampler2D textureMap0;\n", FALSE);
  gen.addDeclaration("uniform int coin_texunit0_model;\n", FALSE);
  gen.addDeclaration("uniform float shadow_alpha;\n", FALSE);
  gen.addDeclaration("uniform float spot_light_exp;\n", FALSE);
  if (this->numtexunitsinscene > 1) {
    gen.addDeclaration("uniform int coin_texunit1_model;\n", FALSE);
    gen.addDeclaration("uniform sampler2D textureMap1;\n", FALSE);
  }
  gen.addDeclaration("uniform int coin_light_model;\n", FALSE);
  if (!this->transparentShadow())
    gen.addDeclaration("uniform int coin_two_sided_lighting;\n", FALSE);

  if (dirspot) {
    gen.addNamedFunction("lights/DirSpotLight", FALSE);
  }

  this->fragmentshader->parameter.setNum(0);

  for (int i = 0; i < numshadowlights; i++) {
    SoShadowLightCache * cache = this->shadowlights[i];

    SoShaderParameter1i * shadowmap = this->shadowlights[i]->shadowmapid;
    SbString str;
    str.sprintf("shadowMap%d", i);
    if (shadowmap->name.getValue() != str) {
      shadowmap->name = str;
    }
    shadowmap->value = cache->texunit;
    this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), shadowmap);

    if (cache->hastransp && cache->alphamap) {
      SoShaderParameter1i * param = this->shadowlights[i]->alphamapid;
      SbString str;
      str.sprintf("alphaMap%d", i);
      if (param->name.getValue() != str) {
        param->name = str;
      }
      param->value = cache->texunit - 1;
      this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), param);

      param = this->shadowlights[i]->alphadepthmapid;
      str.sprintf("alphaDepthMap%d", i);
      if (param->name.getValue() != str) {
        param->name = str;
      }
      param->value = cache->texunit - 2;
      this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), param);
    }
  }

  for (i = 0; i < numshadowlights; i++) {
    SbString str;
    SoShaderParameter1f *farval = this->shadowlights[i]->fragment_farval;
    str.sprintf("farval%d", i);
    if (farval->name.getValue() != str) {
      farval->name = str;
    }
    this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), farval);
  }

  for (i = 0; i < numshadowlights; i++) {
    SbString str;
    SoShaderParameter1f *nearval = this->shadowlights[i]->fragment_nearval;
    str.sprintf("nearval%d", i);
    if (nearval->name.getValue() != str) {
      nearval->name = str;
    }
    this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), nearval);
  }
  SoShaderParameter1i * texmap =
    new SoShaderParameter1i();
  str.sprintf("textureMap0");
  texmap->name = str;
  texmap->value = 0;

  SoShaderParameter1i * texmap1 = NULL;

  if (!this->shadowalpha) {
    this->shadowalpha = new SoShaderParameter1f;
    this->shadowalpha->ref();
    this->shadowalpha->name = "shadow_alpha";
    this->shadowalpha->value = 1.0f;
  }

  if (!this->texunit0) {
    this->texunit0 = new SoShaderParameter1i;
    this->texunit0->ref();
    this->texunit0->name = "coin_texunit0_model";
    this->texunit0->value = 0;
  }

  if (this->numtexunitsinscene > 1) {
    if (!this->texunit1) {
      this->texunit1 = new SoShaderParameter1i;
      this->texunit1->ref();
      this->texunit1->name = "coin_texunit1_model";
      this->texunit1->value = 0;
    }
    texmap1 = new SoShaderParameter1i();
    str.sprintf("textureMap1");
    texmap1->name = str;
    texmap1->value = 1;
  }

  if (!this->lightmodel) {
    this->lightmodel = new SoShaderParameter1i;
    this->lightmodel->ref();
    this->lightmodel->name = "coin_light_model";
    this->lightmodel->value = 1;
  }
  this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), this->shadowalpha);
  this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), texmap);
  if (texmap1) this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), texmap1);
  this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), this->texunit0);
  if (this->numtexunitsinscene > 1) this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), this->texunit1);
  this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), this->lightmodel);

  if (!this->transparentShadow())
    this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), this->twosided);

  for (i = 0; i < numshadowlights; i++) {
    SoShadowLightCache * cache = this->shadowlights[i];

    if (cache->light->isOfType(SoDirectionalLight::getClassTypeId())) {
      SbString str;
      SoDirectionalLight * sl = static_cast<SoDirectionalLight*> (cache->light);
      SoSFFloat *maxShadowDistance = 0;
      if (cache->light->isOfType(SoShadowDirectionalLight::getClassTypeId()))
        maxShadowDistance = &static_cast<SoShadowDirectionalLight*>(cache->light)->maxShadowDistance;
      if (maxShadowDistance && maxShadowDistance->getValue() > 0.0f) {
        SoShaderParameter1f * maxdist = cache->maxshadowdistance;
        maxdist->value.connectFrom(maxShadowDistance);
        str.sprintf("maxshadowdistance%d", i);
        if (maxdist->name.getValue() != str) {
          maxdist->name = str;
        }
        SbString uniform;
        uniform.sprintf("uniform float %s;\n", str.getString());
        gen.addDeclaration(uniform, FALSE);
        this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), maxdist);
      }

      SoShaderParameter4f * lightplane = cache->fragment_lightplane;
      str.sprintf("lightplane%d", i);
      if (lightplane->name.getValue() != str) {
        lightplane->name = str;
      }
      this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), lightplane);
    }
    else if (cache->light->isOfType(SoSpotLight::getClassTypeId())) {
      SbString str;
      SoSpotLight * sl = static_cast<SoSpotLight*> (cache->light);
      if (sl->dropOffRate.getValue() >= 0.0) {
        SoShaderParameter1f * dropoffrate = cache->dropoffrate;
        dropoffrate->value.connectFrom(&sl->dropOffRate);
        str.sprintf("dropoffrate%d", cache->lightid);
        if (dropoffrate->name.getValue() != str) {
            dropoffrate->name = str;
        }
        SbString uniform;
        uniform.sprintf("uniform float %s;\n", str.getString());
        gen.addDeclaration(uniform, FALSE);
        this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), dropoffrate);

        SoShaderParameter1f * coscutoff = cache->coscutoff;
        str.sprintf("coscutoff%d", cache->lightid);
        if (coscutoff->name.getValue() != str) {
            coscutoff->name = str;
        }
        uniform.sprintf("uniform float %s;\n", str.getString());
        gen.addDeclaration(uniform, FALSE);
        this->fragmentshader->parameter.set1Value(this->fragmentshader->parameter.getNum(), coscutoff);
      }
    }
  }

  this->shadowlightsvalid = TRUE;
  // never update unless the program has actually changed. Creating a
  // new GLSL program is very slow on current drivers.
  if (this->fragmentshader->sourceProgram.getValue() != gen.getShaderProgram()) {
    // invalidate spotlights, and make sure the cameratransform variable is updated
    this->cameratransform->value.touch();
    this->fragmentshader->sourceProgram = gen.getShaderProgram();
    this->fragmentshader->sourceType = SoShaderObject::GLSL_PROGRAM;

#if 0 // for debugging
    fprintf(stderr,"new fragment program: %s\n",
            gen.getShaderProgram().getString());
#endif // debugging

  }

  this->fragmentshadercache->set(gen.getShaderProgram());
  state->pop();
  SoCacheElement::setInvalid(storedinvalid);
}

SoShaderProgram *
SoShadowLightCache::createVSMProgram()
{
  SoShaderProgram * program = new SoShaderProgram;

  SoVertexShader * vshader = new SoVertexShader;
  SoFragmentShader * fshader = new SoFragmentShader;

  program->shaderObject.set1Value(0, vshader);
  program->shaderObject.set1Value(1, fshader);

  SoShaderGenerator & vgen = this->vsm_vertex_generator;
  SoShaderGenerator & fgen = this->vsm_fragment_generator;

  vgen.reset(FALSE);
  vgen.setVersion("#version 120");

  SbBool dirlight = this->light->isOfType(SoDirectionalLight::getClassTypeId());

  vgen.addDeclaration("varying vec3 light_vec;", FALSE);
  vgen.addMainStatement("light_vec = (gl_ModelViewMatrix * gl_Vertex).xyz;\n"
                        "gl_Position = ftransform();\n");

  vshader->sourceProgram = vgen.getShaderProgram();
  vshader->sourceType = SoShaderObject::GLSL_PROGRAM;

  fgen.reset(FALSE);
  fgen.setVersion("#version 120");
#ifdef DISTRIBUTE_FACTOR
  SbString str;
  str.sprintf("const float DISTRIBUTE_FACTOR = %.1f;\n", DISTRIBUTE_FACTOR);
  fgen.addDeclaration(str, FALSE);
#endif
  fgen.addDeclaration("varying vec3 light_vec;", FALSE);
  fgen.addDeclaration("uniform float farval;", FALSE);
  fgen.addDeclaration("uniform float nearval;", FALSE);
  if (!dirlight)  {
    fgen.addMainStatement("float l = (length(light_vec) - nearval) / (farval-nearval);\n");
  }
  else {
    fgen.addMainStatement("float l = (-light_vec.z - nearval) / (farval-nearval);\n");
  }
  
  fgen.addMainStatement(
#ifdef DISTRIBUTE_FACTOR
                      "vec2 m = vec2(l, l*l);\n"
                      "vec2 f = fract(m * DISTRIBUTE_FACTOR);\n"

#ifdef USE_NEGATIVE
                      "gl_FragColor.rg = (m - (f / DISTRIBUTE_FACTOR)) * 2.0 - vec2(1.0, 1.0);\n"
                      "gl_FragColor.ba = f * 2.0 - vec2(1.0, 1.0);\n"
#else // USE_NEGATIVE
                      "gl_FragColor.rg = m - (f / DISTRIBUTE_FACTOR);\n"
                      "gl_FragColor.ba = f;\n"
#endif // ! USE_NEGATIVE
#else // DISTRIBUTE_FACTOR
#ifdef USE_NEGATIVE
                      "gl_FragColor = vec4(l*2.0 - 1.0, l*l*2.0 - 1.0, 0.0, 0.0);\n"
#else // USE_NEGATIVE
                      "gl_FragColor = vec4(l, l*l, 0.0, 0.0);\n"
#endif // !USE_NEGATIVE
#endif // !DISTRIBUTE_FACTOR
                      );

  fshader->sourceProgram = fgen.getShaderProgram();
  fshader->sourceType = SoShaderObject::GLSL_PROGRAM;

  if (!this->vsm_farval) {
    this->vsm_farval = new SoShaderParameter1f;
    this->vsm_farval->ref();
    this->vsm_farval->name = "farval";
  }

  if (!this->vsm_nearval) {
    this->vsm_nearval = new SoShaderParameter1f;
    this->vsm_nearval->ref();
    this->vsm_nearval->name = "nearval";
  }

  fshader->parameter = this->vsm_farval;
  fshader->parameter.set1Value(1, this->vsm_nearval);

  return program;
}

SoShaderProgram *
SoShadowLightCache::createAlphaProgram()
{
  SoShaderProgram * program = new SoShaderProgram;

  SoVertexShader * vshader = new SoVertexShader;
  SoFragmentShader * fshader = new SoFragmentShader;

  program->shaderObject.set1Value(0, vshader);
  program->shaderObject.set1Value(1, fshader);

  SoShaderGenerator & vgen = this->vsm_vertex_generator;
  SoShaderGenerator & fgen = this->vsm_fragment_generator;

  vgen.reset(FALSE);

  vgen.addMainStatement("gl_Position = ftransform();\n"
                        "gl_FrontColor = gl_Color;\n"
                        "gl_BackColor = gl_Color;");

  vshader->sourceProgram = vgen.getShaderProgram();
  vshader->sourceType = SoShaderObject::GLSL_PROGRAM;

  fgen.reset(FALSE);
  fgen.addMainStatement("gl_FragColor = gl_Color;");

  fshader->sourceProgram = fgen.getShaderProgram();
  fshader->sourceType = SoShaderObject::GLSL_PROGRAM;
  return program;
}

void
SoShadowGroupP::shader_enable_cb(void * closure,
                                 SoState * state,
                                 const SbBool enable)
{
  SoShadowGroupP * thisp = (SoShadowGroupP*) closure;

  const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));

  for (int i = 0; i < thisp->shadowlights.getLength(); i++) {
    SoShadowLightCache * cache = thisp->shadowlights[i];
    int unit = cache->texunit;
    if (unit == 0) {
      if (enable) glEnable(GL_TEXTURE_2D);
      else glDisable(GL_TEXTURE_2D);
    }
    else {
      cc_glglue_glActiveTexture(glue, (GLenum) (int(GL_TEXTURE0) + unit));
      if (enable) glEnable(GL_TEXTURE_2D);
      else glDisable(GL_TEXTURE_2D);

      if (cache->hastransp && cache->alphamap) {
        cc_glglue_glActiveTexture(glue, (GLenum) (int(GL_TEXTURE0) + unit - 1));
        if (enable) glEnable(GL_TEXTURE_2D);
        else glDisable(GL_TEXTURE_2D);
      }

      cc_glglue_glActiveTexture(glue, GL_TEXTURE0);

      GLenum glerror = sogl_glerror_debugging() ? glGetError() : GL_NO_ERROR;
      while (glerror) {
          SoDebugError::postWarning("SoShadowGroupP::shader_enable_cb",
              "glError() = %d\n", glerror);
          glerror = glGetError();
      }
    }
  }
  if (enable) {
    float alpha = 1.0f;
    if (SoShadowStyleElement::get(state) == SoShadowStyleElement::TRANSPARENT_SHADOWED)
      alpha = 1.0f - SoShadowTransparencyElement::get(state);
    if (thisp->shadowalpha && thisp->shadowalpha->value.getValue() != alpha)
      thisp->shadowalpha->value = alpha;

    // calling updateParameters here not only updates shadowalpha above, but
    // also updates any internal coin parameters
    thisp->vertexshader->updateParameters(state);
    thisp->fragmentshader->updateParameters(state);
  }
}

bool
SoShadowGroupP::supported(const cc_glglue * glue, SbString& reason)
{
  const bool supported =
    cc_glglue_glversion_matches_at_least(glue, 2, 0, 0) &&
    SoGLDriverDatabase::isSupported(glue, SO_GL_FRAMEBUFFER_OBJECT) &&
    SoGLDriverDatabase::isSupported(glue, "GL_ARB_texture_float");

  if (supported) { return true; }

  reason = "Unable to render shadows.";
  if (!SoGLDriverDatabase::isSupported(glue, SO_GL_FRAMEBUFFER_OBJECT)) reason += " Frame buffer objects not supported.";
  if (!cc_glglue_glversion_matches_at_least(glue, 2, 0, 0)) reason += " OpenGL version < 2.0.";
  if (!SoGLDriverDatabase::isSupported(glue, "GL_ARB_texture_float")) reason += " Floating point textures not supported.";

  return false;
}

void
SoShadowGroupP::GLRender(SoGLRenderAction * action, const SbBool inpath)
{
  SoState * state = action->getState();
  const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));

  // FIXME: should store results in a "context -> supported" map.  -mortene.
  SbString reason;
  const bool supported = SoShadowGroupP::supported(glue, reason);
  if (!supported && PUBLIC(this)->isActive.getValue()) {
    static bool first = true;
    if (first) {
      first = false;
      SoDebugError::postWarning("SoShadowGroupP::GLRender", "%s", reason.getString());
    }
  }

  if (!supported || !PUBLIC(this)->isActive.getValue()) {
    if (inpath) PUBLIC(this)->SoSeparator::GLRenderInPath(action);
    else PUBLIC(this)->SoSeparator::GLRenderBelowPath(action);
    return;
  }

  state->push();

  if (!this->vertexshadercache || !this->vertexshadercache->isValid(state)) {
    // a bit hackish, but saves creating yet another cache
    this->shadowlightsvalid = FALSE;
  }

  SbMatrix camtransform = SoViewingMatrixElement::get(state).inverse();
  if (camtransform != this->cameratransform->value.getValue()) {
    this->cameratransform->value = camtransform;
  }

  SoShadowStyleElement::set(state, PUBLIC(this), SoShadowStyleElement::CASTS_SHADOW_AND_SHADOWED);
  SoShapeStyleElement::setShadowMapRendering(state, TRUE);

  this->updateShadowLights(action);

  SoShapeStyleElement::setShadowMapRendering(state, FALSE);

  if (!this->vertexshadercache || !this->vertexshadercache->isValid(state)) {
    this->setVertexShader(state);
  }

  if (!this->fragmentshadercache || !this->fragmentshadercache->isValid(state)) {
    this->setFragmentShader(state);
  }
  this->shaderprogram->GLRender(action);

  SoShapeStyleElement::setShadowsRendering(state, TRUE);
  if (inpath) PUBLIC(this)->SoSeparator::GLRenderInPath(action);
  else PUBLIC(this)->SoSeparator::GLRenderBelowPath(action);
  SoShapeStyleElement::setShadowsRendering(state, FALSE);
  state->pop();
}

SoSeparator *
SoShadowLightCache::createGaussScene(SoShaderProgram * program, SoSceneTexture2 * tex)
{
  SoSeparator * sep = new SoSeparator;
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  SoShapeHints * sh = new SoShapeHints;

  const float verts[][3] = {
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f}

  };

  sh->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  sh->faceType = SoShapeHints::CONVEX;
  sh->shapeType = SoShapeHints::SOLID;

  sep->addChild(sh);

  camera->position = SbVec3f(0.5f, 0.5f, 2.0f);
  camera->height = 1.0f;
  camera->aspectRatio = 1.0f;
  camera->viewportMapping = SoCamera::LEAVE_ALONE;

  sep->addChild(camera);
  SoTextureUnit * unit = new SoTextureUnit;
  unit->unit = 0;
  sep->addChild(unit);

  sep->addChild(tex);
  sep->addChild(program);

  SoCoordinate3 * coord = new SoCoordinate3;
  sep->addChild(coord);

  coord->point.setValues(0,4,verts);

  if (!this->depthtest) {
    this->depthtest = new SoDepthBuffer;
    this->depthtest->ref();
    this->depthtest->test = FALSE;
    this->depthtest->write = FALSE;
  }
  sep->addChild(this->depthtest);

  SoFaceSet * fs = new SoFaceSet;
  fs->numVertices = 4;
  sep->addChild(fs);

  return sep;
}

void
SoShadowLightCache::shadowmap_glcallback(void * COIN_UNUSED_ARG(closure), SoAction * action)
{
  if (action->isOfType(SoGLRenderAction::getClassTypeId())) {

    ((SoGLRenderAction*)action)->resetTransparentShadowObject();

    SoState * state = action->getState();
    SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
    SoTextureQualityElement::set(state, 0.0f);
    SoNormalElement::set(state, NULL, 0, NULL, FALSE);

    SoOverrideElement::setNormalVectorOverride(state, NULL, TRUE);
    SoOverrideElement::setLightModelOverride(state, NULL, TRUE);
    SoTextureOverrideElement::setQualityOverride(state, TRUE);

    SoShapeStyleElement::setTransparentShadowMap(state, FALSE);
  }
}

void
SoShadowLightCache::shadowmap_post_glcallback(void * closure, SoAction * action)
{
  if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
    // for debugging the shadow map
    // reinterpret_cast<SoShadowLightCache*>(closure)->dumpBitmap("/home/pederb/Desktop/shadow.rgb");

    SoShadowLightCache *self = (SoShadowLightCache *)closure;
    self->hastransp = ((SoGLRenderAction*)action)->hasTransparentShadowObject();
  }
}

void
SoShadowLightCache::alphamap_glcallback(void * COIN_UNUSED_ARG(closure), SoAction * action)
{
  if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
    SoState * state = action->getState();
    SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
    // SoTextureQualityElement::set(state, 0.0f);
    SoNormalElement::set(state, NULL, 0, NULL, FALSE);

    SoDepthBufferElement::set(state, TRUE, FALSE, SoDepthBufferElement::LEQUAL, SbVec2f(0.,1.));

    SoOverrideElement::setNormalVectorOverride(state, NULL, TRUE);
    SoOverrideElement::setLightModelOverride(state, NULL, TRUE);
    // SoTextureOverrideElement::setQualityOverride(state, TRUE);

    SoShapeStyleElement::setTransparentShadowMap(state, TRUE);
  }
}

void
SoShadowLightCache::alphadepthmap_glcallback(void * COIN_UNUSED_ARG(closure), SoAction * action)
{
  if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
    SoState * state = action->getState();
    SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
    SoTextureQualityElement::set(state, 0.0f);
    SoNormalElement::set(state, NULL, 0, NULL, FALSE);

    SoOverrideElement::setNormalVectorOverride(state, NULL, TRUE);
    SoOverrideElement::setLightModelOverride(state, NULL, TRUE);
    SoTextureOverrideElement::setQualityOverride(state, TRUE);

    SoShapeStyleElement::setTransparentShadowMap(state, TRUE);
  }
}

#undef PUBLIC
#undef DISTRIBUTE_FACTOR
#undef USE_NEGATIVE

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoShadowGroup * node = new SoShadowGroup;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
// vim: noai:ts=2:sw=2
