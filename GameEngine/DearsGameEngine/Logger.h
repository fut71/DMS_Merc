#pragma once
#include "pch.h"
// �α� ���̺귯��
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Singleton.h"
#include "Utility.h"

// �α� ������ ��Ÿ���� �������Դϴ�.
enum LOG_LEVEL : uint8_t
{
	LOG_INFO = SPDLOG_LEVEL_INFO,			// 2
	LOG_TRACE = SPDLOG_LEVEL_TRACE,			// 0
	LOG_DEBUG = SPDLOG_LEVEL_DEBUG,			// 1
	LOG_WARN = SPDLOG_LEVEL_WARN,			// 3
	LOG_ERROR = SPDLOG_LEVEL_ERROR,			// 4
	LOG_CRITICAL = SPDLOG_LEVEL_CRITICAL,	// 5
	LOG_OFF = SPDLOG_LEVEL_OFF				// 6
};

/// <summary>
/// �ΰ� �������̽��� �����ϴ� �߻� Ŭ�����Դϴ�.
/// 2024.05.07 _Doyo
/// </summary>
class ILogger
{
public:
	virtual ~ILogger() = default;

	/// <summary>
	/// �α׸� ����ϴ� �Լ��Դϴ�.
	/// </summary>
	/// <param name="_logLevel">�ش� �α��� ������ �����մϴ�.</param>
	/// <param name="_message">�α��� �޼����Դϴ�.</param>
	/// <param name="_location">�ش� �α��� ��ġ�� ��Ÿ���ϴ�. 
	/// C++20�� std::source_location Ű���带 ����� �ڵ����� ��ġ ������ �����մϴ�.</param>
	virtual void Log(LOG_LEVEL _logLevel, const std::string_view _message, std::source_location _location = std::source_location::current()) = 0;

	// ���μ����� �α׸� �����ϴ� �Լ��Դϴ�.
	void SetLogLevel(LOG_LEVEL _level = LOG_LEVEL::LOG_INFO)
	{
		m_logLevel = _level;
		if (m_logger)
		{
			m_logger->set_level(GetSpdLogLevel(_level));
		}
	}

	LOG_LEVEL GetLogLevel() const
	{
		return m_logLevel;
	}

protected:
	spdlog::level::level_enum GetSpdLogLevel(LOG_LEVEL _logLevel)
	{
		return static_cast<spdlog::level::level_enum>(_logLevel);
	}

protected:
	LOG_LEVEL m_logLevel = LOG_LEVEL::LOG_INFO;	// ���� ������ �α� ����
	std::shared_ptr<spdlog::logger> m_logger;
};

/// <summary>
/// �ܼ�â�� �α׸� ����ϴ� Ŭ�����Դϴ�. ����� ��忡���� ����� Ŭ�����Դϴ�.
/// 2024.05.07 _Doyo
/// </summary>
class ConsoleLogger : public ILogger
{
public:
	ConsoleLogger()
	{
		m_logger = spdlog::stdout_color_mt("console");
		SetLogLevel(LOG_TRACE);
	}

	~ConsoleLogger() = default;

	/// <summary>
	/// �ܼ�â�� �α׸� ����ϴ� �Լ��Դϴ�.
	/// </summary>
	/// <param name="_logLevel">�ش� �α��� ������ �����մϴ�.</param>
	/// <param name="_message">�α��� �޼����Դϴ�.</param>
	/// <param name="_location">�ش� �α��� ��ġ�� ��Ÿ���ϴ�. 
	/// C++20�� std::source_location Ű���带 ����� �ڵ����� ��ġ ������ �����մϴ�.</param>
	virtual void Log(LOG_LEVEL _logLevel, const std::string_view _message
		, std::source_location _location = std::source_location::current()) override
	{
		//SetLogLevel();
		if (_logLevel >= m_logLevel)
		{
			std::string fileName = std::filesystem::path(_location.file_name()).filename().string();
			//td::clog << "File: " << fileName << '(' << _location.line() << ") : " << _message << std::endl;
			m_logger->log(GetSpdLogLevel(_logLevel), "File: {}({}) : {}", fileName, _location.line(), _message);
		}
	}

	/// <summary>
	/// ���μ����� �α� ������ �����ϴ� �Լ��Դϴ�.
	/// �ܼ� �ΰ� Ŭ������ ����� ����� ����Ʈ �ΰŷ�,
	/// ��� �α׵��� ����մϴ�.
	/// </summary>
	/// <param name="_level">����Ʈ���� LOG_INFO�� �����մϴ�.</param>
	void SetLogLevel(LOG_LEVEL _level = LOG_LEVEL::LOG_INFO)
	{
		m_logLevel = _level;
		m_logger->set_level(GetSpdLogLevel(m_logLevel));
	}

private:
	spdlog::level::level_enum GetSpdLogLevel(LOG_LEVEL _logLevel)
	{
		switch (_logLevel)
		{
			case LOG_INFO:
				return spdlog::level::info;
			case LOG_WARN:
				return spdlog::level::warn;
			case LOG_ERROR:
				return spdlog::level::err;
			default:
				return spdlog::level::debug;
		}
	}
};

