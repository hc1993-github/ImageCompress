#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/log.h>
#include <malloc.h>
typedef uint8_t BYTE;
extern "C"{
 #include "jpeglib.h"
}

void writeImg(BYTE *data, const char *path, int w, int h) {
    struct jpeg_compress_struct jpeg_struct;
    jpeg_error_mgr err;
    jpeg_struct.err = jpeg_std_error(&err);
    jpeg_create_compress(&jpeg_struct);
    FILE *file = fopen(path,"wb");
    jpeg_stdio_dest(&jpeg_struct,file);
    jpeg_struct.image_width = w;
    jpeg_struct.image_height = h;
    jpeg_struct.input_components = 3;
    jpeg_struct.in_color_space = JCS_RGB;
    jpeg_struct.arith_code = FALSE;
    jpeg_set_defaults(&jpeg_struct);
    jpeg_set_quality(&jpeg_struct,30, true);
    jpeg_start_compress(&jpeg_struct,TRUE);
    JSAMPROW row_pointer[1];
    while (jpeg_struct.next_scanline<h){
        row_pointer[0] = &data[jpeg_struct.next_scanline * w * 3];
        jpeg_write_scanlines(&jpeg_struct,row_pointer,1);
    }
    jpeg_finish_compress(&jpeg_struct);
    fclose(file);
    jpeg_destroy_compress(&jpeg_struct);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hc_image_MainActivity_compress(JNIEnv *env, jobject thiz, jobject bitmap, jstring path_) {
    const char * path = env->GetStringUTFChars(path_,0);//java路径 转为 c路径
    AndroidBitmapInfo bitmapInfo;
    AndroidBitmap_getInfo(env,bitmap,&bitmapInfo);//获取图片信息
    int height = bitmapInfo.height;
    int width = bitmapInfo.width;
    BYTE *pixels;
    AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&pixels));//获取图片解码后内存指针
    BYTE *src,*dest;
    src = static_cast<BYTE *>(malloc(width * height * 3));
    dest = src;
    BYTE a,r,g,b;
    int color;
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            color = *((int*)pixels);
            a = (color & 0xFF000000) >>16;
            r = (color & 0x00FF0000) >>16;
            g = (color & 0x0000FF00) >>8;
            b = (color & 0x000000FF);
            *src = b;
            *(src+1) = g;
            *(src+2) = r;
            src +=3;
            pixels+=4;
        }
    }
    writeImg(dest,path,width,height);
    AndroidBitmap_unlockPixels(env,bitmap);
    env->ReleaseStringUTFChars(path_,path);
}

