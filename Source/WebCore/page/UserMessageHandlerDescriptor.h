/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UserMessageHandlerDescriptor_h
#define UserMessageHandlerDescriptor_h

#if ENABLE(USER_MESSAGE_HANDLERS)

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>
#include <wtf/text/AtomicString.h>

namespace WebCore {

class Frame;
class DOMWrapperWorld;
class UserMessageHandler;
class SerializedScriptValue;

class UserMessageHandlerDescriptor : public RefCounted<UserMessageHandlerDescriptor> {
public:
    class Client {
    public:
        virtual ~Client() { }
        virtual void didPostMessage(UserMessageHandler&, SerializedScriptValue*) = 0;
    };

    static Ref<UserMessageHandlerDescriptor> create(const AtomicString& name, DOMWrapperWorld& world, Client& client)
    {
        return adoptRef(*new UserMessageHandlerDescriptor(name, world, client));
    }
    WEBCORE_EXPORT ~UserMessageHandlerDescriptor();

    WEBCORE_EXPORT const AtomicString& name();
    WEBCORE_EXPORT DOMWrapperWorld& world();

    Client* client() const { return m_client; }
    void invalidateClient() { m_client = nullptr; }

private:
    WEBCORE_EXPORT explicit UserMessageHandlerDescriptor(const AtomicString&, DOMWrapperWorld&, Client&);

    AtomicString m_name;
    Ref<DOMWrapperWorld> m_world;
    Client* m_client;
};

} // namespace WebCore

#endif // ENABLE(USER_MESSAGE_HANDLERS)
#endif // UserMessageHandlerDescriptor_h
