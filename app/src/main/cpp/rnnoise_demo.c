//
// Created by Betul on 4/26/2019.
//


#include <stdlib.h>
#include <assert.h>
#include <jni.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>// for log message
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include <math.h>
#include <stdio.h>
#include <zconf.h>
#include "rnnoise.h"
#define LOG_TAG "TESTTTTTTT"
#define FRAME_SIZE 480



JNIEXPORT jboolean JNICALL
Java_com_example_myapplication_MainActivity_rnnoise_1demo(JNIEnv *env, jclass type,jobject inputF, jstring inputFName) {
    ///Bizim Algoritma Karesi => "Exit Point => NOISY : 4059.437211 ||| CLEAR : 3909.596673 ||| PERCENTAGE : 96.308835"
    ///Bizim Algoritma        => "Exit Point => NOISY : 4501.080858 ||| CLEAR : 4060.160638 ||| PERCENTAGE : 90.204126"
    ///Audacity Algoritması   => "Exit Point => NOISY : 17478.195631 ||| CLEAR : 12282.904722 ||| PERCENTAGE : 70.275588"


    const char *filename = (*env)->GetStringUTFChars(env, inputFName, NULL);
    char inputFileExt[] = "output.pcm";
    char outputFileExt[] = "new_output.pcm";
    char *inputFile = malloc(sizeof(char) * 1024);
    char *outputFile = malloc(sizeof(char) * 1024);
    assert(NULL != filename);

    memcpy(inputFile, filename, strlen(filename)+1);
    strcat(inputFile, inputFileExt);

    memcpy(outputFile, filename, strlen(filename)+1);
    strcat(outputFile, outputFileExt);

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Entry Point %s", inputFile);
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Entry Point %s", outputFile);


    double SUM_NOISY_SOUND = 0;
    double SUM_CLEAR_SOUND = 0;
    double COUNTER = 0;


    int i;
    int first = 1;
    float x[FRAME_SIZE];
    FILE *f1, *fout;
    DenoiseState *st;
    st = rnnoise_create();
    f1 = fopen(inputFile, "r");
    fout = fopen(outputFile, "w");

    while (1) {
        short tmp[FRAME_SIZE];
        //__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "1.While");
        fread(tmp, sizeof(short), FRAME_SIZE, f1);
        //__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "2.While");
        if (feof(f1)) break;
        for (i = 0; i < FRAME_SIZE; i++){
            x[i] = tmp[i];
            SUM_NOISY_SOUND += tmp[i]*tmp[i];
            COUNTER++;
        }
        rnnoise_process_frame(st, x, x);
        for (i = 0; i < FRAME_SIZE; i++){
            //SUM_CLEAR_SOUND += (tmp[i]-x[i]) * (tmp[i]-x[i]);
            SUM_CLEAR_SOUND += x[i]*x[i];
            tmp[i] = x[i];
        }
        if (!first) fwrite(tmp, sizeof(short), FRAME_SIZE, fout);
        first = 0;
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "End While");
    rnnoise_destroy(st);
    fclose(f1);
    fclose(fout);


    SUM_NOISY_SOUND = sqrt(SUM_NOISY_SOUND/COUNTER);
    SUM_CLEAR_SOUND = sqrt(SUM_CLEAR_SOUND/COUNTER);
    double PERCENTAGE = (SUM_CLEAR_SOUND/SUM_NOISY_SOUND)*100;

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Exit Point => NOISY : %lf ||| CLEAR : %lf ||| PERCENTAGE : %lf", SUM_NOISY_SOUND, SUM_CLEAR_SOUND, PERCENTAGE);


}

