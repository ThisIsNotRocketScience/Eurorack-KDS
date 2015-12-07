#ifdef __cplusplus
extern "C"{
#endif

extern void GoldfishInit();
extern void GoldfishProcess(int32_t *inputsamples, int32_t * outputsamples, int32_t frames);
extern void GoldfishOctavePressed();

#ifdef __cplusplus
}
#endif
