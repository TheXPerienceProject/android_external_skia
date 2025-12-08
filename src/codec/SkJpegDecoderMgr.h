/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
// QTI_BEGIN: 2025-04-28: Performance: Perf: Add QC support for jpeg decode multithread.
 * Changes from Qualcomm Technologies, Inc. are provided under the following license:
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
// QTI_END: 2025-04-28: Performance: Perf: Add QC support for jpeg decode multithread.
 */

#ifndef SkJpegDecoderMgr_DEFINED
#define SkJpegDecoderMgr_DEFINED

#include "include/codec/SkCodec.h"
#include "include/private/SkEncodedInfo.h"
#include "include/private/base/SkNoncopyable.h"
#include "src/codec/SkJpegPriv.h"
#include "src/codec/SkJpegSourceMgr.h"

// QTI_BEGIN: 2025-04-28: Performance: Perf: Add QC support for jpeg decode multithread.
#ifdef QC_JPEG_MT
#include <dlfcn.h>
#include <pthread.h>
struct qcJpegDecoder_Interface {
    void *mQcJpegInterfaceHandler = nullptr;
    void* (*mQcJpegInit)(jpeg_decompress_struct* cinfo, size_t src_size) = nullptr;
    JDIMENSION (*mQcJpeg_read_scanlines)(void* handler, j_decompress_ptr cinfo,
                                         JSAMPARRAY scanlines,
                                         JDIMENSION max_lines) = nullptr;
    void (*mQcJpegDestroy)(void* handler) = nullptr;
    bool (*mQcIsJpegEnable)() = nullptr;
    bool mAllSymbolsFound = false;
    pthread_once_t mInitControl = PTHREAD_ONCE_INIT;
};
extern qcJpegDecoder_Interface QCJPEG_DECODER;
#endif

// QTI_END: 2025-04-28: Performance: Perf: Add QC support for jpeg decode multithread.
extern "C" {
    #include "jpeglib.h"  // NO_G3_REWRITE
}

#include <memory>

class SkStream;

class JpegDecoderMgr : SkNoncopyable {
public:

    /*
     * Print a useful error message and return false
     */
    bool returnFalse(const char caller[]);

    /*
     * Print a useful error message and return a decode failure
     */
    SkCodec::Result returnFailure(const char caller[], SkCodec::Result result);

    /*
     * Create the decode manager
     * Does not take ownership of stream
     */
    JpegDecoderMgr(SkStream* stream);

    /*
     * Initialize decompress struct
     * Initialize the source manager
     */
    void  init();

    /*
     * Returns true if it successfully sets outColor to the encoded color,
     * and false otherwise.
     */
    bool getEncodedColor(SkEncodedInfo::Color* outColor);

    /*
     * Free memory used by the decode manager
     */
    ~JpegDecoderMgr();

    /*
     * Get the skjpeg_error_mgr in order to set an error return jmp_buf
     */
    skjpeg_error_mgr* errorMgr() { return &fErrorMgr; }

    /*
     * Get function for the decompress info struct
     */
    jpeg_decompress_struct* dinfo() { return &fDInfo; }

    // Get the source manager.
    SkJpegSourceMgr* getSourceMgr();

// QTI_BEGIN: 2025-04-28: Performance: Perf: Add QC support for jpeg decode multithread.
#ifdef QC_JPEG_MT
    void* mQcJpeghandler = nullptr;
#endif

// QTI_END: 2025-04-28: Performance: Perf: Add QC support for jpeg decode multithread.
private:
    // Wrapper that calls into the full SkJpegSourceMgr interface.
    struct SourceMgr : jpeg_source_mgr {
        static void InitSource(j_decompress_ptr dinfo);
        static void SkipInputData(j_decompress_ptr dinfo, long num_bytes_long);
        static boolean FillInputBuffer(j_decompress_ptr dinfo);
        static void TermSource(j_decompress_ptr dinfo);

        SourceMgr(std::unique_ptr<SkJpegSourceMgr> mgr);
        std::unique_ptr<SkJpegSourceMgr> fSourceMgr;
    };

    jpeg_decompress_struct fDInfo;
    SourceMgr              fSrcMgr;
    skjpeg_error_mgr       fErrorMgr;
    jpeg_progress_mgr      fProgressMgr;
    bool                   fInit;
};

#endif
