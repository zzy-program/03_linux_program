#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>

typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#define HAL_MODULE_INFO_SYM_AS_STR  "HMI"
#define MAKE_TAG_CONSTANT(A,B,C,D) (((A) << 24) | ((B) << 16) | ((C) << 8) | (D))

// typedef signed long long   int64_t;
// typedef unsigned long long uint64_t;

// /libhardware/hardware.c
typedef struct hw_module_t {
    /** tag must be initialized to HARDWARE_MODULE_TAG */
    uint32_t tag;

    /**
     * The API version of the implemented module. The module owner is
     * responsible for updating the version when a module interface has
     * changed.
     *
     * The derived modules such as gralloc and audio own and manage this field.
     * The module user must interpret the version field to decide whether or
     * not to inter-operate with the supplied module implementation.
     * For example, SurfaceFlinger is responsible for making sure that
     * it knows how to manage different versions of the gralloc-module API,
     * and AudioFlinger must know how to do the same for audio-module API.
     *
     * The module API version should include a major and a minor component.
     * For example, version 1.0 could be represented as 0x0100. This format
     * implies that versions 0x0100-0x01ff are all API-compatible.
     *
     * In the future, libhardware will expose a hw_get_module_version()
     * (or equivalent) function that will take minimum/maximum supported
     * versions as arguments and would be able to reject modules with
     * versions outside of the supplied range.
     */
    uint16_t module_api_version;
#define version_major module_api_version
    /**                     
     * version_major/version_minor defines are supplied here for temporary
     * source code compatibility. They will be removed in the next version.
     * ALL clients must convert to the new version format.
     */

    /**
     * The API version of the HAL module interface. This is meant to
     * version the hw_module_t, hw_module_methods_t, and hw_device_t
     * structures and definitions.
     *
     * The HAL interface owns this field. Module users/implementations
     * must NOT rely on this value for version information.
     *
     * Presently, 0 is the only valid value.
     */
    uint16_t hal_api_version;
#define version_minor hal_api_version

    /** Identifier of module */
    const char *id;

    /** Name of this module */
    const char *name;

    /** Author/owner/implementor of the module */
    const char *author;

    /** Modules methods */
    struct hw_module_methods_t* methods;

    /** module's dso */
    void* dso;

#ifdef __LP64__
    uint64_t reserved[32-7];
#else
    /** padding to 128 bytes, reserved for future use */
    uint32_t reserved[32-7];
#endif

} hw_module_t;

static int load(const char *path,
        const struct hw_module_t **pHmi)
{
    int status = 0;
    void *handle;
    struct hw_module_t *hmi;

    handle = dlopen(path, RTLD_NOW);
    if (handle == NULL) {
        char const *err_str = dlerror();
        printf("load: module=%s\n%s", path, err_str?err_str:"unknown");
        status = -EINVAL;
        goto done;
    }

    /* Get the address of the struct hal_module_info. */
    const char *sym = HAL_MODULE_INFO_SYM_AS_STR;
    hmi = (struct hw_module_t *)dlsym(handle, sym);
    if (hmi == NULL) {
        printf("load: couldn't find symbol %s", sym);
        status = -EINVAL;
        goto done;
    }

    hmi->dso = handle;
    done:
    if (status != 0) {
        hmi = NULL;
        if (handle != NULL) {
            dlclose(handle);
            handle = NULL;
        }
    } else {
        printf("loaded HAL path=%s hmi=%p handle=%p\n",
                path, *pHmi, handle);
    }

    *pHmi = hmi;

	return status;
}

int main() {
	struct hw_module_t *hmi = NULL;
	int ret = 0;
#ifdef __LP64__
	printf("__LP64__ is defined\n");
#endif

	ret = load("/system/lib64/hw/camera.mt6753.so", (const struct hw_module_t **)&hmi);
	if(0 != ret) {
		printf("load so lib error, ret=%d\n", ret);
		return 0;	
	}
	
	// dump symbol info
	printf("hmi->tag=%p\n", hmi->tag);
	printf("HARDWARE_MODULE_TAG=%p\n", MAKE_TAG_CONSTANT('H', 'W', 'M', 'T'));
	printf("HARDWARE_DEVICE_TAG=%p\n", MAKE_TAG_CONSTANT('H', 'W', 'D', 'T'));
	printf("hmi->name=%s\n", hmi->name);

	return 0;	
}