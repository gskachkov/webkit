/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2010, 2013 Apple Inc. ALl rights reserved.
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

#ifndef HTMLStyleElement_h
#define HTMLStyleElement_h

#include "HTMLElement.h"
#include "InlineStyleSheetOwner.h"

namespace WebCore {

class HTMLStyleElement;
class StyleSheet;

template<typename T> class EventSender;
typedef EventSender<HTMLStyleElement> StyleEventSender;

class HTMLStyleElement final : public HTMLElement {
public:
    static Ref<HTMLStyleElement> create(const QualifiedName&, Document&, bool createdByParser);
    virtual ~HTMLStyleElement();

    CSSStyleSheet* sheet() const { return m_styleSheetOwner.sheet(); }

    bool disabled() const;
    void setDisabled(bool);

    void dispatchPendingEvent(StyleEventSender*);
    static void dispatchPendingLoadEvents();

private:
    HTMLStyleElement(const QualifiedName&, Document&, bool createdByParser);

    // overload from HTMLElement
    void parseAttribute(const QualifiedName&, const AtomicString&) override;
    InsertionNotificationRequest insertedInto(ContainerNode&) override;
    void removedFrom(ContainerNode&) override;
    void childrenChanged(const ChildChange&) override;

    void finishParsingChildren() override;

    bool isLoading() const { return m_styleSheetOwner.isLoading(); }
    bool sheetLoaded() override { return m_styleSheetOwner.sheetLoaded(*this); }
    void notifyLoadedSheetAndAllCriticalSubresources(bool errorOccurred) override;
    void startLoadingDynamicSheet() override { m_styleSheetOwner.startLoadingDynamicSheet(*this); }

    void addSubresourceAttributeURLs(ListHashSet<URL>&) const override;

    InlineStyleSheetOwner m_styleSheetOwner;
    bool m_firedLoad;
    bool m_loadedSheet;
};

} //namespace

#endif
