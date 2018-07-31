//
// Created by zzh on 2018/7/27 0002.
//

#include <jni.h>
#include <android/native_window_jni.h>
#include <log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Square.h"
#include "Triangle.h"
#include "Circle.h"
#include "Texture.h"
#include "FboRenderer.h"
#include "GLCamera.h"
#include "Beauty.h"

Triangle *triangle = nullptr;
Circle *circle = nullptr;
Square *square = nullptr;
Texture *texture = nullptr;
FboRenderer *fboRenderer = nullptr;
GLCamera *glCamera = nullptr;
Beauty *beauty = nullptr;

// --- JniTriangleActivity

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_JniTriangleActivity__1init(JNIEnv *env, jclass type) {
    if (triangle) {
        delete triangle;
    }
    triangle = new Triangle();
    triangle->init();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_JniTriangleActivity__1resize(JNIEnv *env, jclass type,
                                                                      jint width, jint height) {
    if (!triangle) {
        LOGE("triangle does not initialized");
        return;
    }
    triangle->resize(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_JniTriangleActivity__1draw(JNIEnv *env, jclass type) {
    if (!triangle) {
        LOGE("triangle does not initialized");
        return;
    }
    triangle->draw();
}


// --- EGLCircleActivity

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_EGLCircleActivity__1init(JNIEnv *env, jclass type,
                                                                  jobject surface, jint width,
                                                                  jint height) {
    if (circle) {
        circle->stop();
        delete circle;
    }
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    circle = new Circle(window);
    circle->resize(width, height);
    circle->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_EGLCircleActivity__1draw(JNIEnv *env, jclass type) {
    if (!circle) {
        LOGE("draw error, circle is null");
        return;
    }
    circle->draw();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_EGLCircleActivity__1release(JNIEnv *env, jclass type) {
    if (circle) {
        circle->stop();
        delete circle;
        circle = nullptr;
    }
}


// --- VaoVboActivity

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_VaoVboActivity__1init(JNIEnv *env, jclass type,
                                                               jobject surface, jint width,
                                                               jint height) {
    if (square) {
        square->stop();
        delete square;
    }
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    square = new Square(window);
    square->resize(width, height);
    square->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_VaoVboActivity__1draw(JNIEnv *env, jclass type) {
    if (!square) {
        LOGE("draw error, square is null");
        return;
    }
    square->draw();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_VaoVboActivity__1release(JNIEnv *env, jclass type) {
    if (square) {
        square->stop();
        delete square;
        square = nullptr;
    }
}


// --- MatrixTransformActivity

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_MatrixTransformActivity__1init(JNIEnv *env, jclass type,
                                                                        jobject surface, jint width,
                                                                        jint height) {
    if (square) {
        square->stop();
        delete square;
    }
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    square = new Square(window);
    square->resize(width, height);
    square->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_MatrixTransformActivity__1draw(JNIEnv *env, jclass type,
                                                                        jfloatArray matrix_) {
    if (!square) {
        LOGE("draw error, square is null");
        return;
    }

    jfloat *matrix = env->GetFloatArrayElements(matrix_, NULL);

    square->setMatrix(matrix);
    square->draw();

    env->ReleaseFloatArrayElements(matrix_, matrix, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_MatrixTransformActivity__1release(JNIEnv *env,
                                                                           jclass type) {
    if (square) {
        square->stop();
        delete square;
        square = nullptr;
    }
}


// --- TextureActivity

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_TextureActivity__1init(JNIEnv *env, jclass type,
                                                                jobject surface, jint texWidth,
                                                                jint texHeight, jbyteArray pixel_,
                                                                jint pixelDataLen,
                                                                jobject assetManager) {
    jbyte *pixel = env->GetByteArrayElements(pixel_, NULL);

    if (texture) {
        texture->stop();
        delete texture;
    }
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    AAssetManager *manager = AAssetManager_fromJava(env, assetManager);
    texture = new Texture(window);
    texture->setTexWidth(texWidth);
    texture->setTexHeight(texHeight);
    texture->setPixel((uint8_t *) pixel, (size_t) pixelDataLen);
    texture->setAssetManager(manager);
    texture->resize(texWidth, texHeight);
    texture->start();

    env->ReleaseByteArrayElements(pixel_, pixel, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_TextureActivity__1resize(JNIEnv *env, jclass type,
                                                                  jint width, jint height) {
    if (!texture) {
        LOGE("resize error, texture is null");
        return;
    }
    texture->resize(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_TextureActivity__1draw(JNIEnv *env, jclass type,
                                                                jfloatArray matrix_,
                                                                jint filterType,
                                                                jfloatArray filterColor_) {
    if (!texture) {
        LOGE("draw error, texture is null");
        return;
    }
    jfloat *matrix = env->GetFloatArrayElements(matrix_, NULL);
    jfloat *filterColor = env->GetFloatArrayElements(filterColor_, NULL);

    texture->setMatrix(matrix);
    texture->setFilterType(filterType);
    texture->setFilterColor(filterColor);
    texture->draw();

    env->ReleaseFloatArrayElements(matrix_, matrix, 0);
    env->ReleaseFloatArrayElements(filterColor_, filterColor, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_TextureActivity__1release(JNIEnv *env, jclass type) {
    if (texture) {
        texture->stop();
        delete texture;
        texture = nullptr;
    }
}


// --- FboActivity

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_FboActivity__1init(JNIEnv *env, jclass type,
                                                            jobject surface, jint width,
                                                            jint height) {
    if (fboRenderer) {
        fboRenderer->stop();
        delete fboRenderer;
    }
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    fboRenderer = new FboRenderer(window);
    fboRenderer->resize(width, height);
    fboRenderer->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_FboActivity__1draw(JNIEnv *env, jclass type) {
    if (!fboRenderer) {
        LOGE("draw error, fboRenderer is null");
        return;
    }
    fboRenderer->draw();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_FboActivity__1release(JNIEnv *env, jclass type) {
    if (fboRenderer) {
        fboRenderer->stop();
        delete fboRenderer;
        fboRenderer = nullptr;
    }
}


// --- GLCameraActivity

extern "C"
JNIEXPORT jint JNICALL
Java_com_steven_avgraphics_activity_gles_GLCameraActivity__1init(JNIEnv *env, jclass type,
                                                                 jobject surface, jint width,
                                                                 jint height,
                                                                 jobject assetManager) {
    if (glCamera) {
        glCamera->stop();
        delete glCamera;
    }
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    AAssetManager *manager = AAssetManager_fromJava(env, assetManager);
    glCamera = new GLCamera(window);
    glCamera->setAssetManager(manager);
    glCamera->resize(width, height);

    return glCamera->init();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_GLCameraActivity__1draw(JNIEnv *env, jclass type,
                                                                 jfloatArray matrix_) {
    jfloat *matrix = env->GetFloatArrayElements(matrix_, NULL);

    if (!glCamera) {
        LOGE("draw error, fboRenderer is null");
        return;
    }
    glCamera->draw(matrix);

    env->ReleaseFloatArrayElements(matrix_, matrix, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_activity_gles_GLCameraActivity__1release(JNIEnv *env, jclass type) {
    if (glCamera) {
        glCamera->stop();
        delete glCamera;
        glCamera = nullptr;
    }
}


// --- CameraPreviewView

extern "C"
JNIEXPORT jint JNICALL
Java_com_steven_avgraphics_view_CameraPreviewView__1init(JNIEnv *env, jclass type, jobject surface,
                                                         jint width, jint height, jobject manager) {
    if (beauty) {
        beauty->stop();
        delete beauty;
    }
    beauty = new Beauty();
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    AAssetManager *assetManager = AAssetManager_fromJava(env, manager);
    int textureId = beauty->init(assetManager, window, width, height);

    return textureId;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_view_CameraPreviewView__1draw(JNIEnv *env, jclass type,
                                                         jfloatArray matrix_, jfloat beautyLevel,
                                                         jfloat tone, jfloat bright,
                                                         jboolean recording) {
    if (!beauty) {
        LOGE("draw error, beauty is null");
        return;
    }
    jfloat *matrix = env->GetFloatArrayElements(matrix_, NULL);
    beauty->draw(matrix, beautyLevel, tone, bright, recording);
    env->ReleaseFloatArrayElements(matrix_, matrix, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_steven_avgraphics_view_CameraPreviewView__1stop(JNIEnv *env, jclass type) {
    if (beauty) {
        beauty->stop();
        delete beauty;
        beauty = nullptr;
    }
}