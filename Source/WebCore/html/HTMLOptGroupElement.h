/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2010 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef HTMLOptGroupElement_h
#define HTMLOptGroupElement_h

#include "HTMLElement.h"

namespace WebCore {
    
class HTMLSelectElement;

class HTMLOptGroupElement final : public HTMLElement {
public:
    static Ref<HTMLOptGroupElement> create(const QualifiedName&, Document&);

    bool isDisabledFormControl() const override;
    HTMLSelectElement* ownerSelectElement() const;
    
    WEBCORE_EXPORT String groupLabelText() const;

private:
    HTMLOptGroupElement(const QualifiedName&, Document&);

    const AtomicString& formControlType() const;
    bool isFocusable() const override;
    void parseAttribute(const QualifiedName&, const AtomicString&) override;
    bool rendererIsNeeded(const RenderStyle&) override { return false; }

    void childrenChanged(const ChildChange&) override;

    void accessKeyAction(bool sendMouseEvents) override;

    void recalcSelectOptions();
};

} //namespace

#endif
