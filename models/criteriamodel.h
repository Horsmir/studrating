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


#ifndef CRITERIAMODEL_H
#define CRITERIAMODEL_H

#include <QtCore/QAbstractTableModel>
#include "databasestudrating.h"


class CriteriaModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit CriteriaModel(QObject *parent = 0);
	virtual ~CriteriaModel();
	
	void setDb(DataBaseStudRating *db);
	void setParams(quint32 disciplinId, quint32 groupId);
	
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	
private:
	DataBaseStudRating *dBase;
	quint32 currentDisciplinId;
	quint32 currentGroupId;
};

#endif // CRITERIAMODEL_H
