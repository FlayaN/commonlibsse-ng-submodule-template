static void MessageHandler(SKSE::MessagingInterface::Message* a_message)
{
	switch (a_message->type) {
	case SKSE::MessagingInterface::kPostLoad:
		{
			logger::info("{:*^50}", "POST LOAD"sv);
		}
		break;
	default:
		break;
	}
}

static void InitializeLog()
{
	auto path = logger::log_directory();
	if (!path) {
		stl::report_and_fail("Failed to find standard logging directory"sv);
	}
	*path /= std::format("{}.log", Plugin::NAME);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	const auto level = spdlog::level::debug;
#else
	const auto level = spdlog::level::info;
#endif

	log->set_level(level);
	log->flush_on(level);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%H:%M:%S:%e] %v"s);
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();

	SKSE::Init(a_skse);

	auto version = Plugin::VERSION.string();
	logger::info("Loaded {} {}", Plugin::NAME, version);

	const auto messaging = SKSE::GetMessagingInterface();
	messaging->RegisterListener(MessageHandler);

	return true;
}
