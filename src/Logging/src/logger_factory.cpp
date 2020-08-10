#include <litefx/logging.hpp>
#include <spdlog/spdlog.h>

using namespace LiteFX::Logging;

static Array<spdlog::sink_ptr> m_sinks;

Log Logger::get(const String& name)
{
    // Get the log.
    auto log = spdlog::get(name);
    
    // If it does not exist, create it from the current sinks.
    if (log == nullptr)
    {
        auto logger = makeShared<spdlog::logger>(name, std::begin(m_sinks), std::end(m_sinks));

#ifndef NDEBUG
        logger->set_level(spdlog::level::trace);
#else
        logger->set_level(spdlog::level::info);
#endif

        spdlog::register_logger(logger);
    }

    return Log(name);
}

void Logger::sinkTo(const ISink* sink)
{
    if (sink == nullptr)
        throw std::invalid_argument("The provided sink is not initialized.");

    m_sinks.push_back(sink->get());
}