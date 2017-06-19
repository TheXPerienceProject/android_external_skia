/*
 * Copyright 2012 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkMergeImageFilter.h"

#include "SkCanvas.h"
#include "SkReadBuffer.h"
#include "SkSpecialImage.h"
#include "SkSpecialSurface.h"
#include "SkWriteBuffer.h"
#include "SkValidationUtils.h"

sk_sp<SkImageFilter> SkMergeImageFilter::Make(sk_sp<SkImageFilter>* const filters, int count,
                                               const CropRect* cropRect) {
    return sk_sp<SkImageFilter>(new SkMergeImageFilter(filters, count, cropRect));
}

///////////////////////////////////////////////////////////////////////////////

SkMergeImageFilter::SkMergeImageFilter(sk_sp<SkImageFilter>* const filters, int count,
                                       const CropRect* cropRect)
    : INHERITED(filters, count, cropRect) {
    SkASSERT(count >= 0);
}

sk_sp<SkSpecialImage> SkMergeImageFilter::onFilterImage(SkSpecialImage* source, const Context& ctx,
                                                        SkIPoint* offset) const {
    int inputCount = this->countInputs();
    if (inputCount < 1) {
        return nullptr;
    }

    SkIRect bounds;
    bounds.setEmpty();

    std::unique_ptr<sk_sp<SkSpecialImage>[]> inputs(new sk_sp<SkSpecialImage>[inputCount]);
    std::unique_ptr<SkIPoint[]> offsets(new SkIPoint[inputCount]);

    // Filter all of the inputs.
    for (int i = 0; i < inputCount; ++i) {
        offsets[i].setZero();
        inputs[i] = this->filterInput(i, source, ctx, &offsets[i]);
        if (!inputs[i]) {
            continue;
        }
        const SkIRect inputBounds = SkIRect::MakeXYWH(offsets[i].fX, offsets[i].fY,
                                                      inputs[i]->width(), inputs[i]->height());
        bounds.join(inputBounds);
    }
    if (bounds.isEmpty()) {
        return nullptr;
    }

    // Apply the crop rect to the union of the inputs' bounds.
    // Note that the crop rect can only reduce the bounds, since this
    // filter does not affect transparent black.
    bool embiggen = false;
    this->getCropRect().applyTo(bounds, ctx.ctm(), embiggen, &bounds);
    if (!bounds.intersect(ctx.clipBounds())) {
        return nullptr;
    }

    const int x0 = bounds.left();
    const int y0 = bounds.top();

    sk_sp<SkSpecialSurface> surf(source->makeSurface(ctx.outputProperties(), bounds.size()));
    if (!surf) {
        return nullptr;
    }

    SkCanvas* canvas = surf->getCanvas();
    SkASSERT(canvas);

    canvas->clear(0x0);

    // Composite all of the filter inputs.
    for (int i = 0; i < inputCount; ++i) {
        if (!inputs[i]) {
            continue;
        }

        inputs[i]->draw(canvas,
                        SkIntToScalar(offsets[i].x() - x0), SkIntToScalar(offsets[i].y() - y0),
                        nullptr);
    }

    offset->fX = bounds.left();
    offset->fY = bounds.top();
    return surf->makeImageSnapshot();
}

sk_sp<SkImageFilter> SkMergeImageFilter::onMakeColorSpace(SkColorSpaceXformer* xformer) const {
    SkSTArray<5, sk_sp<SkImageFilter>> inputs(this->countInputs());
    for (int i = 0; i < this->countInputs(); i++) {
        inputs.push_back(this->getInput(i) ? this->getInput(i)->makeColorSpace(xformer) : nullptr);
    }

    return SkMergeImageFilter::Make(inputs.begin(), this->countInputs(),
                                    this->getCropRectIfSet());
}

sk_sp<SkFlattenable> SkMergeImageFilter::CreateProc(SkReadBuffer& buffer) {
    Common common;
    if (!common.unflatten(buffer, -1)) {
        return nullptr;
    }

    const int count = common.inputCount();
    if (buffer.isVersionLT(SkReadBuffer::kNoModesInMergeImageFilter_Verison)) {
        bool hasModes = buffer.readBool();
        if (hasModes) {
            // Older pictures may have stored blendmodes, but by inspection we think these were
            // all src-over, so we have removed support for storing these.
            SkAutoSTArray<4, uint8_t> modes8(count);
            if (!buffer.readByteArray(modes8.get(), count)) {
                return nullptr;
            }
            if (!buffer.isValid()) {
                return nullptr;
            }
        }
    }
    return Make(common.inputs(), count, &common.cropRect());
}

void SkMergeImageFilter::flatten(SkWriteBuffer& buffer) const {
    this->INHERITED::flatten(buffer);
}

#ifndef SK_IGNORE_TO_STRING
void SkMergeImageFilter::toString(SkString* str) const {
    str->appendf("SkMergeImageFilter: (");

    for (int i = 0; i < this->countInputs(); ++i) {
        SkImageFilter* filter = this->getInput(i);
        str->appendf("%d: (", i);
        filter->toString(str);
        str->appendf(")");
    }

    str->append(")");
}
#endif
