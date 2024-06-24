#ifndef KSE_PP_TASK_3_LIBRARY_H
#define KSE_PP_TASK_3_LIBRARY_H

extern "C" {
    __declspec(dllexport) char *encrypt(char *message, int key);
    __declspec(dllexport) char *decrypt(char *message, int key);
}

#endif //KSE_PP_TASK_3_LIBRARY_H
