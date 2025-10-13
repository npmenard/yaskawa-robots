#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>

#include <boost/asio.hpp>

#include <thread>
#include "yaskawa_arm.hpp"

using namespace viam::sdk;

int main(int argc, char** argv) {
    const Instance instance;
    boost::asio::io_context io_context(2);

    std::thread io_thread([&io_context]() {
            VIAM_SDK_LOG(info) << "io context running";
            boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(io_context.get_executor());
            io_context.run();
            VIAM_SDK_LOG(info)  << "IO thread shutting down \n";
        });
    std::make_shared<ModuleService>(argc, argv, YaskawaArm::create_model_registrations(io_context))->serve();
    io_context.stop();
    io_thread.join();
    return EXIT_SUCCESS;
};
