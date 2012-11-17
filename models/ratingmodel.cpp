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


#include "ratingmodel.h"

RatingModel::RatingModel(QObject *parent): QAbstractTableModel(parent), currentDisciplinId(0), currentGroupId(0), currentView(-1), dBase(0), precision(-1)
{

}

RatingModel::~RatingModel()
{

}

QVariant RatingModel::data(const QModelIndex &index, int role) const
{
	qreal retr = 0.0, resr = 0.0, retVal = 0.0;
	QVector<qreal> ret, res;
	
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
		if(index.column() == 0)
			return dBase->getGroup(currentGroupId).getStudent(index.data(Qt::UserRole).toUInt()).getName();
		else
		{
			if(currentView == -2)
			{
				retVal = 0.0;
				ret = dBase->getGroup(currentGroupId).getStudent(index.data(Qt::UserRole).toUInt()).getRating(currentDisciplinId, index.column() - 1);
				res = dBase->getCriterionsValues(currentDisciplinId, currentGroupId);
				Q_ASSERT(ret.count() == res.count());
				for(int i = 0; i < ret.count(); i++)
				{
					retVal += ret.at(i) * res.at(i);
				}
				return QString("%1\%").arg(retVal / dBase->getSumCriteria(currentDisciplinId, currentGroupId) * 100.0, 0, 'f', precision);
			}
			else if(currentView == -1)
			{
				retVal = 0.0;
				ret = dBase->getGroup(currentGroupId).getStudent(index.data(Qt::UserRole).toUInt()).getRating(currentDisciplinId, index.column() - 1);
				res = dBase->getCriterionsValues(currentDisciplinId, currentGroupId);
				Q_ASSERT(ret.count() == res.count());
				for(int i = 0; i < ret.count(); i++)
				{
					retVal += ret.at(i) * res.at(i);
				}
				return retVal;
			}
			else
			{
				retr = dBase->getGroup(currentGroupId).getStudent(index.data(Qt::UserRole).toUInt()).getRating(currentDisciplinId, index.column() - 1).at(currentView);
				resr = dBase->getCriterion(currentDisciplinId, currentGroupId).at(currentView).getValue();
				return retr * resr;
			}
		}
	}
	else if(role == Qt::EditRole)
	{
		return dBase->getGroup(currentGroupId).getStudent(index.data(Qt::UserRole).toUInt()).getRating(currentDisciplinId, index.column() - 1).at(currentView);
	}
	else if(role == Qt::UserRole)
	{
		return dBase->getGroup(currentGroupId).getStudent(index.row()).getStudentId();
	}
	return QVariant();
}

int RatingModel::columnCount(const QModelIndex &parent) const
{
	return dBase->getGroup(currentGroupId).getDateCount(currentDisciplinId) + 1;
}

int RatingModel::rowCount(const QModelIndex &parent) const
{
	return dBase->getGroup(currentGroupId).count();
}

bool RatingModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(index.isValid() && role == Qt::EditRole)
	{
		dBase->getGroupPtr(currentGroupId)->getStudentPtr(index.data(Qt::UserRole).toUInt())->replaceRating(currentDisciplinId, index.column() - 1, currentView, value.toFloat());
		return true;
	}
	return false;
}

QVariant RatingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if(section == 0)
			return QObject::trUtf8("Ф.И.О.");
		return dBase->getGroup(currentGroupId).getDate(currentDisciplinId, section - 1);
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags RatingModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if(currentView < 0 || index.column() == 0)
		return flags;
	return (flags | Qt::ItemIsEditable);
}

void RatingModel::setDb(DataBaseStudRating *db)
{
	dBase = db;
	reset();
}

void RatingModel::setParam(quint32 disciplinId, quint32 groupId, qint32 viewId)
{
	currentDisciplinId = disciplinId;
	currentGroupId = groupId;
	currentView = viewId;
	reset();
}

void RatingModel::setPrecision(qint32 precision)
{
	this->precision = precision;
}

#include "ratingmodel.moc"