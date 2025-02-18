/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MeterShadowElement_h
#define MeterShadowElement_h

#if ENABLE(METER_ELEMENT)
#include "HTMLDivElement.h"
#include <wtf/Forward.h>
#include <wtf/NeverDestroyed.h>

namespace WebCore {

class HTMLMeterElement;
class RenderMeter;

class MeterShadowElement : public HTMLDivElement {
public:
    HTMLMeterElement* meterElement() const;

protected:
    MeterShadowElement(Document&);

private:
    bool rendererIsNeeded(const RenderStyle&) override;
};

class MeterInnerElement final : public MeterShadowElement {
public:
    static Ref<MeterInnerElement> create(Document&);

private:
    MeterInnerElement(Document&);

    bool rendererIsNeeded(const RenderStyle&) override;
    RenderPtr<RenderElement> createElementRenderer(Ref<RenderStyle>&&, const RenderTreePosition&) override;
};

inline Ref<MeterInnerElement> MeterInnerElement::create(Document& document)
{
    return adoptRef(*new MeterInnerElement(document));
}

class MeterBarElement final : public MeterShadowElement {
public:
    static Ref<MeterBarElement> create(Document&);

private:
    MeterBarElement(Document& document)
        : MeterShadowElement(document)
    {
        static NeverDestroyed<AtomicString> pseudoId("-webkit-meter-bar", AtomicString::ConstructFromLiteral);
        setPseudo(pseudoId);
    }
};

inline Ref<MeterBarElement> MeterBarElement::create(Document& document)
{
    return adoptRef(*new MeterBarElement(document));
}

class MeterValueElement final : public MeterShadowElement {
public:
    static Ref<MeterValueElement> create(Document&);
    void setWidthPercentage(double);
    void updatePseudo() { setPseudo(valuePseudoId()); }

private:
    MeterValueElement(Document& document)
        : MeterShadowElement(document)
    {
        updatePseudo();
    }

    const AtomicString& valuePseudoId() const;
};

inline Ref<MeterValueElement> MeterValueElement::create(Document& document)
{
    return adoptRef(*new MeterValueElement(document));
}

}
#endif // ENABLE(METER_ELEMENT)
#endif // MeterShadowElement_h
