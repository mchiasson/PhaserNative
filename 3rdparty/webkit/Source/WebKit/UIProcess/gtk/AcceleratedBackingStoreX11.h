/*
 * Copyright (C) 2016 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "AcceleratedBackingStore.h"

#if USE(REDIRECTED_XCOMPOSITE_WINDOW)

#include <WebCore/RefPtrCairo.h>
#include <WebCore/XUniqueResource.h>

namespace WebKit {

class WebPageProxy;

class AcceleratedBackingStoreX11 final : public AcceleratedBackingStore {
    WTF_MAKE_NONCOPYABLE(AcceleratedBackingStoreX11); WTF_MAKE_FAST_ALLOCATED;
public:
    static std::unique_ptr<AcceleratedBackingStoreX11> create(WebPageProxy&);
    ~AcceleratedBackingStoreX11();

private:
    AcceleratedBackingStoreX11(WebPageProxy&);

    void update(const LayerTreeContext&) override;
    bool paint(cairo_t*, const WebCore::IntRect&) override;

    RefPtr<cairo_surface_t> m_surface;
    WebCore::XUniqueDamage m_damage;
};

} // namespace WebKit

#endif // USE(REDIRECTED_XCOMPOSITE_WINDOW)
