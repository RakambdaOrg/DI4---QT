/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "TreeModel.h"

extern DBConnect * db;

TreeModel::TreeModel(QObject * parent) : QAbstractItemModel(parent)
{
    qDebug() << "Creating tree model";
	setupModelData();
}

TreeModel::~TreeModel()
{
    qDebug() << "Destroying tree item";
	delete rootItem;
}

void TreeModel::reload(QTreeView * view)
{
	QList<int> expended = QList<int>();
	if(view != nullptr)
	{
		for(int i = 0; i < rootItem->childCount(); i++)
		{
			QModelIndex index = this->index(i, 0, view->rootIndex());
			if(view->isExpanded(index))
			{
				TreeItem * item = getItem(index);
				expended << item->data(3).toInt();
			}
		}
	}

	qDebug() << expended;

	beginResetModel();
	delete rootItem;
	setupModelData();
	endResetModel();

	if(view != nullptr)
	{
		for(int i = 0; i < rootItem->childCount(); i++)
		{
			QModelIndex index = this->index(i, 0, view->rootIndex());
			TreeItem * item = getItem(index);
			if(expended.contains(item->data(3).toInt()))
			{
				view->setExpanded(index, true);
			}
		}
	}
}

int TreeModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;
	return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	TreeItem * item = getItem(index);

	return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if(!index.isValid() || index.column() == 3)
		return 0;

	return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem * TreeModel::getItem(const QModelIndex &index) const
{
	if(index.isValid())
	{
		TreeItem * item = static_cast<TreeItem *>(index.internalPointer());
		if(item)
			return item;
	}
	return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if(parent.isValid() && parent.column() != 0)
		return QModelIndex();

	TreeItem * parentItem = getItem(parent);

	TreeItem * childItem = parentItem->child(row);
	if(childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginInsertColumns(parent, position, position + columns - 1);
	success = rootItem->insertColumns(position, columns);
	endInsertColumns();

	return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem * parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if(!index.isValid())
		return QModelIndex();

	TreeItem * childItem = getItem(index);
	TreeItem * parentItem = childItem->parent();

	if(parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = rootItem->removeColumns(position, columns);
	endRemoveColumns();

	if(rootItem->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem * parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem * parentItem = getItem(parent);

	return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(role != Qt::EditRole)
		return false;

	TreeItem * item = getItem(index);
	bool result = item->setData(index.column(), value);

	if(result)
			emit dataChanged(index, index);

	return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
	if(role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = rootItem->setData(section, value);

	if(result)
			emit headerDataChanged(orientation, section, section);

	return result;
}

void TreeModel::setupModelData()
{
	QVector<QVariant> rootData;
	rootData << "First name";
	rootData << "Last name";
	rootData << "Category";
	rootData << "Id";

	rootItem = new TreeItem(rootData);

	QList<TreeItem *> parents;
	parents << rootItem;

	QList<ResourceType *> * resources = db->getTypes();
	for(int i = 0; i < resources->count(); i++)
	{
		ResourceType * resource = resources->at(i);
		QVector<QVariant> columnDataResources;
		columnDataResources << resource->getName();
		columnDataResources << "";
		columnDataResources << "";
		columnDataResources << resource->getId();

		TreeItem * parent = parents.last();
		parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
		TreeItem * resItem = parent->child(parent->childCount() - 1);
		for(int column = 0; column < columnDataResources.size(); ++column)
			resItem->setData(column, columnDataResources[column]);

		QList<Staff *> * staffs = db->getStaffByType(resource->getId());
		for(int j = 0; j < staffs->count(); j++)
		{
			Staff * staff = staffs->at(j);

			QVector<QVariant> columnDataStaff;

			columnDataStaff << staff->getFirstName();
			columnDataStaff << staff->getLastName();
			columnDataStaff << staff->getResourceType()->getName();
			columnDataStaff << staff->getId();

			resItem->insertChildren(resItem->childCount(), 1, rootItem->columnCount());
			for(int column = 0; column < columnDataStaff.size(); ++column)
				resItem->child(resItem->childCount() - 1)->setData(column, columnDataStaff[column]);

			delete staff;
		}

		delete staffs;
		delete resource;
	}
	delete resources;
}