/*
 const char *filename = (*env)->GetStringUTFChars(env, inputFName, NULL);
    assert(NULL != filename);

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Girdiii Olummm");

    int i;
    int first = 1;
    float x[FRAME_SIZE];
    FILE *f1, *fout;
    DenoiseState *st;
    st = rnnoise_create();



    //f1 = fopen(inputFName, "r");
    fout = fopen("output.pcm", "w");

    while (1) {
        short *tmp = (short*) malloc(FRAME_SIZE);
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "1. kontrol noktası : %s",filename);

        f1 = fopen(filename, "r");
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "f1 : %s",f1);
        if (f1 != NULL)
        {
            __android_log_write(ANDROID_LOG_DEBUG, LOG_TAG, "fopen()");

            //fread(tmp, 1, FRAME_SIZE, f1);
        }

        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "2. kontrol noktası");
        if (feof(f1)) break;
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "3. kontrol noktası");
        for (i = 0; i < FRAME_SIZE; i++) x[i] = tmp[i];
        rnnoise_process_frame(st, x, x);
        for (i = 0; i < FRAME_SIZE; i++) tmp[i] = x[i];
        if (!first) fwrite(tmp, 1, FRAME_SIZE, fout);
        first = 0;
    }
    rnnoise_destroy(st);
    fclose(f1);
    fclose(fout);

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Çıktıııı Olummm");
 */


/*
  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Girdiii Olummm");


    int i;
    int first = 1;
    float x[FRAME_SIZE];
    FILE *f1, *fout;
    DenoiseState *st;
    st = rnnoise_create();
    f1 = fopen("input.pcm", "r");
    fout = fopen("output.pcm", "w");
    while (1) {
        short tmp[FRAME_SIZE];
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "şık şık  Olummm");
        fread(tmp, sizeof(short), FRAME_SIZE, f1);
        if (feof(f1)) break;
        for (i = 0; i < FRAME_SIZE; i++) x[i] = tmp[i];
        rnnoise_process_frame(st, x, x);
        for (i = 0; i < FRAME_SIZE; i++) tmp[i] = x[i];
        if (!first) fwrite(tmp, sizeof(short), FRAME_SIZE, fout);
        first = 0;
    }
    rnnoise_destroy(st);
    fclose(f1);
    fclose(fout);

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Çıktıııı Olummm");
 */






/*


 JNIEXPORT void JNICALL
Java_com_example_myapplication_MainActivity_rnnoise_1demo(JNIEnv *env, jclass type,jobject assetManager, jstring filename) {





    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Girdiii Olummm");

    SLresult result;

    // convert Java string to UTF-8
    const char *utf8 = (*env)->GetStringUTFChars(env, inputFName, NULL);
    assert(NULL != utf8);

    int i;
    int first = 1;
    float x[FRAME_SIZE];
    FILE *f1, *fout;
    DenoiseState *st;
    st = rnnoise_create();
    f1 = fopen("input.pcm", "r");
    fout = fopen("output.pcm", "w");

    // use asset manager to open asset by filename
    AAssetManager* inputMgr = AAssetManager_fromJava(env, inputF);
    assert(NULL != inputMgr);
    AAsset* inputAsset = AAssetManager_open(inputMgr, utf8, AASSET_MODE_UNKNOWN);



    // release the Java string and UTF-8
    (*env)->ReleaseStringUTFChars(env, inputFName, utf8);

    // the asset might not be found
    if (NULL == inputAsset) {
        return JNI_FALSE;
    }

    while (1) {
        short tmp[FRAME_SIZE];
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "şık şık  Olummm");
        fread(tmp, sizeof(short), FRAME_SIZE, f1);
        if (feof(f1)) break;
        for (i = 0; i < FRAME_SIZE; i++) x[i] = tmp[i];
        rnnoise_process_frame(st, x, x);
        for (i = 0; i < FRAME_SIZE; i++) tmp[i] = x[i];
        if (!first) fwrite(tmp, sizeof(short), FRAME_SIZE, fout);
        first = 0;
    }
    rnnoise_destroy(st);
    fclose(f1);
    fclose(fout);

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Çıktıııı Olummm");






}




 */