/// <summary>
/// �α� ���Ͽ� �α׸� ����ϴ� Ŭ�����Դϴ�. ������ ��忡�� ����� Ŭ�����Դϴ�.
/// ��ȹ�ںе��̳� �׽��� �����鿡�� �޾Ƽ� ����׿� ����� �� �ֽ��ϴ�.
/// 2024.05.07 _Doyo
/// </summary>
class FileLogger : public ILogger
{
public:
	FileLogger()
	{
		// �ν��Ͻ� ���� �� ���ο� �α� ���� ���� �� ����.
		OpenNewLogFile();
	}

	~FileLogger() = default;

	/// <summary>
	/// �α� ���Ͽ� �α׸� ����ϴ� �Լ��Դϴ�.
	/// �α� ������ ũ�Ⱑ ���� �̻� �������� ��� ���ο� �α� ������ �����ϰ� �����մϴ�.
	/// </summary>
	/// <param name="_logLevel">�ش� �α��� ������ �����մϴ�.</param>
	/// <param name="_message">����� �޼����Դϴ�.</param>
	/// <param name="_location">�ش� �α��� ��ġ�� ��Ÿ���ϴ�. 
	/// C++20�� std::source_location Ű���带 ����� �ڵ����� ��ġ ������ �����մϴ�.</param>
	void Log(LOG_LEVEL _logLevel, const std::string_view _message, std::source_location _location = std::source_location::current()) override
	{
		if (_logLevel < m_logLevel)
		{
			return;
		}

		if (m_logger)
		{
			std::string fileName = std::filesystem::path(_location.file_name()).filename().string();
			m_logger->log(GetSpdLogLevel(_logLevel), "File: {}({}) : {}", fileName, _location.line(), _message);
		}
	}

	/// <summary>
	/// �α� ������ �����ϴ� �Լ��Դϴ�.
	/// ���� �ΰ� Ŭ������ ������ ����� �ΰŷ�,
	/// �÷��̿� ������ ���� ������ �α׵��� ����մϴ�.
	/// </summary>
	/// <param name="_level"></param>
	void SetLogLevel(LOG_LEVEL _level = LOG_LEVEL::LOG_INFO)
	{
		m_logLevel = _level;
		if (m_logger)
		{
			m_logger->set_level(GetSpdLogLevel(m_logLevel));
		}
	}

private:
	/// <summary>
	/// ���ο� �α� ������ �����ϰ� �����ϴ� �Լ��Դϴ�.
	/// �α� ������ ���ϸ��� ������ ǥ�� �ð��븦 �������� �մϴ�.
	/// </summary>
	void OpenNewLogFile()
	{
		auto now = std::chrono::system_clock::now();
		auto local_now = std::chrono::zoned_time{ std::chrono::current_zone(), now };
		std::string fileName = std::format("../log/log_{}.log", std::format("{:%Y-%m-%d_%H-%M-%S}", local_now));

		m_logger = spdlog::basic_logger_mt("file_logger", fileName);
		if (!m_logger)
		{
			throw std::runtime_error("Failed to open log file.");
		}
		m_logger->set_level(GetSpdLogLevel(m_logLevel));
	}

	spdlog::level::level_enum GetSpdLogLevel(LOG_LEVEL _logLevel)
	{
		switch (_logLevel)
		{
			case LOG_INFO:
				return spdlog::level::info;
			case LOG_WARN:
				return spdlog::level::warn;
			case LOG_ERROR:
				return spdlog::level::err;
			default:
				return spdlog::level::debug;
		}
	}

private:
	std::ofstream m_logFile;	// ���� �������� ����
	std::size_t m_maxFileSize = 5 * 1024 * 1024;	// �α� ������ �ִ� ũ��

};

// ���� �ΰ� ������ ����
extern ILogger* g_pLogger;

// �ΰ� �ʱ�ȭ �� ���� �Լ� ����
void InitializeLogger();
void PrintLoggerInfo();
void CleanupLogger();

#define DLOG(_logLevel, _message) \
if(g_pLogger) g_pLogger->Log(_logLevel, _message)

#define SET_LOG_LEVEL(level) \
if(g_pLogger) g_pLogger->SetLogLevel(level)