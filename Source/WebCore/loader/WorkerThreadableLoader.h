/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#ifndef WorkerThreadableLoader_h
#define WorkerThreadableLoader_h

#include "ThreadableLoader.h"
#include "ThreadableLoaderClient.h"
#include "ThreadableLoaderClientWrapper.h"

#include <wtf/Threading.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

    class ContentSecurityPolicy;
    class ResourceError;
    class ResourceRequest;
    class WorkerGlobalScope;
    class WorkerLoaderProxy;
    struct CrossThreadResourceResponseData;
    struct CrossThreadResourceRequestData;

    class WorkerThreadableLoader : public RefCounted<WorkerThreadableLoader>, public ThreadableLoader {
        WTF_MAKE_FAST_ALLOCATED;
    public:
        static void loadResourceSynchronously(WorkerGlobalScope*, const ResourceRequest&, ThreadableLoaderClient&, const ThreadableLoaderOptions&);
        static Ref<WorkerThreadableLoader> create(WorkerGlobalScope* workerGlobalScope, ThreadableLoaderClient* client, const String& taskMode, const ResourceRequest& request, const ThreadableLoaderOptions& options)
        {
            return adoptRef(*new WorkerThreadableLoader(workerGlobalScope, client, taskMode, request, options));
        }

        ~WorkerThreadableLoader();

        void cancel() override;

        bool done() const { return m_workerClientWrapper->done(); }

        using RefCounted<WorkerThreadableLoader>::ref;
        using RefCounted<WorkerThreadableLoader>::deref;

    protected:
        void refThreadableLoader() override { ref(); }
        void derefThreadableLoader() override { deref(); }

    private:
        // Creates a loader on the main thread and bridges communication between
        // the main thread and the worker context's thread where WorkerThreadableLoader runs.
        //
        // Regarding the bridge and lifetimes of items used in callbacks, there are a few cases:
        //
        // all cases. All tasks posted from the worker context's thread are ok because
        //    the last task posted always is "mainThreadDestroy", so MainThreadBridge is
        //    around for all tasks that use it on the main thread.
        //
        // case 1. worker.terminate is called.
        //    In this case, no more tasks are posted from the worker object's thread to the worker
        //    context's thread -- WorkerGlobalScopeProxy implementation enforces this.
        //
        // case 2. xhr gets aborted and the worker context continues running.
        //    The ThreadableLoaderClientWrapper has the underlying client cleared, so no more calls
        //    go through it.  All tasks posted from the worker object's thread to the worker context's
        //    thread contain the RefPtr<ThreadableLoaderClientWrapper> object, so the 
        //    ThreadableLoaderClientWrapper instance is there until all tasks are executed.
        class MainThreadBridge : public ThreadableLoaderClient {
        public:
            // All executed on the worker context's thread.
            MainThreadBridge(ThreadableLoaderClientWrapper&, WorkerLoaderProxy&, const String& taskMode, const ResourceRequest&, const ThreadableLoaderOptions&, const String& outgoingReferrer, const SecurityOrigin*, const ContentSecurityPolicy*);
            void cancel();
            void destroy();

        private:
            // Executed on the worker context's thread.
            void clearClientWrapper();

            // All executed on the main thread.
            void didSendData(unsigned long long bytesSent, unsigned long long totalBytesToBeSent) override;
            void didReceiveResponse(unsigned long identifier, const ResourceResponse&) override;
            void didReceiveData(const char*, int dataLength) override;
            void didFinishLoading(unsigned long identifier, double finishTime) override;
            void didFail(const ResourceError&) override;
            void didFailAccessControlCheck(const ResourceError&) override;
            void didFailRedirectCheck() override;

            // Only to be used on the main thread.
            RefPtr<ThreadableLoader> m_mainThreadLoader;

            // ThreadableLoaderClientWrapper is to be used on the worker context thread.
            // The ref counting is done on either thread.
            RefPtr<ThreadableLoaderClientWrapper> m_workerClientWrapper;

            // May be used on either thread.
            WorkerLoaderProxy& m_loaderProxy;

            // For use on the main thread.
            String m_taskMode;
        };

        WorkerThreadableLoader(WorkerGlobalScope*, ThreadableLoaderClient*, const String& taskMode, const ResourceRequest&, const ThreadableLoaderOptions&);

        RefPtr<WorkerGlobalScope> m_workerGlobalScope;
        RefPtr<ThreadableLoaderClientWrapper> m_workerClientWrapper;
        MainThreadBridge& m_bridge;
    };

} // namespace WebCore

#endif // WorkerThreadableLoader_h
