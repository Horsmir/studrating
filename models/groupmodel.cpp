/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Роман Браун <firdragon76@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "groupmodel.h"

GroupModel::GroupModel(QObject *parent): QAbstractItemModel(parent), db(0), currentMod(groupMod), currentGroup(0)
{
	
}

GroupModel::~GroupModel()
{

}

QVariant GroupModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return QVariant();
	
	if(role == Qt::TextAlignmentRole)
	{
		if(index.column() == 0)
			return int(Qt::AlignLeft | Qt::AlignVCenter);
		else
			return int(Qt::AlignHCenter | Qt::AlignVCenter);
	}
	else if(role == Qt::DisplayRole)
	{
		if(currentMod == groupMod)
		{
			Group gr = db->getGroup(index.data(Qt::UserRole).toUInt());
			switch(index.column())
			{
				case 0:
					return gr.getName();
				case 1:
					return gr.count();
				default:
					return QString();
			}
		}
		else if(currentMod == studentMod)
		{
			return studentList.at(index.row()).split("|").at(0);
		}
	}
	else if(role == Qt::UserRole)
	{
		if(currentMod == groupMod)
			return index.row();
		else if(currentMod == studentMod)
			return studentList.at(index.row()).split("|").at(1).toUInt();
	}
	else if(role == TypeModRole)
	{
		return currentMod;
	}
	return QVariant();
}

int GroupModel::columnCount(const QModelIndex &parent) const
{
	if(currentMod == groupMod)
		return 2;
	if(currentMod == studentMod)
		return 1;
}

int GroupModel::rowCount(const QModelIndex &parent) const
{
	if(currentMod == groupMod)
		return db->countGroups();
	if(currentMod == studentMod)
		return studentList.count();
}

bool GroupModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(index.isValid() && role == Qt::EditRole)
	{
		if(currentMod == groupMod)
		{
			if(index.column() == 0)
			{
				db->getGroupPtr(index.data(Qt::UserRole).toUInt())->setName(value.toString());
				return true;
			}
		}
		else if(currentMod == studentMod)
		{
			db->getGroupPtr(currentGroup)->getStudentPtr(index.data(Qt::UserRole).toUInt())->setName(value.toString());
		}
	}
	return false;
}

QVariant GroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if(section == 0)
			return QObject::trUtf8("Название");
		else if(section == 1)
			return QObject::trUtf8("Количество студентов");
	}
	return QVariant();
}

Qt::ItemFlags GroupModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if(index.column() == 0)
		flags |=  Qt::ItemIsEditable;
	return flags;
}

void GroupModel::setDb(DataBaseStudRating *db)
{
	this->db = db;
	QObject::connect(db, SIGNAL(dbChanged()), SLOT(dbChange()));
	reset();
}

void GroupModel::setMod(typeMod mod)
{
	currentMod = mod;
	reset();
}

void GroupModel::setCurrentGroup(quint32 groupId)
{
	currentGroup = groupId;
	studentList.clear();
	studentList = db->getGroup(currentGroup).getStudentNamesList();
	reset();
}

void GroupModel::dbChange()
{
	reset();
}

QModelIndex GroupModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column);
}

QModelIndex GroupModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

#include "groupmodel.moc"