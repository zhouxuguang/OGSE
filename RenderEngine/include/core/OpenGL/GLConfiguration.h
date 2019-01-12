/*
 * GLConfiguration.h
 *
 *  Created on: 2016-11-10
 *      Author: zhouxuguang
 */

#ifndef GLCONFIGURATION_H_
#define GLCONFIGURATION_H_

#include <OpenGL/gl.h>
#include <string>


class GLConfiguration
{
private:
	GLConfiguration();
	~GLConfiguration();

	void GatherGPUInfo();

public:
    
    static GLConfiguration* GetInstance();
    
    bool CheckForGLExtension(const std::string &searchName) const;
    
    /** Whether or not the GPU supports NPOT (Non Power Of Two) textures.
     OpenGL ES 2.0 already supports NPOT (iOS).
     *
     * @return Is true if supports NPOT.
     * @since v0.99.2
     */
	bool IsSupportsNPOT() const;

    /** Whether or not PVR Texture Compressed is supported.
     *
     * @return Is true if supports PVR Texture Compressed.
     */
	bool IsSupportsPVRTC() const;

    /** Whether or not ETC Texture Compressed is supported.
     *
     *
     * @return Is true if supports ETC Texture Compressed.
     */
    bool IsSupportsETC() const;

    /** Whether or not S3TC Texture Compressed is supported.
     *
     * @return Is true if supports S3TC Texture Compressed.
     */
    bool IsSupportsS3TC() const;

    /** Whether or not ATITC Texture Compressed is supported.
     *
     * @return Is true if supports ATITC Texture Compressed.
     */
    bool IsSupportsATITC() const;

	/** Whether or not 3DC Texture Compressed is supported.
     *
     * @return Is true if supports 3DC Texture Compressed.
     */
	bool IsSupports3DC() const;

    /** Whether or not BGRA8888 textures are supported.
     *
     * @return Is true if supports BGRA8888 textures.
     * @since v0.99.2
     */
	bool IsSupportsBGRA8888() const;

    /** Whether or not glDiscardFramebufferEXT is supported.
     * @return Is true if supports glDiscardFramebufferEXT.
     * @since v0.99.2
     */
	bool IsSupportsDiscardFramebuffer() const;

    /** Whether or not shareable VAOs are supported.
     *
     * @return Is true if supports shareable VAOs.
     * @since v2.0.0
     */
	bool IsSupportsShareableVAO() const;

    /** Whether or not OES_depth24 is supported.
     *
     * @return Is true if supports OES_depth24.
     * @since v2.0.0
     */
    bool IsSupportsOESDepth24() const;

    /** Whether or not OES_Packed_depth_stencil is supported.
     *
     * @return Is true if supports OES_Packed_depth_stencil.
     * @since v2.0.0
     */
    bool IsSupportsOESPackedDepthStencil() const;

    /** Whether or not glMapBuffer() is supported.
     *
     * On Desktop it returns `true`.
     * On Mobile it checks for the extension `GL_OES_mapbuffer`
     *
     * @return Whether or not `glMapBuffer()` is supported.
     * @since v3.13
     */
    bool IsSupportsMapBuffer() const;
    
    /** Whether or not GL_OES_standard_derivatives is supported.
     *
     * On Mobile it checks for the extension `GL_OES_standard_derivatives`
     *
     * @return Whether or not `GL_OES_standard_derivatives` is supported.
     * @since v1.1
     */
    bool IsSupportDerivative() const;
    
    bool IsSupportGeometryShader() const;


private:
    GLint           maxTextureSize;
    GLint           maxModelviewStackDepth;
    GLint           maxTextureUnits;
    bool            supportsPVRTC;
    bool            supportsETC1;
    bool            supportsS3TC;
    bool            supportsATITC;
	bool            supports3DC;
    bool            supportsNPOT;
    bool            supportsBGRA8888;
    bool            supportsDiscardFramebuffer;
    bool            supportsShareableVAO;
    bool            supportsOESMapBuffer;
    bool            supportsOESDepth24;
    bool            supportsOESPackedDepthStencil;
    bool            supportsDerivative;
    bool            supportsGeometryShader;
    bool            supportsAnisotropic;       //是否支持各项异性滤波
    
    char* glExtensions;     //gl扩展

};

#endif /* GLCONFIGURATION_H_ */
