/****************************************************************************
**
** Copyright (c) 2009-2010, Jaco Naude
**
** This file is part of Qtilities which is released under the following
** licensing options.
**
** Option 1: Open Source
** Under this license Qtilities is free software: you can
** redistribute it and/or modify it under the terms of the GNU General
** Public License as published by the Free Software Foundation, either
** version 3 of the License, or (at your option) any later version.
**
** Qtilities is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Qtilities. If not, see http://www.gnu.org/licenses/.
**
** Option 2: Commercial
** Alternatively, this library is also released under a commercial license
** that allows the development of closed source proprietary applications
** without restrictions on licensing. For more information on this option,
** please see the project website's licensing page:
** http://www.qtilities.org/licensing.html
**
** If you are unsure which license is appropriate for your use, please
** contact support@qtilities.org.
**
****************************************************************************/

#ifndef TREE_FILE_ITEM_H
#define TREE_FILE_ITEM_H

#include "QtilitiesCoreGui_global.h"
#include "AbstractTreeItem.h"

#include <IModificationNotifier>
#include <IExportable>

namespace Qtilities {
    namespace CoreGui {
        using namespace Qtilities::Core;

        /*!
        \struct TreeFileItemData
        \brief Structure used by TreeFileItem to store private data.
          */
        struct TreeFileItemData;

        /*!
          \class TreeFileItem
          \brief The TreeFileItem class is an item in a tree which can be used to easily represent files.

          This class was added in %Qtilities v0.2.
        */
        class QTILITIES_CORE_GUI_SHARED_EXPORT TreeFileItem : public QObject, public AbstractTreeItem, public IExportable, public IModificationNotifier
        {
            Q_OBJECT
            Q_INTERFACES(Qtilities::Core::Interfaces::IExportable)
            Q_INTERFACES(Qtilities::Core::Interfaces::IModificationNotifier)

        public:
            TreeFileItem(QObject* parent = 0);
            virtual ~TreeFileItem();

            // --------------------------------
            // IObjectBase Implemenation
            // --------------------------------
            QObject* objectBase() { return this; }

            // --------------------------------
            // IExportable Implementation
            // --------------------------------
            ExportModeFlags supportedFormats() const;
            IFactoryData factoryData() const;
            virtual IExportable::Result exportBinary(QDataStream& stream, QList<QVariant> params = QList<QVariant>()) const;
            virtual IExportable::Result importBinary(QDataStream& stream, QList<QPointer<QObject> >& import_list, QList<QVariant> params = QList<QVariant>());
            virtual Result exportXML(QDomDocument* doc, QDomElement* object_node, QList<QVariant> params = QList<QVariant>()) const;
            virtual Result importXML(QDomDocument* doc, QDomElement* object_node, QList<QVariant> params = QList<QVariant>());

            // --------------------------------
            // IModificationNotifier Implementation
            // --------------------------------
            bool isModified() const;
        public slots:
            void setModificationState(bool new_state, IModificationNotifier::NotificationTargets notification_targets = IModificationNotifier::NotifyListeners);
        signals:
            void modificationStateChanged(bool is_modified) const;

        protected:
            TreeFileItemData* itemData;
        };
    }
}

#endif //  TREE_FILE_ITEM_H