#include "Installer.h"

#include <QDir>
#include <QFileInfo>


Installer::Installer(QObject* parent)
    : QObject(parent)
    , m_task_running(false)
    , m_task_failed(false)
{
    m_process.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_process, &QProcess::readyRead, this, &Installer::onProcessReadyRead);
    connect(&m_process, &QProcess::errorOccurred, this, &Installer::onProcessError);
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Installer::onProcessFinished);
}

bool Installer::installed(Frontend* frontend) const
{
    Q_ASSERT(frontend);

    const QString path = frontend->m_exe_path.startsWith('/')
        ? frontend->m_exe_path
        : QStringLiteral("/usr/bin/") + frontend->m_exe_path;

    return QFileInfo::exists(path);
}

void Installer::startInstall(Frontend* frontend)
{
    Q_ASSERT(frontend);
    Q_ASSERT(m_process.state() != QProcess::Running);

    const QStringList arguments {
        QStringLiteral("install"),
        frontend->m_package_name
    };

    m_log = QStringLiteral("retrolx-pacman");
    for (const QString& arg : arguments)
        m_log += ' ' + arg;
    m_log += "`...\n";
    emit logChanged();

    m_task_running = true;
    m_task_failed = false;
    emit taskRunChanged();
    emit taskFailChanged();

    m_process.start(QStringLiteral("retrolx-pacman"), arguments, QIODevice::ReadOnly);
}

void Installer::onProcessReadyRead()
{
    const auto prev_log_len = m_log.length();

    m_log += QString(m_process.readAllStandardOutput());

    if (prev_log_len != m_log.length())
        emit logChanged();
}

void Installer::onProcessError(QProcess::ProcessError)
{
    if (!m_log.isEmpty())
        m_log += '\n';

    m_log += m_process.errorString();
    emit logChanged();

    m_task_running = false;
    m_task_failed = true;
    emit taskRunChanged();
    emit taskFailChanged();
}

void Installer::onProcessFinished(int, QProcess::ExitStatus)
{
    m_task_running = false;
    m_task_failed = false;
    emit taskRunChanged();
    emit taskFailChanged();
}
