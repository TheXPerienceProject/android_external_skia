/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrFixedClip_DEFINED
#define GrFixedClip_DEFINED

#include "src/gpu/GrClip.h"
#include "src/gpu/GrScissorState.h"
#include "src/gpu/GrWindowRectsState.h"

/**
 * Implements GrHardClip with scissor and window rectangles.
 */
class GrFixedClip final : public GrHardClip {
public:
    explicit GrFixedClip(const SkISize& rtDims) : fScissorState(rtDims) {}
    GrFixedClip(const SkISize& rtDims, const SkIRect& scissorRect)
            : GrFixedClip(rtDims) {
        SkAssertResult(fScissorState.set(scissorRect));
    }

    const GrScissorState& scissorState() const { return fScissorState; }
    bool scissorEnabled() const { return fScissorState.enabled(); }
    // Returns the scissor rect or rt bounds if the scissor test is not enabled.
    const SkIRect& scissorRect() const { return fScissorState.rect(); }

    void disableScissor() { fScissorState.setDisabled(); }

    bool SK_WARN_UNUSED_RESULT setScissor(const SkIRect& irect) {
        return fScissorState.set(irect);
    }
    bool SK_WARN_UNUSED_RESULT intersect(const SkIRect& irect) {
        return fScissorState.intersect(irect);
    }

    const GrWindowRectsState& windowRectsState() const { return fWindowRectsState; }
    bool hasWindowRectangles() const { return fWindowRectsState.enabled(); }

    void disableWindowRectangles() { fWindowRectsState.setDisabled(); }

    void setWindowRectangles(const GrWindowRectangles& windows, GrWindowRectsState::Mode mode) {
        fWindowRectsState.set(windows, mode);
    }

    SkIRect getConservativeBounds() const final;
    Effect apply(GrAppliedHardClip*, SkRect*) const final;
    PreClipResult preApply(const SkRect& drawBounds) const final;

private:
    GrScissorState       fScissorState;
    GrWindowRectsState   fWindowRectsState;
};

#endif
