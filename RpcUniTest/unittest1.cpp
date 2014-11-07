#include "stdafx.h"
#include "CppUnitTest.h"

#include "../rpc/rpc_task.h"

#include <thread>
#include <Windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RpcUniTest
{
    TEST_CLASS(TestRpcTask)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
            rpclib::RpcTaskMgr task_mgr;
            std::thread thread(std::bind(&rpclib::RpcTaskMgr::Run, &task_mgr));
            thread.detach();

            int sum = 0;
            task_mgr.Push(rpclib::RpcTask([&](){
                    sum += 2;
                    Sleep(200);
                })
            );
            task_mgr.Push(rpclib::RpcTask([&](){
                    sum += 1;
                })
            );

            Sleep(100);

            task_mgr.Stop();
            task_mgr.Push(rpclib::RpcTask([&](){
                    sum += 1;
                })
            );

            Assert::AreEqual(sum, 2);
        }
    };
}
