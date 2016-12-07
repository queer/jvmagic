/*#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <jvmagic.h> // generated by javah via maven-native-plugin

JNIEXPORT void JNICALL Java_date_willnot_amy_jvmagic_JVMagic_jniCall (JNIEnv * env, jobject obj) {
    puts("Hello from C!");
}*/

#include <stdlib.h>
#include <string.h>
#include <jvmagic.h> // generated by javah via maven-native-plugin
#include "jvmti.h"

jvmtiEnv *jvmti;

JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM *jvm, char *options, void *reserved) {
    fprintf(stderr, "Getting JVMTI env...\n");
    (*jvm)->GetEnv(jvm, &jvmti, JVMTI_VERSION_1_0);

    fprintf(stderr, "Getting desired capabilities...\n");
    jvmtiCapabilities capa;
    jvmtiError err = (*jvmti)->GetPotentialCapabilities(jvmti, &capa);
    if (err == JVMTI_ERROR_NONE) {
        err = (*jvmti)->AddCapabilities(jvmti, &capa);
        if(err == JVMTI_ERROR_NONE) {
            fprintf(stderr, "Succeeded!\n");
        } else {
            fprintf(stderr, "Failed to add capabilities, bailing out!\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Couldn't get capabilities, bailing out!\n");
        return 1;
    }

    fprintf(stderr, "Grabbing classes...\n");
    jint classCount;
    jclass *classes;
    (*jvmti)->GetLoadedClasses(jvmti, &classCount, &classes);

    for(int i = 0; i < classCount; i++) {
        jclass cls = *(classes + i);
        char *signature;
        char *generic;
        err = (*jvmti)->GetClassSignature(jvmti, cls, &signature, &generic);
        if(err == JVMTI_ERROR_NONE) {
            // Just be simple and look for our classes...
            if(strstr(signature, "date/willnot/amy") != NULL) {
                fprintf(stderr, "Signature: %s ( Generic: %s )\n", signature, generic);
            }
        } else {
            fprintf(stderr, "Couldn't check class :(\n");
        }
    }

    fprintf(stderr, "Loaded %d classes.\n", classCount);
    return 0;
}