/*
 * Copyright 2020 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrThreadSafeUniquelyKeyedProxyViewCache_DEFINED
#define GrThreadSafeUniquelyKeyedProxyViewCache_DEFINED

#include "include/private/SkSpinlock.h"
#include "src/core/SkArenaAlloc.h"
#include "src/core/SkTDynamicHash.h"
#include "src/gpu/GrSurfaceProxyView.h"

// Ganesh creates a lot of utility textures (e.g., blurred-rrect masks) that need to be shared
// between the direct context and all the DDL recording contexts. This thread-safe cache
// allows this sharing.
//
// In operation, each thread will first check if the threaded cache possesses the required texture.
//
// If a DDL thread doesn't find a needed texture it will go off and create it on the cpu and then
// attempt to add it to the cache. If another thread had added it in the interim, the losing thread
// will discard its work and use the texture the winning thread had created.
//
// If the thread in possession of the direct context doesn't find the needed texture it should
// add a place holder view and then queue up the draw calls to complete it. In this way the
// gpu-thread has precedence over the recording threads.
//
// The invariants for this cache differ a bit from those of the proxy and resource caches.
// For this cache:
//
//   only this cache knows the unique key - neither the proxy nor backing resource should
//              be discoverable in any other cache by the unique key
//   if a backing resource resides in the resource cache then there should be an entry in this
//              cache
//   an entry in this cache, however, doesn't guarantee that there is a corresponding entry in
//              the resource cache - although the entry here should be able to generate that entry
//              (i.e., be a lazy proxy)
class GrThreadSafeUniquelyKeyedProxyViewCache {
public:
    GrThreadSafeUniquelyKeyedProxyViewCache();
    ~GrThreadSafeUniquelyKeyedProxyViewCache();

#if GR_TEST_UTILS
    int numEntries() const  SK_EXCLUDES(fSpinLock);
    int count() const  SK_EXCLUDES(fSpinLock);
#endif

    void dropAllRefs()  SK_EXCLUDES(fSpinLock);
    void dropAllUniqueRefs()  SK_EXCLUDES(fSpinLock);

    GrSurfaceProxyView find(const GrUniqueKey&)  SK_EXCLUDES(fSpinLock);

    GrSurfaceProxyView add(const GrUniqueKey&, const GrSurfaceProxyView&)  SK_EXCLUDES(fSpinLock);

private:
    struct Entry {
        Entry(const GrUniqueKey& key, const GrSurfaceProxyView& view) : fKey(key), fView(view) {}

        // Note: the unique key is stored here bc it is never attached to a proxy or a GrTexture
        GrUniqueKey        fKey;
        GrSurfaceProxyView fView;
        Entry*             fNext = nullptr;

        // for SkTDynamicHash
        static const GrUniqueKey& GetKey(const Entry& e) { return e.fKey; }
        static uint32_t Hash(const GrUniqueKey& key) { return key.hash(); }
    };

    Entry* getEntry(const GrUniqueKey& key, const GrSurfaceProxyView& view) {
        Entry* newEntry;
        if (fFreeEntryList) {
            newEntry = fFreeEntryList;
            fFreeEntryList = newEntry->fNext;
            newEntry->fNext = nullptr;

            newEntry->fKey = key;
            newEntry->fView = view;
        } else {
            newEntry = fEntryAllocator.make<Entry>(key, view);
        }

        return newEntry;
    }

    void recycleEntry(Entry* dead) {
        dead->fKey.reset();
        dead->fView.reset();
        dead->fNext = fFreeEntryList;
        fFreeEntryList = dead;
    }

    GrSurfaceProxyView internalAdd(const GrUniqueKey&,
                                   const GrSurfaceProxyView&)  SK_REQUIRES(fSpinLock);

    mutable SkSpinlock fSpinLock;

    SkTDynamicHash<Entry, GrUniqueKey> fUniquelyKeyedProxyViews  SK_GUARDED_BY(fSpinLock);

    // TODO: empirically determine this from the skps
    static const int kInitialArenaSize = 64 * sizeof(Entry);

    char                         fStorage[kInitialArenaSize];
    SkArenaAlloc                 fEntryAllocator{fStorage, kInitialArenaSize, kInitialArenaSize};
    Entry*                       fFreeEntryList = nullptr;
};

#endif // GrThreadSafeUniquelyKeyedProxyViewCache_DEFINED
