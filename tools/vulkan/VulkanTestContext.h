
/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef VulkanTestContext_DEFINED
#define VulkanTestContext_DEFINED

#ifdef SK_VULKAN

#include "GrTypes.h"
#include "vk/GrVkBackendContext.h"

class SkSurface;
class GrContext;

class VulkanTestContext {
public:
    ~VulkanTestContext();

    // each platform will have to implement these in its CPP file
    static VkSurfaceKHR createVkSurface(VkInstance, void* platformData);
    static bool canPresent(VkInstance, VkPhysicalDevice, uint32_t queueFamilyIndex);

    static VulkanTestContext* Create(void* platformData, int msaaSampleCount) {
        VulkanTestContext* ctx = new VulkanTestContext(platformData, msaaSampleCount);
        if (!ctx->isValid()) {
            delete ctx;
            return nullptr;
        }
        return ctx;
    }

    SkSurface* getBackbufferSurface();
    void swapBuffers();

    bool makeCurrent() { return true; }
    int getStencilBits() { return 0;  }
    int getSampleCount() { return 0; }

    bool isValid() { return SkToBool(fBackendContext.get()); }

    void resize(uint32_t w, uint32_t h) {
        this->createSwapchain(w, h); 
    }

    GrBackendContext getBackendContext() { return (GrBackendContext)fBackendContext.get(); }

private:
    VulkanTestContext();
    VulkanTestContext(void*, int msaaSampleCount);
    void initializeContext(void*);
    void destroyContext();

    struct BackbufferInfo {
        uint32_t        fImageIndex;          // image this is associated with
        VkSemaphore     fAcquireSemaphore;    // we signal on this for acquisition of image
        VkSemaphore     fRenderSemaphore;     // we wait on this for rendering to be done
        VkCommandBuffer fTransitionCmdBuffers[2]; // to transition layout between present and render
        VkFence         fUsageFences[2];      // used to ensure this data is no longer used on GPU
    };

    BackbufferInfo* getAvailableBackbuffer();
    bool createSwapchain(uint32_t width, uint32_t height);
    void createBuffers(VkFormat format);
    void destroyBuffers();

    SkAutoTUnref<const GrVkBackendContext> fBackendContext;

    // simple wrapper class that exists only to initialize a pointer to NULL
    template <typename FNPTR_TYPE> class VkPtr {
    public:
        VkPtr() : fPtr(NULL) {}
        VkPtr operator=(FNPTR_TYPE ptr) { fPtr = ptr; return *this; }
        operator FNPTR_TYPE() const { return fPtr; }
    private:
        FNPTR_TYPE fPtr;
    };
    
    // WSI interface functions
    VkPtr<PFN_vkDestroySurfaceKHR> fDestroySurfaceKHR;
    VkPtr<PFN_vkGetPhysicalDeviceSurfaceSupportKHR> fGetPhysicalDeviceSurfaceSupportKHR;
    VkPtr<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR> fGetPhysicalDeviceSurfaceCapabilitiesKHR;
    VkPtr<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR> fGetPhysicalDeviceSurfaceFormatsKHR;
    VkPtr<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR> fGetPhysicalDeviceSurfacePresentModesKHR;

    VkPtr<PFN_vkCreateSwapchainKHR> fCreateSwapchainKHR;
    VkPtr<PFN_vkDestroySwapchainKHR> fDestroySwapchainKHR;
    VkPtr<PFN_vkGetSwapchainImagesKHR> fGetSwapchainImagesKHR;
    VkPtr<PFN_vkAcquireNextImageKHR> fAcquireNextImageKHR;
    VkPtr<PFN_vkQueuePresentKHR> fQueuePresentKHR;
    VkPtr<PFN_vkCreateSharedSwapchainsKHR> fCreateSharedSwapchainsKHR;

    GrContext*        fContext;
    VkSurfaceKHR      fSurface;
    VkSwapchainKHR    fSwapchain;
    uint32_t          fPresentQueueIndex;
    VkQueue           fPresentQueue;
    int               fWidth;
    int               fHeight;
    GrPixelConfig     fPixelConfig;

    uint32_t          fImageCount;
    VkImage*          fImages;         // images in the swapchain
    VkImageLayout*    fImageLayouts;   // layouts of these images when not color attachment
    sk_sp<SkSurface>* fSurfaces;       // wrapped surface for those images
    VkCommandPool     fCommandPool;
    BackbufferInfo*   fBackbuffers;
    uint32_t          fCurrentBackbufferIndex;
};

#endif // SK_VULKAN

#endif
