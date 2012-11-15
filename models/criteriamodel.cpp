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


#include "criteriamodel.h"

CriteriaModel::CriteriaModel(QObject *parent): QAbstractTableModel(parent), dBase(0)
{
	currentDisciplinId = 0;
	currentGroupId = 0;
}

CriteriaModel::~CriteriaModel()
{

}


QVariant CriteriaModel::data(const QModelIndex &index, int role) const
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
	else if(role == Qt::DisplayRole || role == Qt::EditRole)
	{
		switch(index.column())
		{
			case 0:
				return dBase->getCriterion(currentDisciplinId, currentGroupId).at(index.row()).getName();
				break;
			case 1:
				return dBase->getCriterion(currentDisciplinId, currentGroupId).at(index.row()).getValue();
				break;
			default:
				return QVariant();
		}
	}
	return QVariant();
}

int CriteriaModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

int CriteriaModel::rowCount(const QModelIndex &parent) const
{
	return dBase->getCriterion(currentDisciplinId, currentGroupId).count();
}

bool CriteriaModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(index.isValid() && role == Qt::EditRole)
	{
		if(index.column() == 0)
			dBase->editCriterionName(currentDisciplinId, currentGroupId, index.row(), value.toString());
		if(index.column() == 1)
			dBase->editCriterionValue(currentDisciplinId, currentGroupId, index.row(), value.toFloat());
		return true;
	}
	return false;
}

QVariant CriteriaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if(section == 0)
			return QObject::trUtf8("Название");
		else if(section == 1)
			return QObject::trUtf8("Значение");
	}
	else if(role == Qt::TextAlignmentRole)
	{
		return int(Qt::AlignHCenter | Qt::AlignVCenter);
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

void CriteriaModel::setDb(DataBaseStudRating *db)
{
	dBase = db;
	reset();
}

void CriteriaModel::setParams(quint32 disciplinId, quint32 groupId)
{
	currentDisciplinId = disciplinId;
	currentGroupId = groupId;
	reset();
}

Qt::ItemFlags CriteriaModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
	return flags;
}

#include "criteriamodel.moc"