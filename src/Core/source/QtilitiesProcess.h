/****************************************************************************
**
** Copyright 2010-2011, CSIR
** Author: JP Naude, jpnaude@csir.co.za
**
****************************************************************************/

#ifndef QTILITIES_PROCESS_H
#define QTILITIES_PROCESS_H

#include "QtilitiesCore_global.h"
#include "Task.h"

#include <QObject>
#include <QProcess>

namespace Qtilities {
    namespace Core {
        /*!
        \struct QtilitiesProcessPrivateData
        \brief Structure used by QtilitiesProcess to store private data.
          */
        struct QtilitiesProcessPrivateData;

        /*!
        \class QtilitiesProcess
        \brief An easy to use way to launch external processes through an extended wrapper around QProcess.

        The QtilitiesProcess class simplifies usage of QProcess and provides ready to use logging and task integration capablities.
          */
        class QTILIITES_CORE_SHARED_EXPORT QtilitiesProcess : public Task
        {
            Q_OBJECT
            Q_INTERFACES(Qtilities::Core::Interfaces::ITask)

        public:
            QtilitiesProcess(const QString& task_name, bool enable_logging = true, QObject* parent = 0);
            virtual ~QtilitiesProcess();

            //! Starts the process, similar to QProcess::start().
            /*!
                \returns True when the task was started successfully (thus waitForStarted() returned true), false otherwise.
              */
            virtual bool startProcess(const QString& program, const QStringList& arguments, QProcess::OpenMode mode = QProcess::ReadWrite);

            //! Access to the QProcess instance contained and used within this object.
            QProcess* process();

        private slots:
            void procStarted();
            void procFinished(int exit_code, QProcess::ExitStatus exit_status);
            void procError(QProcess::ProcessError error);
            void logProgressOutput();
            void logProgressError();

        public slots:
            //! Stops the process.
            /*!
                By default kill() is used on the process. If you need to use terminate() you should
                subclass QtilitiesProcess and reimplement this function.

                Its important to call Task::stop() at the end of your implementation.
            */
            virtual void stopProcess();

        private:
            QtilitiesProcessPrivateData* d;
        };
    }
}

#endif // QTILITIES_PROCESS_H